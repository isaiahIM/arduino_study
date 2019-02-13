int ledPin = 6, led2=7, led3=13, Cnt=0; // LED 연결 핀
int ledState = LOW; // LED는 끈 상태로 시작
long previousMicros = 0, previousMillis=0,  previousMillis2=0,  previousMillis3=0; // 마지막으로 LED 상태를 바꾼 시간
int val=0, val2=0, val3=0;
int flag=1, flag2=1, flag3=1;

void setup() {
    pinMode(ledPin, OUTPUT); // LED 연결 핀을 출력으로 설정
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
}

void loop()
{
  unsigned long currentMillis = millis(); // 현재까지의 시간
  unsigned long currentMicros = micros(); // 현재까지의 시간
  
  if(currentMicros - previousMicros > 20) { // 지정한 시간을 초과한 경우
    Cnt+=1;
    previousMicros = currentMicros; // 마지막 상태 변화 시간 저장
    
    if(Cnt==val)
    {
        digitalWrite(ledPin, 0);
    }
   
   if(Cnt==val2)
    {
        digitalWrite(led2, 0);
    }
    
    if(Cnt==val3)
    {
        digitalWrite(led3, 0);
    }

    
    if(Cnt==100)
    {
        if(val)
        {
            digitalWrite(ledPin, 1);  
        }
        
        if(val2)
        {
            digitalWrite(led2, 1);
        }
        
        if(val3)
        {
            digitalWrite(led3, 1); 
        }
        
        Cnt=0;
    }
  }

if(currentMillis-previousMillis >20)
{
    previousMillis=currentMillis;
    
    if(val==100)
    {
        flag=-1;
    }
    
    else if(val==0)
    {
        flag=1;  
    }
    val+=flag;
}


if(currentMillis-previousMillis2 >10)
{
    previousMillis2=currentMillis;
    
    if(val2==100)
    {
        flag2=-1;
    }
    
    else if(val2==0)
    {
        flag2=1;  
    }
    val2+=flag2;
}

if(currentMillis-previousMillis3 >30)
{
    previousMillis3=currentMillis;
    
    if(val3==100)
    {
        flag3=-1;
    }
    
    else if(val3==0)
    {
        flag3=1;  
    }
    val3+=flag3;
}

}

