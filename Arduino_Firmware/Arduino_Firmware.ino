/*
 * Arduino_Firmware.ino
 * Copyright (C) 2022 - Present, Julien Lecomte - All Rights Reserved
 * Licensed under the MIT License. See the accompanying LICENSE file for terms.
 */

#include <ESP32Servo.h>

constexpr auto DEVICE_GUID = "b45ba2c9-f554-4b4e-a43c-10605ca3b84d";

constexpr auto COMMAND_PING = "COMMAND:PING";
constexpr auto RESULT_PING = "RESULT:PING:OK:";

constexpr auto COMMAND_INFO = "COMMAND:INFO";
constexpr auto RESULT_INFO = "RESULT:DarkSkyGeek's Telescope Cover Firmware v1.0";

constexpr auto COMMAND_GETSTATE = "COMMAND:GETSTATE";
constexpr auto RESULT_STATE_UNKNOWN = "RESULT:STATE:UNKNOWN";
constexpr auto RESULT_STATE_OPEN = "RESULT:STATE:OPEN";
constexpr auto RESULT_STATE_CLOSED = "RESULT:STATE:CLOSED";

constexpr auto COMMAND_OPEN = "COMMAND:OPEN";
constexpr auto COMMAND_CLOSE = "COMMAND:CLOSE";

constexpr auto ERROR_INVALID_COMMAND = "ERROR:INVALID_COMMAND";

enum CoverState {
    openState,
    closedState
} state;

const int SERVO_PIN = 4; //Pin used for servo control, update this to whichever pin you use
const int LED_BUILTIN = 8;

int pos = 0;

Servo servo;

void setup() {
    state = closedState;

    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    Serial.flush();

    // Initialize servo - Update these to match your servo model
    servo.setPeriodHertz(333);
    servo.attach(SERVO_PIN, 700, 2400);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        if (command == COMMAND_PING) {
            handlePing();
        }
        else if (command == COMMAND_INFO) {
            sendFirmwareInfo();
        }
        else if (command == COMMAND_GETSTATE) {
            sendCurrentState();
        }
        else if (command == COMMAND_OPEN) {
            openCover();
        }
        else if (command == COMMAND_CLOSE) {
            closeCover();
        }
        else {
            handleInvalidCommand();
        }
    }
}

void handlePing() {
    Serial.print(RESULT_PING);
    Serial.println(DEVICE_GUID);
}

void sendFirmwareInfo() {
    Serial.println(RESULT_INFO);
}

void sendCurrentState() {
    switch (state) {
    case openState:
        Serial.println(RESULT_STATE_OPEN);
        break;
    case closedState:
        Serial.println(RESULT_STATE_CLOSED);
        break;
    default:
        Serial.println(RESULT_STATE_UNKNOWN);
        break;
    }
}

void openCover() {
    if (pos < 180) {
        for (; pos <= 180; pos++) {
            servo.write(pos);
            //delay(30); Add delay if you want to slow down the servo movement (Delay time in ms)
        }
    }
    state = openState;
}

void closeCover() {
    if (pos > 0) {
        for (; pos >= 0; pos--) {
            servo.write(pos);
        }
    }
    state = closedState;
}

void handleInvalidCommand() {
    Serial.println(ERROR_INVALID_COMMAND);
}
