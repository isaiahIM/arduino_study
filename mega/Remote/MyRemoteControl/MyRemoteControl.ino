#define CUSTOM_CODE 1
#define DATA_CODE 2

#define TRUE 1
#define FALSE 0


int RemotePin=2;
volatile unsigned long g_RemoteData=0, g_CustomCode=0, g_DataCode=0;
volatile int cnt=0;

volatile unsigned long g_PulseStart_us=0, g_PulseEnd_us=0;// Global value

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(4, ISR_ReadData, FALLING);
  
  Serial.begin(9600);
  //digitalWrite(RemotePin, HIGH);
}
void loop() {
  // put your main code here, to run repeatedly:
    //g_RemoteData= DecodeData(g_RemoteData, g_PulseStart_us, g_PulseEnd_us);
    g_CustomCode= MaskData(g_RemoteData, CUSTOM_CODE);
    g_DataCode= MaskData(g_RemoteData, DATA_CODE);

    //Serial.println(g_RemoteData);
   
    if(cnt%36==0 && cnt!=0)
    {
    Serial.println(g_RemoteData);
    Serial.println();
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
        g_RemoteData= DecodeData(g_RemoteData, g_PulseStart_us, g_PulseEnd_us);
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

unsigned long MaskData(unsigned long Data, int MaskType)
{
    unsigned long Decode=0, Reverse=0;
    
    if(MaskType==CUSTOM_CODE)
    {
      Decode= Data&0xff000000;
      Reverse= Data&0x00ff0000;
    }

    else if(MaskType==DATA_CODE)
    {
        Decode=Data&0x0000ff00;
        Reverse=Data&0x000000ff;
    }
    else
    {
        return FALSE;
    }
    
    if( Reverse&Decode==0)
    {
    return Decode;
    }
    
    else
    {
    return FALSE;
    }
}
