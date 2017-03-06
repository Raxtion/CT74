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
  AnsiString m_strApplicationPath;
  AnsiString m_strLogInENGAccount;
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
  double m_dUpperLaserAlarm;
  double m_dDownLaserAlarm;
  double m_dUpperTotalLaserAlarm;
  double m_dLeftGassLeakylimit;
  double m_dRightGassLeakylimit;
  double m_dLaneTransportDelayTime;
  double m_dLamSubDelayTime;
  bool m_bForceEject;
  bool m_bIsUseF911;
  bool m_bIsUseIL030;
  bool m_bIsUseCIM;
  bool m_bIsMochineTestMode;
  bool m_bIsFullLaserMode;
  bool m_bIsUseDBOffset;
  bool m_bIsUpdateOffsetTable;
  int m_nLanguage;

  double m_dLoadCellX[2];    //For Calibrate Load Cell
  double m_dLoadCellY[2];

  double m_dPLimitF;
  double m_dNLimitF;
  double m_dPLimitR;
  double m_dNLimitR;

  double m_dLastLamPress[2];  //Record for LastPressCal. Protect Wrong press Lamination in AutoRun

  AnsiString m_strHeadScals;
  AnsiString m_strModuleScals;

  //pos
  double m_dLamHeight[2];          //1: front 0:rear
  double m_dLamStop[2];
  double m_dLamVacHeight[2];
  double m_dLamGetPos[2];
  double m_dLamSecondHeight[2];
  double m_dLamSecondTime[2];
  double m_dLamThirdHeight[2];
  double m_dLamThirdTime[2];

  int m_nAutoInterval;
  double m_dAutoStopRange;

  double m_dAutoRunTempRange;
  double m_dVacDelayTime;

  double m_dPressCalRange;
  double m_dPressCalTime;

  int m_nDownPercent;                 //for downPress to decrease cylinder friction

  double m_dTempOffsetF;
  double m_dTempOffsetR;


  //-----------------Product-----------------
  int m_nCols;
  int m_nRows;
  double m_dColPitch;
  double m_dRowPitch;

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
  double m_dLaserUpPosEndX[2][4];
  double m_dLaserUpPosEndY[2][4];
  double m_dLaserDownPosEndX[2];
  double m_dLaserDownPosEndY[2];

  double m_dScaleOffsetFront[50];
  double m_dScaleOffsetRear[50];

  int m_nRailOption;
  bool m_bNotLam;

  AnsiString m_strModuleNum[2];
  AnsiString m_strSetupEENum;
  int m_nBoatType;                    //0=Old ; 1=New ;
  int m_nHeadType;                    //0=���; 1=�Ť�;
  AnsiString m_strHeadScal;           //(43X43/31X31)
  AnsiString m_strModuleScal;         //(43X43/31X31)
  int m_nVacummOn;                    //0=FAIL; 1=TRUE
  int m_nPressCheck;                  //0=FAIL; 1=TRUE
  int m_nDummyCheck;                  //0=FAIL; 1=TRUE

  double m_dBLT[5];                   //for product param keyin
  double m_dTilt[5];                  //for product param keyin
  double m_dGap[5];                   //for product param keyin
  double m_dKeyTemp[2][3];            //for product param keyin

  double m_dCoolDownTimeF;
  double m_dCoolDownTimeR;

  double m_dLamSecondKeepTime;
  bool m_bIsLamSecondStop;
  double m_dLamSecondCorrectTimes;
  bool m_bIsLamUpDownCorrect;
  
//Function
public:
  void __fastcall MachineFile(bool bRead);
  void __fastcall ProductFile(char *strFileName,bool bRead);
  AnsiString __fastcall GetErrorString(char *SectionName,int nCode);
  AnsiString __fastcall GetErrorStringENG(char *SectionName,int nCode);
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
