#ifndef MONITOR_H
#define MONITOR_H
#include "string"
#include <stdio.h>


class Monitor
{
private:
    int proRate;
    int dpf;
    int totalVolume;
    int infusedVolume;
public:
    Monitor();
    ~Monitor();
    int rateCalc(int);
    int volumeCalc(int);
    void init(int,int);
};




#endif