String incomingString;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    incomingString = Serial.readStringUntil('\n');
    Serial.println(incomingString);
  }
}