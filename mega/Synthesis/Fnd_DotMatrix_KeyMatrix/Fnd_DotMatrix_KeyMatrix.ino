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

#define SHIFT_LOOP 6
#define SHIFT_NON_LOOP 7

/*Fnd define*/
#define FND_HIGH 0
#define FND_LOW 1
#define FND_POWER_ON 1
#define FND_POWER_OFF 0


/*Key Matrix Value*/
int row[KEY_ROW]={12, 10, 8, 6, 5};
int col[KEY_COL]={13, 11, 9, 7};

int g_KeyData, g_KeyPrevData, g_KeyReadCol, g_KeyReadRow, KeyIntData=0;
String g_ConvKeyData;
unsigned long KeyReadTime=0;

/*Dot Matrix Value*/

int g_Matrix_Row[ROW]={48, 35, 44, 45, 37, 43, 38, 41};
int g_Matrix_Col[COL]={36, 39, 40, 47, 42, 46, 34, 33};
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

/*Common Value*/
unsigned long g_curTime_us=0, g_prevTime_us=0, g_curTime_ms=0;
int g_i;

void setup() {
  // put your setup code here, to run once
InitMatrix(g_Matrix_Row, g_Matrix_Col, COL);
InitKeyMatrix(KEY_COL, KEY_ROW, col, row, KEY_MATRIX_COLUMN_BASE);
InitFnd(8, 2, g_FndPin, g_FndPower);

Serial.begin(9600);
  for(g_i=0; g_i<20; g_i++)
  {
      CloneFont(COL, g_font[g_i], g_ShiftFont[g_i]);
  }
}

void loop() {

  g_curTime_us=micros();
  g_curTime_ms=millis();
  
  /*dot matrix, Fnd display delay count*/
  if(g_curTime_us-g_prevTime_us>=2400)
  {
      g_prevTime_us=g_curTime_us;
      g_MicroCount++;
      g_MicroCount%=8;
      Digit^=1;
      DisplayFont(g_Matrix_Row, g_Matrix_Col, COL, g_ShiftFont[(g_KeyReadRow*KEY_COL)+g_KeyReadCol], g_MicroCount);
      DisplayFnd(KeyIntData, g_FndPower, g_Fnd, g_FndPin, Digit);
  }

  /*Key matrix Read*/
  if(g_curTime_ms-KeyReadTime>=100)
  {
      KeyReadTime=g_curTime_ms;
      g_KeyData=ReadKey(KEY_COL, KEY_ROW, col, row, KEY_MATRIX_COLUMN_BASE, &g_KeyReadCol, &g_KeyReadRow);
      if(g_KeyData!=-1)
      {
          g_ConvKeyData=ConvKeyToNum(g_KeyData);
          KeyIntData=ConvStringToInt(g_ConvKeyData);
      }
  }
  
  if(g_curTime_ms-g_ShiftCount>=1000/8)
  {
      g_ShiftCount=g_curTime_ms;
      DirFlag^=1;

      if(DirFlag)
      {
          ShiftDir=SHIFT_RIGHT;
      }
      else
      {
          //ShiftDir=SHIFT_LEFT;
      }
      ShiftFont(COL, g_ShiftFont[(g_KeyReadRow*KEY_COL)+g_KeyReadCol], g_ShiftFont[(g_KeyReadRow*KEY_COL)+g_KeyReadCol], ShiftDir, SHIFT_LOOP);
  }
}

