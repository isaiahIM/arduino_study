#define LED_KIND 8

#define PULL_UP 1
#define PULL_DOWN 2
#define BTN_PUSH 1
#define BTN_PULL 2

int Delay= 50, Btn=2, i, now=0, prev=2, Light;
int led[LED_KIND]={13, 12, 11, 10, 9, 8, 7, 6};

void setup() {
  // put your setup code here, to run once
  for(i=0; i<LED_KIND; i++)
  {
      pinMode(led[i], OUTPUT);
  }
Serial.begin(9600);
  pinMode(Btn, INPUT);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  now=AntiFloatingRead(Btn, prev, PULL_DOWN);

  if(now!=-1)
  {
      prev=now;
  }
  Light= analogRead(A0);
  Serial.println(Light);
  
  if(now==BTN_PUSH || Light<=50)
  {
      for(i=0; i<LED_KIND; i++)
      {
          digitalWrite(led[i], 1);
          delay(Delay);
      }
  }
  else if(now==BTN_PULL || Light>50)
  {
      for(i=LED_KIND-1; i>=0; i--)
      {
          digitalWrite(led[i], 0);
          delay(Delay);
      }
  }
}

int AntiFloatingRead(int Port, int BeforeState, int Type)
{
    int now;
    
    now= digitalRead(Port);
    delay(5);
    //Serial.println(now);
    /*Pull up type button*/
    if(Type==PULL_UP)
    {
        if(now==0 && BeforeState==2)
        {
            return BTN_PUSH;  
        }
        else if(now==1 && BeforeState==1)
        {
              return BTN_PULL;
        }
        else
        {
            return -1;  
        }
    }
    /*Pull down type button*/
    else
    {
        if(now==1 && BeforeState==2)
        {
            return BTN_PUSH;  
        }
        else if(now==0 && BeforeState==1)
        {
              return BTN_PULL;
        }
        else
        {
            return -1;  
        }
    }
}
