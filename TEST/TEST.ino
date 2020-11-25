
void setup() {

  Serial.begin(9600);
  pinMode(10, INPUT);
  pinMode(18, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(18),isr,FALLING);

}

void loop() {
  Serial.println(huy);
  delay(1000);
}
