
unsigned long DecodeRemote(unsigned long RemoteData, unsigned long StartTime, unsigned long EndTime)
{
    unsigned long Decode=RemoteData;
    
    /*Lead Code(RemoteData clear) */
    if(EndTime-StartTime>=13000 && EndTime-StartTime<=14000)
    {
        return 0;
    }

    /*bit is 1*/
    else if(EndTime-StartTime>=2000 && EndTime-StartTime<=2500)
    {
        Decode=Decode<<1;
        Decode|=0x00000001;
        
    }

    /*bit is 0*/
    else if(EndTime-StartTime>=1000 && EndTime-StartTime<=1250)
    {
        Decode=Decode<<1;
    }

    else
    {
        //g_RemoteCount=0;
        Decode=0;
    }
    return Decode;
}

unsigned short MaskRemote(unsigned long RemoteData, int MaskType, int ChkError)
{
    unsigned short Reverse, Mask;
    
    if(MaskType==CUSTOM_CODE)
    {
        Mask=(RemoteData&0xff000000);
        Reverse=(RemoteData&0x00ff0000);
    }
    
    else if(MaskType==DATA_CODE)
    {
        Mask=(RemoteData&0x0000ff00);
        Reverse=(RemoteData&0x000000ff);
    }
    
    if(ChkError==REVERSE_CHECK)
    {
        if(Mask&Reverse!=0)
        {
            Mask=0;
        }
    }
    
    Mask=Mask>>8;
    return Mask;
}

int MapRemoteDataToNum(unsigned int remoteData)
{
  int mappingData;
  switch(remoteData)
      {
          /*1*/
          case 0x42:
          mappingData=1;
          break;

          /*2*/
          case 0xc2:
          mappingData=2;
          break;

          /*3*/
          case 0xf0:
          mappingData=3;
          break;

          /*4*/
          case 0x78:
          mappingData=4;
          break;

          /*5*/
          case 0xb8:
          mappingData=5;
          break;

          /*6*/
          case 0x38:
           mappingData=6;
          break;

          /*7*/
           mappingData=7;
          break;

          /*8*/
          case 0xa2:
          mappingData=8;
          break;

          /*9*/
          case 0x32:
           mappingData=9;
          break;

          /*0*/
          case 0x6a:
          mappingData=0;
          break;

          default:
          mappingData=-1;
          break;
      }

      return mappingData;
}
