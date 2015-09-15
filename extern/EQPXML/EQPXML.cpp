//---------------------------------------------------------------------------


#pragma hdrstop

#include "EQPXML.h"
#include "IniFile.h"
CEQPXML g_eqpXML;
extern CIniFile g_IniFile;


#define MAX_SEND_BUFF 2500
//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall CEQPXML::CEQPXML()
{
        m_pSocket=NULL;
        m_EqpStatus="I";
        m_CIMStatus="0";
receiveRx=(char *)malloc(30000);//new char[30000];
        receive=(char *)malloc(30000);//new char[30000];

        m_strSubstrate2DCode="SSID";
}
//---------------------------------------------------------------------------
__fastcall CEQPXML::~CEQPXML()
{
        free( receiveRx);
        free( receive);
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::StartComm( TCustomWinSocket *pSocket)
{
        m_pSocket=pSocket;
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::EndComm()
{
        m_pSocket=NULL;
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::ProcessCIM()
{
        long nSize=0;
        //char *receiveRx;
        //char *receive;

        try
        {

        //receiveRx=(char *)VirtualAlloc(NULL, 30000, MEM_COMMIT, PAGE_READWRITE);//(char *)malloc(30000);//new char[30000];
        //receive=(char *)VirtualAlloc(NULL, 30000, MEM_COMMIT, PAGE_READWRITE);///(char *)malloc(30000);//new char[30000];

        //char receive[30000];
        if(receive==NULL || receiveRx==NULL) return;

        memset(receiveRx,0x00,30000);
        memset(receive,0x00,30000);

        m_pSocket->ReceiveBuf(receiveRx,30000);
        //AddLog(receiveRx,30000);

        TiXmlDocument doc;

        int nIndex=0;

        while(1)
        {
                doc.Clear();
                memset(receive,0x00,30000);
                
                if(nIndex>=30000) break;

                nSize=(receiveRx[0+nIndex]<<24)+(receiveRx[1+nIndex]<<16)+(receiveRx[2+nIndex]<<8)+receiveRx[3+nIndex]+4;//+5;
                 if(nSize<=5 || nSize>=30000)
                        break;

                 AnsiString strXX=FormatFloat("Size=0",nSize);
                //AddLog(strXX.c_str(),strXX.Length());

                memcpy(receive,receiveRx+nIndex,nSize);
                nIndex+=nSize;
                if(nIndex>=30000) break;

                //AddLog(receive,nSize);

                doc.Parse(receive+5);
                TiXmlElement* pRoot = doc.FirstChildElement("Root");
                if(pRoot==NULL) break;
                //TiXmlElement* pData=pRoot->FirstChildElement("DATA");
        
                AnsiString strMsgID(pRoot->Attribute("MSGID"));
                //AnsiString strData=pData->GetText();

                if(strMsgID=="HOST_STATUS") doHostStatus(receive+5);
                else if(strMsgID=="DTSET") doDTSet(receive+5);
                else if(strMsgID=="CIMMSG") doCIMMsg(receive+5);
                else if(strMsgID=="QUERY_VID") doQueryVID(receive+5);
                else if(strMsgID=="EVENT_REPORT") doEventReportAck(receive+5);
                else if(strMsgID=="ALARM") doAlarmAck(receive+5);
                else if(strMsgID=="RCMD") doRCMD(receive+5);
                else if(strMsgID=="QUERY_PPID") doQueryPPIDNew(receive+5);
                else if(strMsgID=="QUERY_PPBODY") doQueryPPBody(receive+5);
                else if(strMsgID=="SET_PPBODY") doSetPPBody(receive+5);                

        }

       //delete receive;
       //delete receiveRx;
       
       }
       catch(const EAccessViolation &e)
       {
        //Application->MessageBox(e.Message.c_str(), "Look", MB_OK);
        g_IniFile.AddLog("catch ProcessCIM",16);
        //delete receive;

       }

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::SendXML(char *pMSGID,char *pType,char *pTextName,char *pValue,char *pTID)
{
        char send[MAX_SEND_BUFF]={0};
        char A[MAX_SEND_BUFF]={0};

        try
        {
        sprintf(A,"<?xml version=\"1.0\" encoding=\"utf-8\" ?><Root MSGID=\"%s\" TYPE=\"%s\" TID=\"%s\" ><%s>%s</%s></Root>",pMSGID,pType,pTID,pTextName,pValue,pTextName);
        
        sprintf(send,"%c%c%c%c%c%s",0x00,0x00,0x00,strlen(A)+1,0x00,A);


       if(m_pSocket!=NULL && m_bOnLine)
       {
                m_pSocket->SendBuf(send,strlen(A)+5);
                //m_pSocket->SendBuf(send,MAX_SEND_BUFF);
                //g_IniFile.AddLog(send,MAX_SEND_BUFF);
       }
       }
       catch(const EAccessViolation &e)
       {
        Application->MessageBox(e.Message.c_str(), "Look2", MB_OK);

       }

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doHostStatus(char *pRx)
{
        TiXmlDocument doc;
        doc.Parse(pRx);
        TiXmlElement* pRoot = doc.FirstChildElement("Root");
        TiXmlElement* pData=pRoot->FirstChildElement("DATA");

        AnsiString strMsgID(pRoot->Attribute("MSGID"));
        AnsiString strData=pData->GetText();

        if(strData=="1") m_CIMStatus="1";
        else if(strData=="0") m_CIMStatus="0";
        else if(strData=="2") m_CIMStatus="2";
        else m_CIMStatus="0";

        //SendXML("HOST_STATUS","ACK","ACK","");

        pRoot->SetAttribute("TYPE","ACK");
        pData->SetValue("ACK");
        TiXmlNode *pNode3=pData->FirstChild();
        pNode3->SetValue("");

        SendXML(doc);

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doDTSet(char *pRx)
{
        TiXmlDocument doc;
        doc.Parse(pRx);
        TiXmlElement* pRoot = doc.FirstChildElement("Root");
        TiXmlElement* pData=pRoot->FirstChildElement("DATA");

        AnsiString strMsgID(pRoot->Attribute("MSGID"));
        AnsiString strData=pData->GetText();
        //Set System Date Time Here

        SendXML("DTSET","ACK","ACK","0");           //0:OK 1:NG
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doCIMMsg(char *pRx)
{
        TiXmlDocument doc;
        doc.Parse(pRx);
        TiXmlElement* pRoot = doc.FirstChildElement("Root");
        TiXmlElement* pData=pRoot->FirstChildElement("DATA");

        AnsiString strMsgID(pRoot->Attribute("MSGID"));
        AnsiString strData=pData->GetText();
        //do something whith that message
        //m_strCIMMsg.push_back(strData);

        //g_pAutoRunThread->AddMessage(strData,g_pAutoRunThread->m_bIsRunning);

        //if(!g_pAutoRunThread->m_bIsRunning) g_pAutoRunThread->ShowAlarmMessage();

        SendXML("CIMMSG","ACK","ACK","0");           //0:OK 1:NG
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::SendEventReport(char *pEvent,char *pTID)
{

        SendXML("EVENT_REPORT","RPT","CEID",pEvent,pTID);

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::SendXML(TiXmlDocument &theDoc)
{
try
{
         TiXmlPrinter printer;
        theDoc.Accept(&printer);
        const char *NewString=printer.CStr();

        long nLen=strlen(NewString)+1;
        //char *send=new char[strlen(NewString)+5];
        char *send=new char[MAX_SEND_BUFF];

        char strLen[4]={0};
        strLen[0]=(nLen & 0xff000000)>>24;
        strLen[1]=(nLen & 0x00ff0000)>>16;
        strLen[2]=(nLen & 0x0000ff00)>>8;
        strLen[3]=(nLen & 0x000000ff);

        sprintf(send,"%c%c%c%c%c%s",strLen[0],strLen[1],strLen[2],strLen[3],0x00,NewString);

       if(m_pSocket!=NULL && m_bOnLine)
       {
                m_pSocket->SendBuf(send,strlen(NewString)+5);
                //m_pSocket->SendBuf(send,MAX_SEND_BUFF);
                //g_IniFile.AddLog("Send XML",8);
       }
       delete send;
}
catch(const EAccessViolation &e)
       {
        Application->MessageBox(e.Message.c_str(), "Look3", MB_OK);
        //delete send;
       }
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doQueryVID(char *pRx)
{
         TiXmlDocument doc;
        doc.Parse(pRx);
        TiXmlElement* pRoot = doc.FirstChildElement("Root");
        TiXmlElement* pData=pRoot->FirstChildElement("DATA");

        AnsiString strMsgID(pRoot->Attribute("MSGID"));
        AnsiString strData=pData->GetText();

        const char *pTID=pRoot->Attribute("TID");
        const char *pCEID=pData->Attribute("CEID");
        const char *pSxFy=pData->Attribute("SxFy");
        //do something whith that message

        pRoot->SetAttribute("TYPE","ACK");
        TiXmlElement* pSVID;
        TiXmlElement* pID;
        TiXmlElement* pValue;

        char SVID[13][100]={0};
       TDateTime DT=TDateTime::CurrentDateTime();

       strcpy(SVID[0],DT.FormatString("yyyymmddhhnnss").c_str());
       //strcpy(SVID[1],"CT-11");
       //strcpy(SVID[2],"01");
       //strcpy(SVID[3],"4.0.0.8");
       //strcpy(SVID[4],"OP_ID");
       //strcpy(SVID[5],g_IniFile.m_strLastFileName.c_str());
       //strcpy(SVID[6],m_EqpStatus.c_str());             //Equipment process state (I=Idle, R=Run, D=Down)
       //strcpy(SVID[7],m_CIMStatus.c_str());             //CIM control state (0=Offline, 1=Online/Local, 2=Online/Remote)
       //strcpy(SVID[8],g_IniFile.m_strScheduleNo.c_str());
       //strcpy(SVID[9],m_strSubstrate2DCode.c_str());
       //strcpy(SVID[10],FormatFloat("0",g_IniFile.m_nScheduleQty).c_str());
       //strcpy(SVID[11],FormatFloat("0",g_IniFile.m_bUseLaser).c_str());
       //strcpy(SVID[12],m_strCurrentSchedule.c_str());


       

        for(int nIndex=0;nIndex<13;nIndex++)
        {
        pSVID=new TiXmlElement("SVID");
                pID=new TiXmlElement("ID");
                pID->LinkEndChild(new TiXmlText(FormatFloat("0",nIndex+1).c_str()));
                pValue=new TiXmlElement("VALUE");
                pValue->LinkEndChild(new TiXmlText(SVID[nIndex]));
        pSVID->LinkEndChild(pID);
        pSVID->LinkEndChild(pValue);
        pData->LinkEndChild(pSVID);
        }

        char ECID[9][100]={0};
        //strcpy(ECID[0],FormatFloat("0",g_IniFile.m_bUseSSPicker).c_str());
        //strcpy(ECID[1],FormatFloat("0",g_IniFile.m_bUseCoverPicker).c_str());
        //strcpy(ECID[2],FormatFloat("0",g_IniFile.m_bUseCoverLifter).c_str());
        //strcpy(ECID[3],FormatFloat("0",g_IniFile.m_bUseSafetyDoor).c_str());
        //strcpy(ECID[4],FormatFloat("0",g_IniFile.m_nPickSSDelayTime).c_str());
        //strcpy(ECID[5],FormatFloat("0",g_IniFile.m_nPickSpacerDelayTime).c_str());
        //strcpy(ECID[6],FormatFloat("0",g_IniFile.m_nPickSpacerUpDelayTime).c_str());
        //strcpy(ECID[7],FormatFloat("0",g_IniFile.m_nPickSSUpDelayTime).c_str());
        //strcpy(ECID[8],FormatFloat("0",g_IniFile.m_dSSLifterRelMoveDis).c_str());

        for(int nIndex=0;nIndex<9;nIndex++)
        {
        pSVID=new TiXmlElement("ECID");
                pID=new TiXmlElement("ID");
                pID->LinkEndChild(new TiXmlText(FormatFloat("0",nIndex+501).c_str()));
                pValue=new TiXmlElement("VALUE");
                pValue->LinkEndChild(new TiXmlText(ECID[nIndex]));
        pSVID->LinkEndChild(pID);
        pSVID->LinkEndChild(pValue);
        pData->LinkEndChild(pSVID);
        }

        char DVID[1][100]={0};
        strcpy(DVID[1],"60");

        for(int nIndex=0;nIndex<1;nIndex++)
        {
        pSVID=new TiXmlElement("DVID");
                pID=new TiXmlElement("ID");
                pID->LinkEndChild(new TiXmlText(FormatFloat("0",nIndex+1001).c_str()));
                pValue=new TiXmlElement("VALUE");
                pValue->LinkEndChild(new TiXmlText(DVID[nIndex]));
        pSVID->LinkEndChild(pID);
        pSVID->LinkEndChild(pValue);
        pData->LinkEndChild(pSVID);
        }


       SendXML(doc);

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doEventReportAck(char *pRx)
{
        TiXmlDocument doc;
        doc.Parse(pRx);
        TiXmlElement* pRoot = doc.FirstChildElement("Root");
        TiXmlElement* pData=pRoot->FirstChildElement("ACK");
        const char *pTID=pRoot->Attribute("TID");
        const char *pAckText=pData->GetText();

        if(pTID && pAckText) EventReportAck(pTID,pAckText);
        
        //do something for that
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::SendAlarmMessage(char *pID,char *pText)
{
        TiXmlDocument doc;
        TiXmlElement* pRoot;

        pRoot=new TiXmlElement("Root");
        pRoot->SetAttribute("MSGID","ALARM");
        pRoot->SetAttribute("TYPE","RPT");

        TiXmlElement *pIDE=new TiXmlElement("ID");
        pIDE->LinkEndChild(new TiXmlText(pID));

         TiXmlElement *pTxt=new TiXmlElement("TEXT");
        pTxt->LinkEndChild(new TiXmlText(pText));

        pRoot->LinkEndChild(pIDE);
        pRoot->LinkEndChild(pTxt);

        doc.LinkEndChild(pRoot);
        SendXML(doc);


}

//---------------------------------------------------------------------------
void __fastcall CEQPXML::doAlarmAck(char *pRx)
{
        /*
        TiXmlDocument doc;
        doc.Parse(pRx);
        TiXmlElement* pRoot = doc.FirstChildElement("Root");
        TiXmlElement* pData=pRoot->FirstChildElement("ACK");
        */
        //do something for that
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doRCMD(char *pRx)
{
        bool bRet=false;
        TiXmlDocument doc;
        doc.Parse(pRx);
        TiXmlElement* pRoot = doc.FirstChildElement("Root");
        TiXmlElement* pData=pRoot->FirstChildElement("DATA");

        AnsiString strData=pData->GetText();            //RUN/STOP/PAUSE/RESUME/PPSELECT,PPID/LOCAL/REMOTE
        AnsiString strSub=strData.SubString(0,8);

        if(strSub=="RUN") bRet=StartProcess(true);
        else if(strSub=="RESUME") bRet=StartProcess(true);
        else if(strSub=="STOP") bRet=StartProcess(false);
        else if(strSub=="PAUSE")bRet=StartProcess(false);
        else if(strSub=="PPSELECT")
        {
                AnsiString strName=strData.SubString(10,strData.Length()-9);
                AnsiString strFile;
                strFile.sprintf("C:\\Product Data\\%s\\%s.ini",strName,strName);
                bRet=OpenFile(strFile);

        }
        else if(strSub=="LOCAL")
        {
                m_CIMStatus="1";
                bRet=true;
        }
        else if(strSub=="REMOTE")
        {
                m_CIMStatus="2";
                bRet=true;
        }
        else
        {
                SendXML("RCMD","ACK","ACK","1");
                return;
        }

         if(bRet) SendXML("RCMD","ACK","ACK","0");
         else SendXML("RCMD","ACK","ACK","2");
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doQueryPPID(char *pRx)
{

         TiXmlDocument doc;
        TiXmlElement* pRoot;
        TiXmlElement *pIDE;
        AnsiString strName;
        WIN32_FIND_DATA FindFileData;

        pRoot=new TiXmlElement("Root");
        pRoot->SetAttribute("MSGID","QUERY_PPID");
        pRoot->SetAttribute("TYPE","ACK");

        //strName.sprintf("%s*.ini",IniFile_Dir);
        HANDLE hFile=FindFirstFile(strName.c_str(),&FindFileData);
        if(hFile!=INVALID_HANDLE_VALUE)
        {
                do
                { 
                        //strName.sprintf("%s%s",IniFile_Dir,FindFileData.cFileName);
                        pIDE=new TiXmlElement("PPID");
                        pIDE->LinkEndChild(new TiXmlText(strName.c_str()));
                        pRoot->LinkEndChild(pIDE);
                }while(FindNextFile(hFile,&FindFileData)!=0);
        }
        else
        {
                pIDE=new TiXmlElement("PPID");
                pIDE->LinkEndChild(new TiXmlText(""));
                pRoot->LinkEndChild(pIDE);
        }

        doc.LinkEndChild(pRoot);
        SendXML(doc);


}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doQueryPPIDNew(char *pRx)
{
         TiXmlDocument doc;
        TiXmlElement* pRoot;
        TiXmlElement *pIDE;
        AnsiString strName;
        WIN32_FIND_DATA FindFileData;

        pRoot=new TiXmlElement("Root");
        pRoot->SetAttribute("MSGID","QUERY_PPID");
        pRoot->SetAttribute("TYPE","ACK");

        TSearchRec sr;
        //int iAttributes = 0;

        int nRes=FindFirst("C:\\Product Data\\*",faDirectory,sr);
        if(nRes==0)
        {
                do
                {
                        if(sr.Attr==faDirectory)
                        {
                                if(sr.Name!="." && sr.Name!=".." && sr.Name!="Message History")
                                {
                                        pIDE=new TiXmlElement("PPID");
                                        pIDE->LinkEndChild(new TiXmlText(sr.Name.c_str()));
                                        pRoot->LinkEndChild(pIDE);
                                }
                        }
                }while(FindNext(sr)==0);
        }
        else
        {
                pIDE=new TiXmlElement("PPID");
                pIDE->LinkEndChild(new TiXmlText(""));
                pRoot->LinkEndChild(pIDE);
        }
        FindClose(sr);

        doc.LinkEndChild(pRoot);
        SendXML(doc);


}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doQueryPPBody(char *pRx)
{
        TiXmlDocument doc1;
        doc1.Parse(pRx);
        TiXmlElement* pRoot1 = doc1.FirstChildElement("Root");
        TiXmlElement* pData1=pRoot1->FirstChildElement("DATA");

        AnsiString strData=pData1->GetText();           //Recipe Name

        TIniFile *pIniFile = new TIniFile(strData);

         TiXmlDocument doc;
        TiXmlElement* pRoot;
        TiXmlElement *pData;
        TiXmlElement *pParam;
        AnsiString strName;
        WIN32_FIND_DATA FindFileData;

        pRoot=new TiXmlElement("Root");
        pRoot->SetAttribute("MSGID","QUERY_PPBODY");
        pRoot->SetAttribute("TYPE","ACK");

       AnsiString strTmp;
       pData=new TiXmlElement("DATA");
       pData->SetAttribute("PPID",strData.c_str());

       pParam=new TiXmlElement("PARAMETER");
       strTmp.sprintf("m_strLaserFileName,%s",pIniFile->ReadString(Product_Section,"m_strLaserFileName","Laser"));
       pParam->LinkEndChild(new TiXmlText(strTmp.c_str()));
       pData->LinkEndChild(pParam);

       pParam=new TiXmlElement("PARAMETER");
       strTmp.sprintf("m_strLaserGraphicName,%s",pIniFile->ReadString(Product_Section,"m_strLaserGraphicName","5"));
       pParam->LinkEndChild(new TiXmlText(strTmp.c_str()));
       pData->LinkEndChild(pParam);

       pRoot->LinkEndChild(pData);


        doc.LinkEndChild(pRoot);
        SendXML(doc);

        delete pIniFile;
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doSetPPBody(char *pRx)
{
         TiXmlDocument doc1;
        doc1.Parse(pRx);
        TiXmlElement* pRoot1 = doc1.FirstChildElement("Root");
        TiXmlElement* pData1=pRoot1->FirstChildElement("DATA");

        AnsiString strData=pData1->Attribute("PPID");

        TIniFile *pIniFile = new TIniFile(strData);
        if(FileExists(strData))
        {
                TiXmlElement* pParam=pData1->FirstChildElement("PARAMETER");

                while(1)
                {
                        if(pParam==NULL) break;
                        AnsiString strParam=pParam->GetText();
                        int nPos=strParam.Pos(",");
                        AnsiString strA=strParam.SubString(1,nPos-1);
                        AnsiString strB=strParam.SubString(nPos+1,strParam.Length()-strA.Length()+1);
                        pIniFile->WriteString(Product_Section,strA,strB);

                        pParam=pParam->NextSiblingElement("PARAMETER");
                }

                SendXML("SET_PPBODY","ACK","ACK","0");
        }
        else SendXML("SET_PPBODY","ACK","ACK","3");
        delete pIniFile;
}


/*
<!--EQ上拋EAP，請求上拋上層系統判斷是否OK-->
<!--CMD為命令類別，暫訂"DEVICE_PRECHECK"，若要需求再作修改-->
<!--TID為時間標籤，格式yyyyMMddHHmmsszzzz-->
<!--IP為機台內部IP，例如192.168.101.101-->
<!--RCPID為機台目前使用的Recipe-->
<!--EQPID為機號，例如：K001JL-->
<!--TIME為當前時間，格式yyyy/MM/dd HH:mm:ss-->

<?xml version="1.0" encoding="utf-8"?>
<CMD CMD="DEVICE_PRECHECK" Type="Request" TID="201303080834369056" Sys="mes">
    <HEADER IP="192.168.101.201" PATH="mes" RCPID="12003123" 	EQPID="3013DS" TIME="2013/03/08 08:34:36" />
    <DEVICE_INFO>
		<LOTID VALUE="20D3B425.1" />
		<DEVICE_TYPE VALUE="COVER/CARRIER" />
		<DEVICE_ID VALUE="上蓋或載板ID" />
		<!--其他依user需求再擴充-->
    </DEVICE_INFO>	
</CMD>
*/

