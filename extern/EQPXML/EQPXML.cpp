//---------------------------------------------------------------------------


#pragma hdrstop

#include "EQPXML.h"
#include "IniFile.h"
#include "MainThread.h"
CEQPXML g_eqpXML;
extern CIniFile g_IniFile;
extern CMainThread *g_pMainThread;


#define MAX_SEND_BUFF 30000
//---------------------------------------------------------------------------

#pragma package(smart_init)

void __fastcall CEQPXML::doHostStatus(char *pRx)
{
	TiXmlDocument doc;
	doc.Parse(pRx);
	TiXmlElement* pRoot = doc.FirstChildElement("Root");
	TiXmlElement* pData = pRoot->FirstChildElement("DATA");

	AnsiString strMsgID(pRoot->Attribute("MSGID"));
	AnsiString strData = pData->GetText();

	if (strData == "1") m_CIMStatus = "1";
	else if (strData == "0") m_CIMStatus = "0";
	else if (strData == "2") m_CIMStatus = "2";
	else m_CIMStatus = "0";

	//SendXML("HOST_STATUS","ACK","ACK","");

	pRoot->SetAttribute("TYPE", "ACK");
	pData->SetValue("ACK");
	TiXmlNode *pNode3 = pData->FirstChild();
	pNode3->SetValue("");

	SendXML(doc);

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doDTSet(char *pRx)
{
    /*
	TiXmlDocument doc;
	doc.Parse(pRx);
	TiXmlElement *pRoot = doc.FirstChildElement("Root");
	TiXmlElement *pData = pRoot->FirstChildElement("DATA");

	AnsiString strMsgID(pRoot->Attribute("MSGID"));
	AnsiString strData = pData->GetText();
	//Set System Date Time Here

	SendXML("DTSET", "ACK", "ACK", "0");           //0:OK 1:NG
    */
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doCIMMsg(char *pRx)
{
	TiXmlDocument doc;
	doc.Parse(pRx);
	TiXmlElement *pRoot = doc.FirstChildElement("Root");
	TiXmlElement *pData = pRoot->FirstChildElement("DATA");

	AnsiString strMsgID(pRoot->Attribute("MSGID"));
	AnsiString strData = pData->GetText();
	//do something whith that message
	m_strCIMMsgLog.push_back(strData);

	SendXML("CIMMSG", "ACK", "ACK", "0");           //0:OK 1:NG
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doQueryVID(char *pRx)
{
	TiXmlDocument doc;
	doc.Parse(pRx);
	TiXmlElement *pRoot = doc.FirstChildElement("Root");
	TiXmlElement *pData = pRoot->FirstChildElement("DATA");

	AnsiString strMsgID(pRoot->Attribute("MSGID"));
	AnsiString strData = pData->GetText();

	//const char *pTID = pRoot->Attribute("TID");
	//const char *pCEID = pData->Attribute("CEID");
	//const char *pSxFy = pData->Attribute("SxFy");
	//do something whith that message
	pRoot->SetAttribute("TYPE", "ACK");
	TiXmlElement *pSVID;
	TiXmlElement *pID;
	TiXmlElement *pValue;

    TStringList *strList = SplitString(g_IniFile.m_strLastFileName, "\\");
    AnsiString strLastFilePath = "";
    for (int i=0;i<strList->Count-1;i++)
    {
        strLastFilePath += strList->Strings[i] + "\\";
    }
    delete strList;
    //---------------------------------------------------------------------------
    //Add SVID
    TDateTime DT = TDateTime::CurrentDateTime();
    char SVIDIndex[28][4] = { 0 };
    strcpy(SVIDIndex[0], "16");
    strcpy(SVIDIndex[1], "17");
    strcpy(SVIDIndex[2], "18");
    strcpy(SVIDIndex[3], "19");
    strcpy(SVIDIndex[4], "26");
    strcpy(SVIDIndex[5], "27");
    strcpy(SVIDIndex[6], "28");
    strcpy(SVIDIndex[7], "29");
    strcpy(SVIDIndex[8], "36");
    strcpy(SVIDIndex[9], "37");
    strcpy(SVIDIndex[10], "38");
    strcpy(SVIDIndex[11], "39");
    strcpy(SVIDIndex[12], "251");
    strcpy(SVIDIndex[13], "261");
    strcpy(SVIDIndex[14], "271");
    strcpy(SVIDIndex[15], "1");
    strcpy(SVIDIndex[16], "2");
    strcpy(SVIDIndex[17], "3");
    strcpy(SVIDIndex[18], "4");
    strcpy(SVIDIndex[19], "5");
    strcpy(SVIDIndex[20], "6");
    strcpy(SVIDIndex[21], "7");
    strcpy(SVIDIndex[22], "8");
    strcpy(SVIDIndex[23], "311");
    strcpy(SVIDIndex[24], "312");
    strcpy(SVIDIndex[25], "313");
    strcpy(SVIDIndex[26], "314");
    strcpy(SVIDIndex[27], "315");

    char SVID[28][100] = { 0 };
    strcpy(SVID[0], FormatFloat("0.0", g_pMainThread->m_dRearTempRealTime).c_str());
    strcpy(SVID[1], FormatFloat("0.0", g_IniFile.m_dLamTemp[0]).c_str());
    strcpy(SVID[2], FormatFloat("0.0", g_pMainThread->m_dFrontTempRealTime).c_str());
    strcpy(SVID[3], FormatFloat("0.0", g_IniFile.m_dLamTemp[1]).c_str());
    strcpy(SVID[4], FormatFloat("0.0", 0.0).c_str());
    strcpy(SVID[5], FormatFloat("0.0", g_IniFile.m_dLamPress[0]).c_str());
    strcpy(SVID[6], FormatFloat("0.0", 0.0).c_str());
    strcpy(SVID[7], FormatFloat("0.0", g_IniFile.m_dLamPress[1]).c_str());
    strcpy(SVID[8], FormatFloat("0.0", 0.0).c_str());
    strcpy(SVID[9], FormatFloat("0.0", g_IniFile.m_dLamTime[0]).c_str());
    strcpy(SVID[10], FormatFloat("0.0", 0.0).c_str());
    strcpy(SVID[11], FormatFloat("0.0", g_IniFile.m_dLamTime[1]).c_str());
    strcpy(SVID[12], FormatFloat("0.0", g_IniFile.m_nCols).c_str());
    strcpy(SVID[13], FormatFloat("0.0", g_IniFile.m_nRows).c_str());
    strcpy(SVID[14], strLastFilePath.c_str());
    strcpy(SVID[15], DT.FormatString("yyyymmddhhnnss").c_str());
    strcpy(SVID[16], "CT-74");
    strcpy(SVID[17], "01");
    strcpy(SVID[18], "0.0.0.1");
    strcpy(SVID[19], "OP_ID");
    strcpy(SVID[20], g_IniFile.m_strLastFileName.c_str());
    strcpy(SVID[21], m_EqpStatus.c_str());                           //Equipment process state (I=Idle, R=Run, D=Down)
    strcpy(SVID[22], m_CIMStatus.c_str());                           //CIM control state (0=Offline, 1=Online/Local, 2=Online/Remote)
    strcpy(SVID[23], FormatFloat("0.0", g_IniFile.m_nHeadType).c_str());
    strcpy(SVID[24], FormatFloat("0.0", g_IniFile.m_nHeadScal).c_str());
    strcpy(SVID[25], FormatFloat("0.0", g_IniFile.m_nVacummOn).c_str());
    strcpy(SVID[26], FormatFloat("0.0", g_IniFile.m_nPressCheck).c_str());
    strcpy(SVID[27], FormatFloat("0.0", g_IniFile.m_nDummyCheck).c_str());

	for (int nIndex = 0; nIndex<28; nIndex++)
	{
		pSVID = new TiXmlElement("SVID");
		pID = new TiXmlElement("ID");
		pID->LinkEndChild(new TiXmlText(SVIDIndex[nIndex]));
		pValue = new TiXmlElement("VALUE");
		pValue->LinkEndChild(new TiXmlText(SVID[nIndex]));
		pSVID->LinkEndChild(pID);
		pSVID->LinkEndChild(pValue);
		pData->LinkEndChild(pSVID);
	}

    //---------------------------------------------------------------------------
    //Add ECID
    char ECIDIndex[1][4] = { 0 };
	strcpy(ECIDIndex[0], "621");

	char ECID[1][100] = { 0 };
	strcpy(ECID[0], "C:\\asdw\\sdwqfqw.exe"/*g_IniFile.m_strApplicationPath.c_str()*/);

	for (int nIndex = 0; nIndex<1; nIndex++)
	{
		pSVID = new TiXmlElement("ECID");
		pID = new TiXmlElement("ID");
		pID->LinkEndChild(new TiXmlText(ECIDIndex[nIndex]));
		pValue = new TiXmlElement("VALUE");
		pValue->LinkEndChild(new TiXmlText(ECID[nIndex]));
		pSVID->LinkEndChild(pID);
		pSVID->LinkEndChild(pValue);
		pData->LinkEndChild(pSVID);
	}

    //---------------------------------------------------------------------------
    //Add DVID
    AnsiString strRDLD = "";
    AnsiString strFDLD = "";
    AnsiString strRULD = "";
    AnsiString strFULD = "";

    TStringList *strStrings = new TStringList;
    for (int i=0;i<(g_IniFile.m_nCols*g_IniFile.m_nRows);i++)
    {
        strStrings->Add(FormatFloat("0.00", *g_pMainThread->m_dRearDownLaserDiff[i]).c_str());
    }
    strStrings->Delimiter = '/';
    strRDLD = strStrings->DelimitedText;
    strStrings->Clear();

    for (int i=0;i<(g_IniFile.m_nCols*g_IniFile.m_nRows);i++)
    {
        strStrings->Add(FormatFloat("0.00", *g_pMainThread->m_dFrontDownLaserDiff[i]).c_str());
    }
    strStrings->Delimiter = '/';
    strFDLD = strStrings->DelimitedText;
    strStrings->Clear();

    for (int i=0;i<(g_IniFile.m_nCols*g_IniFile.m_nRows);i++)
    {
        strStrings->Add(FormatFloat("0.00", *g_pMainThread->m_dRearUpperLaserDiff[i]).c_str());
    }
    strStrings->Delimiter = '/';
    strRULD = strStrings->DelimitedText;
    strStrings->Clear();

    for (int i=0;i<(g_IniFile.m_nCols*g_IniFile.m_nRows);i++)
    {
        strStrings->Add(FormatFloat("0.00", *g_pMainThread->m_dFrontUpperLaserDiff[i]).c_str());
    }
    strStrings->Delimiter = '/';
    strFULD = strStrings->DelimitedText;
    strStrings->Clear();
    delete strStrings;

	char DVID[5][100] = { 0 };
	strcpy(DVID[0], g_IniFile.m_strSetupEENum.c_str());
    strcpy(DVID[1], strRDLD.c_str());
    strcpy(DVID[2], strFDLD.c_str());
    strcpy(DVID[3], strRULD.c_str());
    strcpy(DVID[4], strFULD.c_str());

	for (int nIndex = 0; nIndex<5; nIndex++)
	{
		pSVID = new TiXmlElement("DVID");
		pID = new TiXmlElement("ID");
		pID->LinkEndChild(new TiXmlText(FormatFloat("0", nIndex + 1300).c_str()));
		pValue = new TiXmlElement("VALUE");
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
	TiXmlElement* pData = pRoot->FirstChildElement("ACK");
	const char *pTID = pRoot->Attribute("TID");
	const char *pAckText = pData->GetText();

	if (pTID && pAckText) EventReportAck(pTID, pAckText);                              // error there

	//do something for that
    
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

	bool bRet = false;
	TiXmlDocument doc;
	doc.Parse(pRx);
	TiXmlElement *pRoot = doc.FirstChildElement("Root");
	TiXmlElement *pData = pRoot->FirstChildElement("DATA");

	AnsiString strData = pData->GetText();            //RUN/STOP/PAUSE/RESUME/PPSELECT,PPID/LOCAL/REMOTE
	AnsiString strSub = strData.SubString(0, 8);

	if (strSub == "RUN") bRet = StartProcess(true);
	else if (strSub == "RESUME") bRet = StartProcess(true);
	else if (strSub == "STOP") bRet = StartProcess(false);
	else if (strSub == "PAUSE") bRet = StartProcess(false);
	else if (strSub == "PPSELECT")
	{
		AnsiString strName = strData.SubString(10, strData.Length() - 9);
		AnsiString strFile;
		strFile.sprintf("C:\\Product Data\\%s\\%s.ini", strName, strName);
		bRet = OpenFile(strFile);
	}
	else if (strSub == "LOCAL")
	{
		m_CIMStatus = "1";
		bRet = true;
	}
	else if (strSub == "REMOTE")
	{
		m_CIMStatus = "2";
		bRet = true;
	}
	else
	{
		SendXML("RCMD", "ACK", "ACK", "1");
		return;
	}

	if (bRet) SendXML("RCMD", "ACK", "ACK", "0");
	else SendXML("RCMD", "ACK", "ACK", "2");

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doQueryPPID(char *pRx)
{
    /*
	TiXmlDocument doc;
	TiXmlElement *pRoot;
	TiXmlElement *pIDE;
	AnsiString strName;
	WIN32_FIND_DATA FindFileData;

	pRoot = new TiXmlElement("Root");
	pRoot->SetAttribute("MSGID", "QUERY_PPID");
	pRoot->SetAttribute("TYPE", "ACK");

	//strName.sprintf("%s*.ini",IniFile_Dir);
	HANDLE hFile = FindFirstFile(strName.c_str(), &FindFileData);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			//strName.sprintf("%s%s",IniFile_Dir,FindFileData.cFileName);
			pIDE = new TiXmlElement("PPID");
			pIDE->LinkEndChild(new TiXmlText(strName.c_str()));
			pRoot->LinkEndChild(pIDE);
		} while (FindNextFile(hFile, &FindFileData) != 0);
	}
	else
	{
		pIDE = new TiXmlElement("PPID");
		pIDE->LinkEndChild(new TiXmlText(""));
		pRoot->LinkEndChild(pIDE);
	}

	doc.LinkEndChild(pRoot);
	SendXML(doc);
    */
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doQueryPPIDNew(char *pRx)
{
	TiXmlDocument doc;
	TiXmlElement *pRoot;
	TiXmlElement *pIDE;
	AnsiString strName;
	WIN32_FIND_DATA FindFileData;

	pRoot = new TiXmlElement("Root");
	pRoot->SetAttribute("MSGID", "QUERY_PPID");
	pRoot->SetAttribute("TYPE", "ACK");

	TSearchRec sr;
	//int iAttributes = 0;

	int nRes = FindFirst("C:\\Product Data\\*", faDirectory, sr);
	if (nRes == 0)
	{
		do
		{
			if (sr.Attr == faDirectory)
			{
				if (sr.Name != "." && sr.Name != ".." && sr.Name != "Message History" && sr.Name != "Error Message")
				{
					pIDE = new TiXmlElement("PPID");
					pIDE->LinkEndChild(new TiXmlText(sr.Name.c_str()));
					pRoot->LinkEndChild(pIDE);
				}
			}
		} while (FindNext(sr) == 0);
	}
	else
	{
		pIDE = new TiXmlElement("PPID");
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
	AnsiString strSxFx=pData1->Attribute("SxFy");

	TIniFile *pIniFile = new TIniFile("C:\\Product Data\\"+strData+"\\"+strData+".ini");

	TiXmlDocument doc;
	TiXmlElement* pRoot;
	TiXmlElement *pData;
	TiXmlElement *pParam;
	TiXmlElement *pProperty;
	AnsiString strName;
	WIN32_FIND_DATA FindFileData;

	pRoot=new TiXmlElement("Root");
	pRoot->SetAttribute("MSGID","QUERY_PPBODY");
	pRoot->SetAttribute("TYPE","ACK");

	AnsiString strTmp;
	pData=new TiXmlElement("DATA");
	pData->SetAttribute("PPID",strData.c_str());
	pData->SetAttribute("SxFy",strSxFx.c_str());

	char *ParamItem[]={
        "m_nCols","mm","F4","0","10",
		"m_nRows","mm","F4","0","5",
		"m_dColPitch","mm","F4","0","1000",
		"m_dRowPitch","mm","F4","0","1000",

		"m_dLamHeight0","mm","F4","0","200",
		"m_dLamHeight1","mm","F4","0","200",
		"m_dLamStop0","mm","F4","0","200",
		"m_dLamStop1","mm","F4","0","200",
		"m_dLamGetPos0","mm","F4","0","200",
		"m_dLamGetPos1","mm","F4","0","200",
		"m_dLamVacHeight0","mm","F4","0","200",
		"m_dLamVacHeight1","mm","F4","0","200",

		"m_dLamTime0","s","F4","0","1000",
		"m_dLamTemp0","c","F4","0","1000",
		"m_dLamPress0","kg","F4","0","20",
		"m_dLamTime1","s","F4","0","1000",
		"m_dLamTemp1","c","F4","0","1000",
		"m_dLamPress1","kg","F4","0","20",

		"m_dLoadCellPosX0","mm","F4","0","1000",
		"m_dLoadCellPosY0","mm","F4","0","1000",
		"m_dLaserUpPosX00","mm","F4","0","1000",
		"m_dLaserUpPosY00","mm","F4","0","1000",
		"m_dLaserUpPosX01","mm","F4","0","1000",
		"m_dLaserUpPosY01","mm","F4","0","1000",
		"m_dLaserUpPosX02","mm","F4","0","1000",
		"m_dLaserUpPosY02","mm","F4","0","1000",
		"m_dLaserUpPosX03","mm","F4","0","1000",
		"m_dLaserUpPosY03","mm","F4","0","1000",
		"m_dLaserDownPosX0","mm","F4","0","1000",
		"m_dLaserDownPosY0","mm","F4","0","1000",
		"m_dLoadCellPosX1","mm","F4","0","1000",
		"m_dLoadCellPosY1","mm","F4","0","1000",
		"m_dLaserUpPosX10","mm","F4","0","1000",
		"m_dLaserUpPosY10","mm","F4","0","1000",
		"m_dLaserUpPosX11","mm","F4","0","1000",
		"m_dLaserUpPosY11","mm","F4","0","1000",
		"m_dLaserUpPosX12","mm","F4","0","1000",
		"m_dLaserUpPosY12","mm","F4","0","1000",
		"m_dLaserUpPosX13","mm","F4","0","1000",
		"m_dLaserUpPosY13","mm","F4","0","1000",
		"m_dLaserDownPosX1","mm","F4","0","1000",
		"m_dLaserDownPosY1","mm","F4","0","1000",

		"m_nRailOption","mm","F4","0","2",
		"m_bNotLam","mm","F4","0","1",
		"m_dPressCalRange","mm","F4","0","100",
		"m_dPressCalTime","mm","F4","0","100",
		"m_dLaserUpPosX0","mm","F4","0","1000",
		"m_dLaserUpPosY0","mm","F4","0","1000",
		"m_dLaserUpPosX1","mm","F4","0","1000",
		"m_dLaserUpPosY1","mm","F4","0","1000",
		"m_dLamSecondHeight0","mm","F4","0","200",
		"m_dLamSecondHeight1","mm","F4","0","200",
		"m_dLamSecondTime0","mm","F4","0","1000",
		"m_dLamSecondTime1","mm","F4","0","1000",
		"m_dLamThirdHeight0","mm","F4","0","200",
		"m_dLamThirdHeight1","mm","F4","0","200",
		"m_dLamThirdTime0","mm","F4","0","1000",
		"m_dLamThirdTime1","mm","F4","0","1000",

		"m_dAutoInterval","mm","F4","0","1000",
		"m_dAutoStopTimes","mm","F4","0","1000",
		"m_nAutoInterval","mm","F4","0","1000",
		"m_dAutoRunTempRange","mm","F4","0","1000",
		"m_dVacDelayTime","mm","F4","0","1000",
        "m_nHeadType","mm","F4","0","1000",
        "m_nHeadScal","mm","F4","0","1000",
        "m_nVacummOn","mm","F4","0","1",
        "m_nPressCheck","mm","F4","0","1",
        "m_nDummyCheck","mm","F4","0","1",
		"END"};           //E:End

	int nX=0;
	while(1)
	{
		if(strcmp(ParamItem[nX*5],"END")==0) break;
		pParam=new TiXmlElement("PARAMETER");
		pParam->SetAttribute("NAME",ParamItem[nX*5]);
		///strTmp.sprintf("%s,%s",ParamItem[nX],pIniFile->ReadString(Product_Section,ParamItem[nX],"Not Valid"));
		//pParam->LinkEndChild(new TiXmlText(strTmp.c_str()));

		pProperty=new TiXmlElement("PROPERTY");
		pProperty->LinkEndChild(new TiXmlText("TRUE"));
		pParam->LinkEndChild(pProperty);

		for(int t=0;t<3; t++)
		{
			pProperty=new TiXmlElement("PROPERTY");
			pProperty->LinkEndChild(new TiXmlText(ParamItem[nX*5+t]));
			pParam->LinkEndChild(pProperty);
		}

		pProperty=new TiXmlElement("PROPERTY");
		pProperty->LinkEndChild(new TiXmlText(pIniFile->ReadString(Product_Section,ParamItem[nX*5],"0").c_str()));
		pParam->LinkEndChild(pProperty);

		for(int t=3;t<5; t++)
		{
			pProperty=new TiXmlElement("PROPERTY");
			pProperty->LinkEndChild(new TiXmlText(ParamItem[nX*5+t]));
			pParam->LinkEndChild(pProperty);
		}

		pData->LinkEndChild(pParam);
		nX++;
	}

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
	TiXmlElement *pRoot1 = doc1.FirstChildElement("Root");
	TiXmlElement *pData1 = pRoot1->FirstChildElement("DATA");

	AnsiString strData = pData1->Attribute("PPID");

	TIniFile *pIniFile = new TIniFile("C:\\Product Data\\"+strData+"\\"+strData+".ini");
	if (FileExists("C:\\Product Data\\"+strData+"\\"+strData+".ini"))
	{
		TiXmlElement *pParam = pData1->FirstChildElement("PARAMETER");

		while (1)
		{
			if (pParam == NULL) break;
			AnsiString strParam = pParam->GetText();
			int nPos = strParam.Pos(",");
			AnsiString strA = strParam.SubString(1, nPos - 1);
			AnsiString strB = strParam.SubString(nPos + 1, strParam.Length() - strA.Length() + 1);
			pIniFile->WriteString(Product_Section, strA, strB);

			pParam = pParam->NextSiblingElement("PARAMETER");
		}

		SendXML("SET_PPBODY", "ACK", "ACK", "0");
	}
	else SendXML("SET_PPBODY", "ACK", "ACK", "3");
	delete pIniFile;
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::SendXML(char *pMSGID, char *pType, char *pTextName, char *pValue, char *pTID)
{
	char send[MAX_SEND_BUFF] = { 0 };
	char A[MAX_SEND_BUFF] = { 0 };

	try
	{
		sprintf(A, "<?xml version=\"1.0\" encoding=\"utf-8\" ?><Root MSGID=\"%s\" TYPE=\"%s\" TID=\"%s\" ><%s>%s</%s></Root>", pMSGID, pType, pTID, pTextName, pValue, pTextName);

		sprintf(send, "%c%c%c%c%c%s", 0x00, 0x00, 0x00, strlen(A) + 1, 0x00, A);


		if (m_pSocket != NULL && m_bOnLine)
		{
			m_pSocket->SendBuf(send, strlen(A) + 5);
			//m_pSocket->SendBuf(send,MAX_SEND_BUFF);
			//g_IniFile.AddLog(send,MAX_SEND_BUFF);
		}
	}
	catch (const EAccessViolation &e)
	{
		Application->MessageBox(e.Message.c_str(), "Look2", MB_OK);
	}

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::SendXML(TiXmlDocument &theDoc)
{
	try
	{
		TiXmlPrinter printer;
		theDoc.Accept(&printer);
		const char *NewString = printer.CStr();

		long nLen = strlen(NewString) + 1;
		//char *send=new char[strlen(NewString)+5];
		char *send = new char[MAX_SEND_BUFF];

		char strLen[4] = { 0 };
		strLen[0] = (nLen & 0xff000000) >> 24;
		strLen[1] = (nLen & 0x00ff0000) >> 16;
		strLen[2] = (nLen & 0x0000ff00) >> 8;
		strLen[3] = (nLen & 0x000000ff);

		sprintf(send, "%c%c%c%c%c%s", strLen[0], strLen[1], strLen[2], strLen[3], 0x00, NewString);

		if (m_pSocket != NULL && m_bOnLine)
		{
			m_pSocket->SendBuf(send, strlen(NewString) + 5);
			//m_pSocket->SendBuf(send,MAX_SEND_BUFF);                  //note To write log
			//g_IniFile.AddLog("Send XML",8);                          //note To write log

            AnsiString AAA = printer.CStr();
            g_IniFile.AddLog(AAA.c_str(), AAA.Length());
		}
		delete send;
	}
	catch (const EAccessViolation &e)
	{
		Application->MessageBox(e.Message.c_str(), "Look3", MB_OK);
		//delete send;
	}
}
//---------------------------------------------------------------------------

__fastcall CEQPXML::CEQPXML()
{
    m_pSocket = NULL;
    m_EqpStatus = "I";
    m_CIMStatus = "0";
	receiveRx = (char *)malloc(30000);//new char[30000];
    receive = (char *)malloc(30000);//new char[30000];

    m_strSubstrate2DCode = "SSID";
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
     m_pSocket = pSocket;
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::EndComm()
{
     m_pSocket = NULL;
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::ProcessCIM()
{
    long nSize = 0;
    //char *receiveRx;
    //char *receive;

    try
    {

        //receiveRx=(char *)VirtualAlloc(NULL, 30000, MEM_COMMIT, PAGE_READWRITE);//(char *)malloc(30000);//new char[30000];
        //receive=(char *)VirtualAlloc(NULL, 30000, MEM_COMMIT, PAGE_READWRITE);///(char *)malloc(30000);//new char[30000];

        //char receive[30000];
        if (receive==NULL || receiveRx==NULL) return;

        memset(receiveRx,0x00,30000);
        memset(receive,0x00,30000);

        m_pSocket->ReceiveBuf(receiveRx,30000);
        //AddLog(receiveRx,30000);

        TiXmlDocument doc;
        int nIndex = 0;

        while(1)
        {
            doc.Clear();
            memset(receive,0x00,30000);
                
            if(nIndex>=30000) break;

            nSize = (receiveRx[0+nIndex]<<24)+(receiveRx[1+nIndex]<<16)+(receiveRx[2+nIndex]<<8)+receiveRx[3+nIndex]+4;//+5;
            if(nSize<=5 || nSize>=30000)
				break;

			AnsiString strXX=FormatFloat("Size=0",nSize);
            //AddLog(strXX.c_str(),strXX.Length());

            memcpy(receive,receiveRx+nIndex,nSize);
            nIndex+=nSize;
            if (nIndex>=30000) break;

            //AddLog(receive,nSize);

            doc.Parse(receive+5);
            TiXmlElement* pRoot = doc.FirstChildElement("Root");
            if (pRoot==NULL) break;
            //TiXmlElement* pData=pRoot->FirstChildElement("DATA");
        
            AnsiString strMsgID(pRoot->Attribute("MSGID"));
            //AnsiString strData=pData->GetText();

            if (strMsgID=="HOST_STATUS") doHostStatus(receive+5);                         //Local realtime renew CIMAP status
            else if (strMsgID=="DTSET") doDTSet(receive+5);                               // no used
            else if (strMsgID=="CIMMSG") doCIMMsg(receive+5);                             //Secs/Gem S10F3   CIM show TEXT to machine
            else if (strMsgID=="QUERY_VID") doQueryVID(receive+5);                        //Secs/Gem S1F3,S2F13  CIM Use SCID,ECID,DVID get machine param
            else if (strMsgID=="EVENT_REPORT") doEventReportAck(receive+5);               //Local Rise ECID 1,3,115
            else if (strMsgID=="ALARM") doAlarmAck(receive+5);                            // no used
            else if (strMsgID=="RCMD") doRCMD(receive+5);                                 //Secs/Gem S2F21,S2F41 CIM send RCMD, change machine /RUN/STOP/PAUSE/RESUME/PPSELECT,PPID/LOCAL/REMOTE
            else if (strMsgID=="QUERY_PPID") doQueryPPIDNew(receive+5);                   //Secs/Gem S7F19   CIM Find Recipe folder and file name
            else if (strMsgID=="QUERY_PPBODY") doQueryPPBody(receive+5);                  //Secs/Gem S7F25   CIM Use Recipe file name get all recipe param
            else if (strMsgID=="SET_PPBODY") doSetPPBody(receive+5);                      //Secs/Gem S7F23   CIM Use param and value modify recipe, not add a new recipy

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
void __fastcall CEQPXML::SendEventReport(char *pEvent,char *pTID)
{

        SendXML("EVENT_REPORT","RPT","CEID",pEvent,pTID);

}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::SendAlarmMessage(char *pID,char *pText)
{
    TiXmlDocument doc;
    TiXmlElement* pRoot;

    pRoot = new TiXmlElement("Root");
    pRoot->SetAttribute("MSGID","ALARM");
    pRoot->SetAttribute("TYPE","RPT");

    TiXmlElement *pIDE = new TiXmlElement("ID");
    pIDE->LinkEndChild(new TiXmlText(pID));

	TiXmlElement *pTxt = new TiXmlElement("TEXT");
    pTxt->LinkEndChild(new TiXmlText(pText));

    pRoot->LinkEndChild(pIDE);
    pRoot->LinkEndChild(pTxt);

    doc.LinkEndChild(pRoot);
    SendXML(doc);
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

