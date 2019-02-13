#define FND_HIGH 0
#define FND_LOW 1
#define FND_POWER_ON 1
#define FND_POWER_OFF 0

int FndPin[8]={4, 5, 6, 7, 8, 9, 10, 11};
int Fnd[10][8]=
{
    {FND_LOW, FND_LOW, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH},// 0
    {FND_LOW, FND_LOW, FND_LOW, FND_LOW, FND_LOW, FND_LOW, FND_HIGH, FND_HIGH},// 1
    {FND_HIGH, FND_LOW, FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_LOW, FND_HIGH},// 2
    {FND_HIGH, FND_LOW, FND_HIGH, FND_LOW, FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH},// 3
    {FND_HIGH, FND_LOW, FND_LOW, FND_HIGH, FND_LOW, FND_LOW, FND_HIGH, FND_HIGH},// 4, 
    {FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW, FND_HIGH, FND_LOW},// 5
    {FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW},// 6
    {FND_LOW, FND_LOW, FND_HIGH, FND_HIGH, FND_LOW, FND_LOW, FND_HIGH, FND_HIGH},// 7
    {FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_HIGH ,FND_HIGH, FND_HIGH, FND_HIGH},// 8
    {FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH, FND_HIGH, FND_LOW, FND_HIGH, FND_HIGH}// 9
};
int i, j, Num=0;
int FndPower[2]={12, 13};

unsigned long Time, Digit_10Time=0, Digit_1Time=0;
void setup() { 
  
    for(i=0; i<8; i++)
    {
        pinMode(FndPin[i], OUTPUT);
    }
    
    for(i=0; i<2; i++)
    {
        pinMode(FndPower[i], OUTPUT);
    }

    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    Time=millis();
    if(Num>=100)
    {
        Num=0;
    }
    Serial.println(Num);
    DisplayFnd(Num, FndPower, Fnd, FndPin, Time, &Digit_10Time, &Digit_1Time);
   
   if( (Time)%1000<10)
   {
    Serial.println(Num);
      Num+=1;
   }
}

void DisplayFnd(int Num, int *FndPower, int FndFont[][8], int FndPin[], unsigned long Time, unsigned long *Digit_10Time, unsigned long *Digit_1Time)
{
    int Digit_10=0;
    
    if(Num/10)
    {
        Digit_10=Num/10;
        Num-= (Digit_10*10);
    }

    if(Time - (*Digit_10Time)>=10)
    {
    *Digit_10Time=Time;
    digitalWrite(FndPower[0], FND_POWER_OFF);
    digitalWrite(FndPower[1], FND_POWER_ON);
    
    /*10 Digit Display*/
    for(i=0; i<8; i++)
    {
        digitalWrite(FndPin[i], FndFont[Digit_10][i]);
    }
    
    }
    
    if( (*Digit_10Time) - (*Digit_1Time) >=10)
    {
     *Digit_1Time=*Digit_10Time;
    digitalWrite(FndPower[0], FND_POWER_ON);
    digitalWrite(FndPower[1], FND_POWER_OFF);
    
    /*1 Digit Display*/
    for(i=0; i<8; i++)
    {
        digitalWrite(FndPin[i], FndFont[Num][i]);
    }
    
    }

   // Serial.println(Num);
   // Serial.println(Digit_10);
   // Serial.println();
    
}
