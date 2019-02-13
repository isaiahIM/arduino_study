#include "pitches.h"

int count=0;
int prv_time=0;
int cur_time=0;
void setup() 
{
  Serial.begin(9600);
}
void loop() 
{
    cur_time = millis();  
 
    // 음표와 음표 사이의 시간 간격 설정,음표 길이에 30%를 더해서 재생
    if (cur_time - prv_time > 325)
    {
        prv_time = cur_time;

  if(count==0)    
          tone(6, NOTE_G4, 1000/4); // 멜로디 재생 //(1000/4) 음표 길이를 시간으로 변환
  else if(count==1)    
          tone(6, NOTE_G4, 1000/4); // 멜로디 재생 //(1000/4) 음표 길이를 시간으로 변환
  else if(count==2)    
          tone(6, NOTE_A4, 1000/4); // 멜로디 재생 //(1000/4) 음표 길이를 시간으로 변환
  else if(count==3)    
          tone(6, NOTE_A4, 1000/4); // 멜로디 재생 //(1000/4) 음표 길이를 시간으로 변환
  count++;
    }
}
