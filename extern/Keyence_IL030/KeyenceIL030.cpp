//---------------------------------------------------------------------------


#pragma hdrstop

#include "KeyenceIL030.h"
#include "GlobalFunction.h"
#include "stdio.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

CKeyenceIL030 g_IL030SerialUp(9);
CKeyenceIL030 g_IL030SerialDown(8);
//---------------------------------------------------------------------------
__fastcall CKeyenceIL030::CKeyenceIL030(int nPort)
{
        m_bInitOK=OpenPort(nPort);
}
//---------------------------------------------------------------------------
__fastcall CKeyenceIL030::~CKeyenceIL030()
{

}
//---------------------------------------------------------------------------
bool __fastcall CKeyenceIL030::OpenPort(int nPort)
{
        return Open(nPort,B9600,BIT_8|STOP_1|P_NONE);
}
//---------------------------------------------------------------------------
double __fastcall CKeyenceIL030::GetLaserValue()
{
        double dResultValue = 999.999;
        if(!m_bInitOK) return dResultValue;


        AnsiString strSend = "SR,00,038\r\n";
        WriteData(strSend.c_str());
        char strReceive[100]={0};

        if(ReadData(strReceive,'\n'))
        {
            AnsiString strResult = strReceive;
            TStringList *StrList = SplitString(strResult, ",");
            AnsiString strValue = StrList->Strings[3];
            ReplaceString(&strValue, "\r\n", "");
            dResultValue = strValue.ToDouble();
            delete StrList;

            return dResultValue;
        }
        else return dResultValue;
}
//---------------------------------------------------------------------------

