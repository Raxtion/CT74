// MyPISODIO.cpp: implementation of the	CMyPISODIO class.
//
//////////////////////////////////////////////////////////////////////


#include "MyPISODIO.h"
#include "dask.h"
#include "math.h"
   
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]	= __FILE__;
#define	new DEBUG_NEW
#endif


CMyPISODIO g_DIO;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
CMyPISODIO::CMyPISODIO()
{
    m_bIOCardReady = FALSE;
    InitCard();
}
//---------------------------------------------------------------------------
CMyPISODIO::~CMyPISODIO()
{
    if (!m_bIOCardReady)
	return;
	
    U32 nDataA=0x00,nDataB=0x00;    
    DO_WritePort(m_nCardID, P7442_CH0, nDataA);
    DO_WritePort(m_nCardID, P7442_CH1, nDataB);
    
    Release_Card(0);
}
//---------------------------------------------------------------------------
BOOL CMyPISODIO::InitCard()
{
    if ((m_nCardID=Register_Card (PCI_7442, 0)) <0 ) return false;
    
    m_bIOCardReady = TRUE;
    return TRUE;
}
//---------------------------------------------------------------------------
BOOL CMyPISODIO::ReadDIBit(WORD	nBitNo)
{
    if (!m_bIOCardReady)
	return FALSE;
	    
    U32 nData;
    WORD nBit;
    if(nBitNo<32) 
    {    	
    	DI_ReadPort(m_nCardID,  P7442_CH0, &nData);
    	nBit=nBitNo;
    }
    else 
    {
    	DI_ReadPort(m_nCardID,  P7442_CH1, &nData);    	
    	nBit=nBitNo-32;
    }
    
    return (nData & (U32) pow(2, nBit));
}
//---------------------------------------------------------------------------
void CMyPISODIO::WriteDOBitOFF(WORD nBitNo)
{
    if (!m_bIOCardReady)
	return;   
    
    U32 nDataA=0,nDataB=0;    
    DO_ReadPort(m_nCardID, P7442_CH0, &nDataA);
    DO_ReadPort(m_nCardID, P7442_CH1, &nDataB);
    
    if(nBitNo<32)
    {
    	nDataA &= ~(U32) pow(2, nBitNo);
        DO_WritePort(m_nCardID, P7442_CH0, nDataA);
    }
    else
    {
    	nDataB &= ~(U32) pow(2, nBitNo-32);
        DO_WritePort(m_nCardID, P7442_CH1, nDataB);
    }



}
//---------------------------------------------------------------------------
void CMyPISODIO::WriteDOBitON(WORD nBitNo)
{
    if (!m_bIOCardReady)
	return; 
    
    if (!m_bIOCardReady)
	return;   
    
    U32 nDataA,nDataB;
    DO_ReadPort(m_nCardID, P7442_CH0, &nDataA);
    DO_ReadPort(m_nCardID, P7442_CH1, &nDataB);
    
    if(nBitNo<32)
    {
    	nDataA |= (U32) pow(2, nBitNo);
        DO_WritePort(m_nCardID, P7442_CH0, nDataA);
    }
    else
    {
    	nDataB |= (U32) pow(2, nBitNo-32);
        DO_WritePort(m_nCardID, P7442_CH1, nDataB);
    }



}
//---------------------------------------------------------------------------
BOOL CMyPISODIO::ReadDOBit(WORD	nBitNo)
{
    if (!m_bIOCardReady)
	return false;
	    
    U32 nDataA,nDataB;
    DO_ReadPort(m_nCardID, P7442_CH0, &nDataA);
    DO_ReadPort(m_nCardID, P7442_CH1, &nDataB);
    
    U32 nBit;
    U32 nData;
    if(nBitNo<32)
    {
        nBit=nBitNo;	
        nData=nDataA;
    }
    else
    {
    	nBit=nBitNo-32;
    	nData=nDataB;
    }
    
    return ( nData & (U32) pow(2, nBit));    
}
//---------------------------------------------------------------------------
BOOL CMyPISODIO::WaitDIBitON(WORD nBitNo, UINT nTimeOut)
{
    DWORD dwStartTime, dwElapseTime;
    dwStartTime	= GetTickCount();
    while (1)
    {
	if (ReadDIBit(nBitNo))
	    return TRUE;
	dwElapseTime = GetTickCount() -	dwStartTime;
	if (nTimeOut < INFINITE_TIME)
	    if (dwElapseTime > 0 && dwElapseTime >= nTimeOut)
		break;

	::Sleep(10);
    }

    return FALSE;
}
//---------------------------------------------------------------------------
BOOL CMyPISODIO::WaitDIBitOFF(WORD nBitNo, UINT	nTimeOut)
{
    DWORD dwStartTime, dwElapseTime;
    dwStartTime	= GetTickCount();
    while (1)
    {
	if (!ReadDIBit(nBitNo))
	    return TRUE;
	dwElapseTime = GetTickCount() -	dwStartTime;
	if (nTimeOut < INFINITE_TIME)
	    if (dwElapseTime > 0 && dwElapseTime >= nTimeOut)
		break;

	::Sleep(10);
    }

    return FALSE;
}
//---------------------------------------------------------------------------
void __fastcall CMyPISODIO::SetDO(int nBitNo,bool bOnOff)
{
        if(bOnOff) WriteDOBitON(nBitNo);
        else WriteDOBitOFF(nBitNo);
}
//---------------------------------------------------------------------------
unsigned long __fastcall CMyPISODIO::ReadDIPort(int nPort)
{
        if (!m_bIOCardReady)
	return 0;

        U32 nData=0;

        switch(nPort)
        {
                case 0:
                        DI_ReadPort(m_nCardID, P7442_CH0, &nData);
                        break;
                case 1:
                        DI_ReadPort(m_nCardID, P7442_CH1, &nData);
                        break;
        }

        return nData;
}
//---------------------------------------------------------------------------
unsigned long __fastcall CMyPISODIO::ReadDOPort(int nPort)
{
         if (!m_bIOCardReady)
	return 0;

        U32 nData=0;

        switch(nPort)
        {
                case 0:
                        DO_ReadPort(m_nCardID, P7442_CH0, &nData);
                        break;
                case 1:
                        DO_ReadPort(m_nCardID, P7442_CH1, &nData);
                        break;
        }

        return nData;
}
//---------------------------------------------------------------------------
void __fastcall CMyPISODIO::WriteDOPort(int nPort,unsigned long nData)
{
        if (!m_bIOCardReady)
	return ;

       // U32 nData=0;

        switch(nPort)
        {
                case 0:
                        DO_WritePort(m_nCardID, P7442_CH0, nData);
                        break;
                case 1:
                        DO_WritePort(m_nCardID, P7442_CH1, nData);
                        break;
        }

        return ;
}
//---------------------------------------------------------------------------

void __fastcall CMyPISODIO::StartLamp()
{
        /*
     SetDO(DO::LampRed,false);
     SetDO(DO::LampGreen,true);
     SetDO(DO::LampYellow,false);

     SetDO(DO::StartLamp,true);
     SetDO(DO::StopLamp,false);
     SetDO(DO::ResetLamp,false);
     */
}
 //---------------------------------------------------------------------------
void __fastcall CMyPISODIO::StopLamp()
{
        /*
     SetDO(DO::LampRed,false);
     SetDO(DO::LampGreen,false);
     SetDO(DO::LampYellow,true);

     SetDO(DO::StartLamp,false);
     SetDO(DO::StopLamp,true);
     SetDO(DO::ResetLamp,false);
     */
}
 //---------------------------------------------------------------------------
void __fastcall CMyPISODIO::ResetLamp()
{
        /*
     SetDO(DO::LampRed,false);
     SetDO(DO::LampGreen,true);
     SetDO(DO::LampYellow,false);
     SetDO(DO::StartLamp,false);
     SetDO(DO::StopLamp,false);
     SetDO(DO::ResetLamp,true);
     */
}
 //---------------------------------------------------------------------------

void __fastcall CMyPISODIO::AlarmLamp()
{
        /*
     SetDO(DO::LampRed,true);
     SetDO(DO::LampGreen,false);
     SetDO(DO::LampYellow,false);
        */
}
