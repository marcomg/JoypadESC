/****************
 * CODE OPTIONS *
 ****************/
#define DEBUG 0
#define SERIAL_BAUD 9600

// the pin where ESC's signal is put
#define ENGINE_PID 2
// delay for esc commands (in ms)
#define ESC_DELAY 2000
// the pid of the button
#define JOYPAD_BUTTON_PID 8

#define JOYPAD_XAXIS_PID A1
#define JOYPAD_YAXIS_PID A0
#define JOYPAD_UPDATE_TIME 20

/****************
 * DEBUG MACROS *
 ****************/
#if DEBUG == 1
#define NDEBUGPRINT(x) Serial.println("Debug info line: "); Serial.println(__LINE__); Serial.println(" "); Serial.println(x)
#define DEBUGPRINT(x) Serial.println(x)
#define DEBUGNL() Serial.println('\n')
#else
#define NDEBUGPRINT(x)
#define DEBUGPRINT(x)
#define DEBUGNL()
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
    // if stop button is pressed set it to 0
    if (button.press()) {
        NDEBUGPRINT("Button pressed, setting throttle to 0");DEBUGNL();
        setThrottle(0);
        delay(ESC_DELAY);
        DEBUGPRINT("Now throtte is ");DEBUGPRINT(throttle);DEBUGNL();
    }

    // Joypad change throttle
    joystick.loop();

    // if i have to update throttle
    if ((t2 - t1) >= JOYPAD_UPDATE_TIME) {
        // MAGIC NUMBER!!
        throttle += joystick.dY * 0.06;
        if ((int) throttle > 100) {
            NDEBUGPRINT("Throttle is too hight, forcing it to max");DEBUGNL();
            throttle = 100;
        }
        else if ((int) throttle < 0) {
            NDEBUGPRINT("Throttle is too low, forcing it to min");DEBUGNL();
            throttle = 0;
        }

        setThrottle(throttle);
        t1 = millis();
    }

}
