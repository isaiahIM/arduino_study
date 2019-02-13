#include "pitches.h"

/*global value define*/
int SpeakerPin=6, g_cnt=0, g_CutMelody=325;
int g_Melody[] = { // 학교종 멜로디. 0은 쉼표
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0,
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, 0
};

unsigned long g_prevTime=0, g_curTime=0;


/*function Prototype define*/
void ToneInit(int Pin);
void Tone(int Pin, int Frequency, int Time);

void setup() { 
  // put your setup code here, to run once:
ToneInit(6);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:  
    g_curTime=millis();
    if(g_cnt>=28)
    {
    g_cnt=0;
    }
    
    if(g_curTime-g_prevTime >=g_CutMelody)
    {
    g_prevTime=g_curTime;
    Tone(SpeakerPin, g_Melody[g_cnt], 250);
   g_cnt++;
    }
}

void ToneInit(int Pin)
{
    pinMode(Pin, OUTPUT);
}

void Tone(int Pin, int Frequency, int Time)
{
    unsigned long DelayTime, LoopStart=0, LoopEnd=0;
    
    DelayTime=(1000000/Frequency)/2;

    LoopStart=millis();
    LoopEnd=LoopStart;
    
    while( LoopEnd-LoopStart < (Time) )
    {
    digitalWrite(Pin, 1);
    delayMicroseconds(DelayTime);

    digitalWrite(Pin, 0);
    delayMicroseconds(DelayTime);
    LoopEnd=millis();
    //Serial.println(LoopEnd-LoopStart);
    }
    
    digitalWrite(Pin, 0);
}
