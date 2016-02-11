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

    Color(char r=0, char g=0, char b=0);
    void randColor();
    double getRed();
    double getGreen();
    double getBlue();
};

#endif // COLOR_H
