#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define PIN_LATCH 5
#define PIN_SHIFT 4
#define PIN_DATA  1

#define PIN_HIGH(X) digitalWrite(X, HIGH)
#define PIN_LOW(X) digitalWrite(X, LOW)
#define PIN_SET(X,Y) digitalWrite(X,Y)

#define LATCH_HIGH() PIN_SET(PIN_LATCH, HIGH)
#define LATCH_LOW() PIN_SET(PIN_LATCH, LOW)
#define LATCH_SIGNAL() {LATCH_HIGH();LATCH_LOW();}

#define SHIFT_HIGH() PIN_SET(PIN_SHIFT, HIGH)
#define SHIFT_LOW() PIN_SET(PIN_SHIFT, LOW)
#define SHIFT_SIGNAL() {SHIFT_LOW();SHIFT_HIGH();}

#define DATA_SET(X) PIN_SET(PIN_DATA, X)

void pin_init(void);
void write_data(unsigned int data_out, int dp);

// cathode 방식
const int seg7[10] = {
        // 0x?? => 0b A B C D E F G dp  ( Q0~7을 A, B, C, ~ , dp 에 연결 )
		// 1을 켤라면, BC를 켜야 하므로 b0110 0000 == 0x60
		// 2를 켤라면, ABDEG를 켜야 하므로 b1101 1010 == 0xDA
        0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6
};

void write_data(unsigned int data_out, int dp)
{
        int i;
        unsigned hold;

        data_out |= dp; // dp : either 0 or 1

        for (i = 0; i < 8; i++) {
                DATA_SET( (data_out >> i) & 0x01 );
                SHIFT_SIGNAL();
        }
        LATCH_SIGNAL();
}

//#define TEST
int main(void)
{        
        pin_init();

#ifdef TEST
        int pos = 1;
        while (1) {
                write_data(pos);
                printf("%d\n", pos);
                pos = (pos >= 128) ? 1 : (pos << 1);
                delay(1000);
        }
#endif

#ifndef TEST
        int counter = 0;
        int dp = 0; // dot  :  either 0 or 1
        while (1) {
                write_data(seg7[counter], dp);

                printf("%d\n", counter);

                if (counter >= 9) {
                        counter = 0;
                        dp ^= 1; //toggle
                } else {
                        counter++;
                }

                delay(1000);
        }
#endif

        return 0;
}

void pin_init(void)
{
        wiringPiSetup();
        pinMode(PIN_LATCH, OUTPUT);
        pinMode(PIN_SHIFT, OUTPUT);
        pinMode(PIN_DATA,  OUTPUT);

		SHIFT_LOW();
        LATCH_HIGH();
        DATA_SET(LOW);
        
}

