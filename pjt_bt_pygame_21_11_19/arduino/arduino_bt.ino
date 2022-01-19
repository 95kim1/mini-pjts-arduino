#include <SoftwareSerial.h>

#define x_pin A0
#define y_pin A1
#define blue_rx 6
#define blue_tx 7

SoftwareSerial blueSerial(blue_rx, blue_tx);

unsigned long prevMillis_joy = 0;
int interval = 10;
int xVal, yVal;
int val;

int setting_ok = 0;

void setup() {
  Serial.begin(9600);
  blueSerial.begin(9600);
}

int i = 9;

void loop() {
  unsigned long currentMillis = millis(); 

  if (currentMillis - prevMillis_joy >= interval) {
    prevMillis_joy = currentMillis;
    xVal = analogRead(x_pin);
    yVal = analogRead(y_pin);

    val = 0;
    
    if (xVal > 900) {
      val |= (1<<3); //right
    } else if (xVal < 100) {
      val |= (1<<1); //left
    }
    
    if (yVal > 900) {
      val |= (1<<2); //down
    } else if (yVal < 100) {
      val |= (1<<0); //up
    }
    
    blueSerial.write(val);
  }

  if (blueSerial.available()) {
    Serial.write(blueSerial.read());
  }
}
