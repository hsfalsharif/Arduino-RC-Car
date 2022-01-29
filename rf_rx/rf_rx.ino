#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <ctype.h>

RF24 rf_device(9, 8); // CE , CSN
const byte address[6] = "00001";
const char text1[] = "M1";
const char text2[] = "M2";
const char text3[] = "M1 OFF";
const char text4[] = "M2 OFF";

//const char text2[] = "Direction 2";
char text[32] = {0};
char motor_sel;
char speed_str[8] = {0};

// Motor part
uint8_t enA = 6;
uint8_t in1 = 7;
uint8_t in2 = 4;

uint8_t enB = 5;
uint8_t in3 = 3;
uint8_t in4 = 2;
uint16_t speed_val = 255;
int input_speed = 0;

void setup() {
    Serial.begin(9600);
    
    rf_device.begin();
    rf_device.openReadingPipe(0, address);
    rf_device.startListening();
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);

    // Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
    analogWrite(enA, speed_val);
	analogWrite(enB, speed_val);
}

void loop() {
    analogWrite(enA, speed_val);
	analogWrite(enB, speed_val);
    if (rf_device.available()) {
        rf_device.read(&text, sizeof(text));
        if (text[0] == 'A' || text[0] == 'B') {
            motor_sel = text[0];
            text[0] = ' ';
            input_speed = atoi(text);

            if (motor_sel == 'A') {
                if (input_speed < 0) {
                    speed_val = input_speed * -1;
                    digitalWrite(in1, LOW);
                    digitalWrite(in2, HIGH);
                }
                else {
                    speed_val = input_speed;
                    digitalWrite(in1, HIGH);
                    digitalWrite(in2, LOW);
                }
            }

            if (motor_sel == 'B') {
                if (input_speed < 0) {
                    speed_val = input_speed * -1;
                    digitalWrite(in3, LOW);
                    digitalWrite(in4, HIGH);
                }
                else {
                    speed_val = input_speed;
                    digitalWrite(in3, HIGH);
                    digitalWrite(in4, LOW);
                }
            }
        }

        if  (strcmp(text, text3) == 0) {
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
        }

        if  (strcmp(text, text4) == 0) {
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
        }
    }
}
