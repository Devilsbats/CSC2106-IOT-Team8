#include <RH_RF95.h>
#include <SPI.h>
#include <time.h> // For time functions
#include <SoftwareSerial.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

#define NETWORK_ID 0x1ae5

// Define the RX and TX pins for the SoftwareSerial connection
#define RX_PIN 0 
#define TX_PIN 1 

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Define the SoftwareSerial object with the RX and TX pins
SoftwareSerial mySerial(0, 1); // RX, TX

// // Unique node ID
// const uint8_t nodeId = 1;

// Message structure with header, payload, and checksum
struct Message {

  uint8_t preamble; // Preamble for synchronization
  uint16_t networkId; // Unique Id to identify out network
  uint8_t nodeId; // Node ID associated with the message
  uint16_t crc; // Checksum for payload
  uint32_t timestamp; // Unix timestamp
  uint8_t ttl; // Time to live
  char data[75]; // Payload data

};

// Routing table entry
struct RoutingTableEntry {
 uint8_t nodeId;
 uint8_t latest_timestamp;
};

// Simple routing table for demonstration
int routingTable_size = 0;      // Current size of the dynamic array
RoutingTableEntry *routingTable = nullptr;

// Function prototype for sendMessage
void sendMessage(Message* msg = nullptr);

void addEntry(RoutingTableEntry entry){
  ++routingTable_size;
  Serial.print("Adding New Entry: Size = ");
  Serial.println(routingTable_size);
  // Create a new array with the increased capacity
  RoutingTableEntry *tempArray = new RoutingTableEntry[routingTable_size];

  // Copy elements from the old array to the new array
  if (routingTable != nullptr) {
    for (int i = 0; i < (routingTable_size - 1); i++) {
      tempArray[i] = routingTable[i];
    }
  }

  delete[] routingTable;

  routingTable = tempArray;

  routingTable[routingTable_size - 1] = entry;

  // Print the updated routing table
  Serial.println("");
  Serial.println("");
  Serial.println("Updated Routing Table:");

  for (int i = 0; i < routingTable_size; i++) {
    Serial.print("Node ID: ");
    Serial.print(routingTable[i].nodeId);
    Serial.print(", Latest Timestamp: ");
    Serial.println(routingTable[i].latest_timestamp);
  }

  Serial.println("");
  Serial.println("");
}

void setup() {
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // Start the software serial connection
  while(!Serial);
  mySerial.begin(9600);


  if (!rf95.init()) {
      Serial.println("init failed");
      while (1);
  }
  if (!rf95.setFrequency(915.0)) {
      Serial.println("setFrequency failed");
      while (1);
  }
  rf95.setTxPower(23, false);
}

void absorbMessage(Message* msg)
{
  if(msg->networkId == NETWORK_ID){
    bool found = false;
    for (int i = 0; i < routingTable_size; i++) {
      if (routingTable[i].nodeId == msg->nodeId) {
        found = true;

        if(msg->timestamp > routingTable[i].latest_timestamp)
        {
          routingTable[i].latest_timestamp = msg->timestamp;

          //New Message Recieved
          Serial.println("New Message Received:");
          Serial.print("Node ID: ");
          Serial.println(msg->nodeId);
          // Send through UART to m5
          Serial.println(msg->data);
          mySerial.println(msg->data);

          Serial.println("");
        }
        else {
          Serial.print("Dropped Message");
        }
      }
    }

    //Add new Entry
    if(!found)
    {
      RoutingTableEntry newEntry;
      newEntry.nodeId = msg->nodeId;
      newEntry.latest_timestamp = msg->timestamp;
      addEntry(newEntry);

      //New Message Received
      Serial.println("New Entry Found:");
      Serial.print("Node ID: ");
      Serial.println(msg->nodeId);
      // Serial.print("Latest Timestamp: ");
      // Serial.println(msg->timestamp);
      // Serial.print("Data: ");
      // Serial.println(msg->data);
      Serial.println("");
    }

    // Print the updated routing table
    Serial.println("");
    Serial.println("");
    Serial.println("Updated Routing Table:");
    for (int i = 0; i < routingTable_size; i++) {
      Serial.print("Node ID: ");
      Serial.print(routingTable[i].nodeId);
      Serial.print(", Latest Timestamp: ");
      Serial.println(routingTable[i].latest_timestamp);
    }
    Serial.println("");
    Serial.println("");
    }
}

void loop() {
  // Check for incoming messages
  if (rf95.available()) {  //Opening my port publicly
      uint8_t buf[sizeof(Message)]; //Message receive from all the strangers
      uint8_t len = sizeof(buf); 

      if (rf95.recv(buf, &len)) { //If the buffer != null

        Message* msg = (Message*)buf;

        //Data Processing - Display message from other nodes
        Serial.print("Received message: ");
        Serial.println(msg->data);
        Serial.print("Timestamp: ");
        Serial.println(msg->timestamp);
        Serial.print("TTL: ");
        Serial.println(msg->ttl);

        absorbMessage(msg);

      } else {
        Serial.println("Receive failed");
      }
 }
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