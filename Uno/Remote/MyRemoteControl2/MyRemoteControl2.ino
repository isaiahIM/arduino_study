#define CUSTOM_CODE 1
#define DATA_CODE 2

#define TRUE 1
#define FALSE 0


int RemotePin=2;
unsigned short g_CustomCode=0, g_DataCode=0;
 
volatile unsigned long g_RemoteData=0;
volatile int cnt=0;

volatile unsigned long g_PulseStart_us=0, g_PulseEnd_us=0;// Global value

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(0, ISR_ReadData, FALLING);
  
  Serial.begin(9600);
  //digitalWrite(RemotePin, HIGH);
}
void loop() {
  // put your main code here, to run repeatedly:
  
    //g_CustomCode= MaskData(g_RemoteData, CUSTOM_CODE);
    g_DataCode= MaskData(g_RemoteData, DATA_CODE);

    //Serial.println(g_RemoteData);
   
    if(cnt%36==0 && cnt!=0)
    {
      
      switch(g_DataCode)
      {
          case 0x42:
          Serial.println("Push 1 button");
          break;
          
          case 0xc2:
          Serial.println("Push 2 button");
          break;

          case 0xf0:
          Serial.println("Push 3 button");
          break;

          case 0x78:
          Serial.println("Push 4 button");
          break;

          case 0xb8:
          Serial.println("Push 5 button");
          break;

          case 0x38:
          Serial.println("Push 6 button");
          break;

          case 0x18:
          Serial.println("Push 7 button");
          break;

          case 0xa2:
          Serial.println("Push 8 button");
          break;

          case 0x32:
          Serial.println("Push 9 button");
          break;

          case 0x6a:
          Serial.println("Push 0 button");
          break;

          default:
          //Serial.println("Error!! g_RemoteData is "+ g_RemoteData);
          Serial.print("Error!! g_RemoteData is: ");
          Serial.println(g_RemoteData);
          break;
      }
      
    //Serial.println();
    cnt=0;
    }
}

void ISR_ReadData(void)
{
    if(cnt==0)
    {
        g_PulseStart_us=micros();
        cnt+=1;
    }
    else
    {
        g_PulseEnd_us=micros();

        if(g_PulseEnd_us - g_PulseEnd_us>=13000 && g_PulseEnd_us - g_PulseEnd_us<=14000)
        {
            g_RemoteData= 0;
        }
      
        /*bit is 0 */
        else if(g_PulseEnd_us - g_PulseStart_us>=1000 && g_PulseEnd_us - g_PulseStart_us<=1250)
        {
            g_RemoteData=g_RemoteData<<1;
        }
      
        /*bit is 1*/
        else if(g_PulseEnd_us - g_PulseStart_us>=2000 && g_PulseEnd_us - g_PulseStart_us<=2500)
        {
            g_RemoteData=g_RemoteData<<1;
            g_RemoteData|=0x00000001;  
        }
        
        g_PulseStart_us=g_PulseEnd_us;
        cnt+=1;
    }
}


unsigned long DecodeData(unsigned long Data, unsigned long PulseStart_us, unsigned long PulseEnd_us)
{
  /*Lead Code(Start Interface)*/
  if(PulseEnd_us - PulseStart_us>=13000 && PulseEnd_us - PulseStart_us<=14000)
  {
      return 0;
  }

  /*bit is 0 */
  else if(PulseEnd_us - PulseStart_us>=1000 && PulseEnd_us - PulseStart_us<=1250)
  {
      //Data&=0xfffffffe;
      Data=Data<<1;
      //c++;
  }

  /*bit is 1*/
  else if(PulseEnd_us - PulseStart_us>=2000 && PulseEnd_us - PulseStart_us<=2500)
  {
      Data=Data<<1;
      Data|=0x00000001;  
  }
  
  return Data;
}

unsigned short MaskData(unsigned long Data, int MaskType)
{
    unsigned short Decode=0, Reverse=0;
    
    if(MaskType==CUSTOM_CODE)
    {
      Decode= Data&0xff000000;
      Reverse= Data&0x00ff0000;
    }

    else if(MaskType==DATA_CODE)
    {
        Decode=Data&0x0000ff00;
        Reverse=Data&0x000000ff;
        /*
        Serial.println(Decode);
        Serial.println(Reverse);
        Serial.println(Decode&Reverse);
        Serial.println(Decode>>8);
        Serial.println();
        */
    }
    else
    {
        return FALSE;
    }
    
    //Serial.println(Decode&Reverse);
    
    if( (Decode&Reverse)==0x0000 )
    {
      //Serial.println(ChkDecode);
      //Serial.println("p");
    Decode=Decode>>8;
    return Decode;
    }
    
    else
    {
    return FALSE;
    }
    
}
