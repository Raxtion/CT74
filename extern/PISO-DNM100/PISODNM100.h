//---------------------------------------------------------------------------

#ifndef PISODNM100H
#define PISODNM100H

#define MAX_DN_CARD 16
#define MAX_DEVICELIST 250
//---------------------------------------------------------------------------

class CPISODNM100
{
private:
        int m_nCardNo;
        
        bool m_bInitOk;
        unsigned char m_nTotalBoards;
        unsigned char m_cBoardIDList[MAX_DN_CARD];
        WORD m_nTotalDevices;
        unsigned char m_cDesMACIDList[MAX_DEVICELIST];
        unsigned char m_nTypeList[MAX_DEVICELIST];
        WORD m_nInputLenList[MAX_DEVICELIST];
        WORD m_nOutputLenList[MAX_DEVICELIST];
        WORD m_nEPR_List[MAX_DEVICELIST];
public:
        __int16 m_nInputData[50]; //for SMC 2Byte * 50
        __int16 m_nOutputData[50]; //for SMC 2Byte * 50

public:
        __fastcall CPISODNM100(int ncardNo);
        __fastcall ~CPISODNM100();

        bool __fastcall Init();

        int __fastcall ReadPollData(int nMacID);   //16bit
        void __fastcall ReadAllData();       //for SMC 2Byte * 50
        bool __fastcall WritePollData(int nMacID,__int16 nData);
        void __fastcall WriteAllData();       //for SMC 2Byte * 50

        int __fastcall ReadPollData(int nMacID,int nChannel);   //16byte for CAN-2017D

        double __fastcall GetKg(int nIndex) {return (m_nInputData[nIndex]/4096.0)*0.5*10.2; };       // MAX 0.5Mpa,1 MPa=10.2Kg/cm2
        double __fastcall GetSetKg(int nIndex) {return (m_nOutputData[nIndex]/4096.0)*0.5*10.2; };       // MAX 0.5Mpa,1 MPa=10.2Kg/cm2
        void __fastcall SetKg(int nIndex,double dKg);

        double __fastcall GetMM(int nMacID,int nChannel) {return ReadPollData(nMacID,nChannel)*(30.0/13106.8)-22.5; };

        void __fastcall ClearOutData(){memset(m_nOutputData,0x00,sizeof(__int16)*50);};



};
#endif
 