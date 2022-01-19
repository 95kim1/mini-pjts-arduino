const int ledBasePin = 8;
const int btnBasePin = 2;

void setup() {
  int i = 0;
  for (i = 0; i < 4; i++) {
    pinMode(ledBasePin + i, OUTPUT);
    pinMode(btnBasePin + i, INPUT);
  }
  //Serial.begin(115200);
}

void loop() {
  int i = 0;
  int btnState;
  for (i = 0; i < 4; i++) {
    btnState = digitalRead(btnBasePin + i);
    digitalWrite(ledBasePin + i, btnState);
    //Serial.println(btnState);
  }
}