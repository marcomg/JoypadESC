#ifndef MYLIBH
#define MYLIBH

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

class Joystick {
    public:
        int xPin;
        int yPin;
        float dX;
        float dY;

        Joystick(int x, int y);

        void loop();
};

Joystick::Joystick(int x, int y) {
    xPin = x;
    yPin = y;
    dX = 0;
    dY = 0;
}

void Joystick::loop() {
    float x = analogRead(xPin);
    x = fmap(x, 0, 1023, 0, 100);

    float y = analogRead(xPin);
    y = fmap(x, 0, 1023, 0, 100);

    dX = x - 50;
    dY = y - 50;

}

#endif
