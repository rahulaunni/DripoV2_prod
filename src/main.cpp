#include <mbed.h>
#include "Adafruit_SSD1306.h"
#include "string"
#include "lora.h"
#include "menu.h"
#include "oled.h"

#define STM32_UUID ((uint32_t *)0x1FFF7A10)

//variables
char topic[40]="dripo/";
char message[20]="I-A101-%d-125-77-%d";
char menu1Title[6] = "Beds";
char menu1Content[][10] = {"B101","B102","B103","B104","B105","B106","B107","B108","B109"};
int menu1Size=0;
uint8_t *rxMessage;
char *test;

int LastaState,aState,bState,rate=0,messageNumber=0,dropCount=0;
float lastDropTime=0,presentDropTime=0,presentTime=0;
bool sendMessageFlag=false,blockFlag = true;
int state = 0;
int uiState =0;

Timer timer;
Ticker ticker;
Lora lora;
Oled oled;
Menu menu(oled);



// class SPIPreInit : public SPI
// {
// public:
//     SPIPreInit(PinName mosi, PinName miso, PinName clk) : SPI(mosi,miso,clk)
//     {
//         format(8,3);
//         frequency(2000000);
//     };
// };
// SPIPreInit gSpi(PA_7,NC,PA_5);
// Adafruit_SSD1306_Spi gOled1(gSpi,PA_6,PA_10,PA_4,64,128);


InterruptIn sw(PB_2);
InterruptIn swA(PB_1);
InterruptIn swB(PB_0);
InterruptIn drop(PA_12);


uint32_t idPart1 = STM32_UUID[0];
uint32_t idPart2 = STM32_UUID[1];
uint32_t idPart3 = STM32_UUID[2];


void listBed(){
    menu.displayMenu(oled,menu1Size,menu1Title);
    menu.selector(oled);
//     gOled1.clearDisplay();
//     gOled1.setTextCursor(0,0);
//    for(int i = 0; i < 3; i++)
//     {
//         gOled1.printf("%s\n", bed_arr + i);
//     }
//     gOled1.display();
//     wait_ms(1000);
//     uiState = 1;
 }


void listDpf(){
//     gOled1.clearDisplay();
//     gOled1.setTextCursor(0,0);
//     gOled1.printf("DPF\n");
//    for(int j = 0; j < 3; j++)
//     {
//         gOled1.printf("\n");
//         gOled1.printf("%d\n", dpf_arr[j]);
//     }
//     gOled1.display();

}
void block(){
    // gOled1.clearDisplay();
    // gOled1.setTextCursor(0,50);
    // gOled1.setTextSize(2);
    // gOled1.printf("BLOCK");
    // gOled1.display();
}
void encoder_handler(void) {
   aState =swA.read();
   bState =swB.read();

    if(aState != LastaState){
        if(bState != aState){
            printf("ccw\n\r");
            menu.up(oled);

        }
        else{
            printf("cw\n\r");
            menu.down(oled);
        }
    }
    LastaState = aState;
}
void button_handler(void) {
    printf("switch %p\r\n");
    // Toggle LED
    //led1 = !led1;
}
void drop_handler(void) {
    presentDropTime = timer.read_ms();
    if((presentDropTime-lastDropTime)>200){
        rate = (60000/(presentDropTime - lastDropTime));
        lastDropTime = presentDropTime;
        dropCount=dropCount+1;
        blockFlag = true;
        // gOled1.clearDisplay();
        // gOled1.setTextCursor(5,50);
        // gOled1.setTextSize(3);
        // gOled1.printf("%d",rate);
        // gOled1.setTextCursor(25,80);
        // gOled1.setTextSize(1);
        // gOled1.printf("DPM");
        // gOled1.display();
        printf("drop count %d\n\r",dropCount);
        if(dropCount % 10 == 0){
            sendMessageFlag = true;
            sprintf(topic,"dripo/D-%d",idPart3);
            sprintf(message," I-A101-%d-125-77-%d",rate,messageNumber);
            sendMessageFlag = lora.send(sendMessageFlag,topic,message);
        }
    }
}
void block_check(){
    presentTime = timer.read_ms();
    if((presentTime - lastDropTime)>5000 && blockFlag==true){
        sendMessageFlag = true;
        blockFlag = false;
        sprintf(topic,"dripo/D-%d",idPart3);
        sprintf(message," B-A101-%d-125-77-%d",rate,messageNumber);
        sendMessageFlag = lora.send(sendMessageFlag,topic,message);
    }
}

void (* stateFun[10])() = {encoder_handler};
void(* uiFun[10])()={listBed,listDpf,block};

int main( void ) 
{
    lora.init();
    oled.init();
    menu1Size = (sizeof(menu1Content)/10);
    menu.init(oled,false,menu1Title,menu1Content,menu1Size);
    //menu.init(oled,menu1Title,menu1Content,7);
    //menu.displayMenu(oled,7);
    // test=menu.getSelect();
    // printf("%s\n",test);
    //menu.selector(oled);
    // gOled1.setRotation(1);
    // gOled1.splash();
    //wait(2.0);
    
    // gOled1.clearDisplay();
    // gOled1.setTextCursor(15,50);
    // gOled1.setTextSize(1.5);
    // gOled1.printf("RATE %d",rate);
    // gOled1.display();

    // oled.init(0, 8000000);
    // oled.clear(ALL);
    // oled.setFontType(1);
    // oled.setCursor(0,10);
    // oled.printf("Hello all! \n How are you?");
    // oled.display();

    timer.start();
    //t.start(callback(&queue, &EventQueue::dispatch_forever));
    
    sw.mode(PullUp);
    //sw.fall(queue.event(button_handler));

    drop.mode(PullUp);
    drop.rise(&drop_handler);

    swA.mode(PullUp);
    LastaState = swA.read();
    swA.rise(&encoder_handler);

    swB.mode(PullUp);
    swB.rise(&encoder_handler);

    ticker.attach(&block_check, 6.0);


   while(1){
        oled.clear();
         uiFun[uiState]();
         oled.display(); 
    //sendMessageFlag = lora.send(sendMessageFlag,topic,message);
       //Lora send 
    //   lora.send(topic,message);
    //   wait(3.0);
        //LoRa rexieve
    //    lora.rxMode(3500);
    //    wait(3.0);
    //    rxMessage=lora.getMessage();
    //    printf("recieved message rxMessage -- %s\n\r",rxMessage);
   }
    

}

