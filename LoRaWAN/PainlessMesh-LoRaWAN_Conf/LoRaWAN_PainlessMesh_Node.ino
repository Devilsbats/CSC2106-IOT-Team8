#include <painlessMesh.h>
#include <M5StickCPlus.h>
#include <Arduinojson.h>
#include "DHT.h"

// Serial
#define SERIAL_RX 0
#define SERIAL_TX 25

// Ultrasonic
#define TRIG 32
#define ECHO 33

// Temperature
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define MESH_SSID "joshua"
#define MESH_PASSWORD "meshpotatoes"
#define MESH_PORT 5555
#define MAX_MESSAGES 10

// Prototypes
void sendMessage();
void receivedCallback(uint32_t from, String& msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
void delayReceivedCallback(uint32_t from, int32_t delay);

Scheduler userScheduler;  // to control your personal task
painlessMesh mesh;

bool calc_delay = false;
SimpleList<uint32_t> nodes;

// Prototype
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage);  // start with a one second interval

// Task to blink the number of nodes
Task blinkNoNodes;
bool onFlag = false;

// Task to blink the number of nodes
unsigned long lastMsg = 0;

struct Message {
  int node;
  int distance;
  float temperature;
};

void setupMesh(){
  mesh.setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see error messages

  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void lcdReset(){
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.printf("Wireless Mesh", 0);
}

void setup() {
  dht.begin();
  M5.begin();
  lcdReset();
  // Serial1.begin(115200, SERIAL_8N1, SERIAL_RX, SERIAL_TX);
  setUpUltrasonic(TRIG, ECHO);
  setupMesh();
}

void setUpUltrasonic(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(triggerPin, LOW);
}

int getDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int dist = duration * 0.017;
  return dist;
}

float getTemperature() {
  float temp = dht.readTemperature();
  return temp;
}

void loop() {
  // int distance = getDistance(TRIG, ECHO);
  // float temperature = getTemperature();
  
  // String data = "Distance: " + String(distance) + " cm, Temperature: " + String(temperature) + " Â°C";
  // Serial.println(data);
  // Serial1.write(data.c_str());

  mesh.update();

  unsigned long now = millis();  // Obtain the host startup duration.  
  if (now - lastMsg > 5000) {
    lastMsg = now;
    //reset screen every 5 seconds
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
  }
  // delay(2000);
}

void sendMessage() {
  const size_t CAPACITY = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<CAPACITY> doc;

  JsonDocument customMsg = doc.to<JsonObject>();
  customMsg["node"] = 3;
  customMsg["distance"] = getDistance(TRIG, ECHO);
  customMsg["temperature"] = getTemperature();
  String msg;
  serializeJson(customMsg, msg);

  mesh.sendBroadcast(msg);

  if (calc_delay) {
    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end()) {
      mesh.startDelayMeas(*node);
      node++;
    }
    calc_delay = false;
  }
  Serial.printf("Sending message: %s\n", msg.c_str());

  taskSendMessage.setInterval(random(TASK_SECOND * 3, TASK_SECOND * 5));  // between 1 and 5 seconds
}


void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  String msgObject = msg.c_str();

  // Transfer through UART to LoRa
  // Serial1.println(msgObject);

  const size_t CAPACITY = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<CAPACITY> doc;
  deserializeJson(doc, msgObject);
  JsonObject parsedMsg = doc.as<JsonObject>();

  unsigned long targetId = parsedMsg["node"];
  int distance = parsedMsg["distance"];
  float temperature = parsedMsg["temperature"];

  M5.Lcd.printf("Received from %lu\n", targetId);
  M5.Lcd.printf("Distance: %u cm\n", distance);
  M5.Lcd.printf("temperature: %u C\n",temperature);
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");

  nodes = mesh.getNodeList();

  Serial.printf("Num nodes: %d\n", nodes.size());
  Serial.printf("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end()) {
    Serial.printf(" %u", *node);
    node++;
  }
  Serial.println();
  calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay) {
  Serial.printf("Delay to node %u is %d us\n", from, delay);
}