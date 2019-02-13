/*Dot Matrix define*/
#define DOTMATRIX_POWER_ON 1
#define DOTMATRIX_POWER_OFF 0
#define ROW 8
#define COL 8

#define SHIFT_LEFT 2
#define SHIFT_RIGHT 3
#define SHIFT_UP 4
#define SHIFT_DOWN 5

#define SHIFT_LOOP 6
#define SHIFT_NON_LOOP 7

/*Fnd define*/
#define FND_HIGH 0
#define FND_LOW 1
#define FND_POWER_ON 1
#define FND_POWER_OFF 0


/*Dot Matrix Value*/
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

unsigned long g_MicroCount=0, g_MillisCount=0, g_FontCount=0;
unsigned long g_ShiftState=1, g_ShiftState_old=1, g_ShiftCount=0, ShiftDir=SHIFT_RIGHT, DirFlag=1;

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

unsigned long g_Digit_10Time=0, g_Digit_1Time=0, g_CountTime=0;


/*Common Value*/
unsigned long g_curTime_us=0, g_prevTime_us=0;
int g_i;

void setup() {
  // put your setup code here, to run once
InitMatrix(g_Matrix_Row, g_Matrix_Col, COL);
Serial.begin(9600);
  for(g_i=0; g_i<10; g_i++)
  {
      CloneFont(COL, g_font[g_i], g_ShiftFont[g_i]);
  }

   for(g_i=0; g_i<8; g_i++)
    {
        pinMode(g_FndPin[g_i], OUTPUT);
    }
    
    for(g_i=0; g_i<2; g_i++)
    {
        pinMode(g_FndPower[g_i], OUTPUT);
    }
    
}

void loop() {

  g_curTime_us=micros();

  /*dot matrix display delay count*/
  if(g_curTime_us-g_prevTime_us>=2000)
  {
      g_prevTime_us=g_curTime_us;
      g_MicroCount++;
      g_MicroCount%=8;
      Digit^=1;
      DisplayFont(g_Matrix_Row, g_Matrix_Col, COL, g_ShiftFont[g_FontCount], g_MicroCount);
      DisplayFnd(g_FndNum, g_FndPower, g_Fnd, g_FndPin, Digit);
  }

  /*number count*/
  if(g_curTime_us-g_MillisCount>=1000000)
  {
      g_MillisCount=g_curTime_us;
      
      g_FontCount++;
      g_FndNum++;
      
      g_FndNum%=100;
      g_FontCount%=10;
      
  }
  
  if(g_curTime_us-g_ShiftCount>=1000000/8)
  {
      g_ShiftCount=g_curTime_us;
      DirFlag^=1;

      if(DirFlag)
      {
          ShiftDir=SHIFT_RIGHT;
      }
      else
      {
          //ShiftDir=SHIFT_LEFT;
      }
      ShiftFont(COL, g_ShiftFont[g_FontCount], g_ShiftFont[g_FontCount], ShiftDir, SHIFT_LOOP);
  }
}

