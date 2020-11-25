#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  delay(500);
}

void loop() {
  if (Serial.find("hello")) {
    mySerial.write("Hello Mega");
    Serial.readString();
  }

  if (mySerial.find("Hello Uno!")) {
    Serial.println("Hello Uno!");
    mySerial.readString();
  }
}
