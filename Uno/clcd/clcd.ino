int i;
const int dataPin[8]={9, 8, 7, 6, 5, 4, 3, 2};
const int rs=11;
const int en=10;
char data[12]={"hello world"};
void setup() {
  // put your setup code here, to run once:
  for(i=0; i<8; i++)
  {
    pinMode(dataPin[i], OUTPUT);
    digitalWrite(dataPin, LOW);
  }
  pinMode(rs, OUTPUT);
  digitalWrite(rs, 0);
  
  pinMode(en, OUTPUT);
  digitalWrite(en, 0);
  
  CLCD_Config();
  CLCD_DisplayString(data, 11);
//  CLCD_Display('a');
}

void loop() {
  // put your main code here, to run repeatedly:
//CLCD_Display(0x38);
}


void CLCD_Write(uint8_t data)
{
  int i, mask=0x01;
  
  digitalWrite(en, HIGH);
  for(i=0; i<8; i++)
  {
    if( (data&mask)!=0)
    {
      digitalWrite(dataPin[i], HIGH);
    }
    else
    {
      digitalWrite(dataPin[i], LOW);
    }
    mask<<=1;
  }
  digitalWrite(en ,LOW);
}

void CLCD_Config(void)
{
  CLCD_Write(0X38);
  CLCD_Write(0x0f);
  CLCD_Write(0x06);
}

void CLCD_Display(uint8_t data)
{
  digitalWrite(rs, HIGH);
  CLCD_Write(data);
  digitalWrite(rs, LOW);
}

void CLCD_DisplayString(uint8_t data[], int Size)
{
  int i;
  for(i=0; i<Size; i++)
  {
    CLCD_Display(data[i]);
  }
}
