int pushButton =2;
int buttonStatePrevious = LOW;
int buttonStateCurrent = LOW; // 현재 버튼 상태
void setup()
{
Serial.begin(9600); // 시리얼 통신 초기화
pinMode(pushButton, INPUT); // 버튼 연결 핀을 입력으로 설정
}
void loop()
{
buttonStateCurrent = digitalRead(pushButton); // 버튼 상태 읽기
if(buttonStateCurrent != buttonStatePrevious){ // 이전과 버튼 상태가 바뀐 경우
buttonStatePrevious = buttonStateCurrent; // 이전 상태 갱신
if(buttonStateCurrent == HIGH) // 버튼이 눌러진 경우
Serial.println("Button is on.");
else if(buttonStateCurrent == LOW)
Serial.println("Button is off.");
}
delay(10); // 안정적 입력을 위한 버튼 입력 사이의 시간 지연
}
