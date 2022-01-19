#include <SoftwareSerial.h>
#define x_pin A0
#define y_pin A1

#define rx_uart 2
#define tx_uart 3

SoftwareSerial Serial_(rx_uart, tx_uart);

int xVal, yVal;
int val;

void setup() {
  Serial.begin(9600);
  Serial_.begin(9600);
}

void loop() {
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

    Serial.println(val);
    Serial_.write(val);
}
