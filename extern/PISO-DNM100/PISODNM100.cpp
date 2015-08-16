//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop 

#include "PISODNM100.h"
#include "DNM100API.h"
#pragma package(smart_init)
CPISODNM100 g_DNPort0(0);
CPISODNM100 g_DNPort1(1);
//---------------------------------------------------------------------------
__fastcall CPISODNM100::CPISODNM100(int ncardNo)
{
        memset(m_nOutputData,0x00,sizeof(__int16)*50);
                //Default MAC ID:0
                //Default Baudrate:125K

        m_nCardNo=ncardNo;
        m_bInitOk=Init();
}
//---------------------------------------------------------------------------
__fastcall CPISODNM100::~CPISODNM100()
{
        //for(int nIndex=0;nIndex<m_nTotalBoards;nIndex++)
        {
                DNM100_StopAllDevice(m_nCardNo);
                DNM100_CloseBoard(m_nCardNo);
        }
}
//---------------------------------------------------------------------------
bool __fastcall CPISODNM100::Init()
{
        DWORD Ret;

        DNM100_TotalDNM100Board(&m_nTotalBoards,m_cBoardIDList);

        if(m_nTotalBoards<1)
        {
                ShowMessage("DeviceNet Card not Found!!"); 
                return false;
        }

        //for(int nIndex=0;nIndex<m_nTotalBoards;nIndex++)
        {
                Ret = DNM100_ActiveBoard(m_nCardNo);
                if(Ret)
                {
                        ShowMessage("DeviceNet ActiveBoard "+FormatFloat("0",m_nCardNo) +"Error !!");
                        return false;
                }

                Ret =DNM100_GetScanList(m_nCardNo,&m_nTotalDevices,&m_cDesMACIDList[0],
                        &m_nTypeList[0],
                        &m_nInputLenList[0],
                        &m_nOutputLenList[0],
                        &m_nEPR_List[0]);

                Ret =DNM100_StartAllDevice(m_nCardNo);
        }

        /*
        m_nOutputData[0]=0x100;
         m_nOutputData[1]=0x500;
       WriteAllData(0);
        m_nOutputData[0]=0x00;
         m_nOutputData[1]=0x100;
       WriteAllData(0);
        */

        return true;
}
//---------------------------------------------------------------------------
int __fastcall CPISODNM100::ReadPollData(int nMacID)
{
        BYTE Data[10];
        WORD Len;
        DWORD Ret;

        Ret = DNM100_ReadInputData(m_nCardNo,nMacID,ConType_Poll,&Len,Data);

        __int16 ReadData=0;

        if(Ret) return 0;
        else
        {
                for(int nIndex=0;nIndex<Len;nIndex++)
                {
                        ReadData+=Data[nIndex]<<(nIndex*8);
                }
        }

        return ReadData;

}
//---------------------------------------------------------------------------
void __fastcall CPISODNM100::ReadAllData()
{
        BYTE Data[100];
        WORD Len;
        DWORD Ret;

        Ret=DNM100_ReadInputArea(m_nCardNo,0,100,Data);

        __int16 ReadData[50]={0};

        if(Ret)
        {

        }
        else
        {
                for(int nIndex=0;nIndex<50;nIndex++)
                {
                        ReadData[nIndex]=(Data[nIndex*2] & 0x00ff)+(Data[nIndex*2+1]<<8 & 0xff00);
                }
        }

        memcpy(m_nInputData,ReadData,sizeof(__int16)*50);
}

//---------------------------------------------------------------------------
bool __fastcall CPISODNM100::WritePollData(int nMacID,__int16 nData)
{
        BYTE Data[2];
        WORD Len;
        DWORD Ret;

        Data[0]=nData & 0x00ff;
        Data[1]=(nData & 0xff00)>>8;

        Ret = DNM100_WriteOutputData(m_nCardNo,nMacID,ConType_Poll,2,Data);

        if(Ret) return false;
        else return true;

}
//---------------------------------------------------------------------------
void __fastcall CPISODNM100::WriteAllData()
{
        BYTE Data[100];
        WORD Len;
        DWORD Ret;

        for(int nIndex=0;nIndex<50;nIndex++)
        {
                Data[nIndex*2]=m_nOutputData[nIndex] & 0x00ff;
                Data[nIndex*2+1]=(m_nOutputData[nIndex] & 0xff00)>>8;
        }

        Ret=DNM100_WriteOutputArea(m_nCardNo,0,100,Data);

}
//---------------------------------------------------------------------------
int __fastcall CPISODNM100::ReadPollData(int nMacID,int nChannel)
{
        BYTE Data[16];
        WORD Len;
        DWORD Ret;

        Ret = DNM100_ReadInputData(m_nCardNo,nMacID,ConType_Poll,&Len,Data);

        __int16 ReadData=0;

        if(Ret) return 0;
        else
        {
                for(int nIndex=0;nIndex<2;nIndex++)
                {
                        ReadData+=Data[nIndex+nChannel*2]<<(nIndex*8);
                }
        }

        return ReadData;

}
//---------------------------------------------------------------------------
void __fastcall CPISODNM100::SetKg(int nIndex,double dKg)
{
        __int16 nData;
        if(dKg<0) dKg=0.0;
        double dData=(dKg/(10.2*0.5))*4096;      //0.5Mpa
        m_nOutputData[nIndex]=dData;//0xfff     //
}
