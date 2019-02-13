#include "pitches.h"

#define CUSTOMCODE 1
#define DATACODE 2
#define REVERSE_CHECK 3

/*global value define*/
int SpeakerPin=6, RemotePin=2, g_MelodyCount=0, g_RemoteCount=0, g_CutMelody=325;
int g_Melody[] = { // 학교종 멜로디.0은 쉼표
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0,
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, 0
};
unsigned int g_RemoteNum[]=
{
    0x6a, 0x42, 0xc2, 0xf0, 0x78, 0xb8, 0x38, 0x18, 0xa2, 0x32
};

unsigned short g_CustomCode, g_DataCode;
unsigned long g_prevTime=0, g_curTime=0, g_RemoteStartTime=0, g_RemoteEndTime=0, g_RemoteData;


/*function Prototype define*/
void ToneInit(int Pin);
void Tone(int Pin, int Frequency,int Time);
void ISR_ReadRemote(void);

unsigned long DecodeRemote(unsigned long RemoteData, unsigned long StartTime, unsigned long EndTime);
unsigned short MaskRemote(unsigned long RemoteData, int MaskType, int ChkError);

void setup() { 
  // put your setup code here, to run once:
ToneInit(6);
Serial.begin(9600);
attachInterrupt(0, ISR_ReadRemote, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:  
    g_CustomCode=MaskRemote(g_RemoteData, CUSTOMCODE, REVERSE_CHECK);
    g_DataCode= MaskRemote(g_RemoteData, DATACODE, 0);
    
    g_curTime=millis();
    
    if(g_RemoteCount>=36)
    {
        g_RemoteCount=0;
    }
    
    if(g_MelodyCount>=28)
    {
        g_MelodyCount=0;
    }
    
    //Serial.println(g_RemoteData);
    
    if(g_DataCode== g_RemoteNum[0])
    {
    if(g_curTime-g_prevTime >=g_CutMelody)
    {
        g_prevTime=g_curTime;
        Tone(SpeakerPin, g_Melody[g_MelodyCount], 250);
        g_MelodyCount++;
    }
    }
    
}//end of loop

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

void ISR_ReadRemote(void)
{    
    if(g_RemoteCount==0)
    {
        g_RemoteStartTime=micros();
        g_RemoteCount++;
    }
    else
    {
        g_RemoteEndTime =micros();
        g_RemoteData= DecodeRemote(g_RemoteData, g_RemoteStartTime, g_RemoteEndTime);
        
        g_RemoteStartTime=g_RemoteEndTime;
        g_RemoteCount++;
    }
}

unsigned long DecodeRemote(unsigned long RemoteData, unsigned long StartTime, unsigned long EndTime)
{
    unsigned long Decode=RemoteData;
    
    /*Lead Code(RemoteData clear) */
    if(EndTime-StartTime>=13000 && EndTime-StartTime<=14000)
    {
        return 0;
    }

    /*bit is 1*/
    else if(EndTime-StartTime>=2000 && EndTime-StartTime<=2500)
    {
        Decode=Decode<<1;
        Decode|=0x00000001;
        
    }

    /*bit is 0*/
    else if(EndTime-StartTime>=1000 && EndTime-StartTime<=1250)
    {
        Decode=Decode<<1;
    }
    
    
    return Decode;
}

unsigned short MaskRemote(unsigned long RemoteData, int MaskType, int ChkError)
{
    unsigned short Reverse, Mask;
    
    if(MaskType==CUSTOMCODE)
    {
        Mask=(RemoteData&0xff000000);
        Reverse=(RemoteData&0x00ff0000);
    }
    
    else if(MaskType==DATACODE)
    {
        Mask=(RemoteData&0x0000ff00);
        Reverse=(RemoteData&0x000000ff);
    }
    
    if(ChkError==REVERSE_CHECK)
    {
        if(Mask&Reverse!=0)
        {
            Mask=0;
        }
    }
    
    Mask=Mask>>8;
    return Mask;
}
