#ifndef OLED_H
#define OLED_H
#include "Adafruit_SSD1306.h"
#include "string"
class Oled
{
    private:
        int volume;
        int state;
    public:
        Oled();
        ~Oled();
        void init();
        void displayText(int,bool,int,int,char *);
        void drawBox(int,int,int,int);
        void eraseBox(int,int,int,int);
        void display();
        void clear();
        void setVolume();
        void up();
        void down();
        int getVolume();
        void monitor(int,int);
        void startStop();
        void blink();
        int getMonStatus();
        void infusedVolume(int);
        void block();

};
#endif