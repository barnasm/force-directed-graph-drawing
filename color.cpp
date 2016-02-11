#include "color.h"

Color::Color(char r, char g, char b)
{
    red = r;
    green = g;
    blue = b;
}
void Color::randColor(){
    red = (rand() % 256);
    green = (rand() % 256);
    blue = (rand() % 256);
}

double Color::getRed(){
    return (double)red / 255.0;
}
double Color::getGreen(){
    return (double)green / 255.0;
}
double Color::getBlue(){
    return (double)blue / 255.0;
}
