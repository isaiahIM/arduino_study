#define BTN_PUSH 1
#define BTN_POP 0
#define CHATERING_COUNT 2

int Btn=2, Led=5;// actuator
unsigned long Btn_Down_Time=0, Btn_Up_Time=0, ClickTime;// Time
int BtnState=0, PrevBtnState=0;// Button Stataus
volatile int ChateringCount=0;

void setup() {
  // put your setup code here, to run once:
pinMode(Btn, INPUT);
pinMode(Led, OUTPUT);

Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    
    BtnState=digitalRead(Btn);

   
    if(BtnState!=PrevBtnState)
    {          
          if(ChateringCount==0)
          {
              ChateringCount=CHATERING_COUNT;
          }
          else
          {
              ChateringCount--;
              
              if(ChateringCount==0)
              {
                  PrevBtnState=BtnState;
                  if(BtnState==BTN_PUSH)
                  {
                      
                      Btn_Down_Time=millis();
                      if(Btn_Down_Time-Btn_Up_Time>10)
                      {
                          digitalWrite(Led, 1);
                      }
                      
                  }
        
                  else if(BtnState==BTN_POP)
                  {
                      Btn_Up_Time=millis();
                      ClickTime= Btn_Up_Time - Btn_Down_Time;
                      
                      /*Anti Chattering*/
                      if(ClickTime>10)
                      {
                      digitalWrite(Led, 0);
                      Serial.println(ClickTime);
                      Serial.println();
                      }
                  }  
              }
          }
          
          
    }
    
}
