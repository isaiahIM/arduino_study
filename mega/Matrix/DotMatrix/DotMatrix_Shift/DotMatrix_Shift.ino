/*Dot Matrix define*/
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

/*Dot Matrix Value*/
typedef unsigned char Font;
int i;
int g_Matrix_Row[ROW]={48, 35, 44, 45, 37, 43, 38, 41};
int g_Matrix_Col[COL]={36, 39, 40, 47, 42, 46, 34, 33};
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

void setup() {
  // put your setup code here, to run once
InitMatrix(g_Matrix_Row, g_Matrix_Col, COL);
Serial.begin(9600);
  for(i=0; i<10; i++)
  {
      CloneFont(COL, g_font[i], g_ShiftFont[i]);
  }
}

void loop() {

  g_curTime=micros();

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

