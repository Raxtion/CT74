//---------------------------------------------------------------------------


#pragma hdrstop

#include "IniFile.h"
#include <IniFiles.hpp>
#include "time.h"
#include <dir.h>
#include "stdlib.h"
#include <stdio.h>

#define ErrorMessage_Dir "Error Message\\"
#define ErrorMessageFileName "C74ErrorMessage.ini"
#define ErrorMessageEngFileName "C74ErrorMessageEng.ini"
#define UserName "User.sis"

#define IniFile_Dir "C:\\Product_Data\\"
#define MachineFileName "C74Machine.sis"

CIniFile g_IniFile;

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

void __fastcall CIniFile::MachineFile(bool bRead)
{
  AnsiString strFile;
  TIniFile *pIniFile;
                    
  strFile.sprintf("%s%s",IniFile_Dir,MachineFileName);
  pIniFile = new TIniFile(strFile);

  //System
  DDXFile_Int(bRead,pIniFile,System_Section,"Language",m_nLanguageMode);
  DDXFile_Int(bRead,pIniFile,System_Section,"Priviledge",m_nPriviledge);
  DDXFile_String(bRead,pIniFile,System_Section,"ARTPassword",m_strARTPassword,"CLARE");
  DDXFile_String(bRead,pIniFile,System_Section,"ENGPassword",m_strENGPassword,"123");
  DDXFile_String(bRead,pIniFile,System_Section,"LastFileName",m_strLastFileName,"C:\\Product_Data\\Default.ini");
  DDXFile_String(bRead,pIniFile,System_Section,"LogInENGAccount",m_strLogInENGAccount,"OP");

  //Machine
  DDXFile_Float(bRead,pIniFile,Machine_Section,"JogSpeed1",m_dJogSpeed[0],30.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"JogSpeed2",m_dJogSpeed[1],30.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"JogSpeed3",m_dJogSpeed[2],30.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"JogSpeed4",m_dJogSpeed[3],30.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"JogSpeed5",m_dJogSpeed[4],30.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"JogSpeed6",m_dJogSpeed[5],30.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"JogSpeed7",m_dJogSpeed[6],30.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"JogSpeed8",m_dJogSpeed[7],30.0);


  DDXFile_Float(bRead,pIniFile,Machine_Section,"WorkSpeed1",m_dWorkSpeed[0],50.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"WorkSpeed2",m_dWorkSpeed[1],50.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"WorkSpeed3",m_dWorkSpeed[2],50.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"WorkSpeed4",m_dWorkSpeed[3],50.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"WorkSpeed5",m_dWorkSpeed[4],50.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"WorkSpeed6",m_dWorkSpeed[5],50.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"WorkSpeed7",m_dWorkSpeed[6],50.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"WorkSpeed8",m_dWorkSpeed[7],50.0);


  DDXFile_Float(bRead,pIniFile,Machine_Section,"ACCSpeed1",m_dACCSpeed[0],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"ACCSpeed2",m_dACCSpeed[1],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"ACCSpeed3",m_dACCSpeed[2],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"ACCSpeed4",m_dACCSpeed[3],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"ACCSpeed5",m_dACCSpeed[4],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"ACCSpeed6",m_dACCSpeed[5],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"ACCSpeed7",m_dACCSpeed[6],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"ACCSpeed8",m_dACCSpeed[7],0.1);

  DDXFile_Float(bRead,pIniFile,Machine_Section,"DECSpeed1",m_dDECSpeed[0],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"DECSpeed2",m_dDECSpeed[1],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"DECSpeed3",m_dDECSpeed[2],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"DECSpeed4",m_dDECSpeed[3],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"DECSpeed5",m_dDECSpeed[4],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"DECSpeed6",m_dDECSpeed[5],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"DECSpeed7",m_dDECSpeed[6],0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"DECSpeed8",m_dDECSpeed[7],0.1);

  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLCEntryPos",m_dLCEntryPos,0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLCFrontPos",m_dLCFrontPos,0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLCRearPos",m_dLCRearPos,0);

  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dSafePos",m_dSafePos,0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dUpperLaserAlarm",m_dUpperLaserAlarm,0.1);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dDownLaserAlarm",m_dDownLaserAlarm,0.2);
  DDXFile_Bool(bRead,pIniFile,Machine_Section,"m_bForceEject",m_bForceEject,false);
  DDXFile_Bool(bRead,pIniFile,Machine_Section,"m_bIsUseCIM",m_bIsUseCIM,false);

  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dPLimitF",m_dPLimitF,0.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dNLimitF",m_dNLimitF,0.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dPLimitR",m_dPLimitR,0.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dNLimitR",m_dNLimitR,0.0);

  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLoadCellX0",m_dLoadCellX[0],0.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLoadCellX1",m_dLoadCellX[1],0.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLoadCellY0",m_dLoadCellY[0],0.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLoadCellY1",m_dLoadCellY[1],0.0);

  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLastLamPress0",m_dLastLamPress[0],0.0);
  DDXFile_Float(bRead,pIniFile,Machine_Section,"m_dLastLamPress1",m_dLastLamPress[1],0.0);

  DDXFile_String(bRead,pIniFile,Machine_Section,"m_strHeadScals",m_strHeadScals,"2x2/3x3/4x4/");
  DDXFile_String(bRead,pIniFile,Machine_Section,"m_strModuleScals",m_strModuleScals,"2x2/3x3/4x4/");
  delete pIniFile;
}
//---------------------------------------------------------------------------
void __fastcall CIniFile::ProductFile(char *strFileName,bool bRead)
{
  AnsiString strName;
  TIniFile *pIniFile;
  pIniFile = new TIniFile(strFileName);

  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nCols",m_nCols,8);
  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nRows",m_nRows,3);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dColPitch",m_dColPitch,10.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dRowPitch",m_dRowPitch,10.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamHeight0",m_dLamHeight[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamHeight1",m_dLamHeight[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamStop0",m_dLamStop[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamStop1",m_dLamStop[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamGetPos0",m_dLamGetPos[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamGetPos1",m_dLamGetPos[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamVacHeight0",m_dLamVacHeight[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamVacHeight1",m_dLamVacHeight[1],0.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamSecondHeight0",m_dLamSecondHeight[0],10.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamSecondHeight1",m_dLamSecondHeight[1],10.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamSecondTime0",m_dLamSecondTime[0],1.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamSecondTime1",m_dLamSecondTime[1],1.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamThirdHeight0",m_dLamThirdHeight[0],1.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamThirdHeight1",m_dLamThirdHeight[1],1.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamThirdTime0",m_dLamThirdTime[0],60.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamThirdTime1",m_dLamThirdTime[1],60.0);

  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nAutoInterval",m_nAutoInterval,3);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dAutoStopRange",m_dAutoStopRange,2.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamTime0",m_dLamTime[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamTemp0",m_dLamTemp[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamPress0",m_dLamPress[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamTime1",m_dLamTime[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamTemp1",m_dLamTemp[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLamPress1",m_dLamPress[1],0.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLoadCellPosX0",m_dLoadCellPosX[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLoadCellPosY0",m_dLoadCellPosY[0],0.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosX00",m_dLaserUpPosX[0][0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosY00",m_dLaserUpPosY[0][0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosX01",m_dLaserUpPosX[0][1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosY01",m_dLaserUpPosY[0][1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosX02",m_dLaserUpPosX[0][2],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosY02",m_dLaserUpPosY[0][2],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosX03",m_dLaserUpPosX[0][3],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosY03",m_dLaserUpPosY[0][3],0.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserDownPosX0",m_dLaserDownPosX[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserDownPosY0",m_dLaserDownPosY[0],0.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLoadCellPosX1",m_dLoadCellPosX[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLoadCellPosY1",m_dLoadCellPosY[1],0.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosX10",m_dLaserUpPosX[1][0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosY10",m_dLaserUpPosY[1][0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosX11",m_dLaserUpPosX[1][1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosY11",m_dLaserUpPosY[1][1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosX12",m_dLaserUpPosX[1][2],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosY12",m_dLaserUpPosY[1][2],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosX13",m_dLaserUpPosX[1][3],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserUpPosY13",m_dLaserUpPosY[1][3],0.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserDownPosX1",m_dLaserDownPosX[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dLaserDownPosY1",m_dLaserDownPosY[1],0.0);

  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nRailOption",m_nRailOption,0);
  DDXFile_Bool(bRead,pIniFile,Product_Section,"m_bNotLam",m_bNotLam,false);

  for(int nIndex=0;nIndex<50;nIndex++)
  {
        DDXFile_Float(bRead,pIniFile,"Offset_Table","m_dScaleOffsetFront"+FormatFloat("0",nIndex),m_dScaleOffsetFront[nIndex],0.0);
        DDXFile_Float(bRead,pIniFile,"Offset_Table","m_dScaleOffsetRear"+FormatFloat("0",nIndex),m_dScaleOffsetRear[nIndex],0.0);
  }

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dPressCalRange",m_dPressCalRange,50.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dPressCalTime",m_dPressCalTime,5.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dAutoRunTempRange",m_dAutoRunTempRange,0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dVacDelayTime",m_dVacDelayTime,0.0);

  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nHeadType",m_nHeadType,0);
  DDXFile_String(bRead,pIniFile,Product_Section,"m_strHeadScal",m_strHeadScal,0);
  DDXFile_String(bRead,pIniFile,Product_Section,"m_strModuleScal",m_strModuleScal,0);
  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nVacummOn",m_nVacummOn,0);
  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nPressCheck",m_nPressCheck,0);
  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nDummyCheck",m_nDummyCheck,0);
  DDXFile_String(bRead,pIniFile,Product_Section,"m_strModuleNum",m_strModuleNum,"");
  DDXFile_String(bRead,pIniFile,Product_Section,"m_strSetupEENum",m_strSetupEENum,"");

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dBLT0",m_dBLT[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dBLT1",m_dBLT[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dBLT2",m_dBLT[2],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dBLT3",m_dBLT[3],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dBLT4",m_dBLT[4],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dTilt0",m_dTilt[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dTilt1",m_dTilt[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dTilt2",m_dTilt[2],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dTilt3",m_dTilt[3],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dTilt4",m_dTilt[4],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dGap0",m_dGap[0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dGap1",m_dGap[1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dGap2",m_dGap[2],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dGap3",m_dGap[3],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dGap4",m_dGap[4],0.0);

  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dKeyTemp00",g_IniFile.m_dKeyTemp[0][0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dKeyTemp01",g_IniFile.m_dKeyTemp[0][1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dKeyTemp02",g_IniFile.m_dKeyTemp[0][2],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dKeyTemp10",g_IniFile.m_dKeyTemp[1][0],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dKeyTemp11",g_IniFile.m_dKeyTemp[1][1],0.0);
  DDXFile_Float(bRead,pIniFile,Product_Section,"m_dKeyTemp12",g_IniFile.m_dKeyTemp[1][2],0.0);

  DDXFile_Int(bRead,pIniFile,Product_Section,"m_nDownPercent",m_nDownPercent,30);

  delete pIniFile;
}
//---------------------------------------------------------------------------
AnsiString __fastcall CIniFile::GetErrorString(char *SectionName,int nCode)
{
  AnsiString strFileName,strErrMsg,strErrMsg1,strCode,strErrName;
  TIniFile *pIniFile;

  if(m_nLanguageMode==0) strErrName=ErrorMessageFileName;
  else strErrName=ErrorMessageEngFileName;

  strFileName.sprintf("%s%s%s",IniFile_Dir,ErrorMessage_Dir,strErrName);
  pIniFile = new TIniFile(strFileName);

  strCode.sprintf("E%04d",nCode);
  if(m_nLanguageMode==0) strErrMsg=pIniFile->ReadString(SectionName,strCode,"未定義錯誤碼!!");
  else strErrMsg=pIniFile->ReadString(SectionName,strCode,"Not Defined Error Code!!!");
  strErrMsg1.sprintf("[%s] %s=%s",SectionName,strCode,strErrMsg);
  delete pIniFile;

  return strErrMsg1+"\n";
}
//---------------------------------------------------------------------------
AnsiString __fastcall CIniFile::GetErrorStringENG(char *SectionName,int nCode)
{
  AnsiString strFileName,strErrMsg,strErrMsg1,strCode,strErrName;
  TIniFile *pIniFile;

  strErrName=ErrorMessageEngFileName;

  strFileName.sprintf("%s%s%s",IniFile_Dir,ErrorMessage_Dir,strErrName);
  pIniFile = new TIniFile(strFileName);

  strCode.sprintf("E%04d",nCode);
  if(m_nLanguageMode==0) strErrMsg=pIniFile->ReadString(SectionName,strCode,"未定義錯誤碼!!");
  else strErrMsg=pIniFile->ReadString(SectionName,strCode,"Not Defined Error Code!!!");
  strErrMsg1.sprintf("[%s] %s=%s",SectionName,strCode,strErrMsg);
  delete pIniFile;

  return strErrMsg1+"\n";
}
//---------------------------------------------------------------------------
void __fastcall CIniFile::StoreHistory(int nYear,int nMonth,int nDate,AnsiString strMessage)
{
  AnsiString strFileName;
  TIniFile *pIniFile;
  int nFileMonth,nIndex; 

  strFileName.sprintf("%s\\Message History\\%4d_%02d_%02d.ini",IniFile_Dir,nYear,nMonth,nDate);
  pIniFile = new TIniFile(strFileName);

  nFileMonth=pIniFile->ReadInteger("Control","Month",1);
  nIndex=pIniFile->ReadInteger("Control","Index",1);

  if(nFileMonth!=nMonth)
  {
    nIndex=1;
    pIniFile->WriteInteger("Control","Month",nMonth);
  }

  AnsiString strID;
  strID.sprintf("No%08d",nIndex);
  pIniFile->WriteString("History",strID,strMessage);

  nIndex++;

  strID.sprintf("No%08d",nIndex);
  pIniFile->WriteString("History",strID,"//-------------以下做作廢--------------------//");

  pIniFile->WriteInteger("Control","Index",nIndex);

  delete pIniFile;
}
//---------------------------------------------------------------------------
void __fastcall CIniFile::StoreHistoryNew()
{
  AnsiString strFileName;
  TIniFile *pIniFile;
  int nFileMonth,nIndex;
  time_t timer;
  struct tm *tblock;

  int nSize=m_vecMsg.size();
  if(nSize==0) return;

  AnsiString strMsg;
  strMsg.sprintf("訊息共有 %d 筆\n每筆花費時間約 10 ms\n是否要儲存訊息??",nSize);
  if(Application->MessageBoxA(strMsg.c_str(),"Confirm",MB_ICONQUESTION|MB_OKCANCEL)!=IDOK) return;

   /* gets time of day */
   timer = time(NULL);
   /* converts date/time to a structure */
   tblock = localtime(&timer);
   int nMonth=tblock->tm_mon;
   int nDate=tblock->tm_mday;

  strFileName.sprintf("%s\\Message History\\%d.ini",IniFile_Dir,nDate);
  pIniFile = new TIniFile(strFileName);

  nFileMonth=pIniFile->ReadInteger("Control","Month",1);
  nIndex=pIniFile->ReadInteger("Control","Index",1);

  if(nFileMonth!=nMonth)
  {
    DeleteFile(strFileName);
    nIndex=1;
    pIniFile->WriteInteger("Control","Month",nMonth);
  }

  AnsiString strID;

  for(int nSz=0;nSz<nSize;nSz++)
  {
    strID.sprintf("No%08d",nIndex);
    pIniFile->WriteString("History",strID,m_vecMsg[nSz]);

    nIndex++;
  }
 
  m_vecMsg.clear();
  
  strID.sprintf("No%08d",nIndex);
  pIniFile->WriteString("History",strID,"//-------------以下做作廢--------------------//");

  pIniFile->WriteInteger("Control","Index",nIndex);

  delete pIniFile;
}
//---------------------------------------------------------------------------
bool __fastcall CIniFile::SearchFile(AnsiString strBarCode,AnsiString &strFileName)
{
  WIN32_FIND_DATA FindFileData;
  AnsiString strName;
  char *pFileName;
  TIniFile *pIniFile;
  bool bResult=false;
  
  strName.sprintf("%s*.ini",IniFile_Dir);
  HANDLE hFile=FindFirstFile(strName.c_str(),&FindFileData);
  if(hFile!=INVALID_HANDLE_VALUE)
  {
    while(FindNextFile(hFile,&FindFileData)!=0)
    {
      pFileName=FindFileData.cFileName;
      strName.sprintf("%s%s",IniFile_Dir,pFileName);
      pIniFile = new TIniFile(strName);
      AnsiString strCode=pIniFile->ReadString(Product_Section,"BarCode","0000");
      delete pIniFile;
      if(strCode==strBarCode)
      {
        strFileName=strName;
        bResult=true;
        break;
      }
    }
    FindClose(hFile);
  }

  return bResult;
}
//---------------------------------------------------------------------------
void __fastcall CIniFile::AddNewID(int nLevel,char *strID,char *strPassword)
{
  AnsiString strFile;
  TIniFile *pIniFile;
  char *Level[]={"Operator","Engineer","Supervisor"};

  if(nLevel<0 || nLevel>2) nLevel=0;
                    
  strFile.sprintf("%s%s",IniFile_Dir,UserName);
  pIniFile = new TIniFile(strFile);

  pIniFile->WriteString(Level[nLevel],strID,strPassword);

  delete pIniFile;
}
//---------------------------------------------------------------------------
AnsiString __fastcall CIniFile::ReadPassword(int nLevel,char *strID)
{
         AnsiString strFile;
  TIniFile *pIniFile;
  char *Level[]={"Operator","Engineer","Supervisor"};

  if(nLevel<0 || nLevel>2) nLevel=0;
                    
  strFile.sprintf("%s%s",IniFile_Dir,UserName);
  pIniFile = new TIniFile(strFile);

  AnsiString Pwd=pIniFile->ReadString(Level[nLevel],strID,"NOTFOUND");

  delete pIniFile;

  return Pwd;
}
//---------------------------------------------------------------------------
void __fastcall CIniFile::AddLog(char *pRx,int nSize)
 {
    
        AnsiString strTime;
        time_t timer;
        struct tm *tblock;

        timer = time(NULL);
        tblock = localtime(&timer);  

        try{

                FILE *pFile;

                strTime.sprintf("C:\\C74 Log\\%04d_%02d_%02d.txt",tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday);

                pFile=fopen(strTime.c_str(),"a+");

                if(pFile!=NULL)
                {
                        fprintf(pFile,"\n[%2d:%02d:%02d] ",tblock->tm_hour,tblock->tm_min, tblock->tm_sec);

                        int nIndex=0;
                        while(nIndex<nSize)
                        {
                                //if(pRx[nIndex]=='>') fprintf(pFile,"%c\n  ",pRx[nIndex]);
                                //else
                                fprintf(pFile,"%c",pRx[nIndex]);
                                nIndex++;
                        }
                        fclose(pFile);
                }

        } catch(const EAccessViolation &e)
        {

                //Application->MessageBox(e.Message.c_str(), "AddLog Exception",MB_OK);
        }
 }
 //---------------------------------------------------------------------------
 __fastcall CIniFile::CIniFile()
 {
        mkdir("C:\\C74 Log");
        m_strApplicationPath = ExtractFilePath(Application->ExeName);
 }
 //---------------------------------------------------------------------------
 AnsiString __fastcall CIniFile::GetFileNameWithoutExt(char *strFileName)
 {
        char drive[MAXDRIVE];
        char dir[MAXDIR];
        char file[MAXFILE];
        char ext[MAXEXT];
        int flags;

        flags=fnsplit(strFileName,drive,dir,file,ext);

        return file;
 }
 //---------------------------------------------------------------------------
 AnsiString __fastcall CIniFile::GetFileNameWithNewExt(char *strFileName,char *strExtension)
 {
        char drive[MAXDRIVE];
        char dir[MAXDIR];
        char file[MAXFILE];
        char ext[MAXEXT];
        int flags;

        flags=fnsplit(strFileName,drive,dir,file,ext);
        AnsiString strName;
        strName.sprintf("%s%s%s.%s",drive,dir,file,strExtension);

        return strName;
 }

