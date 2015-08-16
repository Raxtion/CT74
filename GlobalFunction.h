//---------------------------------------------------------------------------

#ifndef GlobalFunctionH
#define GlobalFunctionH

#include <vcl.h>
#include <ComCtrls.hpp>
#include <IniFiles.hpp>

enum {LoaderSection=0,FluxToolSection,BallToolSection,
  LifterSection,InspecSection,LaneChangeSection,RailSection,BallPoolSection};
//---------------------------------------------------------------------------
struct DPoint
{
  double dX;
  double dY;
};

bool NumKeyboard(TEdit *pEdit,double dMaxValue,double dMinValue,bool bSign=false,bool bDot=false);
bool StrKeyboard(TEdit *pEdit);
bool StrPasswordKeyboard(TEdit *pEdit);
//Motion Control Dialog
/*
void LoaderMotion(TStaticText *pTextTitle,TEdit *pEditY,TEdit *pEditZ);
void FluxToolMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY,TEdit *pEditZ);
void BallToolMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY,TEdit *pEditZ);
void LifterMotion(TStaticText *pTextTitle,TEdit *pEditY,TEdit *pEditZ,TEdit *pEditT);
void InspecMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY,TEdit *pEditZL);
void LaneChangeMotion(TStaticText *pTextTitle,TEdit *pEditY);
void RailMotion(TStaticText *pTextTitle,TEdit *pEditW1,TEdit *pEditW2,TEdit *pEditW3,TEdit *pEditW4);
void BallPoolMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY);
void PosCCDMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY,TEdit *pEditZ);
*/
double GetAngle(DPoint &ptA,DPoint &ptB);
double GetAngleEX(DPoint &ptA,DPoint &ptB);
void GetXYCenter(DPoint &ptA,DPoint &ptB,double dDeltaAngle,DPoint &ptCenter,DPoint &ptDest);

//DDX Parameter
void DDX_Float(bool bRead,double &dParameter,TEdit *pEdit,double dUnit=1.0);
void DDX_Int(bool bRead,int &nParameter,TEdit *pEdit,double dUnit=1);
void DDX_Check(bool bRead,bool &bParameter,TCheckBox *pCheckBox);
void DDX_Radio(bool bRead,bool &bParameter,TRadioGroup *pRadio);
void DDX_Radio(bool bRead,int &nParameter,TRadioGroup *pRadio);
void DDX_TrackBar(bool bRead,int &nParameter,TTrackBar *pTrackBar);
void DDX_TrackBarFloat(bool bRead,double &dParameter,TTrackBar *pTrackBar,double dScale);
void DDX_String(bool bRead,AnsiString &strParameter,TEdit *pEdit);
//DDX File
void DDXFile_Int(bool bRead,TIniFile *pIniFile,AnsiString strSection,
  AnsiString strIdent,int &nParameter,int nDefault=0);
void DDXFile_Float(bool bRead,TIniFile *pIniFile,AnsiString strSection,
  AnsiString strIdent,double &dParameter,double dDefault=0.0);
void DDXFile_Bool(bool bRead,TIniFile *pIniFile,AnsiString strSection,
  AnsiString strIdent,bool &bParameter,bool bDefault=false);
void DDXFile_String(bool bRead,TIniFile *pIniFile,AnsiString strSection,
  AnsiString strIdent,AnsiString &strParameter,AnsiString strDefault="");


void FindFileName(char *strSource,char *strDest);




#endif
