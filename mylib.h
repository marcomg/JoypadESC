#ifndef MYLIBH
#define MYLIBH

class Joystick {
    public:
        int xPin;
        int yPin;
        int dX;
        int dY;

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
    int x = analogRead(xPin);
    x = map(x, 0, 1023, 0, 100);

    int y = analogRead(xPin);
    y = map(x, 0, 1023, 0, 100);

    dX = x - 50;
    dY = y - 50;

}

#endif
