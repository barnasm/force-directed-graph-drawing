#ifndef COLOR_H
#define COLOR_H
#include <stdlib.h>

class Color
{
private:
    unsigned char red;
    unsigned char green;
    unsigned char blue;

public:

    Color(char r, char g, char b);
    Color(const Color &c);
    Color();

    void randColor();
    double getRed();
    double getGreen();
    double getBlue();
};

#endif // COLOR_H
