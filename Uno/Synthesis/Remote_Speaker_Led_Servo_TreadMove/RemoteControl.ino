
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
        g_RemoteCount=0;
    }
    return Decode;
}

unsigned short MaskRemote(unsigned long RemoteData, int MaskType, int ChkError)
{
    unsigned short Reverse, Mask;
    
    if(MaskType==CUSTOMCODE)
    {
        Mask=(RemoteData&0xff000000);
        Reverse=(RemoteData&0x00ff0000);
    }
    
    else if(MaskType==DATACODE)
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
