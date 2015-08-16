//---------------------------------------------------------------------------


#pragma hdrstop

#include "TA5Serial.h"
#include "stdio.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CTA5Serial g_Balance(2);
//---------------------------------------------------------------------------
__fastcall CTA5Serial::CTA5Serial(int nPort)
{
        m_bInitOK=OpenPort(nPort);

        m_dX[0]=0.0;
        m_dX[1]=0.0;
        m_dY[0]=0.0;
        m_dY[1]=0.0;
}
//---------------------------------------------------------------------------
__fastcall CTA5Serial::~CTA5Serial()
{

}
//---------------------------------------------------------------------------
bool __fastcall CTA5Serial::OpenPort(int nPort)
{
        return Open(nPort,B9600,BIT_8|STOP_1|P_NONE);
}
//---------------------------------------------------------------------------
AnsiString __fastcall CTA5Serial::GetID()
{
        if(!m_bInitOK) return "Error!";
        
        char strSend[]="$ID?\x0d";
        WriteData(strSend);
        char strReceive[100]={0};

        if(ReadData(strReceive,'\x0d'))
        {

                return strReceive;
        }
        else return "Error!!";
}
//---------------------------------------------------------------------------
int __fastcall CTA5Serial::GetData(int nID)
{
         if(!m_bInitOK) return -1;

        char strSend[100]={0};//="$DA01?\x0d";		//DEFAULT ID 01
        sprintf(strSend,"$DA%02d?\x0d",nID);

        WriteData(strSend);
        char strReceive[100]={0};
        char strValue[15]={0};
        if(ReadData(strReceive,'\x0d'))
        {
                if(strReceive[0]!= '$') return -999;
                strncpy(strValue,strReceive+4,7);
                if(strReceive[3]== '-')
                        return -atoi(strValue);
                else return atoi(strValue);
        }
        else return -1;

}
//---------------------------------------------------------------------------
bool __fastcall CTA5Serial::EnableZero(int nID)
{
        if(!m_bInitOK) return false;
        
        char strSend[100]={0};
        sprintf(strSend,"$ZE%02d\x0d",nID);

        WriteData(strSend);
        char strReceive[100]={0};
        char strValue[15]={0};
        if(ReadData(strReceive,'\x0d'))
        {
                if(strReceive[0]!= '$') return false;
                else true;
        }
        else return false;

}
//---------------------------------------------------------------------------
bool __fastcall CTA5Serial::DisableZero(int nID)
{
        if(!m_bInitOK) return false;
        
        char strSend[100]={0};
        sprintf(strSend,"$ZD%02d\x0d",nID);

        WriteData(strSend);
        char strReceive[100]={0};
        char strValue[15]={0};
        if(ReadData(strReceive,'\x0d'))
        {
                if(strReceive[0]!= '$') return false;
                else true;
        }
        else return false;

}
//---------------------------------------------------------------------------
double __fastcall CTA5Serial::GetKg(int nID)
{
        double a=0;
        double x=GetData(nID);

        nTmpData=x;

        if(m_dX[0]==m_dX[1]) a=1;
        else
        {
                a=(m_dY[0]-m_dY[1])/(m_dX[0]-m_dX[1]);
        }
        
        double y=a*x+m_dY[1]-a*m_dX[1];

        return y;
}


