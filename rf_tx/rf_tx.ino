#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// RF
RF24 rf_device(9, 8); // CE , CSN
const char text1[] = "M1";
const char text2[] = "M2";
const char text3[] = "M1 OFF";
const char text4[] = "M2 OFF";

char speed_tx1[6];
char speed_tx2[6];

const byte address[6] = "00001";

// Joystick 1
int VRx1 = A0;
int VRy1 = A1;
int SW1 = 4;
int xPosition1 = 0;
int yPosition1 = 0;
int mapX1 = 0;
int mapY1 = 0;
int SW_state1 = 0;

// Joystick 2
int VRx2 = A2;
int VRy2 = A3;
int SW2 = 5;
int xPosition2 = 0;
int yPosition2 = 0;
int mapX2 = 0;
int mapY2 = 0;
int SW_state2 = 0;

void setup() {
    Serial.begin(9600);

    // RF module initialization
    rf_device.begin();
    rf_device.openWritingPipe(address);
    rf_device.stopListening();

    // Joystick 1 initialization
    pinMode(VRx1, INPUT);
    pinMode(VRy1, INPUT);
    pinMode(SW1, INPUT_PULLUP);

    // Joystick 2 initialization
    pinMode(VRx2, INPUT);
    pinMode(VRy2, INPUT);
    pinMode(SW2, INPUT_PULLUP);
}

void loop() {
    xPosition1 = analogRead(VRx1);
    yPosition1 = analogRead(VRy1);
    SW_state1  = digitalRead(SW1);
    mapX1 = map(xPosition1, 0, 1023, -255, 255);
    mapY1 = map(yPosition1, 0, 1023, -255, 255);

    xPosition2 = analogRead(VRx2);
    yPosition2 = analogRead(VRy2);
    SW_state2  = digitalRead(SW2);
    mapX2 = map(xPosition2, 0, 1023, -255, 255);
    mapY2 = map(yPosition2, 0, 1023, -255, 255);

    if (mapX1 < -6 || mapX1 > 1) {
        sprintf(speed_tx1, "A%d", mapX1);
        rf_device.write(&speed_tx1, sizeof(speed_tx1));
        Serial.println(speed_tx1);
    }

    if (mapX2 < -6 || mapX2 > 1) {
        sprintf(speed_tx2, "B%d", mapX2);
        rf_device.write(&speed_tx2, sizeof(speed_tx2));
        Serial.println(speed_tx2);
    }
    
    if (SW_state1 == LOW) {
        stop_motor1();
    }

    if (SW_state2 == LOW) {
        stop_motor2();
    }
}

void stop_motor1() {
    rf_device.write(&text3, sizeof(text3));
    Serial.println(text3);
}

void stop_motor2() {
    rf_device.write(&text4, sizeof(text4));
    Serial.println(text4);
}
