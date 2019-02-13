int distance;
int triggerPin = 18;
int echoPin = 2;

void setup() 
{
Serial.begin(9600); // 직렬 통신 초기화
pinMode(triggerPin, OUTPUT); // trigger 핀 출력으로 설정
pinMode(echoPin, INPUT); // echo 핀 입력으로 설정
attachInterrupt(0, TEST_ISR, RISING);
}

void loop()
{
// trigger 핀으로 10㎲의 펄스를 발생
//digitalWrite(triggerPin, HIGH);
//delayMicroseconds(10);
//digitalWrite(triggerPin, LOW);
//// echo 핀 입력으로부터 거리를 cm 단위로 계산
//distance = pulseIn(echoPin, HIGH) / 58;
//Serial.println("Distance(cm) = " + String(distance));
//delay(1000);
}
void TEST_ISR(void)
{
  Serial.println("Inrurrupt!");
}
