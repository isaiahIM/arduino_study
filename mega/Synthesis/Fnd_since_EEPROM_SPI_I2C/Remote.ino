#define CUSTOM_CODE 1
#define DATA_CODE 2
#define TRUE 1
#define FALSE 0

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


int MapRemote(unsigned short CustomData)
{
      if(CustomData!=FALSE )
      {
          switch(CustomData)
          {
              case 0x42:
              Serial.println("Push 1 button");
              return 1;
              break;
              
              case 0xc2:
              Serial.println("Push 2 button");
              return 2;
              break;
    
              case 0xf0:
              Serial.println("Push 3 button");
              return 3;
              break;
    
              case 0x78:
              Serial.println("Push 4 button");
              return 4;
              break;
    
              case 0xb8:
              Serial.println("Push 5 button");
              return 5;
              break;
    
              case 0x38:
              Serial.println("Push 6 button");
              return 6;
              break;
    
              case 0x18:
              Serial.println("Push 7 button");
              return 7;
              break;
    
              case 0xa2:
              Serial.println("Push 8 button");
              return 8;
              break;
    
              case 0x32:
              Serial.println("Push 9 button");
              return 9;
              break;
    
              case 0x6a:
              Serial.println("Push 0 button");
              return 0;
              break;
              
              case 0xb0:
              Serial.println("Push Down button");
              return 10;
              break;
              
              case 0xe8:
              Serial.println("Push Up button");
              return 11;
              break;
              default:
              Serial.print("Error!! Check custom data!!: ");
              
              Serial.println(CustomData);
              return -1;
              break;
          }
      }  
}
