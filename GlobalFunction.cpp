//---------------------------------------------------------------------------


#pragma hdrstop

#include "GlobalFunction.h"
#include "keyboard.h"
//#include "fmBusy.h"
//#include "MotionControlDlg.h"

#include "math.h"
//#include "Inifile.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

//extern CIniFile g_IniFile;
//---------------------------------------------------------------------------
bool NumKeyboard(TEdit *pEdit,double dMaxValue,
  double dMinValue,bool bSign,bool bDot)
{
  double dValue;
  AnsiString strMessage;

  if(pEdit==NULL) return false;
  TKeyboardWnd *pKeyboardWnd=new TKeyboardWnd(Application->MainForm);
  strMessage.sprintf("Max Value=%4.3f",dMaxValue);
  pKeyboardWnd->labelMax->Caption=strMessage;
  strMessage.sprintf("Min Value=%4.3f",dMinValue);
  pKeyboardWnd->labelMin->Caption=strMessage;
  pKeyboardWnd->m_bUseDot=bDot;
  pKeyboardWnd->m_bUseNegative=bSign;
  pKeyboardWnd->textDefaultValue->Caption=pEdit->Text;
  //pKeyboardWnd->editSetValue->Text=pEdit->Text;
  pKeyboardWnd->editSetValue->Text="";
  if(pKeyboardWnd->ShowModal()==mrOk)
  {
        if(!pKeyboardWnd->editSetValue->Text.IsEmpty())
        {
                pEdit->Text=pKeyboardWnd->editSetValue->Text;
                dValue=pEdit->Text.ToDouble();
        }
        else dValue=0;
        
    if(dValue >dMaxValue || dValue <dMinValue)
    {
      strMessage.sprintf("輸入值超出範圍\n輸入數字需介於 %.3f ~ %.3f 之間!",dMinValue,dMaxValue);
      ShowMessage(strMessage);
      pEdit->Text=pKeyboardWnd->textDefaultValue->Caption;
    }
    
  }

  delete pKeyboardWnd;

  return true;
}
//---------------------------------------------------------------------------
bool StrKeyboard(TEdit *pEdit)
{
  if(pEdit==NULL) return false;
  TKeyboardWnd *pKeyboardWnd=new TKeyboardWnd(Application->MainForm);
  pKeyboardWnd->SetBigStyle();
  pKeyboardWnd->textDefaultValue->Caption=pEdit->Text;
  pKeyboardWnd->editSetValue->Text=pEdit->Text;
  
  if(pKeyboardWnd->ShowModal()==mrOk)
    pEdit->Text=pKeyboardWnd->editSetValue->Text;

  delete pKeyboardWnd;

  return true;
}
//---------------------------------------------------------------------------
bool StrPassworkKeyboard(TEdit *pEdit)
{
  if(pEdit==NULL) return false;
  TKeyboardWnd *pKeyboardWnd=new TKeyboardWnd(Application->MainForm);
  pKeyboardWnd->SetBigStyle();
  pKeyboardWnd->textDefaultValue->Caption=pEdit->Text;
  pKeyboardWnd->editSetValue->Text=pEdit->Text;
  pKeyboardWnd->editSetValue->PasswordChar='*';
  if(pKeyboardWnd->ShowModal()==mrOk)
    pEdit->Text=pKeyboardWnd->editSetValue->Text;

  delete pKeyboardWnd;

  return true;
}
//---------------------------------------------------------------------------
double GetAngle(DPoint &ptA,DPoint &ptB)
{
  const double PI=4*atan(1);
  double dDegree=atan( (ptA.dY-ptB.dY)/(ptA.dX-ptB.dX))/PI*180.0;

  //if((ptA.dX-ptB.dX)<0) dDegree+=180.0;
  

  return dDegree;
}
//---------------------------------------------------------------------------
double GetAngleEx(DPoint &ptA,DPoint &ptB)
{
  const double PI=4*atan(1);
  double dDegree=atan( (ptB.dY-ptA.dY)/(ptB.dX-ptA.dX))/PI*180.0;

  if((ptB.dX-ptA.dX)<0) dDegree+=180.0;
  

  return dDegree;
}
//---------------------------------------------------------------------------
void GetXYCenter(DPoint &ptA,DPoint &ptB,double dDeltaAngle,DPoint &ptCenter,DPoint &ptDest)
{
  const double PI=4*atan(1);
  double dAngleA=GetAngleEx(ptCenter,ptA);
  double dAngleB=GetAngleEx(ptCenter,ptB);

  dAngleA+=dDeltaAngle;
  dAngleB+=dDeltaAngle;
  double dTinA=dAngleA*PI/180.0;
  double dTinB=dAngleB*PI/180.0;
  DPoint ptZ;//,ptNewZ;
  DPoint ptNewA,ptNewB;

  double dRA=sqrt( pow(ptA.dX-ptCenter.dX,2.0)+pow(ptA.dY-ptCenter.dY,2.0) );
  double dRB=sqrt( pow(ptB.dX-ptCenter.dX,2.0)+pow(ptB.dY-ptCenter.dY,2.0) );
  ptNewA.dX=ptCenter.dX+dRA*cos(dTinA);
  ptNewA.dY=ptCenter.dY+dRA*sin(dTinA);
  ptNewB.dX=ptCenter.dX+dRB*cos(dTinB);
  ptNewB.dY=ptCenter.dY+dRB*sin(dTinB);

  ptDest.dX=(ptNewA.dX+ptNewB.dX)/2.0;
  ptDest.dY=(ptNewA.dY+ptNewB.dY)/2.0;
}
//---------------------------------------------------------------------------
void DDX_Float(bool bRead,double &dParameter,TEdit *pEdit,double dUnit)
{
  if(bRead) pEdit->Text=dParameter/dUnit;
  else dParameter=pEdit->Text.ToDouble()*dUnit;
}
//---------------------------------------------------------------------------
void DDX_Int(bool bRead,int &nParameter,TEdit *pEdit,double dUnit)
{
  if(bRead) pEdit->Text=floor((nParameter/dUnit)*1000.0)/1000.0;
  else nParameter=ceil(pEdit->Text.ToDouble()*dUnit);
}
//---------------------------------------------------------------------------
void DDX_Check(bool bRead,bool &bParameter,TCheckBox *pCheckBox)
{
  if(bRead) pCheckBox->Checked=bParameter;
  else bParameter=pCheckBox->Checked;
}
//---------------------------------------------------------------------------
void DDX_Radio(bool bRead,bool &bParameter,TRadioGroup *pRadio)
{
  if(bRead) pRadio->ItemIndex=bParameter;
  else bParameter=pRadio->ItemIndex;
}
//---------------------------------------------------------------------------
void DDX_Radio(bool bRead,int &nParameter,TRadioGroup *pRadio)
{
  if(bRead) pRadio->ItemIndex=nParameter;
  else nParameter=pRadio->ItemIndex;
}
//---------------------------------------------------------------------------
void DDX_TrackBar(bool bRead,int &nParameter,TTrackBar *pTrackBar)
{
  if(bRead) pTrackBar->Position=nParameter;
  else nParameter=pTrackBar->Position;
}
//---------------------------------------------------------------------------
void DDX_TrackBarFloat(bool bRead,double &dParameter,TTrackBar *pTrackBar,double dScale)
{
  if(bRead) pTrackBar->Position=dParameter*dScale;
  else dParameter=pTrackBar->Position/dScale;
}
//---------------------------------------------------------------------------
void DDX_String(bool bRead,AnsiString &strParameter,TEdit *pEdit)
{
  if(bRead) pEdit->Text=strParameter;
  else strParameter=pEdit->Text;
}
//---------------------------------------------------------------------------
void DDXFile_Int(bool bRead,TIniFile *pIniFile,AnsiString strSection,
  AnsiString strIdent,int &nParameter,int nDefault)
{
  if(bRead) nParameter=pIniFile->ReadInteger(strSection,strIdent,nDefault);
  else pIniFile->WriteInteger(strSection,strIdent,nParameter);
}
//---------------------------------------------------------------------------
void DDXFile_Float(bool bRead,TIniFile *pIniFile,AnsiString strSection,
  AnsiString strIdent,double &dParameter,double dDefault)
{
  if(bRead) dParameter=pIniFile->ReadFloat(strSection,strIdent,dDefault);
  else pIniFile->WriteFloat(strSection,strIdent,dParameter);
}
//---------------------------------------------------------------------------
void DDXFile_Bool(bool bRead,TIniFile *pIniFile,AnsiString strSection,
  AnsiString strIdent,bool &bParameter,bool bDefault)
{
  if(bRead) bParameter=pIniFile->ReadBool(strSection,strIdent,bDefault);
  else pIniFile->WriteBool(strSection,strIdent,bParameter);
}
//---------------------------------------------------------------------------
void DDXFile_String(bool bRead,TIniFile *pIniFile,AnsiString strSection,
  AnsiString strIdent,AnsiString &strParameter,AnsiString strDefault)
{
  if(bRead) strParameter=pIniFile->ReadString(strSection,strIdent,strDefault);
  else pIniFile->WriteString(strSection,strIdent,strParameter);
}
//---------------------------------------------------------------------------
/*
void LoaderMotion(TStaticText *pTextTitle,TEdit *pEditY,TEdit *pEditZ)
{
  TMotionControlWnd *pWnd=new TMotionControlWnd(Application->MainForm);
  if(pTextTitle!=NULL) pWnd->Caption=pTextTitle->Caption;
  if(pTextTitle!=NULL) pTextTitle->Color=clActiveCaption;
  pWnd->PageControl1->ActivePageIndex=LoaderSection;

  if(pWnd->ShowModal()==mrOk)
  {
    if(pEditY!=NULL) pEditY->Text=pWnd->m_nLoaderY/PLC_mm;
    if(pEditZ!=NULL) pEditZ->Text=pWnd->m_nLoaderZ/PLC_mm;
  }
   if(pTextTitle!=NULL) pTextTitle->Color=clMoneyGreen;
  delete pWnd;

}
//---------------------------------------------------------------------------
void FluxToolMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY,TEdit *pEditZ)
{
  TMotionControlWnd *pWnd=new TMotionControlWnd(Application->MainForm);
  if(pTextTitle!=NULL) pWnd->Caption=pTextTitle->Caption;
  if(pTextTitle!=NULL) pTextTitle->Color=clActiveCaption;
  pWnd->PageControl1->ActivePageIndex=FluxToolSection;

  if(pWnd->ShowModal()==mrOk)
  {
    if(pEditX!=NULL) pEditX->Text=pWnd->m_nFluxX/PLC_mm;
    if(pEditY!=NULL) pEditY->Text=pWnd->m_nBallFluxY/PLC_mm;
    if(pEditZ!=NULL) pEditZ->Text=pWnd->m_nFluxZ/PLC_mm;
  }
   if(pTextTitle!=NULL) pTextTitle->Color=clMoneyGreen;
  delete pWnd;
}
//---------------------------------------------------------------------------
void BallToolMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY,TEdit *pEditZ)
{
  TMotionControlWnd *pWnd=new TMotionControlWnd(Application->MainForm);

  if(pTextTitle!=NULL) pTextTitle->Color=clActiveCaption;
  pWnd->PageControl1->ActivePageIndex=BallToolSection;
  if(pTextTitle!=NULL) pWnd->Caption=pTextTitle->Caption;
  if(pWnd->ShowModal()==mrOk)
  {
    if(pEditX!=NULL) pEditX->Text=pWnd->m_nBallX/PLC_mm;
    if(pEditY!=NULL) pEditY->Text=pWnd->m_nBallFluxY/PLC_mm;
    if(pEditZ!=NULL) pEditZ->Text=pWnd->m_nBallZ/PLC_mm;
  }
   if(pTextTitle!=NULL) pTextTitle->Color=clMoneyGreen;
  delete pWnd;
}
//---------------------------------------------------------------------------
void LifterMotion(TStaticText *pTextTitle,TEdit *pEditY,TEdit *pEditZ,TEdit *pEditT)
{
  TMotionControlWnd *pWnd=new TMotionControlWnd(Application->MainForm);

  if(pTextTitle!=NULL) pTextTitle->Color=clActiveCaption;
  pWnd->PageControl1->ActivePageIndex=LifterSection;
  if(pTextTitle!=NULL) pWnd->Caption=pTextTitle->Caption;
  if(pWnd->ShowModal()==mrOk)
  {
    if(pEditY!=NULL) pEditY->Text=pWnd->m_nLifterY/PLC_mm;
    if(pEditZ!=NULL) pEditZ->Text=pWnd->m_nLifterZ/PLC_mm;
    if(pEditT!=NULL) pEditT->Text=pWnd->m_nLifterT/PLC_Degree;
  }
   if(pTextTitle!=NULL) pTextTitle->Color=clMoneyGreen;
  delete pWnd;
}
//---------------------------------------------------------------------------
void InspecMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY,TEdit *pEditZL)
{
  TMotionControlWnd *pWnd=new TMotionControlWnd(Application->MainForm);

  if(pTextTitle!=NULL) pTextTitle->Color=clActiveCaption;
  pWnd->PageControl1->ActivePageIndex=InspecSection;
  if(pTextTitle!=NULL) pWnd->Caption=pTextTitle->Caption;
  if(pWnd->ShowModal()==mrOk)
  {
    if(pEditX!=NULL) pEditX->Text=pWnd->m_nInspecX/PLC_mm;
    if(pEditY!=NULL) pEditY->Text=pWnd->m_nInspecY/PLC_mm;
    if(pEditZL!=NULL) pEditZL->Text=pWnd->m_nInspecZL/PLC_mm;
  }
  if(pTextTitle!=NULL)  pTextTitle->Color=clMoneyGreen;
  delete pWnd;
}
//---------------------------------------------------------------------------
void LaneChangeMotion(TStaticText *pTextTitle,TEdit *pEditY)
{
  TMotionControlWnd *pWnd=new TMotionControlWnd(Application->MainForm);

  if(pTextTitle!=NULL) pTextTitle->Color=clActiveCaption;
  pWnd->PageControl1->ActivePageIndex=LaneChangeSection;
  if(pTextTitle!=NULL) pWnd->Caption=pTextTitle->Caption;
  if(pWnd->ShowModal()==mrOk)
  {
    if(pEditY!=NULL) pEditY->Text=pWnd->m_nLaneChangeY/PLC_mm;
  }
  if(pTextTitle!=NULL) pTextTitle->Color=clMoneyGreen;
  delete pWnd;
}
//---------------------------------------------------------------------------
void RailMotion(TStaticText *pTextTitle,TEdit *pEditW1,TEdit *pEditW2,TEdit *pEditW3,TEdit *pEditW4)
{
  TMotionControlWnd *pWnd=new TMotionControlWnd(Application->MainForm);

  if(pTextTitle!=NULL) pTextTitle->Color=clActiveCaption;
  pWnd->PageControl1->ActivePageIndex=RailSection;
  if(pTextTitle!=NULL) pWnd->Caption=pTextTitle->Caption;
  if(pWnd->ShowModal()==mrOk)
  {
    if(pEditW1!=NULL) pEditW1->Text=pWnd->m_nLoaderW/PLC_mm;
    if(pEditW2!=NULL) pEditW2->Text=pWnd->m_nLifterW/PLC_mm;
    if(pEditW3!=NULL) pEditW3->Text=pWnd->m_nInspecW/PLC_mm;
    if(pEditW4!=NULL) pEditW4->Text=pWnd->m_nLaneChangeW/PLC_mm;
  }
   if(pTextTitle!=NULL) pTextTitle->Color=clMoneyGreen;
  delete pWnd;
}
//---------------------------------------------------------------------------
void BallPoolMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY)
{
  TMotionControlWnd *pWnd=new TMotionControlWnd(Application->MainForm);

  if(pTextTitle!=NULL) pTextTitle->Color=clActiveCaption;
  pWnd->PageControl1->ActivePageIndex=BallPoolSection;
  if(pTextTitle!=NULL) pWnd->Caption=pTextTitle->Caption;
  if(pWnd->ShowModal()==mrOk)
  {
    if(pEditX!=NULL) pEditX->Text=pWnd->m_nBallPoolX/PLC_BallPool_Degree;
    if(pEditY!=NULL) pEditY->Text=pWnd->m_nBallPoolY/PLC_mm;
  }
   if(pTextTitle!=NULL) pTextTitle->Color=clMoneyGreen;
  delete pWnd;
}
//---------------------------------------------------------------------------
void PosCCDMotion(TStaticText *pTextTitle,TEdit *pEditX,TEdit *pEditY,TEdit *pEditZ)
{
  #ifdef CCD_On_Flux
  FluxToolMotion(pTextTitle,pEditX,pEditY,pEditZ);
  #endif
  #ifdef CCD_On_Ball
  BallToolMotion(pTextTitle,pEditX,pEditY,pEditZ);
  #endif
}
*/
//---------------------------------------------------------------------------
void FindFileName(char *strSource,char *strDest)
{
    char *p,*pLast;
    char strName[200];

    strcpy(strName,strSource);
    p = strtok(strName, "\\");    //remove path

    while(p!=NULL)
    {
      pLast=p;
      p = strtok(NULL, "\\");
    }
    strcpy(strDest,pLast);

    p = strtok(strDest, ".");    //remove extention name
    while(p!=NULL)
    {
      pLast=p;
      p = strtok(NULL, ".");
    }
    *pLast='\0';
    
}

