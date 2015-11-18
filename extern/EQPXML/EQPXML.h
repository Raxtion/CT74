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
struct LaserDiff
{
    AnsiString strRDLD;
    AnsiString strFDLD;
    AnsiString strRULD;
    AnsiString strFULD;
};

//---------------------------------------------------------------------------
class CEQPXML
{
private:
	TCustomWinSocket *m_pSocket;
	unsigned char *receiveRx;
	unsigned char *receive;

private:
	void __fastcall doHostStatus(char *pRx);
	void __fastcall doDTSet(char *pRx);
	void __fastcall doCIMMsg(char *pRx);
	void __fastcall doEventReportAck(char *pRx);
	void __fastcall doAlarmAck(char *pRx);
	void __fastcall doRCMD(char *pRx);
	void __fastcall doQueryPPID(char *pRx);
	void __fastcall doQueryPPIDNew(char *pRx);
    void __fastcall doQueryPPIDFullPath(char *pRx);
	void __fastcall doQueryPPBody(char *pRx);
	void __fastcall doSetPPBody(char *pRx);

	void __fastcall SendXML(char *pMSGID, char *pType, char *pName, char *pValue, char *pTID = "TID");    //同下 只送特定的MsgID
	void __fastcall SendXML(TiXmlDocument &theDoc);                                                //利用m_pSocket->SendBuf() 送出xml資料

    void __fastcall generateLaserDiff(LaserDiff &Inp);

public:
    std::list<AnsiString> m_strCIMMsgLog;
	void __fastcall doQueryVID(char *pRx);
	bool m_bOnLine;
	AnsiString m_EqpStatus;                                                                        //I:Idle R:Run D:Down
	AnsiString m_CIMStatus;                                                                        //0:Offline 1:online/local 2:online/remote
	AnsiString m_strSubstrate2DCode;
	AnsiString m_strCurrentSchedule;
    std::list<AnsiString> m_liststrForderName;
    std::list<AnsiString> m_liststrFileName;

public:
	__fastcall CEQPXML();
	__fastcall ~CEQPXML();

	void __fastcall StartComm( TCustomWinSocket *pSocket);                                         //將外部*pSocket轉入,m_pSocket
	void __fastcall EndComm();                                                                     //m_pSocket = NULL
	void __fastcall ProcessCIM();                                                                  //處理來自CIM的資料，取得strMsgID 依對應的function動作
	void __fastcall SendEventReport(char *pEvent,char *pTID="TID");                                //SendXML("EVENT_REPORT","RPT","CEID",pEvent,pTID);
	void __fastcall SendAlarmMessage(char *pID,char *pText);                                       //SendAlarmMessage("E0051","LoadCell is not at Safe Location") 實作在Timer1 Error Code

	bool (*StartProcess)(bool bStart);
	bool (*OpenFile)(AnsiString strFileName);                                                      
	bool (*EventReportAck)(AnsiString strSSID,AnsiString strAckCode);                              //接收到回傳的EventReportAck時,要做的事情.一般收到Ack是不做反應的,只用於追查訊息傳遞

};
#endif

