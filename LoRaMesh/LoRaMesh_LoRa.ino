#include <RH_RF95.h>
#include <SPI.h>
#include <time.h> // For time functions
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

#define NETWORK_ID 0x1ae5

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Unique node ID
const uint8_t nodeId = 2;

// Sequence Number (Timestamp)
uint32_t sequenceNumber = 0;


// Define the SoftwareSerial object with the RX and TX pins
SoftwareSerial mySerial(0, 2); // RX, TX

// Data from M5StickCPlus
int distance; 
float temperature;
int m5ID;
String incomingData;
char str_temp[6];


// Check for 1 comma or 2 comma
bool commastate;

// Message structure with header, payload, and checksum
struct Message {

  uint8_t preamble; // Preamble for synchronization
  uint16_t networkId; // Unique Id to identify out network
  uint8_t nodeId; // Node ID associated with the message
  uint16_t crc; // Checksum for payload
  uint32_t timestamp; // Unix timestamp
  uint8_t ttl; // Time to live
  char data[80]; // Payload data

};

// Function prototype for sendMessage
void sendMessage(Message* msg = nullptr);

//Initialize
void setup() {
  //Resetting the RFM95_RST Module
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  //Set up serial connection with Baud Rate 115200 from M5StickCPlus
  while(!mySerial);
  mySerial.begin(115200);

  //Set up serial connection with Braud Rate 9600
  while (!Serial);
  Serial.begin(9600); 

  //Initialise LoRa Module
  if (!rf95.init()) {
      Serial.println("init failed");
      while (1);
  }

  //Set Frequency LoRa Module
  if (!rf95.setFrequency(915.0)) {
      Serial.println("setFrequency failed");
      while (1);
  }

  //Set Power of Lora Module
  rf95.setTxPower(23, false);
}

void loop() {
  // Check for incoming messages and assign the variables
  if (mySerial.available() > 0) {
      incomingData = mySerial.readStringUntil('\n'); // Read until newline character
      int separatorIndex1 = incomingData.indexOf(','); // Find the first separator
      int separatorIndex2 = incomingData.indexOf(',', separatorIndex1 + 1); // Find the second separator
      
      // Check if the string contains only one comma
      bool oneComma = separatorIndex1 != -1 && separatorIndex2 == -1;
      
      if (separatorIndex1 != -1 && (separatorIndex2 != -1 || oneComma)) {
          String distanceStr;
          String M5IDStr;
          String temperatureStr;

          if (oneComma) {
              // If there's only one comma, parse accordingly
              distanceStr = incomingData.substring(0, separatorIndex1);
              M5IDStr = incomingData.substring(separatorIndex1 + 1);

              // Convert strings to appropriate data types
              distance = distanceStr.toInt();
              m5ID =  M5IDStr.toInt(); // Assuming the third value is an integer

              // Print received values
              Serial.print("Received Distance: ");
              Serial.print(distance);
              Serial.print(", m5ID: ");
              Serial.print(m5ID);

              commastate = true;

              // Call your sendMessage function here
              sendMessage();

          } else {
              // If there are two commas, parse accordingly
              distanceStr = incomingData.substring(0, separatorIndex1);
              M5IDStr = incomingData.substring(separatorIndex1 + 1, separatorIndex2);
              temperatureStr = incomingData.substring(separatorIndex2 + 1);

              // Convert strings to appropriate data types
              distance = distanceStr.toInt();
              temperature = temperatureStr.toFloat();
              m5ID =  M5IDStr.toInt(); // Assuming the third value is an integer

              // Print received values
              Serial.print("Received Distance: ");
              Serial.print(distance);
              Serial.print(", m5ID: ");
              Serial.print(m5ID);
              Serial.print(", Temperature: ");
              Serial.println(temperature);

              /* 4 is mininum width, 2 is precision; float value is copied onto str_temp*/
              dtostrf(temperature, 4, 2, str_temp);
              commastate = false;
              // Call your sendMessage function here
              sendMessage();
          }
      }
  }


  //Opening my "port" publicly
  if (rf95.available()) {  
      uint8_t buf[sizeof(Message)]; //Message receive from all the strangers
      uint8_t len = sizeof(buf); 

      if (rf95.recv(buf, &len)) { //If the buffer != null

        Message* msg = (Message*)buf;

        // Process the message
        processMessage(msg);

      } else {
        Serial.println("Receive failed");
      }
 }
}

void processMessage(Message* msg) {

  //Check if this packet belongs to our network
  if(msg->networkId == NETWORK_ID){
    // Decrement TTL and check if the message should be forwarded
    if (msg->ttl > 0) {
        msg->ttl--; //Decrement TTL then forward

        // Forward the message
        sendMessage(msg);
    }
  }
}

void sendMessage(Message* msg) {
  // Construct a message and send to other node
  Message newMsg;

  //New Message
  if (msg == nullptr) {

      newMsg.preamble = 12; // Example preamble length
      newMsg.networkId = NETWORK_ID; //Set the Network Id 
      newMsg.nodeId = nodeId; // Set the node ID
      newMsg.timestamp = sequenceNumber; // Using the SequenceNumber as a timestamp
      newMsg.ttl = 3; // Set initial TTL

      Serial.println(m5ID);
      if(commastate){
        snprintf(newMsg.data, sizeof(newMsg.data), "%d,%d", distance, m5ID);
      }else{
        snprintf(newMsg.data, sizeof(newMsg.data), "%d,%d,%s", distance, m5ID, str_temp);
      }
      // Calculate CRC for payload
      //newMsg.crc = calculateCRC(newMsg.data, strlen(newMsg.data));

      Serial.println(" ");
      Serial.print("Current_Timestamp: ");
      Serial.println(sequenceNumber);

      Serial.print("Broadcasting New Data from B: ");
      Serial.println(newMsg.data);
      Serial.println(" ");
      //Increment of the sequenceNumber
      ++sequenceNumber;
  } else {
      // Use the provided message
      newMsg = *msg; 
  }

  // Sending the new message
  rf95.send((uint8_t*)&newMsg, sizeof(newMsg));
  rf95.waitPacketSent();

  Serial.println("Message Sent");
}

//no need checksum
uint16_t calculateCRC(const char* data, size_t length) {
  uint16_t crc = 0xFFFF; // Initial value (Key)
  
  for (size_t i = 0; i < length; i++) {
      crc ^= (uint16_t)data[i]; // XOR byte into least significant byte of crc
      for (uint8_t j = 0; j < 8; j++) { // Loop over each bit
        if ((crc & 0x0001) != 0) { // If the least significant bit is set
          crc = (crc >> 1) ^ 0xA001; // Shift right and XOR 0xA001
        } else {
          crc = crc >> 1; // Just shift right
        }
      }
  }
  return crc;
}