//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmAD.h"
#include "TA5Serial.h";
#include "IniFile.h"


extern CTA5Serial g_Balance;
extern CIniFile g_IniFile;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmAD *frmAD;
//---------------------------------------------------------------------------
__fastcall TfrmAD::TfrmAD(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmAD::Timer1Timer(TObject *Sender)
{
        Timer1->Enabled=false;

        //Label1->Caption=g_Balance.GetData(1);
        Label2->Caption="Wiight:"+FormatFloat("0.000",g_Balance.GetKg(1))+" Kg";
        Label1->Caption=g_Balance.nTmpData;

        Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAD::SpeedButton1Click(TObject *Sender)
{
        g_Balance.EnableZero(1);        
}
//---------------------------------------------------------------------------


void __fastcall TfrmAD::SpeedButton2Click(TObject *Sender)
{
        if(Application->MessageBox("請於荷重元上放置指定砝碼然後按下確定", "Look", MB_OKCANCEL)==IDCANCEL) return;
        
        double dWeight=editWeight->Text.ToDouble();      //g
        
        ShowMessage("再次確認");
        int nDiv=g_Balance.GetData(1);

        if(nDiv==-1)
        {
                return ShowMessage("荷重元讀取異常 請檢查連線!!");
        }

        g_IniFile.m_dLoadCellX[0]=0.0;
        g_IniFile.m_dLoadCellX[1]=nDiv;
        g_IniFile.m_dLoadCellY[0]=0.0;
        g_IniFile.m_dLoadCellY[1]=dWeight;

        g_Balance.SetXY(0.0,nDiv,0.0,dWeight);

        ShowMessage("校正完成,請取下砝碼");




}
//---------------------------------------------------------------------------

