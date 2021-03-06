/****************
 * CODE OPTIONS *
 ****************/
#define DEBUG 0
#define SERIAL_BAUD 9600

// the pin where ESC's signal is put
#define ENGINE_PID 2
// delay for esc commands (in ms)
#define ESC_DELAY 2000

// Joypad
// the pid of the button
#define JOYPAD_BUTTON_PID 8
// analogic input x axis joypad
#define JOYPAD_XAXIS_PID A1
// analogic input y axis joypad
#define JOYPAD_YAXIS_PID A0
// delay for updating input in ms
#define JOYPAD_UPDATE_TIME 20
// joypad multiplicative factor for d speed having dX or dY
#define JOYPAD_MULTIPLICATIVE_FACTOR 0.02

/****************
 * DEBUG MACROS *
 ****************/
#if DEBUG == 1
#define NDEBUGPRINT(x) Serial.print(millis()); \
    Serial.print(": "); \
    Serial.print(__PRETTY_FUNCTION__); \
    Serial.print(' '); \
    Serial.print(__FILE__); \
    Serial.print(':'); \
    Serial.print(__LINE__); \
    Serial.print(' '); \
    Serial.println(x)
#define DEBUGPRINT(x) Serial.println(x)
#else
#define NDEBUGPRINT(x)
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
float throttle = 0; // throttle

Button button(JOYPAD_BUTTON_PID, Button::INTERNAL_PULLUP); // button manager

Joystick joystick(JOYPAD_XAXIS_PID, JOYPAD_YAXIS_PID); // joystick manager

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
 * control throttle
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
#if DEBUG == 1
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
    // if stop button is pressed set throttle to 0
    if (button.press()) {
        NDEBUGPRINT("Button pressed, setting throttle to 0");
        setThrottle(0);
        delay(ESC_DELAY);
        DEBUGPRINT("Now throtte is " + (String) throttle);
    }

    // Joypad change throttle
    joystick.loop();

    //DEBUGPRINT("Throttle: " + (String) throttle);
    //DEBUGPRINT("Degrees: " + (String) conv100To180(throttle));

    // if i have to update throttle
    if ((t2 - t1) >= JOYPAD_UPDATE_TIME) {
        // MAGIC NUMBER!!
        throttle += joystick.dY * JOYPAD_MULTIPLICATIVE_FACTOR;
        //NDEBUGPRINT(joystick.dY);
        if ((int) throttle > 100) {
            NDEBUGPRINT("Throttle is too hight, forcing it to max");
            throttle = 100;
        }
        else if ((int) throttle < 0) {
            NDEBUGPRINT("Throttle is too low, forcing it to min");
            throttle = 0;
        }

        setThrottle(throttle);
        t1 = millis();
    }

}
