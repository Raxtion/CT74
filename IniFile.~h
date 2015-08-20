//---------------------------------------------------------------------------

#ifndef IniFileH
#define IniFileH

#include "vcl.h"
#include <vector>
#include "GlobalFunction.h"


#define System_Section "System"
#define Machine_Section "Machine"
#define Product_Section "Product"






//---------------------------------------------------------------------------
class CIniFile
{
//Variable
public:
   int m_nErrorCode;
  //System
  int m_nLanguageMode;    //0=Chinese 1=English
  int m_nPriviledge;
  AnsiString m_strARTPassword;
  AnsiString m_strENGPassword;
  AnsiString m_strLastFileName;
  double m_dCycleTimeOut;

  std::vector<AnsiString> m_vecMsg;

  __fastcall CIniFile();

  //-----------------Machine-----------------
   double m_dACCSpeed[8];
  double m_dDECSpeed[8];
  double m_dJogSpeed[8];
  double m_dWorkSpeed[8];

  double m_dLCEntryPos;
  double m_dLCFrontPos;
  double m_dLCRearPos;

  double m_dSafePos;           //Safe Position
  bool m_bForceEject;
  int m_nLanguage;

  double m_dLoadCellX[2];    //For Calibrate Load Cell
  double m_dLoadCellY[2];

  double m_dPLimitF;
  double m_dNLimitF;
  double m_dPLimitR;
  double m_dNLimitR;

  //-----------------Product-----------------
  int m_nCols;
  int m_nRows;
  double m_dColPitch;
  double m_dRowPitch;
  //pos
  double m_dLamHeight[2];          //1: front 0:rear
  double m_dLamStop[2];
  double m_dLamVacHeight[2];
  double m_dLamGetPos[2];
  double m_dLamSecondHeight[2];
  double m_dLamSecondTime[2];
  double m_dLamThirdHeight[2];
  double m_dLamThirdTime[2];

  //                                //1:front 0:rear
  double m_dLamTime[2];
  double m_dLamTemp[2];
  double m_dLamPress[2];

  double m_dLoadCellPosX[2];
  double m_dLoadCellPosY[2];
  double m_dLaserUpPosX[2][4];
  double m_dLaserUpPosY[2][4];
  double m_dLaserDownPosX[2];
  double m_dLaserDownPosY[2];

  int m_nAutoInterval;
  double m_dAutoStopTimes;

  double m_dScaleOffsetFront[50];
  double m_dScaleOffsetRear[50];

  int m_nRailOption;
  bool m_bNotLam;

  double m_dPressCalRange;
  double m_dPressCalTime;

  double m_dAutoRunTempRange;
  double m_dVacDelayTime;
                     
//Function
public:
  void __fastcall MachineFile(bool bRead);
  void __fastcall ProductFile(char *strFileName,bool bRead);
  AnsiString __fastcall GetErrorString(char *SectionName,int nCode);
  void __fastcall StoreHistory(int nYear,int nMonth,int nDate,AnsiString strMessage);
  void __fastcall StoreHistoryNew();
  bool __fastcall SearchFile(AnsiString strBarCode,AnsiString &strFileName);

  void __fastcall AddNewID(int nLevel,char *strID,char *strPassword);
  AnsiString __fastcall ReadPassword(int nLevel,char *strID);
  void __fastcall AddLog(char *pRx,int nSize);

  AnsiString __fastcall GetFileNameWithNewExt(char *strFileName,char *strExtension);
  AnsiString __fastcall GetFileNameWithoutExt(char *strFileName);
};
#endif
