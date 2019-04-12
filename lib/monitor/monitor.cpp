#include "monitor.h"

Monitor::Monitor()
{
}

void Monitor::init(int Dpf,int Volume)
{
    dpf = Dpf;
    totalVolume = Volume;
    proRate=0;
    infusedVolume =0;
}

Monitor::~Monitor()
{
}

int Monitor::rateCalc(int rate){ 
    //process rate 
    return proRate;
}

int Monitor::volumeCalc(int drops){
    infusedVolume =  (drops/dpf);
    return infusedVolume; 
}