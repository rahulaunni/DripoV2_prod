#include "oled.h"
//initialization of display driver
// an SPI sub-class that provides a constructed default
class SPIPreInit : public SPI
{
public:
    SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk)
    {
        format(8,3);
        frequency(2000000);
    };
};
SPIPreInit gSpi(PA_7,NC,PA_5);
Adafruit_SSD1306_Spi gOled(gSpi,PA_6,PA_10,PA_4,64,128);

//constructor
Oled::Oled(){    
}
void Oled::init(){
    gOled.setRotation(1);
    gOled.splash();
    wait(2.0);
    gOled.clearDisplay();
}
//To display a text
void Oled::displayText(int fontSize,bool clearDisplay,int x,int y,char *data){
    if(clearDisplay == true){
      //  gOled.clearDisplay();
        gOled.setTextSize(fontSize);
        gOled.setTextCursor(x,y);
        gOled.printf("%s\n",data);
      //  gOled.display();
    }
    else{
       gOled.setTextSize(fontSize);
       gOled.setTextCursor(x,y);
       gOled.printf("%s\n",data);
      // gOled.display(); 
    }
    
}
void Oled::drawBox(int x,int y,int w,int h){
    //gOled.clearDisplay();
    gOled.drawRect(x,y,w,h,WHITE);
   // gOled.display();
}

void Oled::eraseBox(int x,int y,int w,int h){
    gOled.drawRect(x,y,w,h,BLACK);
  //  gOled.display(); 
}
void Oled::display(){
    gOled.display();
}
void Oled::clear(){
    gOled.clearDisplay();
}
//destructor function
Oled::~Oled(){}