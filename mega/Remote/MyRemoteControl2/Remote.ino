unsigned long DecodeData(unsigned long Data, unsigned long PulseStart_us, unsigned long PulseEnd_us)
{
  /*Lead Code(Start Interface)*/
  if(PulseEnd_us - PulseStart_us>=13000 && PulseEnd_us - PulseStart_us<=14000)
  {
      retun 0;
  }r

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
//        Serial.println(Decode);
//        Serial.println(Reverse);
//        Serial.println(Decode&Reverse);
//        Serial.println();
    }
    else
    {
        return FALSE;
    }
    
    //Serial.println(Decode&Reverse);
    
    if( (Decode&Reverse)==0 )
    {
      if(MaskType==CUSTOM_CODE)
      {
        Decode=Decode>>32;
      }

      else if(MaskType==DATA_CODE)
      {
          Decode=Decode>>8;
      }
    //Decodoe=Decode>>8;
    return (unsigned short)Decode;
    }
    else
    {
    return FALSE;
    }
    
}
