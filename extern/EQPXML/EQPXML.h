//---------------------------------------------------------------------------

#ifndef EQPXMLH
#define EQPXMLH
#include <vcl>
#include <mem.h>
#include "string.h"
#include "tinyxml.h"
#include "tinystr.h"


#include <ScktComp.hpp>
#include <vector>
#include <list>
//---------------------------------------------------------------------------
class CEQPXML
{
private:
        TCustomWinSocket *m_pSocket;
        //std::vector<AnsiString> m_strCIMMsg;
       unsigned char *receiveRx;
       unsigned char *receive;

public:
        bool m_bOnLine;
        AnsiString m_EqpStatus;       //I:Idle R:Run D:Down
        AnsiString m_CIMStatus;       //0:Offline 1:online/local 2:online/remote

        AnsiString m_strSubstrate2DCode;
        AnsiString m_strCurrentSchedule;

public:
        __fastcall CEQPXML();
        __fastcall ~CEQPXML();

        void __fastcall StartComm( TCustomWinSocket *pSocket);
        void __fastcall EndComm();

        void __fastcall ProcessCIM();
        void __fastcall SendEventReport(char *pEvent,char *pTID="TID");
        void __fastcall SendAlarmMessage(char *pID,char *pText);

        bool (*StartProcess)(bool bStart);
        bool (*OpenFile)(AnsiString strFileName);
        bool (*EventReportAck)(AnsiString strSSID,AnsiString strAckCode);

private:
        void __fastcall doHostStatus(char *pRx);
        void __fastcall doDTSet(char *pRx);
        void __fastcall doCIMMsg(char *pRx);
        void __fastcall doQueryVID(char *pRx);
        void __fastcall doEventReportAck(char *pRx);
        void __fastcall doAlarmAck(char *pRx);
        void __fastcall doRCMD(char *pRx);
        void __fastcall doQueryPPID(char *pRx);
        void __fastcall doQueryPPIDNew(char *pRx);
        void __fastcall doQueryPPBody(char *pRx);
        void __fastcall doSetPPBody(char *pRx);

        void __fastcall SendXML(char *pMSGID,char *pType,char *pName,char *pValue,char *pTID="TID");
        void __fastcall SendXML(TiXmlDocument &theDoc);
};
#endif
