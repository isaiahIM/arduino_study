/*Dot Matrix define*/
#define COLUMN_BASE

#define POWER_ON 1
#define POWER_OFF 0
#define ROW 8
#define COL 8

#define SHIFT_LEFT 2
#define SHIFT_RIGHT 3
#define SHIFT_LOOP 4
#define SHIFT_NON_LOOP 5

/*IR Remote define*/
#define CUSTOM_CODE 1
#define DATA_CODE 2
#define REVERSE_CHECK 3

/*Dot Matrix Value*/
int i;
int g_Matrix_Row[ROW]={A5, A4, A3, A2, A1, A0, 12, 13 };
int g_Matrix_Col[COL]={4, 5, 6, 7, 8, 9, 10, 11};

unsigned char g_font[10][ROW]=
  {
    {0x00, 0x38, 0x44, 0x4c, 0x54, 0x64, 0x44, 0x38},// 0
    {0x00, 0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x7c},// 1
    {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c},// 2
    {0x00, 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38},// 3
    {0x00, 0x08, 0x18, 0x28, 0x48, 0x7c, 0x08, 0x08},// 4
    {0x00, 0x7c, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38},// 5
    {0x00, 0x38, 0x40, 0x40, 0x78, 0x44, 0x44, 0x38},// 6
    {0x00, 0x7c, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20},// 7
    {0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38},// 8
    {0x00, 0x38, 0x44, 0x44, 0x3c, 0x04, 0x44, 0x38},// 9
  };
unsigned char g_ShiftFont[10][ROW];

unsigned long g_curTime=0, g_prevTime=0;
unsigned long g_MicroCount=0, g_MillisCount=0, g_FontCount=0;
unsigned long g_ShiftState=1, g_ShiftState_old=1, g_ShiftCount=0, ShiftDir=SHIFT_RIGHT, DirFlag=1;

/*IR Remote Value*/
int RemotePin=2, g_RemoteCount=0, g_RemoteNum=0;
unsigned long g_RemoteStartTime, g_RemoteEndTime, g_RemoteData=0;
unsigned short g_CustomCode, g_DataCode;

void setup() {
InitMatrix(g_Matrix_Row, g_Matrix_Col, COL);
Serial.begin(9600);

  for(i=0; i<10; i++)
  {
      CloneFont(COL, g_font[i], g_ShiftFont[i]);
  }
  attachInterrupt(0, ISR_ReadRemote, FALLING);
}

void loop() {

  g_curTime=micros();
  
    if(g_RemoteCount>=36)
    {
        g_CustomCode=MaskRemote(g_RemoteData, CUSTOM_CODE, REVERSE_CHECK);
        if(g_CustomCode!=0)
        {
        g_DataCode= MaskRemote(g_RemoteData, DATA_CODE, 0);
         
        }
        else
        {
          Serial.println("custom code error!! code:"+ (String)g_RemoteData);
        }
        g_RemoteNum=MapRemoteDataToNum(g_DataCode);
        g_RemoteCount=0;
        Serial.println(g_RemoteNum);
        
    }
  
  /*dot matrix display delay count*/
  if(g_curTime-g_prevTime>=2000)
  {
      g_prevTime=g_curTime;
      g_MicroCount++;
      g_MicroCount%=8;
      
      DisplayFont(g_Matrix_Row, g_Matrix_Col, COL, g_ShiftFont[g_FontCount], g_MicroCount);
  }

  /*number count*/
  if(g_curTime-g_MillisCount>=1000000)
  {
      g_MillisCount=g_curTime;
      g_FontCount++;
      g_FontCount%=10;
  }
  
  if(g_curTime-g_ShiftCount>=1000000/8)
  {
      g_ShiftCount=g_curTime;
      DirFlag^=1;

      if(DirFlag)
      {
          ShiftDir=SHIFT_RIGHT;
      }
      else
      {
          //ShiftDir=SHIFT_LEFT;
      }
      //ShiftFont(COL, g_ShiftFont[g_FontCount], g_ShiftFont[g_FontCount], ShiftDir, SHIFT_LOOP);
  }
}


void ISR_ReadRemote(void)
{    
    if(g_RemoteCount==0)
    {
        g_RemoteStartTime=micros();
        g_RemoteCount++;
    }
    else
    {
        g_RemoteEndTime =micros();
        g_RemoteData= DecodeRemote(g_RemoteData, g_RemoteStartTime, g_RemoteEndTime);
        
        g_RemoteStartTime=g_RemoteEndTime;
        g_RemoteCount++;
    }
}
