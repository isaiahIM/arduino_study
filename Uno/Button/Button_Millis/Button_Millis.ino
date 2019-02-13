#define BTN_PUSH 1
#define BTN_POP 0

int Btn=2;// actuator
unsigned long Btn_Down_Time, Btn_Up_Time, ClickTime;// Time
int BtnState=0, PrevBtnState=0;// Button Stataus

void setup() {
  // put your setup code here, to run once:
pinMode(Btn, INPUT);

Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    
    BtnState=digitalRead(Btn);
    delay(5);
    
    if(BtnState!=PrevBtnState)
    {
          PrevBtnState=BtnState;
          
          if(BtnState==BTN_PUSH)
          {
              Btn_Down_Time=millis();
          }

          else if(BtnState==BTN_POP)
          {
              Btn_Up_Time=millis();
              
              ClickTime= Btn_Up_Time - Btn_Down_Time;
              Serial.println(ClickTime);
              Serial.println();
          }
    }
    
}
