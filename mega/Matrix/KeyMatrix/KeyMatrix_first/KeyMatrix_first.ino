#define KEY_MATRIX_COLUMN_BASE 1
#define KEY_MATRIX_ROW_BASE 0

#define KEY_CLICK 0
#define KEY_NON_CLICK 1

#define KEY_POWER_ON 0
#define KEY_POWER_OFF 1

#define KEY_ROW 5
#define KEY_COL 4

int row[KEY_ROW]={12, 10, 8, 6, 5};
int col[KEY_COL]={13, 11, 9, 7};

int g_KeyData, g_KeyPrevData;
String g_ConvKeyData;
unsigned long g_curTime, g_prevTime=0;
int a;
void setup() {
  // put your setup code here, to run once:
InitKeyMatrix(KEY_COL, KEY_ROW, col, row, KEY_MATRIX_COLUMN_BASE);
Serial.begin(9600);
Serial.println("℃");
//Serial.write(-17275);
}

void loop() {
  // put your main code here, to run repeatedly:
  g_curTime=millis();
  
  if(g_curTime-g_prevTime>=100)
  {
  g_prevTime=g_curTime;
  
  g_KeyData=ReadKey(KEY_COL, KEY_ROW, col, row, KEY_MATRIX_COLUMN_BASE);
  
  if(g_KeyData!=-1 && g_KeyData!=g_KeyPrevData)
  {
      g_ConvKeyData=ConvKeyToNum(g_KeyData);
      Serial.println(g_ConvKeyData);
//      a=ConvStringToInt(g_ConvKeyData);
//      Serial.println(a);
  }
  
  g_KeyPrevData=g_KeyData;
  }
}
