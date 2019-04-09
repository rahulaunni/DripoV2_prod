#ifndef OLED_H
#define OLED_H
#include "Adafruit_SSD1306.h"
#include "string"
class Oled
{
    private:
        /* data */
    public:
        Oled();
        ~Oled();
        void init();
        void displayText(int,bool,int,int,char *);
        void drawBox(int,int,int,int);
        void eraseBox(int,int,int,int);
        void display();
        void clear();

};
#endif