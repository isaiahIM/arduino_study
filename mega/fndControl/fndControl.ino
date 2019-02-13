#define FND_HIGH 0
#define FND_LOW 1
#define FND_POWER_ON 1
#define FND_POWER_OFF 0

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
    {FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_HIGH ,FND_HIGH, FND_HIGH, FND_LOW},// 8
    {FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_LOW}// 9
};
 int g_i, g_Num=0;
int g_FndPower[2]={32, 23};

unsigned long g_Time, g_Digit_10Time=0, g_Digit_1Time=0, g_CountTime=0;

void setup() { 
  
    for(g_i=0; g_i<8; g_i++)
    {
        pinMode(g_FndPin[g_i], OUTPUT);
    }
    
    for(g_i=0; g_i<2; g_i++)
    {
        pinMode(g_FndPower[g_i], OUTPUT);
    }

    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    
    g_Time=millis();
    
    if(g_Num>=100)
    {
        g_Num=0;
    }
    
    DisplayFnd(g_Num, g_FndPower, g_Fnd, g_FndPin, g_Time, &g_Digit_10Time, &g_Digit_1Time);
   
   if( g_Time-g_CountTime>1000)
   {
      g_CountTime=g_Time;
      //Serial.println(g_Num);
      g_Num++;
   }
}

