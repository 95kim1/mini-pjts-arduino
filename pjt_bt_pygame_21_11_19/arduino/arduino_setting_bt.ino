#include <SoftwareSerial.h>

#define blue_rx 3
#define blue_tx 2

SoftwareSerial BlueSerial(blue_rx, blue_tx);

void setup() {
  Serial.begin(9600);
  BlueSerial.begin(9600);
}

void loop() {
  if (BlueSerial.available()) {
    Serial.write(BlueSerial.read());
  }

  if (Serial.available()) {
    BlueSerial.write(Serial.read());
  }
}

// AT -> OK  (연결 확인)
// AT+NAME{name} -> OKsetname
//    AT+NAMEkshee
// AT+PIN{pw} -> OKsetPIN
//    AT+PIN0425
