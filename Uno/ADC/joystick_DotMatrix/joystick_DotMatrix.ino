#define COLUMN_BASE

#define POWER_ON 1
#define POWER_OFF 0
#define ROW 8
#define COL 8

#define SHIFT_LEFT 2
#define SHIFT_RIGHT 3
#define SHIFT_UP 4
#define SHIFT_DOWN 5

#define SHIFT_LOOP 6
#define SHIFT_NON_LOOP 7


int g_joyStick_X=A4, g_joyStick_Y=A5;
int g_JoystickVal_X, g_JoystickVal_Y;

int g_Matrix_Row[ROW]={A3, A2, A1, A0, 10, 11, 12, 13 };
int g_Matrix_Col[COL]={1, 3, 4, 5, 6, 7, 8, 9};

unsigned char g_font[ROW]={0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00};
unsigned char g_ShiftFont[ROW];

unsigned long g_curTime=0, g_prevTime=0;
unsigned long g_MicroCount=0, g_MillisCount=0, g_FontCount=0;

void setup() {
  // put your setup code here, to run once:
   attachInterrupt(0, GotoCenter_ISR, FALLING);
  
  InitMatrix(g_Matrix_Row, g_Matrix_Col, COL);
   CloneFont(COL, g_font, g_ShiftFont);
}

void loop() {
  // put your main code here, to run repeatedly:
  g_curTime=micros();

   if(g_curTime-g_prevTime>=2000)
  {
      g_prevTime=g_curTime;
      g_MicroCount++;
      g_MicroCount%=8;
      
      DisplayFont(g_Matrix_Row, g_Matrix_Col, COL, g_ShiftFont, g_MicroCount);
  }

  if(g_curTime-g_MillisCount>=200000)
  {
      g_MillisCount=g_curTime;
      
      g_JoystickVal_X=analogRead(g_joyStick_X);
      g_JoystickVal_Y=analogRead(g_joyStick_Y);

      //Serial.println(g_JoystickVal_Y);
      if(g_JoystickVal_X>=1000)
      {
          ShiftFont(COL, g_ShiftFont, g_ShiftFont, SHIFT_RIGHT, SHIFT_LOOP);
      }
      else if(g_JoystickVal_X<=10)
      {
          ShiftFont(COL, g_ShiftFont, g_ShiftFont, SHIFT_LEFT, SHIFT_LOOP);
      }

      if(g_JoystickVal_Y>=1000)
      {
          ShiftFont(COL, g_ShiftFont, g_ShiftFont, SHIFT_DOWN, SHIFT_LOOP);
      }
      else if(g_JoystickVal_Y<=10)
      {
          ShiftFont(COL, g_ShiftFont, g_ShiftFont, SHIFT_UP, SHIFT_LOOP);
      }
  }
  
}

void GotoCenter_ISR(void)
{ 
    int i;
    for(i=0; i<ROW; i++)
    {
    g_ShiftFont[i]=g_font[i];
    }
}
