#include "menu.h"

Menu::Menu(Oled gOled){
}
//default constructor
Menu::Menu(){}

//deconstructor
Menu::~Menu(){}

//function to initialize menu
void Menu::init(bool backEnable,char title[],char items[][10],int n){
    title = title;
    if(backEnable == true){
        menuItems[0] = "<<back";
        number = n+1;
        state=1;
        int i=0;
	    for(i=0;i<n;i++)
	    {
            menuItems[i+1]=items[i];
	    }
    }
    else{
        number = n;
        state=0;
        int i=0;
	    for(i=0;i<n;i++)
	    {
            menuItems[i]=items[i];
	    } 
    }   
}
void Menu::displayMenu(Oled gOled,int n,char title[20]){
    int pos=0,last_item = n-1,first =0,last=4;
    if (last_item < 4)
    {
        last = last_item;
    }
    if (state > 2 && state < (last_item - 1) && last_item > 4)
    {
        first = state - 2;
        last = state + 2;
    }
    if ( state >= (last_item - 1) && last_item > 4)
    {
        first = last_item - 4;
        last = last_item;
    }
    int titleLength =sizeof(title);
    gOled.displayText(0,false,(22-(titleLength/2)),2,title);
    int strLength =0;
    for (int i = first, pos = 20; i <= last; i++)
    {
        strLength = sizeof(menuItems[i]);
        gOled.displayText(0,false,(22-(strLength/2)),(pos),menuItems[i]);
        pos = pos + 22;
    }

    //gOled.drawBox(0,state*20,64,20);
     if (state == 0)
  {
    gOled.drawBox(0, 13, 64, 22);
  }

  else if (state == 1)
  {
    gOled.drawBox(0, 35, 64, 22);
  }
  else if (state == 2 && last_item < 4)
  {
    gOled.drawBox(0, 57, 64, 22);
  }
  else if (state == 3 && last_item < 4)
  {
    gOled.drawBox(0, 79, 64, 22);
  }


  else if (state == last_item - 1)
  {
    gOled.drawBox(0, 79, 64, 22);
  }

  else if (state == last_item)
  {
    gOled.drawBox(0, 101, 64, 22);
  }

  else {
    gOled.drawBox(0, 57, 64, 22);
  }


}

char* Menu::getSelect(){
    return menuItems[state];
}
void Menu::up(){
    if(state <(number-1)){
        state++;
        printf("%d\n",state);

    }
}

void Menu::down(){
    if(state >0){
        state--;
        printf("%d\n",state);
    }
}