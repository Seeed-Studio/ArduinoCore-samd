#include <TimerTC3.h>


bool isLEDOn = false;
char time = 0;

void setup() 
{
    //SerialUSB.begin(115200);
    //while(!SerialUSB);
    
    pinMode(13, OUTPUT);    

    TimerTc3.initialize(1000000);
    TimerTc3.attachInterrupt(timerIsr);
}
 
void loop()
{
    /*
    time ++;
    if(time == 10)TimerTc3.detachInterrupt();
    else if(time == 20)
    {
       TimerTc3.attachInterrupt(timerIsr);
       time = 0;
    }
    delay(1000);
    */
}

void timerIsr()
{    
    digitalWrite(13, isLEDOn);
    isLEDOn = !isLEDOn;
}