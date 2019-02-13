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


/*fnd define*/
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

/*SPI define*/
#define SEQUENCE_FINISH 1
#define SEQUENCE_NON_FINISH 0

#define CLK 52
#define MISO 50
#define MOSI 51
#define CS 49

/*SPI eeprom Register*/
#define SPI_WREN 0x06// write enable
#define SPI_READ 0X03// read mode
#define SPI_WRITE 0X02// write mode

/*I2C define*/
#define SLAVE_RECEVE_DATA 1
#define SLAVE_NO_RECEVE_DATA 2

#define SCL 21
#define SDA 20
#define READ_MODE 2
#define WRITE_MODE 3

/*I2C EEPROM Sequence*/
enum I2C_EEPRom_WriteTask
{
W_START_I2C,
W_DEVICE_ADDRESS_I2C,
W_ACK1_I2C,
W_WORD_ADDRESS_I2C,
W_ACK2_I2C,
W_DATA_I2C,
W_ACK3_I2C,
W_STOP_I2C  
};
enum I2C_EEPRom_ReadTask
{
R_START_I2C,
R_DEVICE_ADDRESS_I2C,
R_ACK1_I2C,
R_WORD_ADDRESS_I2C,
R_ACK2_I2C,
R_ReStart_I2C,
R_DEVICE_ADDRESS2_I2C,
R_ACK3_I2C,
R_READ_DATA_I2C,
R_SEND_ACK_I2C,
R_STOP_I2C
};

/*SPI EEPROM Sequence*/
enum SPI_EEPRom_WriteTask
{
W_START_SPI,
W_DEVICE_ADDRESS_SPI,
W_ACK1_SPI,
W_WORD_ADDRESS_SPI,
W_ACK2_SPI,
W_DATA_SPI,
W_ACK3_SPI,
W_STOP_SPI
};
enum SPI_EEPRom_ReadTask
{
R_START_SPI,
R_DEVICE_ADDRESS_SPI,
R_ACK1_SPI,
R_WORD_ADDRESS_SPI,
R_ACK2_SPI,
R_ReStart_SPI,
R_DEVICE_ADDRESS2_SPI,
R_ACK3_SPI,
R_READ_DATA_SPI,
R_SEND_ACK_SPI,
R_STOP_SPI
};

/*Key Matrix Value*/
static const int KeyMatrix_row[KEY_ROW]={12, 10, 8, 6, 5};
static const int KeyMatrix_col[KEY_COL]={13, 11, 9, 7};
static int KeyData, KeyPrevData, KeyReadCol, KeyReadRow, KeyIntData=-1;
static String ConvKeyData;
static unsigned long KeyReadTime=0;


/*Dot Matrix Value*/
static const int DotMatrix_Row[ROW]={48, 35, 44, 45, 37, 43, 38, 41};
static const int DotMatrix_Col[COL]={36, 39, 40, 47, 42, 46, 34, 33};
static unsigned char font[20][ROW]=
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
static unsigned char shiftFont[20][ROW];

static uint32_t RowCount=0, prevTime_us=0;
static uint32_t ShiftState=1, ShiftState_old=1, ShiftCount=0, matrixShiftDir=SHIFT_RIGHT, matrixShiftFlag=-1;


/*fnd Value*/
static const int fndPin[8]={30, 24, 25, 27, 26, 31, 28, 29};// a, b, c, d, e, f, g, dp
static int fnd[10][8]=
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
static unsigned int fndNum=0, digit;
static const int fndPower[2]={32, 23};
static uint32_t fndTime=0;


/*Speaker Value*/
int speakerPin=17, melodyCount=0;
int Melody[MELODY_SIZE] = { // 학교종 멜로디. 0은 쉼표
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0,
NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, 0,
NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, 0
};
static int soundState=0;
static uint32_t CutMelody=325, melodyDelayTime_ms=250, soundChangeTime_us=0, melodyTime_us=0;


/*Potentiometer Value*/
int poten_pin=A5, poten_val;

/*CDS Value*/
static int CDS_pin=A6, CDS_val;

/*Joystick Value*/
const static int joystick_pin_X=A8, joystick_pin_Y=A9, joystick_Pin_Btn=A10;
static int joystick_val_X, joystick_val_Y;
static unsigned int joystick_val_Btn=0, joystick_val_prevBtn=0;
static unsigned long joystick_ReadTime=0;


/*UltlaSonic Value*/
static int Sonic_trig=18, Sonic_echo=2, Sonic_dist, SonicTrigState=0;
static uint32_t SonicTrigTime, EchoHighTime, EchoLowTime, SonicReadTime=0;


/*Remote Value*/
static int RemotePin=19, remotePulseCnt=0, remoteMapData;
static unsigned short remoteDataCode=0;
 
static uint32_t remoteData=0;
static volatile uint32_t remotePulseStart_us=0, remotePulseEnd_us=0;// Global value


/*Servo motor Value*/
static const int servo1_pin=4, servo2_pin=3;
static int servo_Angle1=0, servo_Angle2=0, servo_Flag1=2, servo_Flag2=1;
static float ServoCount=0.0;// Scale Count
static uint32_t servoCountTime=0, angleChangeTime=0;


/*ShifrRegister Value*/
const int latchPin=A7, clockPin=52, dataPin=51, enablePin=53;
static uint8_t Data[8]={1, 2, 4, 8, 16, 32, 64, 128};

static uint32_t shiftRegister_Cycle=20000, shiftTime_PWM[8]={0, }, shiftTime_duty=0;
static double shiftRegister_duty[8]={0, 1, 2, 3, 4, 5, 6, 7};


/*SPI Value*/
volatile static uint8_t readVal=0;
static int firstLoop=1, spi_wiriteTask=0, spi_readTask=0, bitCount=0, SPI_sequenceNextState=0;
static uint8_t timeCount=0;
static uint8_t SPI_wordAddr, SPI_writeData=0;

/*I2C Value*/
static int I2C_WriteTask=0, I2C_ReadTask=0, byteCheck=0;
static uint8_t DeviceAddr_w, DeviceAddr_r, WordAddr, ackState=0, I2C_sequenceNextState=0, I2C_writeData=0;


/*Common Value*/ 
static uint32_t curTime_us=0, curTime_ms=0, modeCheckTime=0, interface_Time=0, interface_Speed=50;
static int i, mode=REMOTE_MODE, servoSelect=0, eeprom_writeState=1, eeprom_readState=0;
static char commend;


void setup() {
  /*Initial actuator*/
Init_Matrix(DotMatrix_Row, DotMatrix_Col, COL);// Dot Matrix
Init_KeyMatrix(KEY_COL, KEY_ROW, KeyMatrix_col, KeyMatrix_row, KEY_MATRIX_COLUMN_BASE);// Key Matrix

Init_Fnd(8, 2, fndPin, fndPower);// fnd

Init_Speaker(speakerPin);// Buzzer(Speaker)

Init_Servo(servo1_pin);// Servo motor1
Init_Servo(servo2_pin);// Servo motor2

Init_Shift(dataPin, clockPin, latchPin);// Shift register
Init_SiftEnable(enablePin);
ShiftEnable(enablePin);


Init_eepRom(SCL, SDA);// i2c init
  DeviceAddr_w= 0xA0;
  DeviceAddr_r= 0xA1;
  WordAddr= 0x01;

Init_SPI(CLK, MOSI, MISO, CS);// spi init
SPI_Disable(CS);

attachInterrupt(4, ISR_Read_RemoteData, FALLING);// remote
attachInterrupt(0, ISR_Read_UltraSonic, CHANGE);// ultra sonic

  for(i=0; i<20; i++)
  {
      CloneFont(COL, font[i], shiftFont[i]);
  }

  pinMode(Sonic_trig, OUTPUT);
  Serial.begin(9600);

  Serial.println("Write commend:");
}


void loop() {

  curTime_us=micros();
  curTime_ms=millis();

  if(timeCount>=4)
  {
    timeCount=0;
  } 

  if(curTime_us-interface_Time>=interface_Speed || firstLoop==1)
  {
    interface_Time=curTime_us;

    if(firstLoop==1)
    {
      firstLoop=0;
      timeCount=0;
    }
    else
    {
      timeCount+=1;
    }
  }

  if(Serial.available() )
  {
    commend=Serial.read();

    if(commend=='r')
    {
      eeprom_writeState=0; 
      eeprom_readState=1;
    }
  }

  
  /*Servo control(10us)*/
  if(curTime_us-servoCountTime>=10)
  {
      ServoCount+=(curTime_us-servoCountTime)/10;
      servoCountTime=curTime_us;
      
      if(ServoCount>=2000)
      {
          ServoReset(servo1_pin, &ServoCount);
          ServoReset(servo2_pin, &ServoCount);
          
          servo_Angle1+=servo_Flag1;
          servo_Angle2+=servo_Flag2;
          
          if(servo_Angle1==160 ||servo_Angle1==0)
          {
              servo_Flag1*=-1;
          }
    
          if(servo_Angle2==160 || servo_Angle2==0)
          {
              servo_Flag2*=-1;
          }
      }
      ServoMove(servo1_pin, servo_Angle1, ServoCount);
      ServoMove(servo2_pin, servo_Angle2, ServoCount);
  }

  /*Shift Register*/
  for(i=0; i<8; i++)
  {
      if(Chk_ShiftPWM(shiftRegister_Cycle, shiftRegister_duty[i], curTime_us, &shiftTime_PWM[i]) ==RUN_PWM) 
      { 
        ShiftByte(dataPin, clockPin, latchPin, MSB_FIRST, Data[i]);
      }
      
      else if(Chk_ShiftPWM(shiftRegister_Cycle, shiftRegister_duty[i], curTime_us, &shiftTime_PWM[i])==STOP_PWM)
      {
        ShiftByte(dataPin, clockPin, latchPin, MSB_FIRST, 0x00);
      }
  }
  
  if(curTime_us-shiftTime_duty>=20000)
  {
      shiftTime_duty=curTime_us;
      for(i=0; i<8; i++)
      {
          shiftRegister_duty[i]+=1;
          if(shiftRegister_duty[i]>=100)
          {
            shiftRegister_duty[i]=0;
//            Serial.println(i);
          }
      }
  }
  
  
  /*Ultra sonic(200ms  Sycle)*/
  if(curTime_ms-SonicReadTime>=200)
  {
      SonicReadTime=curTime_ms;
      UltraSonic_Pulse(Sonic_trig, curTime_us, &SonicTrigTime, &SonicTrigState);
  }
  
  /*Remote*/
  if(remotePulseCnt==REMOTE_PULSE_COUNT && remotePulseCnt!=0)
  {
    remotePulseCnt=0;
    remoteDataCode=MaskData(remoteData, DATA_CODE);
    remoteMapData=MapRemote(remoteDataCode);
    
    matrixShiftFlag=1;
    
    if( !(remoteMapData>=0 && remoteMapData<=11) )
    {
        Serial.println(remoteData);
    }
  }

  /*Remote Signal Error sounds(speaker)*/
  if(remoteMapData==-1)
  {
      if(curTime_us-melodyTime_us <= melodyDelayTime_ms*1000)
      {
          Tone(speakerPin, Melody[melodyCount], curTime_us, &soundChangeTime_us, &soundState);
      }
      else
      {
          if(curTime_us-melodyTime_us <= CutMelody*1000)
          {
            MuteTone(speakerPin);
          }
          else
          {
              melodyTime_us=micros();
              melodyCount++;
              
              /*Speaker Melody Control*/
              if(melodyCount>=MELODY_SIZE)
              {
                  melodyCount=0;
              }
          }
      }
  }
  
  /*2400us Cycle(dot matrix)*/
  if(curTime_us-prevTime_us>=2400)
  {
      prevTime_us=curTime_us;
      
      RowCount++;
      if(RowCount==8)
      {
        RowCount=0;
      }
      DisplayFont(DotMatrix_Row, DotMatrix_Col, COL, shiftFont[(KeyReadRow*KEY_COL)+KeyReadCol], RowCount);
  }
  
  /*10ms Cycle(fnd)*/
  if(curTime_ms-fndTime>=10)
  {
    fndTime=curTime_ms;
    digit^=1;
    Display_Fnd(Sonic_dist, fndPower, fnd, fndPin, digit);
  }

  /*Dot Matrix Shift Font(mode)*/
  if(curTime_ms-ShiftCount>=125)
  {
      ShiftCount=curTime_ms;
      
      matrixShiftDir=NON_SHIFT;
      
      if(mode==REMOTE_MODE)
      {
          if(CDS_val<=150)
          {
              if(poten_val>=700)
              {
                  matrixShiftDir=SHIFT_LEFT;
              }
              
              else if(poten_val<=400)
              {
                  matrixShiftDir=SHIFT_RIGHT;
              }
          }
          
          if(remoteMapData==11 && matrixShiftFlag==1)
          {
              matrixShiftFlag=-1;
              matrixShiftDir=SHIFT_UP;
          }
    
          else if(remoteMapData==10 && matrixShiftFlag==1)
          {
              matrixShiftFlag=-1;
              matrixShiftDir=SHIFT_DOWN;
          }
      }

      else if(mode==JOYSTICK_MODE)
      {
          if(joystick_val_X>=700)
          {
            matrixShiftDir=SHIFT_RIGHT;
          }

          else if(joystick_val_X<400)
          {
            matrixShiftDir=SHIFT_LEFT;
          }

          
          if(joystick_val_Y<400)
          {
            matrixShiftDir=SHIFT_UP;
          }
          
          else if(joystick_val_Y>700)
          {
            matrixShiftDir=SHIFT_DOWN;
          }
      }
      
      ShiftFont(COL, shiftFont[(KeyReadRow*KEY_COL)+KeyReadCol], shiftFont[(KeyReadRow*KEY_COL)+KeyReadCol], matrixShiftDir, SHIFT_LOOP);
  }

   /*mode check(100ms)*/
   if(curTime_ms-modeCheckTime>=100)
   {
       modeCheckTime=curTime_ms;
       
       joystick_val_Btn = digitalRead(joystick_Pin_Btn);
       
      if(joystick_val_prevBtn==0 &&joystick_val_Btn==1)
      {
          servo_Angle1=0;
          servo_Angle2=0;
          
          servo_Flag1=2;
          servo_Flag2=1;
          
          mode++;
          mode%=2;
          
          Serial.println("mode change!!");
          
          for(i=0; i<20; i++)
          {
              CloneFont(COL, font[i], shiftFont[i]);
          }
      }
      
      joystick_val_prevBtn=joystick_val_Btn;

      if(mode==REMOTE_MODE)
      {   
          if(CDS_val<=150)
          {
              if(poten_val>=700)
              {
                  matrixShiftDir=SHIFT_LEFT;
              }
              
              else if(poten_val<=400)
              {
                  matrixShiftDir=SHIFT_RIGHT;
              }
          }
              
          if(remoteMapData==11 && matrixShiftFlag==1)
          {
              matrixShiftFlag=-1;
              matrixShiftDir=SHIFT_UP;
          }
        
          else if(remoteMapData==10 && matrixShiftFlag==1)
          {
              matrixShiftFlag=-1;
              matrixShiftDir=SHIFT_DOWN;
          }

          servo_Flag1=0;
          servo_Flag2=0;
          servo_Angle1= map(joystick_val_X, 0, 1023, 0, 160);
          servo_Angle2= map(joystick_val_Y, 0, 1023, 0, 160);
      }
   }
    
  /*100ms Cycle(Joystick X,Y)*/
  if(curTime_ms-joystick_ReadTime>=100)
  {
    joystick_ReadTime=curTime_ms;
    joystick_val_X= ReadDAC(joystick_pin_X);
    joystick_val_Y= ReadDAC(joystick_pin_Y);
  }
  
  /* 100ms Cycle(Key Matrix, Potentiometer, CDS)*/
  if(curTime_ms-KeyReadTime>=100)
  {
      KeyReadTime=curTime_ms;
      KeyData=ReadKey(KEY_COL, KEY_ROW, KeyMatrix_col, KeyMatrix_row, KEY_MATRIX_COLUMN_BASE, &KeyReadCol, &KeyReadRow);
      
      poten_val=ReadDAC(poten_pin);
      CDS_val=ReadDAC(CDS_pin);
//      Serial.println(poten_val);
      
      if(KeyData!=-1)
      {
          ConvKeyData=ConvKeyToNum(KeyData);
          KeyIntData=ConvStringToInt(ConvKeyData);
      }
  }
  
}


void ISR_Read_RemoteData(void)
{
    if(remotePulseCnt==0)
    {
        remotePulseStart_us=micros();
        remotePulseCnt++;
    }
    else
    {
        remotePulseEnd_us=micros();
        
        /*LeadCode*/
        if(remotePulseEnd_us - remotePulseStart_us>=12000 && remotePulseEnd_us - remotePulseStart_us<=15000)
        {
            remoteData= 0;
            remotePulseStart_us=remotePulseEnd_us;
        }
      
        /*bit is 0 */
        else if(remotePulseEnd_us - remotePulseStart_us>=800 && remotePulseEnd_us - remotePulseStart_us<=1400)
        {
            remoteData=remoteData<<1;
            remotePulseStart_us=remotePulseEnd_us;
        }
      
        /*bit is 1*/
        else if(remotePulseEnd_us - remotePulseStart_us>=1900 && remotePulseEnd_us - remotePulseStart_us<=2600)
        {
            remoteData=remoteData<<1;
            remoteData|=0x00000001;  
            remotePulseStart_us=remotePulseEnd_us;
        }
        remotePulseCnt++;
    }
}

void ISR_Read_UltraSonic(void)
{
    int EchoState;
    EchoState= digitalRead(Sonic_echo);
    if(EchoState==1)
    {
        EchoHighTime=micros();
    }

    else if(EchoState==0)
    {
        EchoLowTime=micros();
        
        Sonic_dist= CalculateDist(EchoHighTime, EchoLowTime);
    }
}

