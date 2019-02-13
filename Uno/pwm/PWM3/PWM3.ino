int ledPin = 6, Cnt=0; // LED 연결 핀
int ledState = LOW; // LED는 끈 상태로 시작
long previousMicros = 0, previousMillis=0; // 마지막으로 LED 상태를 바꾼 시간
int val=0;
int flag=2;

void setup() {
    pinMode(ledPin, OUTPUT); // LED 연결 핀을 출력으로 설정
}

void loop()
{
  unsigned long currentMillis = millis(); // 현재까지의 시간
      unsigned long currentMicros = micros(); // 현재까지의 시간

  if(currentMicros - previousMicros > 20) { // 지정한 시간을 초과한 경우
    Cnt+=2;
    previousMicros = currentMicros; // 마지막 상태 변화 시간 저장
    if(Cnt==val)
    {
        digitalWrite(ledPin, 0);
    }
    if(Cnt==100)
    {
        digitalWrite(ledPin, 1); 
        Cnt=0;
    }
  }

if(currentMillis-previousMillis >20)
{
    previousMillis=currentMillis;
    val+=flag;
    if(val==100)
    {
        flag=-2;
    }
    
    else if(val==0)
    {
        flag=2;  
    }
}
  
}

