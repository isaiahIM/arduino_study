#define KEY_MATRIX_COLUMN_BASE 1
#define KEY_MATRIX_ROW_BASE 0

#define KEY_CLICK 0
#define KEY_NON_CLICK 1

#define KEY_POWER_ON 0
#define KEY_POWER_OFF 1

#define KEY_SIZE 4

int row[KEY_SIZE]={4, 5, 6, 7};
int col[KEY_SIZE]={8, 9, 10, 11};
int g_KeyData, g_KeyPrevData;
char g_ConvKeyData;
unsigned long g_curTime, g_prevTime=0;

void setup() {
  // put your setup code here, to run once:
InitKeyMatrix(KEY_SIZE, col, row, KEY_MATRIX_COLUMN_BASE);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  g_curTime=millis();
  
  if(g_curTime-g_prevTime>=100)
  {
  g_prevTime=g_curTime;
  
  g_KeyData=ReadKey(KEY_SIZE, col, row, KEY_MATRIX_COLUMN_BASE);
  
  if(g_KeyData!=-1 && g_KeyData!=g_KeyPrevData)
  {
      g_ConvKeyData=ConvKeyToNum(g_KeyData);
      Serial.println(g_ConvKeyData);
  }
  
  g_KeyPrevData=g_KeyData;
  }
}
