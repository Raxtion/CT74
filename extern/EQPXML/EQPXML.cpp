//---------------------------------------------------------------------------


#pragma hdrstop

#include "EQPXML.h"
#include "IniFile.h"
#include "MainThread.h"
#include "dir.h"
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

    //---------------------------------------------------------------------------
    //Set param for SVID
    TStringList *strList = SplitString(g_IniFile.m_strLastFileName, "\\");
    AnsiString strLastFilePath = "";
    for (int i=0;i<strList->Count-1;i++)
    {
        strLastFilePath += strList->Strings[i] + "\\";
    }
    delete strList;

    AnsiString strHeadType; (g_IniFile.m_nHeadType == 0) ? strHeadType = "SOLID" : strHeadType = "HOLLOW";
    AnsiString strVacummOn; (g_IniFile.m_nVacummOn == 0) ? strVacummOn = "FAIL" : strVacummOn = "TRUE";
    AnsiString strPressCheck; (g_IniFile.m_nPressCheck == 0) ? strPressCheck = "FAIL" : strPressCheck = "TRUE";
    AnsiString strDummyCheck; (g_IniFile.m_nDummyCheck == 0) ? strDummyCheck = "FAIL" : strDummyCheck = "TRUE";

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
    strcpy(SVIDIndex[28], "316");

    char SVID[29][100] = { 0 };
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
    strcpy(SVID[23], strHeadType.c_str());
    strcpy(SVID[24], g_IniFile.m_strHeadScal.c_str());
    strcpy(SVID[25], strVacummOn.c_str());
    strcpy(SVID[26], strPressCheck.c_str());
    strcpy(SVID[27], strDummyCheck.c_str());
    strcpy(SVID[28], g_IniFile.m_strModuleScal.c_str());

	for (int nIndex = 0; nIndex<29; nIndex++)
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
	strcpy(ECID[0], g_IniFile.m_strApplicationPath.c_str());

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
    LaserDiff m_strLaserDiff;
    m_strLaserDiff.strRDLD = "";
    m_strLaserDiff.strFDLD = "";
    m_strLaserDiff.strRULD = "";
    m_strLaserDiff.strFULD = "";
    generateLaserDiff(m_strLaserDiff);
    AnsiString strRDLD = m_strLaserDiff.strRDLD;
    AnsiString strFDLD = m_strLaserDiff.strFDLD;
    AnsiString strRULD = m_strLaserDiff.strRULD;
    AnsiString strFULD = m_strLaserDiff.strFULD;

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
    /*
	TiXmlDocument doc;
	doc.Parse(pRx);
	TiXmlElement* pRoot = doc.FirstChildElement("Root");
	TiXmlElement* pData = pRoot->FirstChildElement("ACK");
	const char *pTID = pRoot->Attribute("TID");
	const char *pAckText = pData->GetText();

	if (pTID && pAckText) EventReportAck(pTID, pAckText);

	//do something for that
    */
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
        TStringList *StrList = SplitString(strData, ",");
        //AnsiString strName = strData.SubString(10, strData.Length() - 9);
		//AnsiString strFile;
		//strFile.sprintf("C:\\Product_Data\\%s\\%s.ini", strName, strName);
		//bRet = OpenFile(strFile);
        bRet = OpenFile(StrList->Strings[1]);

        delete StrList;
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
    /*
	TiXmlDocument doc;
	TiXmlElement *pRoot;
	TiXmlElement *pIDE;
	AnsiString strName;
	WIN32_FIND_DATA FindFileData;

	pRoot = new TiXmlElement("Root");
	pRoot->SetAttribute("MSGID", "QUERY_PPID");
	pRoot->SetAttribute("TYPE", "ACK");

	TSearchRec sr;
	int nRes = FindFirst("C:\\Product_Data\\*", faDirectory, sr);
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
    */
}
//---------------------------------------------------------------------------
void __fastcall CEQPXML::doQueryPPIDFullPath(char *pRx)
{
    TiXmlDocument doc;
	TiXmlElement *pRoot;
	TiXmlElement *pIDE;
	AnsiString strName;
	WIN32_FIND_DATA FindFileData;

	pRoot = new TiXmlElement("Root");
	pRoot->SetAttribute("MSGID", "QUERY_PPID");
	pRoot->SetAttribute("TYPE", "ACK");

    m_liststrFileName.clear();
    m_liststrForderName.clear();
    DirExplore("C:\\Product_Data", m_liststrForderName, m_liststrFileName);

    while (true)
    {
        if (m_liststrFileName.size()==0)
        {
            pIDE = new TiXmlElement("PPID");
		    pIDE->LinkEndChild(new TiXmlText(""));
		    pRoot->LinkEndChild(pIDE);
            break;
        }

        TStringList* strSplitPath = SplitString(m_liststrFileName.front(), "\\");
        if (strSplitPath->operator [](2) != "Error Message" && strSplitPath->Count != 3)
        {
            pIDE = new TiXmlElement("PPID");
            pIDE->LinkEndChild(new TiXmlText(m_liststrFileName.front().c_str()));
            pRoot->LinkEndChild(pIDE);
        }
        delete strSplitPath;
        m_liststrFileName.pop_front();

        if (m_liststrFileName.size()==0) break;
    }

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

	//TIniFile *pIniFile = new TIniFile("C:\\Product_Data\\"+strData+"\\"+strData+".ini");
    TIniFile *pIniFile = new TIniFile(strData);

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
        "m_nCols","mm","F4","0","10",                                      //on Paper
		"m_nRows","mm","F4","0","5",                                       //on Paper
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

		"m_dLamTime0","s","F4","0","1000",                                 //on Paper
		"m_dLamTemp0","c","F4","0","1000",                                 //on Paper
		"m_dLamPress0","kg","F4","0","20",                                 //on Paper
		"m_dLamTime1","s","F4","0","1000",                                 //on Paper
		"m_dLamTemp1","c","F4","0","1000",                                 //on Paper
		"m_dLamPress1","kg","F4","0","20",                                 //on Paper

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
		"m_dLamSecondHeight0","mm","F4","0","200",
		"m_dLamSecondHeight1","mm","F4","0","200",
		"m_dLamSecondTime0","mm","F4","0","1000",
		"m_dLamSecondTime1","mm","F4","0","1000",
		"m_dLamThirdHeight0","mm","F4","0","200",
		"m_dLamThirdHeight1","mm","F4","0","200",
		"m_dLamThirdTime0","mm","F4","0","1000",
		"m_dLamThirdTime1","mm","F4","0","1000",

		"m_dAutoStopRange","mm","F4","0","1000",
		"m_nAutoInterval","mm","F4","0","1000",
		"m_dAutoRunTempRange","mm","F4","0","1000",
		"m_dVacDelayTime","mm","F4","0","1000",
        "m_dScaleOffsetFront","mm","F4","0","1000",
        "m_dScaleOffsetRear","mm","F4","0","1000",
        "m_nDownPercent","mm","F4","0","1000",

        "m_nHeadType","mm","F4","0","1000",                                //on Paper
        "m_strHeadScal","mm","F4","0","1000",                              //on Paper
        "m_strModuleScal","mm","F4","0","1000",                            //on Paper
        "m_nVacummOn","mm","F4","0","1",                                   //on Paper
        "m_nPressCheck","mm","F4","0","1",                                 //on Paper
        "m_nDummyCheck","mm","F4","0","1",                                 //on Paper
        "m_strModuleNum","mm","F4","0","1000",                             //on Paper
        "m_strSetupEENum","mm","F4","0","1000",                            //on Paper

        "m_dBLT","mm","F4","0","1000",                                     //on Paper
        "m_dTilt","mm","F4","0","1000",                                    //on Paper
        "m_dGap","mm","F4","0","1000",                                     //on Paper
        "m_dKeyTemp00","mm","F4","0","1000",                                //on Paper
        "m_dKeyTemp01","mm","F4","0","1000",                                //on Paper
        "m_dKeyTemp02","mm","F4","0","1000",                                //on Paper
        "m_dKeyTemp10","mm","F4","0","1000",                                //on Paper
        "m_dKeyTemp11","mm","F4","0","1000",                                //on Paper
        "m_dKeyTemp12","mm","F4","0","1000",                                //on Paper
        //以上為產品參數

        //以下為機台變數
        "LastFilePath","mm","F4","0","1000",                               //on Paper
        "LastFileName","mm","F4","0","1000",                               //on Paper

        "FrontPressCal","mm","F4","0","1000",                              //on Paper
        "RearPressCal","mm","F4","0","1000",                               //on Paper

        "FrontDownLaserDiff","mm","F4","0","1000",                         //on Paper
        "RearDownLaserDiff","mm","F4","0","1000",                          //on Paper
        "FrontUpperLaserDiff","mm","F4","0","1000",                        //on Paper
        "RearUpperLaserDiff","mm","F4","0","1000",                         //on Paper

        "m_strLogInENGAccount","mm","F4","0","1000",
		"END"};           //E:End

    if (FileExists(strData))
	{
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


            if (strcmp(ParamItem[nX*5],"m_dScaleOffsetFront")==0)
            {
                AnsiString strScaleOffset = "";
                for (int nx=0;nx<50;nx++)
                {
                    strScaleOffset += pIniFile->ReadString("Offset_Table","m_dScaleOffsetFront"+FormatFloat("0", nx),"0") + "/";
                }
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strScaleOffset;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"m_dScaleOffsetRear")==0)
            {
                AnsiString strScaleOffset = "";
                for (int nx=0;nx<50;nx++)
                {
                    strScaleOffset += pIniFile->ReadString("Offset_Table","m_dScaleOffsetRear"+FormatFloat("0", nx),"0") + "/";
                }
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strScaleOffset;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"m_nHeadType")==0)
            {
                AnsiString strHeadType; (pIniFile->ReadString(Product_Section,ParamItem[nX*5],"0") == "0") ? strHeadType = "SOLID" : strHeadType = "HOLLOW";
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strHeadType;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"m_nVacummOn")==0)
            {
                AnsiString strVacummOn; (pIniFile->ReadString(Product_Section,ParamItem[nX*5],"0") == "0") ? strVacummOn = "FAIL" : strVacummOn = "TRUE";
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strVacummOn;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"m_nPressCheck")==0)
            {
                AnsiString strPressCheck; (pIniFile->ReadString(Product_Section,ParamItem[nX*5],"0") == "0") ? strPressCheck = "FAIL" : strPressCheck = "TRUE";
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strPressCheck;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"m_nDummyCheck")==0)
            {
                AnsiString strDummyCheck; (pIniFile->ReadString(Product_Section,ParamItem[nX*5],"0") == "0") ? strDummyCheck = "FAIL" : strDummyCheck = "TRUE";
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strDummyCheck;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"m_strSetupEENum")==0)
            {
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = g_IniFile.m_strSetupEENum;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            //-------------------------------------------------------------------------------------------------------------------------
            else if (strcmp(ParamItem[nX*5],"m_dBLT")==0)
            {
                AnsiString strBLT = "";
                strBLT += pIniFile->ReadString(Product_Section,"m_dBLT0","0") + "/";
                strBLT += pIniFile->ReadString(Product_Section,"m_dBLT1","0") + "/";
                strBLT += pIniFile->ReadString(Product_Section,"m_dBLT2","0") + "/";
                strBLT += pIniFile->ReadString(Product_Section,"m_dBLT3","0") + "/";
                strBLT += pIniFile->ReadString(Product_Section,"m_dBLT4","0") + "/";
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strBLT;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"m_dTilt")==0)
            {
                AnsiString m_dTilt = "";
                m_dTilt += pIniFile->ReadString(Product_Section,"m_dTilt0","0") + "/";
                m_dTilt += pIniFile->ReadString(Product_Section,"m_dTilt1","0") + "/";
                m_dTilt += pIniFile->ReadString(Product_Section,"m_dTilt2","0") + "/";
                m_dTilt += pIniFile->ReadString(Product_Section,"m_dTilt3","0") + "/";
                m_dTilt += pIniFile->ReadString(Product_Section,"m_dTilt4","0") + "/";
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = m_dTilt;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"m_dGap")==0)
            {
                AnsiString m_dGap = "";
                m_dGap += pIniFile->ReadString(Product_Section,"m_dGap0","0") + "/";
                m_dGap += pIniFile->ReadString(Product_Section,"m_dGap1","0") + "/";
                m_dGap += pIniFile->ReadString(Product_Section,"m_dGap2","0") + "/";
                m_dGap += pIniFile->ReadString(Product_Section,"m_dGap3","0") + "/";
                m_dGap += pIniFile->ReadString(Product_Section,"m_dGap4","0") + "/";
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = m_dGap;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            //-------------------------------------------------------------------------------------------------------------------------
            else if (strcmp(ParamItem[nX*5],"LastFilePath")==0)
            {
                TStringList *strList = SplitString(g_IniFile.m_strLastFileName, "\\");
                AnsiString strLastFilePath = "";
                for (int i=0;i<strList->Count-1;i++)
                {
                    strLastFilePath += strList->Strings[i] + "\\";
                }
                delete strList;

                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strLastFilePath;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"LastFileName")==0)
            {
                TStringList *strList = SplitString(g_IniFile.m_strLastFileName, "\\");
                AnsiString strLastFileName = strList->operator [](strList->Count-1);
                delete strList;

                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strLastFileName;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            //-------------------------------------------------------------------------------------------------------------------------
            else if (strcmp(ParamItem[nX*5],"FrontPressCal")==0)
            {
                AnsiString strFrontPressCal = "";
                for (int i=0;i<50;i++)
                {
                    strFrontPressCal += FormatFloat("0.000", g_pMainThread->m_dFrontPressCal[i])+"/";
                }
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strFrontPressCal;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"RearPressCal")==0)
            {
                AnsiString strRearPressCal = "";
                for (int i=0;i<50;i++)
                {
                    strRearPressCal += FormatFloat("0.000", g_pMainThread->m_dRearPressCal[i])+"/";
                }
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strRearPressCal;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            //-------------------------------------------------------------------------------------------------------------------------
            else if (strcmp(ParamItem[nX*5],"FrontDownLaserDiff")==0)
            {
                TStringList *strStrings = new TStringList;
                for (int i=0;i<50;i++)
                {
                    if (i == 10*(g_IniFile.m_nRows-1)+(g_IniFile.m_nCols-1))
                    {
                        strStrings->Add(FormatFloat("0.000", g_pMainThread->m_dFrontDownLaserDiff[i][0]).c_str());
                    }
                }
                strStrings->Delimiter = '/';
                AnsiString strFDLD = strStrings->DelimitedText;
                strStrings->Clear();
                delete strStrings;

                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strFDLD;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"RearDownLaserDiff")==0)
            {
                TStringList *strStrings = new TStringList;
                for (int i=0;i<50;i++)
                {
                    if (i == 10*(g_IniFile.m_nRows-1)+(g_IniFile.m_nCols-1))
                    {
                        strStrings->Add(FormatFloat("0.000", g_pMainThread->m_dRearDownLaserDiff[i][0]).c_str());
                    }
                }
                strStrings->Delimiter = '/';
                AnsiString strRDLD = strStrings->DelimitedText;
                strStrings->Clear();
                delete strStrings;

                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strRDLD;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"FrontUpperLaserDiff")==0)
            {
                int m_nCalCol = 0;
                int m_nCalRow = 0;
                TStringList *strStrings = new TStringList;
                for (int i=0;i<50;i++)
                {
                    m_nCalCol = i % 10;
                    m_nCalRow = i / 10;
                    if (m_nCalCol<g_IniFile.m_nCols && m_nCalRow< g_IniFile.m_nRows)
                    {
                        strStrings->Add(FormatFloat("0.000", g_pMainThread->m_dFrontUpperLaserDiff[i][0]).c_str());
                    }
                }
                strStrings->Delimiter = '/';
                AnsiString strFULD = strStrings->DelimitedText;
                strStrings->Clear();
                delete strStrings;

                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strFULD;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else if (strcmp(ParamItem[nX*5],"RearUpperLaserDiff")==0)
            {
                int m_nCalCol = 0;
                int m_nCalRow = 0;
                TStringList *strStrings = new TStringList;
                for (int i=0;i<50;i++)
                {
                    m_nCalCol = i % 10;
                    m_nCalRow = i / 10;
                    if (m_nCalCol<g_IniFile.m_nCols && m_nCalRow< g_IniFile.m_nRows)
                    {
                        strStrings->Add(FormatFloat("0.000", g_pMainThread->m_dRearUpperLaserDiff[i][0]).c_str());
                    }
                }
                strStrings->Delimiter = '/';
                AnsiString strRULD = strStrings->DelimitedText;
                strStrings->Clear();
                delete strStrings;

                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = strRULD;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            //-------------------------------------------------------------------------------------------------------------------------
            else if (strcmp(ParamItem[nX*5],"m_strLogInENGAccount")==0)
            {
                pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = g_IniFile.m_strLogInENGAccount;
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }
            else
            {
		        pProperty=new TiXmlElement("PROPERTY");
                AnsiString strResult = pIniFile->ReadString(Product_Section,ParamItem[nX*5],"0");
                if (strResult.IsEmpty()) strResult = "NULL";
		        pProperty->LinkEndChild(new TiXmlText(strResult.c_str()));
		        pParam->LinkEndChild(pProperty);
            }

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
    }
    else
    {
        pRoot->LinkEndChild(pData);
	    doc.LinkEndChild(pRoot);
	    SendXML(doc);
    }
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

    char *ParamItem[]={
        "m_nCols","mm","F4","0","10",                 //Layout行數                     //on Paper
		"m_nRows","mm","F4","0","5",                  //Layout列數                     //on Paper
		"m_dColPitch","mm","F4","0","1000",           //Layout行間距
		"m_dRowPitch","mm","F4","0","1000",           //Layout列間距

		"m_dLamHeight0","mm","F4","0","200",          //後流道 壓合高度
		"m_dLamHeight1","mm","F4","0","200",          //前流道 壓合高度
		"m_dLamStop0","mm","F4","0","200",            //後流道 接片高度
		"m_dLamStop1","mm","F4","0","200",            //前流道 接片高度
		"m_dLamGetPos0","mm","F4","0","200",          //後流道 出片高度
		"m_dLamGetPos1","mm","F4","0","200",          //前流道 出片高度
		"m_dLamVacHeight0","mm","F4","0","200",       // no used
		"m_dLamVacHeight1","mm","F4","0","200",       // no used

		"m_dLamTime0","s","F4","0","1000",            //後流道 壓合時間                     //on Paper
		"m_dLamTemp0","c","F4","0","1000",            //後流道 壓合溫度                    //on Paper
		"m_dLamPress0","kg","F4","0","20",            //後流道 壓合力量                    //on Paper
		"m_dLamTime1","s","F4","0","1000",            //前流道 壓合時間                    //on Paper
		"m_dLamTemp1","c","F4","0","1000",            //前流道 壓合溫度                    //on Paper
		"m_dLamPress1","kg","F4","0","20",            //前流道 壓合力量                    //on Paper

		"m_dLoadCellPosX0","mm","F4","0","1000",      //後流道 測重起始X
		"m_dLoadCellPosY0","mm","F4","0","1000",      //後流道 測重起始Y
		"m_dLaserUpPosX00","mm","F4","0","1000",      //後流道 上模測高X1
		"m_dLaserUpPosY00","mm","F4","0","1000",      //後流道 上模測高Y1
		"m_dLaserUpPosX01","mm","F4","0","1000",      //後流道 上模測高X2
		"m_dLaserUpPosY01","mm","F4","0","1000",      //後流道 上模測高Y2
		"m_dLaserUpPosX02","mm","F4","0","1000",      //後流道 上模測高X3
		"m_dLaserUpPosY02","mm","F4","0","1000",      //後流道 上模測高Y3
		"m_dLaserUpPosX03","mm","F4","0","1000",      //後流道 上模測高X4
		"m_dLaserUpPosY03","mm","F4","0","1000",      //後流道 上模測高Y4
		"m_dLaserDownPosX0","mm","F4","0","1000",     //後流道 下模測高X
		"m_dLaserDownPosY0","mm","F4","0","1000",     //後流道 下模測高Y
		"m_dLoadCellPosX1","mm","F4","0","1000",      //前流道 測重起始X
		"m_dLoadCellPosY1","mm","F4","0","1000",      //前流道 測重起始Y
		"m_dLaserUpPosX10","mm","F4","0","1000",      //前流道 上模測高X1
		"m_dLaserUpPosY10","mm","F4","0","1000",      //前流道 上模測高Y1
		"m_dLaserUpPosX11","mm","F4","0","1000",      //前流道 上模測高X2
		"m_dLaserUpPosY11","mm","F4","0","1000",      //前流道 上模測高Y2
		"m_dLaserUpPosX12","mm","F4","0","1000",      //前流道 上模測高X3
		"m_dLaserUpPosY12","mm","F4","0","1000",      //前流道 上模測高Y3
		"m_dLaserUpPosX13","mm","F4","0","1000",      //前流道 上模測高X4
		"m_dLaserUpPosY13","mm","F4","0","1000",      //前流道 上模測高Y4
		"m_dLaserDownPosX1","mm","F4","0","1000",     //前流道 下模測高X
		"m_dLaserDownPosY1","mm","F4","0","1000",     //前流道 下模測高Y

		"m_nRailOption","mm","F4","0","2",            //流到選擇 0=全開;1=前流道;2=後流道
		"m_bNotLam","mm","F4","0","1",                //不壓合 0=false;1=true
		"m_dPressCalRange","mm","F4","0","100",       //容許誤差(%)
		"m_dPressCalTime","mm","F4","0","100",        //穩定時間
		"m_dLamSecondHeight0","mm","F4","0","200",    //後流道 第二段高度
		"m_dLamSecondHeight1","mm","F4","0","200",    //前流道 第二段高度
		"m_dLamSecondTime0","mm","F4","0","1000",     //後流道 第二段速度(秒)
		"m_dLamSecondTime1","mm","F4","0","1000",     //前流道 第二段速度(秒)
		"m_dLamThirdHeight0","mm","F4","0","200",     //後流道 第三段高度
		"m_dLamThirdHeight1","mm","F4","0","200",     //前流道 第三段高度
		"m_dLamThirdTime0","mm","F4","0","1000",      //後流道 第三段速度(秒)
		"m_dLamThirdTime1","mm","F4","0","1000",      //前流道 第三段速度(秒)

		"m_dAutoStopRange","mm","F4","0","1000",      //自動校正 自動停機誤差(%)
		"m_nAutoInterval","mm","F4","0","1000",       //自動校正 間隔盤數
		"m_dAutoRunTempRange","mm","F4","0","1000",   //自動模式 容許溫度
		"m_dVacDelayTime","mm","F4","0","1000",       //第二次真空檢測延遲時間
        "m_dScaleOffsetFront","mm","F4","0","1000",   //前流道 壓力補償表
        "m_dScaleOffsetRear","mm","F4","0","1000",    //後流道 壓力補償表
        "m_nDownPercent","mm","F4","0","1000",        //緩衝壓力下降(%)

        "m_nHeadType","mm","F4","0","1000",           //熱壓頭型式                     //on Paper
        "m_strHeadScal","mm","F4","0","1000",         //熱壓頭尺寸                     //on Paper
        "m_strModuleScal","mm","F4","0","1000",       //下模尺寸                       //on Paper
        "m_nVacummOn","mm","F4","0","1",              //真空開啟                       //on Paper
        "m_nPressCheck","mm","F4","0","1",            //壓力認證                       //on Paper
        "m_nDummyCheck","mm","F4","0","1",            //DummyCheck                     //on Paper
        "m_strModuleNum","mm","F4","0","1000",        //模具編號                       //on Paper
        "m_strSetupEENum","mm","F4","0","1000",       //SetupEE                        //on Paper

        "m_dBLT","mm","F4","0","1000",                //BLT                   //on Paper
        "m_dTilt","mm","F4","0","1000",               //Tilt                  //on Paper
        "m_dGap","mm","F4","0","1000",                //Gap                   //on Paper
        "m_dKeyTemp00","mm","F4","0","1000",          //後 手key溫度1         //on Paper
        "m_dKeyTemp01","mm","F4","0","1000",          //後 手key溫度2         //on Paper
        "m_dKeyTemp02","mm","F4","0","1000",          //後 手key溫度3         //on Paper
        "m_dKeyTemp10","mm","F4","0","1000",          //前 手key溫度1         //on Paper
        "m_dKeyTemp11","mm","F4","0","1000",          //前 手key溫度2         //on Paper
        "m_dKeyTemp12","mm","F4","0","1000",          //前 手key溫度3         //on Paper
        //以上為產品參數

        //以下為機台變數
        "LastFilePath","mm","F4","0","1000",          //機台當下 FilePath                      //on Paper
        "LastFileName","mm","F4","0","1000",          //機台當下 FileName                      //on Paper

        "FrontPressCal","mm","F4","0","1000",         //機台當下 前流道 量測壓力                    //on Paper
        "RearPressCal","mm","F4","0","1000",          //機台當下 後流道 量測壓力                    //on Paper

        "FrontDownLaserDiff","mm","F4","0","1000",    //機台當下 前流道 下模誤差                   //on Paper
        "RearDownLaserDiff","mm","F4","0","1000",     //機台當下 後流道 下模誤差                   //on Paper
        "FrontUpperLaserDiff","mm","F4","0","1000",   //機台當下 前流道 上模誤差                   //on Paper
        "RearUpperLaserDiff","mm","F4","0","1000",    //機台當下 後流道 上模誤差                   //on Paper

        "m_strLogInENGAccount","mm","F4","0","1000",  //機台當下 登入者
		"END"};           //E:End


    TiXmlElement *pParam = pData1->FirstChildElement("PARAMETER");
    bool bResult = false;
    int nSize = 0;
    int nX=0;
    while (true)
    {
        if (pParam == NULL) break;
        AnsiString strParam = pParam->GetText();
        int nPos = strParam.Pos(",");
        AnsiString strA = strParam.SubString(1, nPos - 1);
        if (strcmp(ParamItem[nX*5],"END")==0) {bResult = true; break;}
        else if (strcmp(ParamItem[nX*5],strA.c_str())!=0) {bResult = false; break;}
        else bResult = true;
        nSize++;
        nX++;
        pParam = pParam->NextSiblingElement("PARAMETER");
    }
    if (nSize != 87) bResult = false;

    if (bResult == true)
    {
        //If there is no following path, to add new folder
        TStringList* strSplitPath = SplitString(strData, "\\");
        AnsiString strPath = strSplitPath->operator [](0) + "\\";
        for (int nX=0;nX<strSplitPath->Count-2;nX++)
        {
            strPath += strSplitPath->operator [](nX+1) + "\\";
            if (!FileExists(strPath.c_str())) _mkdir(strPath.c_str());
        }
        delete strSplitPath;

        //new IniFile
        TIniFile *pIniFile = new TIniFile(strData);

        TiXmlElement *pParam = pData1->FirstChildElement("PARAMETER");

        while (1)
        {
            if (pParam == NULL) break;
            AnsiString strParam = pParam->GetText();
            int nPos = strParam.Pos(",");
            AnsiString strA = strParam.SubString(1, nPos - 1);
            AnsiString strB = strParam.SubString(nPos + 1, strParam.Length() - strA.Length() + 1);


            if (strA == "m_strHeadScal")
            {
                //if not in MachineParam then add.
                TStringList* strList = SplitString(g_IniFile.m_strHeadScals, "/");
                bool bIsInputINstrList = false;
                for (int i=0;i<strList->Count;i++)
                {
                    if (strB == strList->Strings[i])
                    {
                        bIsInputINstrList = true;
                        break;
                    }
                    else bIsInputINstrList = false;
                }
                if (bIsInputINstrList == false && strB != "")
                {
                    g_IniFile.m_strHeadScals += (strB+"/");
                    g_IniFile.MachineFile(false);
                }
                delete strList;
                //add in ProductParam
                pIniFile->WriteString(Product_Section, strA, strB);
            }
            else if (strA == "m_strModuleScal")
            {
                //if not in MachineParam then add.
                TStringList* strList = SplitString(g_IniFile.m_strModuleScals, "/");
                bool bIsInputINstrList = false;
                for (int i=0;i<strList->Count;i++)
                {
                    if (strB == strList->Strings[i])
                    {
                        bIsInputINstrList = true;
                        break;
                    }
                    else bIsInputINstrList = false;
                }
                if (bIsInputINstrList == false && strB != "")
                {
                    g_IniFile.m_strModuleScals += (strB+"/");
                    g_IniFile.MachineFile(false);
                }
                delete strList;
                //add in ProductParam
                pIniFile->WriteString(Product_Section, strA, strB);
            }
            else if (strA == "m_dScaleOffsetFront")
            {
                TStringList *StrList = SplitString(strB, '/');
                for (int nx=0;nx<StrList->Count;nx++)
                {
                    pIniFile->WriteString("Offset_Table", strA+FormatFloat("0",nx), StrList->Strings[nx]);
                }
                delete StrList;
            }
            else if (strA == "m_dScaleOffsetRear")
            {
                TStringList *StrList = SplitString(strB, '/');
                for (int nx=0;nx<StrList->Count;nx++)
                {
                    pIniFile->WriteString("Offset_Table", strA+FormatFloat("0",nx), StrList->Strings[nx]);
                }
                delete StrList;
            }
            else if (strA == "m_nHeadType")
            {
                if (strB == "SOLID") pIniFile->WriteString(Product_Section, strA, "0");
                else if (strB == "HOLLOW") pIniFile->WriteString(Product_Section, strA, "1");
                else pIniFile->WriteString(Product_Section, strA, "0");
            }
            else if (strA == "m_nVacummOn")
            {
                if (strB == "FAIL") pIniFile->WriteString(Product_Section, strA, "0");
                else if (strB == "TRUE") pIniFile->WriteString(Product_Section, strA, "1");
                else pIniFile->WriteString(Product_Section, strA, "0");
            }
            else if (strA == "m_nPressCheck")
            {
                if (strB == "FAIL") pIniFile->WriteString(Product_Section, strA, "0");
                else if (strB == "TRUE") pIniFile->WriteString(Product_Section, strA, "1");
                else pIniFile->WriteString(Product_Section, strA, "0");
            }
            else if (strA == "m_nDummyCheck")
            {
                if (strB == "FAIL") pIniFile->WriteString(Product_Section, strA, "0");
                else if (strB == "TRUE") pIniFile->WriteString(Product_Section, strA, "1");
                else pIniFile->WriteString(Product_Section, strA, "0");
            }
            else if (strA == "m_strSetupEENum")
            {
                pIniFile->WriteString(Product_Section, strA, "CIM");
            }
            else if (strA == "m_dBLT")
            {
                TStringList* strInp = SplitString(strB, "/");
                for (int nX=0;nX<strInp->Count-1;nX++)
                {
                    pIniFile->WriteString(Product_Section, "m_dBLT"+IntToStr(nX), strInp->operator [](nX));
                }
                delete strInp;
            }
            else if (strA == "m_dTilt")
            {
                TStringList* strInp = SplitString(strB, "/");
                for (int nX=0;nX<strInp->Count-1;nX++)
                {
                    pIniFile->WriteString(Product_Section, "m_dTilt"+IntToStr(nX), strInp->operator [](nX));
                }
                delete strInp;
            }
            else if (strA == "m_dGap")
            {
                TStringList* strInp = SplitString(strB, "/");
                for (int nX=0;nX<strInp->Count-1;nX++)
                {
                    pIniFile->WriteString(Product_Section, "m_dGap"+IntToStr(nX), strInp->operator [](nX));
                }
                delete strInp;
            }
            else if (strA == "LastFilePath") {}
            else if (strA == "LastFileName") {}
            else if (strA == "FrontPressCal") {}
            else if (strA == "RearPressCal") {}
            else if (strA == "FrontDownLaserDiff") {}
            else if (strA == "RearDownLaserDiff") {}
            else if (strA == "FrontUpperLaserDiff") {}
            else if (strA == "RearUpperLaserDiff") {}
            else if (strA == "m_strLogInENGAccount") {}
            else
            {
                pIniFile->WriteString(Product_Section, strA, strB);
            }
            pParam = pParam->NextSiblingElement("PARAMETER");
        }

        SendXML("SET_PPBODY", "ACK", "ACK", "0");
        
	    delete pIniFile;
    }
    else SendXML("SET_PPBODY", "ACK", "ACK", "3");

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
    m_strBuffer = "";
}
//---------------------------------------------------------------------------
__fastcall CEQPXML::~CEQPXML()
{
    free( receiveRx);
    free( receive);
}
//---------------------------------------------------------------------------

void __fastcall CEQPXML::StartComm(TCustomWinSocket *pSocket)
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
    /*
    long nSize = 0;

    try
    {
        //receiveRx=(char *)VirtualAlloc(NULL, 30000, MEM_COMMIT, PAGE_READWRITE);//(char *)malloc(30000);//new char[30000];
        //receive=(char *)VirtualAlloc(NULL, 30000, MEM_COMMIT, PAGE_READWRITE);///(char *)malloc(30000);//new char[30000];

        if (receive==NULL || receiveRx==NULL) return;

        memset(receiveRx,0x00,30000);
        memset(receive,0x00,30000);

        m_pSocket->ReceiveBuf(receiveRx,30000);

        TiXmlDocument doc;
        int nIndex = 0;

        while(1)
        {

            doc.Clear();
            memset(receive,0x00,30000);

            if(nIndex>=30000) break;

            nSize=(receiveRx[0+nIndex]<<24)+(receiveRx[1+nIndex]<<16)+(receiveRx[2+nIndex]<<8)+receiveRx[3+nIndex]+4;//+5;
            if(nSize<=5 || nSize>=30000)
                break;

            memcpy(receive,receiveRx+nIndex,nSize);
            nIndex+=nSize;
            if(nIndex>=30000) break;


            doc.Parse(receive+5);
            TiXmlElement* pRoot = doc.FirstChildElement("Root");
            if(pRoot==NULL) break;
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
            else if (strMsgID=="QUERY_PPID") doQueryPPIDFullPath(receive+5);              //Secs/Gem S7F19   CIM Find Recipe folder and file name
            else if (strMsgID=="QUERY_PPBODY") doQueryPPBody(receive+5);                  //Secs/Gem S7F25   CIM Use Recipe file name get all recipe param
            else if (strMsgID=="SET_PPBODY") doSetPPBody(receive+5);                      //Secs/Gem S7F23   CIM Use param and value modify recipe, not add a new recipy

        }
	}
	catch(const EAccessViolation &e)
	{
		//Application->MessageBox(e.Message.c_str(), "Look", MB_OK);
		g_IniFile.AddLog("catch ProcessCIM",16);
		//delete receive;
    }
    */
}

//---------------------------------------------------------------------------
void __fastcall CEQPXML::ProcessCIMstr()
{
    try
    {
        TiXmlDocument doc;
        //-------------------------------------------------------------
        AnsiString strGet = m_pSocket->ReceiveText();

        while (true)
        {
            doc.Clear();

            if (m_strBuffer != "")
            {
                m_strBuffer += strGet;
                if (m_nSizeBuffer > m_strBuffer.Length())
                {
                    break;
                }
                else
                {
                    strGet = m_strBuffer;
                    m_strBuffer = "";
                    m_nSizeBuffer = 0;
                }
            }

            AnsiString strSize = strGet.SubString(0,5);
            int nSize0; ((int)strSize.c_str()[0]>=0) ? nSize0 = (int)strSize.c_str()[0]: nSize0 = (256+(int)strSize.c_str()[0]);
            int nSize1; ((int)strSize.c_str()[1]>=0) ? nSize1 = (int)strSize.c_str()[1]: nSize1 = (256+(int)strSize.c_str()[1]);
            int nSize2; ((int)strSize.c_str()[2]>=0) ? nSize2 = (int)strSize.c_str()[2]: nSize2 = (256+(int)strSize.c_str()[2]);
            int nSize3; ((int)strSize.c_str()[3]>=0) ? nSize3 = (int)strSize.c_str()[3]: nSize3 = (256+(int)strSize.c_str()[3]);
            int nSize4; ((int)strSize.c_str()[4]>=0) ? nSize4 = (int)strSize.c_str()[4]: nSize4 = (256+(int)strSize.c_str()[4]);
            int nSize = (nSize0<<24)+(nSize1<<16)+(nSize2<<8)+(nSize3<<0)+nSize4+4;

            if (strGet.Length()<nSize)
            {
                m_nSizeBuffer = nSize;
                m_strBuffer += strGet;
                break;
            }

            AnsiString strSubGet = strGet.SubString(6, nSize-7);

            doc.Parse(strSubGet.c_str());
            TiXmlElement* pRoot = doc.FirstChildElement("Root");
            if(pRoot==NULL) break;
            AnsiString strMsgID(pRoot->Attribute("MSGID"));
            if (strMsgID=="HOST_STATUS") doHostStatus(strSubGet.c_str());                         //Local realtime renew CIMAP status
            else if (strMsgID=="DTSET") doDTSet(strSubGet.c_str());                               // no used
            else if (strMsgID=="CIMMSG") doCIMMsg(strSubGet.c_str());                             //Secs/Gem S10F3   CIM show TEXT to machine
            else if (strMsgID=="QUERY_VID") doQueryVID(strSubGet.c_str());                        //Secs/Gem S1F3,S2F13  CIM Use SCID,ECID,DVID get machine param
            else if (strMsgID=="EVENT_REPORT") doEventReportAck(strSubGet.c_str());               //Local Rise ECID 1,3,115
            else if (strMsgID=="ALARM") doAlarmAck(strSubGet.c_str());                            // no used
            else if (strMsgID=="RCMD") doRCMD(strSubGet.c_str());                                 //Secs/Gem S2F21,S2F41 CIM send RCMD, change machine /RUN/STOP/PAUSE/RESUME/PPSELECT,PPID/LOCAL/REMOTE
            else if (strMsgID=="QUERY_PPID") doQueryPPIDFullPath(strSubGet.c_str());              //Secs/Gem S7F19   CIM Find Recipe folder and file name
            else if (strMsgID=="QUERY_PPBODY") doQueryPPBody(strSubGet.c_str());                  //Secs/Gem S7F25   CIM Use Recipe file name get all recipe param
            else if (strMsgID=="SET_PPBODY") doSetPPBody(strSubGet.c_str());                      //Secs/Gem S7F23   CIM Use param and value modify recipe, not add a new recipy


            strGet = strGet.SubString(nSize+1, strGet.Length()-nSize);
            //g_IniFile.AddLog("--------------------",20);
            //g_IniFile.AddLog(strGet.c_str(),strGet.Length());
            //g_IniFile.AddLog(IntToStr(strGet.Length()).c_str(),IntToStr(strGet.Length()).Length());
            //g_IniFile.AddLog("--------------------",20);
            if (strGet == "")
                break;
        }
	}
	catch(const EAccessViolation &e)
	{
		//Application->MessageBox(e.Message.c_str(), "Look", MB_OK);
		g_IniFile.AddLog("catch ProcessCIM",16);
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


//---------------------------------------------------------------------------
//common function
void __fastcall CEQPXML::generateLaserDiff(LaserDiff &Inp)
{
    AnsiString strRDLD = "";
    AnsiString strFDLD = "";
    AnsiString strRULD = "";
    AnsiString strFULD = "";
    int m_nCalCol = 0;
    int m_nCalRow = 0;

    TStringList *strStrings = new TStringList;
    for (int i=0;i<50;i++)
    {
        if (i == 10*(g_IniFile.m_nRows-1)+(g_IniFile.m_nCols-1))
        {
            strStrings->Add(FormatFloat("0.000", g_pMainThread->m_dRearDownLaserDiff[i][0]).c_str());
        }
    }
    strStrings->Delimiter = '/';
    strRDLD = strStrings->DelimitedText;
    strStrings->Clear();

    for (int i=0;i<50;i++)
    {
        if (i == 10*(g_IniFile.m_nRows-1)+(g_IniFile.m_nCols-1))
        {
            strStrings->Add(FormatFloat("0.000", g_pMainThread->m_dFrontDownLaserDiff[i][0]).c_str());
        }
    }
    strStrings->Delimiter = '/';
    strFDLD = strStrings->DelimitedText;
    strStrings->Clear();

    for (int i=0;i<50;i++)
    {
        m_nCalCol = i % 10;
        m_nCalRow = i / 10;
        if (m_nCalCol<g_IniFile.m_nCols && m_nCalRow< g_IniFile.m_nRows)
        {
            strStrings->Add(FormatFloat("0.000", g_pMainThread->m_dRearUpperLaserDiff[i][0]).c_str());
        }
    }
    strStrings->Delimiter = '/';
    strRULD = strStrings->DelimitedText;
    strStrings->Clear();

    for (int i=0;i<50;i++)
    {
        m_nCalCol = i % 10;
        m_nCalRow = i / 10;
        if (m_nCalCol<g_IniFile.m_nCols && m_nCalRow< g_IniFile.m_nRows)
        {
            strStrings->Add(FormatFloat("0.000", g_pMainThread->m_dFrontUpperLaserDiff[i][0]).c_str());
        }
    }
    strStrings->Delimiter = '/';
    strFULD = strStrings->DelimitedText;
    strStrings->Clear();
    delete strStrings;

    Inp.strRDLD = strRDLD;
    Inp.strFDLD = strFDLD;
    Inp.strRULD = strRULD;
    Inp.strFULD = strFULD;
}
