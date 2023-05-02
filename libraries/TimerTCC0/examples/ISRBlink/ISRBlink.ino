#include <TimerTCC0.h>


bool isLEDOn = false;
char time = 0;

void setup() 
{
    //SerialUSB.begin(115200);
    //while(!SerialUSB);
    
    pinMode(13, OUTPUT);    

    TimerTcc0.initialize(1000000);
    TimerTcc0.attachInterrupt(timerIsr);
}
 
void loop()
{
    /*
    time ++;
    if(time == 10)TimerTcc0.detachInterrupt();
    else if(time == 20)
    {
       TimerTcc0.attachInterrupt(timerIsr);
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