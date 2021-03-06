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
    x = fmap(x, 0, 1023, 0, 200);

    float y = analogRead(yPin);
    y = fmap(y, 0, 1023, 0, 200);

    dX = -(x - 100);
    dY = -(y - 100);

    // Setting a game for the input
    dX = (dX >= -1.5 && dX <= 1.5) ? 0 : dX;
    dY = (dY >= -1.5 && dY <= 1.5) ? 0 : dY;

}

#endif
