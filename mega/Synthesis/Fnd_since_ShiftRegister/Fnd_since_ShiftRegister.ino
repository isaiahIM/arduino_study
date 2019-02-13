/*
 * MODE:
 * Remote mode: 
 * remote mode is shift Dotmatrix font by Remote.
 * if you push the UP button, font move up.
 * if you push the DOWN button, font move down.
 * if you Potentiometer rotate and CDS sensor hide from light, font move right or left.
 * if you joystick move, Servo move scale of how many joystick move.
 * 
 * 
 * JoyStick mode:
 * you can control font by joystick.
 * 
 * 
 * Standard Manual:
 * 1. basic setting is Remote mode.
 * 2. if you push the joystick button, mode is change(remote, joystick)
 * 3. FND always distance from Ultrasonic sensor to material.
 * 4. if you push without 0~9 or UP/DOWN button, speaker buzz.
 * 5. dot matrix font is control Key matrix. if you Key matrix click, Dot matrix show what kind of button you clicked.
*/

#include "pitches.h"

#define TRUE 1
#define FALSE 0

#define REMOTE_MODE 0
#define JOYSTICK_MODE 1

/*Key Matrix define*/
#define KEY_MATRIX_COLUMN_BASE 1
#define KEY_MATRIX_ROW_BASE 0

#define KEY_CLICK 0
#define KEY_NON_CLICK 1

#define KEY_POWER_ON 0
#define KEY_POWER_OFF 1
#define KEY_ROW 5
#define KEY_COL 4


/*Dot Matrix define*/
#define DOTMATRIX_POWER_ON 1
#define DOTMATRIX_POWER_OFF 0
#define ROW 8
#define COL 8

#define SHIFT_LEFT 2
#define SHIFT_RIGHT 3
#define SHIFT_UP 4
#define SHIFT_DOWN 5
#define NON_SHIFT 9

#define SHIFT_LOOP 6
#define SHIFT_NON_LOOP 7


/*Fnd define*/
#define FND_HIGH 0
#define FND_LOW 1
#define FND_POWER_ON 1
#define FND_POWER_OFF 0


/*Speaker define*/
#define MELODY_SIZE 28

/*Remote define*/
#define CUSTOM_CODE 1
#define DATA_CODE 2
#define REMOTE_PULSE_COUNT 38

/*ShiftRegister define*/
#define MSB_FIRST 1
#define LSB_FIRST 2

#define STOP_PWM 0
#define RUN_PWM 1


/*Key Matrix Value*/
const int g_KeyMatrix_row[KEY_ROW]={12, 10, 8, 6, 5};
const int g_KeyMatrix_col[KEY_COL]={13, 11, 9, 7};
int g_KeyData, g_KeyPrevData, g_KeyReadCol, g_KeyReadRow, KeyIntData=-1;
String g_ConvKeyData;
unsigned long KeyReadTime=0;


/*Dot Matrix Value*/
const int g_DotMatrix_Row[ROW]={48, 35, 44, 45, 37, 43, 38, 41};
const int g_DotMatrix_Col[COL]={36, 39, 40, 47, 42, 46, 34, 33};
unsigned char g_font[20][ROW]=
  {
  {0x00,0x3C,0x40,0x40,0x40,0x40,0x3C,0x00},// C
  {0x00,0x24,0x42,0x42,0x42,0x24,0x00,0x00},// ()  
  {0x00,0x32,0x34,0x08,0x10,0x2c,0x4c,0x00},// % 
  {0x00,0x00,0x04,0x08,0x10,0x20,0x40,0x00},// /
  
  {0x00,0x7C,0x04,0x08,0x10,0x20,0x20,0x20},// 7
  {0x00,0x38,0x44,0x44,0x38,0x44,0x44,0x38},// 8
  {0x00,0x38,0x44,0x44,0x3C,0x04,0x44,0x38},// 9
  {0x00,0x00,0x44,0x28,0x10,0x28,0x44,0x00},// x

  {0x00,0x08,0x18,0x28,0x48,0x7C,0x08,0x08},// 4
  {0x00,0x7C,0x40,0x78,0x04,0x04,0x44,0x38},// 5
  {0x00,0x38,0x40,0x40,0x78,0x44,0x44,0x38},// 6
  {0x00,0x00,0x00,0x7c,0x00,0x00,0x00,0x00},// -

  {0x00,0x10,0x30,0x50,0x10,0x10,0x10,0x7C},// 1
  {0x00,0x38,0x44,0x04,0x08,0x10,0x20,0x7c},// 2
  {0x00,0x38,0x44,0x04,0x18,0x04,0x44,0x38},// 3
  {0x00,0x10,0x10,0x7c,0x10,0x10,0x00,0x00},// + 

  {0x00,0x00,0x00,0x00,0x00,0x0c,0x0c,0x00},// .  
  {0x00,0x38,0x44,0x4C,0x54,0x64,0x44,0x38},// 0
  {0x20,0x72,0x24,0x08,0x10,0x27,0x40,0x00},// +/-
  {0x00,0x00,0x3c,0x00,0x00,0x3c,0x00,0x00}// =
  };
unsigned char g_ShiftFont[20][ROW];

unsigned long g_RowCount=0;
unsigned long g_ShiftState=1, g_ShiftState_old=1, g_ShiftCount=0, g_matrixShiftDir=SHIFT_RIGHT, g_matrixShiftFlag=-1;


/*Fnd Value*/
int g_FndPin[8]={30, 24, 25, 27, 26, 31, 28, 29};// a, b, c, d, e, f, g, dp
int g_Fnd[10][8]=
{
    {FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW, FND_LOW},// 0
    {FND_LOW, FND_HIGH, FND_HIGH, FND_LOW, FND_LOW, FND_LOW, FND_LOW, FND_LOW},// 1
    {FND_HIGH, FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_LOW, FND_HIGH, FND_LOW},// 2
    {FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW, FND_LOW, FND_HIGH, FND_LOW},// 3
    {FND_LOW, FND_HIGH, FND_HIGH, FND_LOW, FND_LOW, FND_HIGH, FND_HIGH, FND_LOW},// 4, 
    {FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_LOW},// 5
    {FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW},// 6
    {FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW, FND_LOW, FND_HIGH, FND_LOW, FND_LOW},// 7
    {FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH ,FND_HIGH, FND_HIGH, FND_LOW},// 8
    {FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_LOW}// 9
};
unsigned int g_FndNum=0, Digit;
int g_FndPower[2]={32, 23};
unsigned long g_fndTime=0;


/*Speaker Value*/
int g_speakerPin=17, g_melodyCount=0;
int g_Melody[MELODY_SIZE] = { // 학교종 멜로디. 0은 쉼표
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0,
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, 0
};
int g_soundState=0;
unsigned long g_CutMelody=325, g_melodyDelayTime_ms=250, g_soundChangeTime_us=0, g_melodyTime_us=0;


/*Potentiometer Value*/
int g_poten_pin=A5, g_poten_val;

/*CDS Value*/
static int CDS_pin=A6, CDS_val;

/*Joystick Value*/
const static int g_joystick_pin_X=A8, g_joystick_pin_Y=A9, g_joystick_Pin_Btn=A10;
static int g_joystick_val_X, g_joystick_val_Y;
static unsigned int g_joystick_val_Btn=0, g_joystick_val_prevBtn=0;
static unsigned long g_joystick_ReadTime=0;


/*UltlaSonic Value*/
static int g_Sonic_trig=18, g_Sonic_echo=2, g_Sonic_dist, g_SonicTrigState=0;
unsigned long g_SonicTrigTime, g_EchoHighTime, g_EchoLowTime, g_SonicReadTime=0;


/*Remote Value*/
int RemotePin=19, g_remotePulseCnt=0, g_remoteMapData;
unsigned short g_remoteDataCode=0;
 
unsigned long g_remoteData=0;
volatile unsigned long g_remotePulseStart_us=0, g_remotePulseEnd_us=0;// Global value


/*Servo motor Value*/
static int g_servo1_pin=4, g_servo2_pin=3;
int g_servo_Angle1=0, g_servo_Angle2=0, g_servo_Flag1=2, g_servo_Flag2=1;
float g_ServoCount=0.0;// Scale Count
unsigned long g_servoCountTime=0, g_angleChangeTime=0;


/*ShifrRegister Value*/
const int g_latchPin=A7, g_clockPin=52, g_dataPin=51, g_enablePin=53;
uint8_t g_Data[8]={1, 2, 4, 8, 16, 32, 64, 128};

static uint32_t g_shiftRegister_Cycle=20000, g_shiftTime_PWM[8]={0, }, g_shiftTime_duty=0;
double g_shiftRegister_duty[8]={0, 1, 2, 3, 4, 5, 6, 7};

/*Common Value*/
unsigned long g_curTime_us=0, g_prevTime_us=0, g_curTime_ms=0, g_modeCheckTime=0;
int g_i, g_mode=REMOTE_MODE, g_servoSelect=0;

void setup() {
  
  /*Initial actuator*/
Init_Matrix(g_DotMatrix_Row, g_DotMatrix_Col, COL);// Dot Matrix
Init_KeyMatrix(KEY_COL, KEY_ROW, g_KeyMatrix_col, g_KeyMatrix_row, KEY_MATRIX_COLUMN_BASE);// Key Matrix

Init_Fnd(8, 2, g_FndPin, g_FndPower);// Fnd

Init_Speaker(g_speakerPin);// Buzzer(Speaker)

Init_Servo(g_servo1_pin);// Servo motor1
Init_Servo(g_servo2_pin);// Servo motor2

Init_Shift(g_dataPin, g_clockPin, g_latchPin);// Shift register
Init_SiftEnable(g_enablePin);
ShiftEnable(g_enablePin);


attachInterrupt(4, ISR_Read_RemoteData, FALLING);// remote
attachInterrupt(0, ISR_Read_UltraSonic, CHANGE);// ultra sonic

  for(g_i=0; g_i<20; g_i++)
  {
      CloneFont(COL, g_font[g_i], g_ShiftFont[g_i]);
  }

  pinMode(g_Sonic_trig, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  g_curTime_us=micros();
  g_curTime_ms=millis();

  /*Servo control(10us)*/
  if(g_curTime_us-g_servoCountTime>=10)
  {
      g_ServoCount+=(g_curTime_us-g_servoCountTime)/10;
      g_servoCountTime=g_curTime_us;
      
      if(g_ServoCount>=2000)
      {
          ServoReset(g_servo1_pin, &g_ServoCount);
          ServoReset(g_servo2_pin, &g_ServoCount);
          
          g_servo_Angle1+=g_servo_Flag1;
          g_servo_Angle2+=g_servo_Flag2;
          
          if(g_servo_Angle1==160 ||g_servo_Angle1==0)
          {
              g_servo_Flag1*=-1;
          }
    
          if(g_servo_Angle2==160 || g_servo_Angle2==0)
          {
              g_servo_Flag2*=-1;
          }
      }
      ServoMove(g_servo1_pin, g_servo_Angle1, g_ServoCount);
      ServoMove(g_servo2_pin, g_servo_Angle2, g_ServoCount);
  }

  /*Shift Register*/
  for(g_i=0; g_i<8; g_i++)
  {
      if(Chk_ShiftPWM(g_shiftRegister_Cycle, g_shiftRegister_duty[g_i], g_curTime_us, &g_shiftTime_PWM[g_i]) ==RUN_PWM) 
      { 
        ShiftByte(g_dataPin, g_clockPin, g_latchPin, MSB_FIRST, g_Data[g_i]);
      }
      
      else if(Chk_ShiftPWM(g_shiftRegister_Cycle, g_shiftRegister_duty[g_i], g_curTime_us, &g_shiftTime_PWM[g_i])==STOP_PWM)
      {
        ShiftByte(g_dataPin, g_clockPin, g_latchPin, MSB_FIRST, 0x00);
      }
  }
  
  if(g_curTime_us-g_shiftTime_duty>=20000)
  {
      g_shiftTime_duty=g_curTime_us;
      for(g_i=0; g_i<8; g_i++)
      {
          g_shiftRegister_duty[g_i]+=1;
          if(g_shiftRegister_duty[g_i]>=100)
          {
            g_shiftRegister_duty[g_i]=0;
//            Serial.println(g_i);
          }
      }
  }
  
  
  /*Ultra sonic(200ms  Sycle)*/
  if(g_curTime_ms-g_SonicReadTime>=200)
  {
      g_SonicReadTime=g_curTime_ms;
      UltraSonic_Pulse(g_Sonic_trig, g_curTime_us, &g_SonicTrigTime, &g_SonicTrigState);
  }
  
  /*Remote*/
  if(g_remotePulseCnt==REMOTE_PULSE_COUNT && g_remotePulseCnt!=0)
  {
    g_remotePulseCnt=0;
    g_remoteDataCode=MaskData(g_remoteData, DATA_CODE);
    g_remoteMapData=MapRemote(g_remoteDataCode);
    
    g_matrixShiftFlag=1;
    
    if( !(g_remoteMapData>=0 && g_remoteMapData<=11) )
    {
        Serial.println(g_remoteData);
    }
  }

  /*Remote Signal Error sounds(speaker)*/
  if(g_remoteMapData==-1)
  {
      if(g_curTime_us-g_melodyTime_us <= g_melodyDelayTime_ms*1000)
      {
          Tone(g_speakerPin, g_Melody[g_melodyCount], g_curTime_us, &g_soundChangeTime_us, &g_soundState);
      }
      else
      {
          if(g_curTime_us-g_melodyTime_us <= g_CutMelody*1000)
          {
            MuteTone(g_speakerPin);
          }
          else
          {
              g_melodyTime_us=micros();
              g_melodyCount++;
              
              /*Speaker Melody Control*/
              if(g_melodyCount>=MELODY_SIZE)
              {
                  g_melodyCount=0;
              }
          }
      }
  }
  
  /*2400us Cycle(dot matrix)*/
  if(g_curTime_us-g_prevTime_us>=2400)
  {
      g_prevTime_us=g_curTime_us;
      
      g_RowCount++;
      if(g_RowCount==8)
      {
        g_RowCount=0;
      }
      DisplayFont(g_DotMatrix_Row, g_DotMatrix_Col, COL, g_ShiftFont[(g_KeyReadRow*KEY_COL)+g_KeyReadCol], g_RowCount);
  }
  
  /*10ms Cycle(fnd)*/
  if(g_curTime_ms-g_fndTime>=10)
  {
    g_fndTime=g_curTime_ms;
    Digit^=1;
    DisplayFnd(g_Sonic_dist, g_FndPower, g_Fnd, g_FndPin, Digit);
  }

  /*Dot Matrix Shift Font(mode)*/
  if(g_curTime_ms-g_ShiftCount>=125)
  {
      g_ShiftCount=g_curTime_ms;
      
      g_matrixShiftDir=NON_SHIFT;
      
      if(g_mode==REMOTE_MODE)
      {
          if(CDS_val<=150)
          {
              if(g_poten_val>=700)
              {
                  g_matrixShiftDir=SHIFT_LEFT;
              }
              
              else if(g_poten_val<=400)
              {
                  g_matrixShiftDir=SHIFT_RIGHT;
              }
          }
          
          if(g_remoteMapData==11 && g_matrixShiftFlag==1)
          {
              g_matrixShiftFlag=-1;
              g_matrixShiftDir=SHIFT_UP;
          }
    
          else if(g_remoteMapData==10 && g_matrixShiftFlag==1)
          {
              g_matrixShiftFlag=-1;
              g_matrixShiftDir=SHIFT_DOWN;
          }
      }

      else if(g_mode==JOYSTICK_MODE)
      {
          if(g_joystick_val_X>=700)
          {
            g_matrixShiftDir=SHIFT_RIGHT;
          }

          else if(g_joystick_val_X<400)
          {
            g_matrixShiftDir=SHIFT_LEFT;
          }

          
          if(g_joystick_val_Y<400)
          {
            g_matrixShiftDir=SHIFT_UP;
          }
          
          else if(g_joystick_val_Y>700)
          {
            g_matrixShiftDir=SHIFT_DOWN;
          }
      }
      
      ShiftFont(COL, g_ShiftFont[(g_KeyReadRow*KEY_COL)+g_KeyReadCol], g_ShiftFont[(g_KeyReadRow*KEY_COL)+g_KeyReadCol], g_matrixShiftDir, SHIFT_LOOP);
  }

   /*mode check(100ms)*/
   if(g_curTime_ms-g_modeCheckTime>=100)
   {
       g_modeCheckTime=g_curTime_ms;
       
       g_joystick_val_Btn = digitalRead(g_joystick_Pin_Btn);
       
      if(g_joystick_val_prevBtn==0 &&g_joystick_val_Btn==1)
      {
          g_servo_Angle1=0;
          g_servo_Angle2=0;
          
          g_servo_Flag1=2;
          g_servo_Flag2=1;
          
          g_mode++;
          g_mode%=2;
          
          Serial.println("mode change!!");
          
          for(g_i=0; g_i<20; g_i++)
          {
              CloneFont(COL, g_font[g_i], g_ShiftFont[g_i]);
          }
      }
      
      g_joystick_val_prevBtn=g_joystick_val_Btn;

      if(g_mode==REMOTE_MODE)
      {   
          if(CDS_val<=150)
          {
              if(g_poten_val>=700)
              {
                  g_matrixShiftDir=SHIFT_LEFT;
              }
              
              else if(g_poten_val<=400)
              {
                  g_matrixShiftDir=SHIFT_RIGHT;
              }
          }
              
          if(g_remoteMapData==11 && g_matrixShiftFlag==1)
          {
              g_matrixShiftFlag=-1;
              g_matrixShiftDir=SHIFT_UP;
          }
        
          else if(g_remoteMapData==10 && g_matrixShiftFlag==1)
          {
              g_matrixShiftFlag=-1;
              g_matrixShiftDir=SHIFT_DOWN;
          }

          g_servo_Flag1=0;
          g_servo_Flag2=0;
          g_servo_Angle1= map(g_joystick_val_X, 0, 1023, 0, 160);
          g_servo_Angle2= map(g_joystick_val_Y, 0, 1023, 0, 160);
      }
   }
    
  /*100ms Cycle(Joystick X,Y)*/
  if(g_curTime_ms-g_joystick_ReadTime>=100)
  {
    g_joystick_ReadTime=g_curTime_ms;
    g_joystick_val_X= ReadDAC(g_joystick_pin_X);
    g_joystick_val_Y= ReadDAC(g_joystick_pin_Y);
  }
  
  /* 100ms Cycle(Key Matrix, Potentiometer, CDS)*/
  if(g_curTime_ms-KeyReadTime>=100)
  {
      KeyReadTime=g_curTime_ms;
      g_KeyData=ReadKey(KEY_COL, KEY_ROW, g_KeyMatrix_col, g_KeyMatrix_row, KEY_MATRIX_COLUMN_BASE, &g_KeyReadCol, &g_KeyReadRow);
      
      g_poten_val=ReadDAC(g_poten_pin);
      CDS_val=ReadDAC(CDS_pin);
//      Serial.println(g_poten_val);
      
      if(g_KeyData!=-1)
      {
          g_ConvKeyData=ConvKeyToNum(g_KeyData);
          KeyIntData=ConvStringToInt(g_ConvKeyData);
      }
  }
  
}


void ISR_Read_RemoteData(void)
{
    if(g_remotePulseCnt==0)
    {
        g_remotePulseStart_us=micros();
        g_remotePulseCnt++;
    }
    else
    {
        g_remotePulseEnd_us=micros();
        
        /*LeadCode*/
        if(g_remotePulseEnd_us - g_remotePulseStart_us>=12000 && g_remotePulseEnd_us - g_remotePulseStart_us<=15000)
        {
            g_remoteData= 0;
            g_remotePulseStart_us=g_remotePulseEnd_us;
        }
      
        /*bit is 0 */
        else if(g_remotePulseEnd_us - g_remotePulseStart_us>=800 && g_remotePulseEnd_us - g_remotePulseStart_us<=1400)
        {
            g_remoteData=g_remoteData<<1;
            g_remotePulseStart_us=g_remotePulseEnd_us;
        }
      
        /*bit is 1*/
        else if(g_remotePulseEnd_us - g_remotePulseStart_us>=1900 && g_remotePulseEnd_us - g_remotePulseStart_us<=2600)
        {
            g_remoteData=g_remoteData<<1;
            g_remoteData|=0x00000001;  
            g_remotePulseStart_us=g_remotePulseEnd_us;
        }
        g_remotePulseCnt++;
    }
}

void ISR_Read_UltraSonic(void)
{
    int EchoState;
    EchoState= digitalRead(g_Sonic_echo);
    if(EchoState==1)
    {
        g_EchoHighTime=micros();
    }

    else if(EchoState==0)
    {
        g_EchoLowTime=micros();
        
        g_Sonic_dist= CalculateDist(g_EchoHighTime, g_EchoLowTime);
    }
}

