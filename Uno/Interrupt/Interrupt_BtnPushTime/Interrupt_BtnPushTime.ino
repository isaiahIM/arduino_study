#define BTN_PUSH 1
#define BTN_PULL 0

int Button=2, BtnState=0;
unsigned long Btn_Push, Btn_Pull;

void setup() {
  // put your setup code here, to run once:
attachInterrupt(0, BtnClick_ISR, CHANGE);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void BtnClick_ISR(void)
{
    BtnState=digitalRead(Button);
    
    if(BtnState==BTN_PUSH)
    {
       Btn_Push= millis();
    }
    
    else if(BtnState==BTN_PULL)
    {
        Btn_Pull=millis();

        /*Chattering Filter*/
        if(Btn_Pull-Btn_Push>=10)
        {
            Serial.println(Btn_Pull-Btn_Push);
        }
    }
//EIFR &=~(1<<2);
    
}

