#include <M5StickCPlus.h>
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

void setup() {
  M5.begin();
  dht.begin();
  Serial1.begin(115200, SERIAL_8N1, SERIAL_RX, SERIAL_TX);
  setUpUltrasonic(TRIG, ECHO);
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
  int distance = getDistance(TRIG, ECHO);
  float temperature = getTemperature();
  
  String data = "Distance: " + String(distance) + " cm, Temperature: " + String(temperature) + " °C";
  Serial.println(data);
  Serial1.write(data.c_str());
  delay(1000);
}
