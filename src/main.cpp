#include <mbed.h>
#include "Adafruit_SSD1306.h"
#include "string"
#include "lora.h"
#include "menu.h"
#include "oled.h"
#include "monitor.h"

#define STM32_UUID ((uint32_t *)0x1FFF7A10)

//variables
char topic[40]="dripo/";
char message[20]="I-%s-%d-%d-77-%d";
char menu1Title[6] = "Beds";
char menu1Content[][10] = {"B101","B102","B103","B104","B105","B106","B107","B108","B109"};
int menu1Size=0;
char menu2Title[4] = "DPF";
char menu2Content[3][10] = {"15","20","60"};
int menu2Size=0;
uint8_t *rxMessage;
char *menu1SelectedItem;
char *menu2SelectedItem;
int selectedDpf=0;
int selectedVolume=0;

int LastaState,aState,bState,rate2Set=0,rate=0,proRate=0,infusedVolume=0,messageNumber=0,dropCount=0,BTN=0;
float lastDropTime=0,presentDropTime=0,presentTime=0,presentPressTime=0,lastPressTime=0,pressStartTime=0,
        pressEndTime=0,presentRotateTime=0,lastRotateTime=0;
bool sendMessageFlag=false,blockFlag = true,longPressFlag=false;
int state = 0;
int uiState =0;
int buttonStatus=1;
int monStatus=0;

Timer timer;
Lora lora;
Oled oled;
Menu menu1(oled),menu2(oled);
Monitor mon;
Ticker ticker;



InterruptIn sw(PB_2);
InterruptIn swA(PB_1);
InterruptIn swB(PB_0);
InterruptIn drop(PA_12);


uint32_t idPart1 = STM32_UUID[0];
uint32_t idPart2 = STM32_UUID[1];
uint32_t idPart3 = STM32_UUID[2];

void block_check();
int get_button()
{
  int button = BTN;
  BTN = 0;
  return button;

}

void sendMessage(){
    if(monStatus == 1){
        sendMessageFlag = true;
        printf(topic,"dripo/D-%d",idPart3);
        sprintf(message," I-%s-%d-%d-77-%d",menu1SelectedItem,selectedVolume,rate,messageNumber);
        sendMessageFlag = lora.send(sendMessageFlag,topic,message);
        messageNumber++;
    }
}
bool longPress_handler(bool flag){
    if(flag == true && buttonStatus==0 && (timer.read_ms()-pressStartTime)>5000){
        BTN=1;
        printf("Long press\n");
        return false;
    }
    else if(flag == true && buttonStatus==0){
        return true;
    }
    else{
        return false;

    }
}

//ui functions and state functions
//#0
void listBed_ui(){
    menu1.displayMenu(oled,menu1Size,menu1Title);
 }
 void listBed_state(){
    switch(get_button()){
        case 1:
            //go to settings
            break;
        case 2:
            menu1SelectedItem = menu1.getSelect();
            printf("Selected: %s\n",menu1SelectedItem);
            uiState = 1;
            state=1;
            break;
        case 3:
            menu1.up();
            break;
        case 4:
            menu1.down();
            break;
    }
 }

//#1
void listDpf_ui(){
    menu2.displayMenu(oled,(menu2Size+1),menu2Title);
}
void listDpf_state(){
    switch(get_button()){
        case 1:
            break;
        case 2:
            menu2SelectedItem = menu2.getSelect();
            if(menu2SelectedItem == "<<back"){
                uiState=0;
                state=0;
            }
            else{
                selectedDpf = atoi(menu2SelectedItem);
                printf("Selected Dpf: %d\n",selectedDpf);
                uiState=2;
                state=2;
            }
            break;
        case 3:
            menu2.up();
            break;
        case 4:
            menu2.down();
            break;
    }
}

//#2

void setVolume_ui(){
    oled.setVolume();
}
void setVolume_state(){

     switch(get_button()){
        case 2:
            selectedVolume = oled.getVolume();
            uiState = 4;
            state =4;
            break;
        case 3:
            oled.up();
            break;
        case 4:
            oled.down();
            break;
    }
}
//#3
void calibration_ui(){

}
void calibration_state(){

}
//#4
void monitor_ui(){
    oled.monitor(rate,rate2Set);
    if(monStatus==0){
        oled.startStop();
    }
    else if(monStatus==1){
        infusedVolume = mon.volumeCalc(dropCount);
        oled.infusedVolume(infusedVolume);
    }
    else{
        oled.block();
    }
}
void monitor_state(){
    switch(get_button()){
        case 2:
            switch(monStatus){
                case 0:
                    monStatus = oled.getMonStatus();
                    rate2Set = rate;
                    printf("mon status: %d",monStatus);
                break;
                case 1:
                    monStatus = 0;
                break;
                case 2:
                    monStatus = 0;
                break;
            }
            break;
        case 3:
            oled.up();
            break;
        case 4:
            oled.down();
            break;
    }
}

void encoder_handler(void) {
   aState =swA.read();
   bState =swB.read();
    presentRotateTime = timer.read_ms();
    if(aState != LastaState && (presentRotateTime-lastRotateTime)>600){
        if(bState != aState){
            BTN=3;

        }
        else{
            BTN=4;
        }
        lastRotateTime = presentPressTime;
    }
    LastaState = aState;
}
void button_handler(void) {
    presentPressTime = timer.read_ms();
    // if((presentPressTime-pressStartTime)>5000){
    //     printf("long press \r\n");
    //     BTN = 1;
    // }
    if((presentPressTime-lastPressTime)>200 && (presentPressTime-pressStartTime)<5000){
        BTN = 2;
        lastPressTime = presentPressTime;
    }
    buttonStatus = sw.read();

}
void longpress_handler(void){
    pressStartTime = timer.read_ms();
    longPressFlag = true;
    buttonStatus = sw.read();
}
void drop_handler(void) {
    presentDropTime = timer.read_ms();
    if((presentDropTime-lastDropTime)>200){
        rate = (60000/(presentDropTime - lastDropTime));
        lastDropTime = presentDropTime;
        dropCount=dropCount+1;
        if(monStatus == 2){
            monStatus =1;
        }
        blockFlag = true;
        printf("drop count %d\n\r",dropCount);
        oled.blink();
        // if(dropCount % 10 == 0){
        //     sendMessageFlag = true;
        //     sprintf(topic,"dripo/D-%d",idPart3);
        //     sprintf(message," I-A101-%d-125-77-%d",rate,messageNumber);
        //     sendMessageFlag = lora.send(sendMessageFlag,topic,message);
        // }
    }
}
bool block_check(bool blockFlag){
    presentTime = timer.read_ms();
    if((presentTime - lastDropTime)>5000 && blockFlag==true && monStatus == 1 && messageNumber >1){
        sendMessageFlag = true;
        // blockFlag = false;
        monStatus = 2;
        sprintf(topic,"dripo/D-%d",idPart3);
        sprintf(message," B-%s-%d-%d-77-%d",menu1SelectedItem,selectedVolume,rate,messageNumber);
        sendMessageFlag = lora.send(sendMessageFlag,topic,message);
        messageNumber++;
        return false;
    }
    else{
        return true;
    }
}
void(* uiFun[10])()={listBed_ui,listDpf_ui,setVolume_ui,calibration_ui,monitor_ui};
void (* stateFun[10])() = {listBed_state,listDpf_state,setVolume_state,calibration_state,monitor_state};

int main( void ) 
{
    lora.init();
    oled.init();
    menu1Size = (sizeof(menu1Content)/10);
    menu1.init(false,menu1Title,menu1Content,menu1Size);
    menu2Size = (sizeof(menu2Content)/10);
    menu2.init(true,menu2Title,menu2Content,(menu2Size+1));
    mon.init(15,500);
    timer.start();
    ticker.attach(&sendMessage, 120.0);


    sw.mode(PullUp);
    sw.rise(callback(button_handler));
    sw.fall(callback(longpress_handler));

    drop.mode(PullUp);
    drop.rise(&drop_handler);

    swA.mode(PullUp);
    LastaState = swA.read();
    swA.rise(&encoder_handler);

    swB.mode(PullUp);
    swB.rise(&encoder_handler);



   while(1){
        oled.clear();
        stateFun[state]();
        uiFun[uiState]();
        longPressFlag = longPress_handler(longPressFlag);
        blockFlag = block_check(blockFlag);
        sendMessageFlag = lora.send(sendMessageFlag,topic,message);
        oled.display(); 
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

