/*
 * 
 * ServoControl부분에서 ServoMove함수 개선 및 수정 필요(각도 안맞음, 모터 진동 발생)
 * 
 * 
 * 
 * 
*/

#define CUSTOMCODE 1
#define DATACODE 2
#define REVERSE_CHECK 3

#include "pitches.h"

/*global value define*/
unsigned long g_prevTime=0, g_curTime=0;


int g_Melody[] = { // 학교종 멜로디.0은 쉼표
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0,
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, 0
};
unsigned long SpeakerPin=6, g_MelodyCount=0, g_CutMelody=325;
unsigned long g_SoundTime_us=0;


unsigned int g_RemoteNum[]=
{
    0x6a, 0x42, 0xc2, 0xf0, 0x78, 0xb8, 0x38, 0x18, 0xa2, 0x32
};
int RemotePin=2, g_RemoteCount=0;
unsigned short g_CustomCode, g_DataCode;
unsigned long g_RemoteStartTime=0, g_RemoteEndTime=0, g_RemoteData;


int ServoPin=8, g_ServoAngle=0;
unsigned long g_ServoTime=0;

/*function Prototype define*/
void ISR_ReadRemote(void);

void setup() { 
  // put your setup code here, to run once:
    ToneInit(SpeakerPin);
    ServoInit(ServoPin);
    
    Serial.begin(9600);
    attachInterrupt(0, ISR_ReadRemote, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:  
    g_CustomCode=MaskRemote(g_RemoteData, CUSTOMCODE, REVERSE_CHECK);
    g_DataCode= MaskRemote(g_RemoteData, DATACODE, 0);
    
    g_curTime=micros();
    
    ServoMove(ServoPin, , g_curTime, &g_ServoTime);
    //Serial.println(g_ServoAngle);
    
    if(g_RemoteCount>=36)
    {
        g_RemoteCount=0;
    }
    
    if(g_MelodyCount>=28)
    {
        g_MelodyCount=0;
    }

    if(g_ServoAngle>=160)
    {
        g_ServoAngle=0;
    }
       
    if(g_curTime-g_prevTime <=(g_CutMelody)*1000 )
    {   
        Tone(SpeakerPin, g_Melody[g_MelodyCount], 250, g_curTime, &g_SoundTime_us);
    }
    else
    {
        g_prevTime=g_curTime;
        g_MelodyCount++;
        g_ServoAngle++;
    }
    //PWM(int Pin, unsigned long Cycle, double Duty, unsigned long Time)
    
}//end of loop


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


