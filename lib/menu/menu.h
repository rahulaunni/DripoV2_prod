#ifndef MENU_H
#define MENU_H
#include "string"
#include "oled.h"

class Menu
{
  private:
    int strwidth;
    // std::string menuItems[50];
    char * menuItems[50];
    char title[20];
    int state;
    int number;
    int last_item;
  public:
    explicit Menu(Oled gOled);
    Menu();
    ~Menu();
    void init(Oled gOled,bool,char[],char [][10],int);
    void displayMenu(Oled gOled,int,char[]);
    void selector(Oled gOled);
    void up(Oled gOled);
    void down(Oled gOled);
    char * getSelect();
};

#endif
