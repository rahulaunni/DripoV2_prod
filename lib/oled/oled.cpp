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
  volume = 500;
  state=1; 
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
       gOled.setTextColor(WHITE);
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

void Oled::setVolume(){
  gOled.drawRect(0,0,64,20,WHITE);
  gOled.fillRect(0,0,64,20,WHITE);
  gOled.setTextCursor(15,5);
  gOled.setTextColor(BLACK);
  gOled.setTextSize(1);
  gOled.printf("VOLUME");
  gOled.setTextCursor(30,35);
  gOled.setTextColor(WHITE);
  gOled.printf("+");
  gOled.drawRect(0,44,64,30,WHITE);
  gOled.fillRect(0,44,64,30,WHITE);
  gOled.setTextCursor(13,54);
  gOled.setTextColor(BLACK);
  gOled.setTextSize(2);
  gOled.printf("%d",getVolume());
  gOled.setTextCursor(30,84);
  gOled.setTextColor(WHITE);
  gOled.setTextSize(0);
  gOled.printf("-");
  gOled.drawRect(0,108,64,20,WHITE);
  gOled.fillRect(0,108,64,20,WHITE);
  gOled.setTextCursor(25,113);
  gOled.setTextColor(BLACK);
  gOled.setTextSize(1);
  gOled.printf("OK");
}
void Oled::up(){
  volume = volume + 25;
  state=1;
}
void Oled::down(){
  volume = volume - 25;
  state=0;
}
int Oled::getVolume(){
  return volume;
}
void Oled::monitor(int rate,int rate2Set){
  gOled.setTextCursor(17,20);
  gOled.setTextColor(WHITE);
  gOled.setTextSize(2);
  gOled.printf("%d",rate);
  gOled.drawRoundRect(20,35,30,10,5,WHITE);
  gOled.fillRoundRect(20,35,30,10,5,WHITE);
  gOled.setTextColor(BLACK);
  gOled.setTextCursor(25,37);
  gOled.setTextSize(0);
  if(rate2Set==0){
    gOled.printf("---");
  }
  else{
    gOled.printf("%d",rate2Set);
  }
  gOled.drawRect(17,50,35,12,WHITE);
  gOled.fillRect(17,50,35,12,WHITE);
  gOled.setTextColor(BLACK);
  gOled.setTextCursor(22,52);
  gOled.setTextSize(0);
  gOled.printf("ml/hr");

}
void Oled::blink(){
  gOled.drawRect(17,50,35,12,BLACK);
  gOled.fillRect(17,50,35,12,BLACK);
}
void Oled::startStop(){
  gOled.drawRect(0,65,64,40,WHITE);
  if(state ==1){
    gOled.drawRect(0,65,64,20,WHITE);
    gOled.fillRect(0,65,64,20,WHITE);
    gOled.setTextColor(BLACK);
    gOled.setTextCursor(18,70);
    gOled.setTextSize(0);
    gOled.printf("start");
    gOled.setTextColor(WHITE);
    gOled.setTextCursor(22,90);
    gOled.setTextSize(0);
    gOled.printf("stop");
  }
  else{
    gOled.drawRect(0,85,64,20,WHITE);
    gOled.fillRect(0,85,64,20,WHITE);
    gOled.setTextColor(WHITE);
    gOled.setTextCursor(18,70);
    gOled.setTextSize(0);
    gOled.printf("start");
    gOled.setTextColor(BLACK);
    gOled.setTextCursor(22,90);
    gOled.setTextSize(0);
    gOled.printf("stop");
  }
}
int Oled::getMonStatus(){
  return state;
}
void Oled::infusedVolume(int volume){
  gOled.setTextColor(WHITE);
  gOled.setTextCursor(5,90);
  gOled.setTextSize(1);
  gOled.printf("%d",volume);
  gOled.setTextCursor(50,100);
  gOled.setTextSize(1);
  gOled.printf("ml");
}
void Oled::block(){
  gOled.drawRect(0,85,64,20,WHITE);
  gOled.fillRect(0,85,64,20,WHITE);
  gOled.setTextColor(BLACK);
  gOled.setTextCursor(18,90);
  gOled.setTextSize(0);
  gOled.printf("BLOCK");
}
//destructor function
Oled::~Oled(){}