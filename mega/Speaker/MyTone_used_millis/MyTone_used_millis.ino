#include "pitches.h"

/*global value define*/
int SpeakerPin=17, g_cnt=0;

int g_Melody[] = { // 학교종 멜로디. 0은 쉼표
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0,
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, 0
};

unsigned long g_prevTime=0, g_curTime=0, g_SoundTime=0, g_CutMelody=325, g_melodyTime=250;
int g_soundState=0;



void setup() { 
  // put your setup code h, ere, to run once:
  pinMode(17, OUTPUT);
//ToneInit(1i7);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:  
    g_curTime=micros();
    
    if(g_cnt>=28)
    {
       g_cnt=0;
    }
//        if(g_curTime-g_SoundTime>=10)
//        {
//           
//            g_SoundTime=g_curTime;
//            g_soundState^=1;
//       
//       if(g_soundState)
//       digitalWrite(SpeakerPin, 1);
//
//       else
//       digitalWrite(SpeakerPin, 0);
//        }

     if(g_curTime-g_prevTime <=(g_melodyTime)*1000)
    {
        Tone(SpeakerPin, g_Melody[g_cnt], g_curTime, &g_SoundTime, &g_soundState);
    }
    else
    {
        if(g_curTime-g_prevTime <=(g_CutMelody)*1000)
        {
          MuteTone(SpeakerPin);
          //digitalWrite(SpeakerPin, 0);
        }
        else
        {
        g_prevTime=g_curTime;
        g_cnt++;
        }
    }
}



/*
    if(g_curTime-g_prevTime <=(g_melodyTime)*1000)
    {
        g_cycle=ConvFrequencyToCycle(g_Melody[g_cnt]);
        
        if(g_curTime-g_SoundTime>= g_cycle/2)// Frequency to cycle
        {
            g_SoundTime=g_curTime;
            g_soundState^=1;// toggle(50% Duty)
            
            if(g_soundState)
            {
            digitalWrite(SpeakerPin, 1);
            }
            else
            {
            digitalWrite(SpeakerPin, 0);
            }
        }
        
    }
    else
    {
        if(g_curTime-g_prevTime <=(g_CutMelody)*1000)
        {
          digitalWrite(SpeakerPin, 0);
        }
        else
        {
        g_prevTime=g_curTime;
        g_cnt++;
        }
    }
*/
