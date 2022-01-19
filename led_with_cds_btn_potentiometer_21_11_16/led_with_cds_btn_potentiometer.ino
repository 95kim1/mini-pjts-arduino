static struct button {
  int pin;
  int led_pin;
  int state; //btn pushed: HIGH, not: LOW

  unsigned long prev_millis;
  int interval;
} btn {
  .pin = 2,
  .led_pin = 3,
  .state =LOW,
  .prev_millis = 0,
  .interval = 300,
};

static struct cds_sensor {
  int pin;
  int lux;
  unsigned long prev_millis;
  int interval;
} cds = {
  .pin = A0,
  .lux = 0,
  .prev_millis = 0,
  .interval = 20,
};

static struct potentiometer {
  int pin;
  int val;
} ptmr = {
  .pin = A1,
  .val = 0,
};

static struct led_sensor {
  int pin;
  int lux;
  int on;
} led = {
  .pin = 5,
  .lux = 0,
  .on = LOW,
};

static int get_lux(int val)
{
  return (val + 1) / 4.0 - 1;
}

void setup() {
  Serial.begin(9600);
  pinMode(btn.pin, INPUT);
  pinMode(btn.led_pin, OUTPUT);
}

void loop() {
  unsigned long cur_millis = millis();

  if (cur_millis - btn.prev_millis >= btn.interval && digitalRead(btn.pin) == HIGH) {
    btn.prev_millis = cur_millis;
    btn.state ^= 1; // btn toggle
    Serial.println(btn.state);
    digitalWrite(btn.led_pin, btn.state);

    if (btn.state == LOW) {
      analogWrite(led.pin, 0);
    }
  }

  if (btn.state == HIGH && cur_millis - cds.prev_millis >= cds.interval) {
    cds.prev_millis = cur_millis;
    cds.lux = analogRead(cds.pin); // ~ 밝기
    ptmr.val = analogRead(ptmr.pin); // ~ 전압

    // led on/off
    if (cds.lux < 450 && led.on == LOW) {
      Serial.println("ON");
      led.on = HIGH;
    } else if (cds.lux >= 550 && led.on == HIGH) {
      Serial.println("OFF");
      led.on = LOW;
    }

    // set led 밝기
    if (led.on) {
      led.lux = get_lux(ptmr.val);
    } else {
      led.lux = 0;
    }

    // led 켜기 (led.lux == 0 -> led 끄기)
    analogWrite(led.pin, led.lux);
  }
}