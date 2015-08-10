/* 
 * File:   main.c
 * Author: emmanuelsantana
 *
 * Created on August 5, 2015, 5:46 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
/*
 * 
 */
void forward();
void backward();
void setStep(short x1, short x2, short y1, short y2);
void init();
void toggleLed();
void stopMotor();
void runMotor(short direction);
void displayNumber(unsigned short number);

short A1 = 17;
short A2 = 22;
short B1 = 23;
short B2 = 24;
short a = 5;
short b = 6;
short c = 12;
short d = 13;
// 5,6,12,13
short buttonPin = 4;
short buttonPin2 = 2;
short ledPin = 18;

short stepCount = 8;
int delayTime;
int buttonCounter = 0;
int buttonSpeedCounter = 0;
int ledCounter = 0;
unsigned int motorCounter = 0;

short direction = 0;
short ledStatus = 0;
short currentSpeed = 0;
short stopped = 1;
short forwardStepCount;
short backwardStepCount = 8;
short count = 0;
short sequence[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1},
};

int main(int argc, char** argv) {
    init();
    stopMotor();
    displayNumber(0);
    for (;;) {
        motorCounter++;
        buttonCounter++;
        buttonSpeedCounter++;
        ledCounter++;


        if (motorCounter == delayTime) {
            if (stopped == 0) {
                printf("Run motor");
                runMotor(direction);
            } else {
                printf("DETENIDO\n\r");
            }
            motorCounter = 0;
        }
        printf("Avanzando con delay: %d > Contador: %d y Direccion: %d, Esta detenido? %d\n\r", delayTime, motorCounter, direction, stopped);

        if (buttonCounter == 50000) {
            if (digitalRead(buttonPin)) {
                direction = 1;
            } else {
                direction = 0;
            }
            buttonCounter = 0;
        }
        if (buttonSpeedCounter == 10000) {

            if (digitalRead(buttonPin2) == 0) // Button is released if this returns 1
            {
                switch (currentSpeed) {
                    case 0:
                        displayNumber(1);
                        stopped = 0;
                        motorCounter = 0;
                        delayTime = 150;
                        currentSpeed = 1;
                        break;
                    case 1:
                        displayNumber(2);
                        stopped = 0;
                        motorCounter = 0;
                        delayTime = 100;
                        currentSpeed = 2;
                        break;
                    case 2:
                        displayNumber(3);
                        motorCounter = 0;
                        stopped = 0;
                        delayTime = 50;
                        currentSpeed = 3;
                        break;
                    case 3:
                        displayNumber(0);
                        motorCounter = 0;
                        stopped = 1;
                        stopMotor();
                        currentSpeed = 0;
                        break;
                }
                printf("Changed speed to: %d\n\r", currentSpeed);
                printf("Stopped?: %d\n\r", stopped);
            }
            buttonSpeedCounter = 0;
        }

        if (ledCounter == 10000) {
            toggleLed();
            ledCounter = 0;
        }
    }

    return (EXIT_SUCCESS);
}

void setStep(short x1, short x2, short y1, short y2) {
    // printf("Step: %d, %d, %d, %d\n\r", x1, x2, y1, y2);

    digitalWrite(A1, x1);
    digitalWrite(A2, x2);
    digitalWrite(B1, y1);
    digitalWrite(B2, y2);
}

void forward() {
    forwardStepCount++;
    if (forwardStepCount < stepCount) {
        setStep(sequence[forwardStepCount][0], sequence[forwardStepCount][1], sequence[forwardStepCount][2], sequence[forwardStepCount][3]);
    } else {
        forwardStepCount = 0;
    }
}

void backward() {
    backwardStepCount--;
    if (backwardStepCount > 0) {
        setStep(sequence[backwardStepCount][0], sequence[backwardStepCount][1], sequence[backwardStepCount][2], sequence[backwardStepCount][3]);
    } else {
        backwardStepCount = stepCount;
    }
}

void toggleLed() {
    if (ledStatus == 0) {
        digitalWrite(ledPin, LOW);
        ledStatus = 1;
    } else {
        digitalWrite(ledPin, HIGH);
        ledStatus = 0;
    }
}

void stopMotor() {
    setStep(0, 0, 0, 0);
}

void runMotor(short direction) {
    if (direction == 1) {
        forward();
    } else {
        backward();
    }

}

void init() {
    printf("Configuring wiringPi...\n\r");
    wiringPiSetupGpio();
    printf("Setting pin modes...\n\r");
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(B1, OUTPUT);
    pinMode(B2, OUTPUT);
    pinMode(buttonPin, INPUT);
    pinMode(buttonPin2, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(a, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(c, OUTPUT);
    pinMode(d, OUTPUT);
}

void displayNumber(unsigned short number) {
    switch (number) {
        case 0:
            digitalWrite(a, LOW);
            digitalWrite(b, LOW);
            digitalWrite(c, LOW);
            digitalWrite(d, LOW);
            break;
        case 1:
            digitalWrite(a, HIGH);
            digitalWrite(b, LOW);
            digitalWrite(c, LOW);
            digitalWrite(d, LOW);
            break;
        case 2:
            digitalWrite(a, LOW);
            digitalWrite(b, HIGH);
            digitalWrite(c, LOW);
            digitalWrite(d, LOW);
            break;
        case 3:
            digitalWrite(a, HIGH);
            digitalWrite(b, HIGH);
            digitalWrite(c, LOW);
            digitalWrite(d, LOW);
            break;
    }

    void drawScreen() {
        int i;
        for (i = 0; i < 100; i++) {
            printf("*");
            for (i = 0; i < 30; i++) {
                if (i == 0 || i == 99) {
                    printf("*");
                }
            }
        }

    }
}