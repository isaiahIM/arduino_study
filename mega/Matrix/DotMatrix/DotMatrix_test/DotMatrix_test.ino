#define ROW 8
#define COL 8
#define POWER_ON 0
#define POWER_OFF 1

int g_Matrix_Row[ROW]={48, 35, 44, 45, 37, 43, 38, 41};
int g_Matrix_Col[COL]={36, 39, 40, 47, 42, 46, 34, 33};
int i, g_col=0, g_row=0;

unsigned char g_font[ROW][COL]=
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 1, 0, 1, 1, 0, 0},
  {0, 1, 1, 0, 1, 0, 1, 0},
  {0, 0, 0, 0, 1, 0, 1, 0},
  {0, 0, 0, 0, 1, 0, 1, 0},
  {0, 1, 1, 0, 1, 0, 1, 0},
  {0, 1, 1, 0, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
unsigned long g_curTime=0, g_prevTime=0;

void setup() {
  // put your setup code here, to run once:
InitMatrix(g_Matrix_Row, g_Matrix_Col, COL);
Serial.begin(9600);
}

void loop() {

  if(g_row>=8)
  {
      g_row=0;
      g_col++;
  }
  if(g_col>=8)
  {
      g_row=0;
      g_col=0;
  }
  
  ClearMatrix(g_Matrix_Row, g_Matrix_Col, COL);
  DisplayIndividual(g_Matrix_Row, g_Matrix_Col, g_col, g_row);
  //DisplayFont(g_Matrix_Row, g_Matrix_Col, COL, g_font);
  delay(50);
  g_row++;
  
}


