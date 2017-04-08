/****************
 * CODE OPTIONS *
 ****************/
#define DEBUG 1
#define SERIAL_BAUD 9600

// the pin where ESC's signal is put
#define ENGINE_PID 6
// delay for esc commands (in ms)
#define ESC_DELAY 2000
// the pid of the button
#define JOYPAD_BUTTON_PID 8

#define JOYPAD_XAXIS_PID 12
#define JOYPAD_YAXIS_PID 13
#define JOYPAD_UPDATE_TIME 40

/****************
 * DEBUG MACROS *
 ****************/
#if DEBUG == 1
    #define DEBUGPRINT(x) Serial.println(__LINE__); Serial.println(x)
#else
    #define DEBUGPRINT(x)
#endif

/*****************
 *   LIBRARIES   *
 *****************/
#include <Servo.h>
#include <Button.h>
#include "mylib.h"

/********************
 * GLOBAL VARIABLES *
 ********************/
Servo engine; // my engine
float throttle = 0; // throotle
Button button(JOYPAD_BUTTON_PID, Button::INTERNAL_PULLUP);

Joystick joystick(JOYPAD_XAXIS_PID, JOYPAD_YAXIS_PID);

/*******************
 *   FUNCTIONS     *
 *******************/

/*
 * input int speed (the speed value from 0 to 100)
 * return int speed (the speed degree from 0 to 180)
 * use ESC like a servo
 */
int conv100To180(int speed) {
    return map(speed, 0, 100, 0, 180);
}

/*
 * input int throttle (the throttle of motor from 0 to 100%)
 * output none
 */
void setThrottle(float inThrottle) {
    engine.write(conv100To180((int) inThrottle));
    throttle = inThrottle;
}

/*******************
 *     SETUP       *
 *******************/
void setup() {
    // Debug instrunctions
#ifdef DEBUG
    Serial.begin(SERIAL_BAUD);
#endif

    // Engine initialize
    engine.attach(ENGINE_PID);
    setThrottle(0);
    // Wait for ESC arm
    delay(ESC_DELAY);
}

/*******************
 *      LOOP       *
 *******************/
void loop() {
    static unsigned long t1 = millis();
    static unsigned long t2;
    t2 = millis();

    // Button stop
    button.process();
    // if stop button is pressed set it to 0
    if (button.press()) {
        setThrottle(0);
    }

    // Joypad change throttle
    joystick.loop();

    // if i have to update throttle
    if ((t2 - t1) >= JOYPAD_UPDATE_TIME) {
        throttle += joystick.dY * 0.02;

        if ((int) throttle > 100)
            throttle = 100;
        else if ((int) throttle < 0)
            throttle = 0;

        setThrottle(throttle);
        t1 = millis();
    }

}
