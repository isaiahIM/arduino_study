#define ROW 8
#define COL 8
#define POWER_ON 1
#define POWER_OFF 0

int g_Matrix_Row[ROW]={A3, A2, A1, A0, 10, 11, 12, 13};
int g_Matrix_Col[COL]={2, 3, 4, 5, 6, 7, 8, 9};
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
  //DisplayIndividual(g_Matrix_Row, g_Matrix_Col, g_col, g_row);
  DisplayFont(g_Matrix_Row, g_Matrix_Col, COL, g_font);
//delay(50);
  g_row++;
  
}


