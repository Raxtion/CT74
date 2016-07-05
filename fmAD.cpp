//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmAD.h"
#include "TA5Serial.h";
#include "IniFile.h"
#include "SensoLinkF911.h"
#include "DeltaPLC.h"

extern CTA5Serial g_Balance;
extern CIniFile g_IniFile;
//extern CSensoLinkF911 g_F911ModBus; //RS485 used. and merge to DeltaPLC.
extern CDeltaPLC g_ModBus;
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

        if (!g_IniFile.m_bIsUseF911)
        {
            Label1->Caption=g_Balance.GetData(1);
            Label2->Caption="Wiight:"+FormatFloat("0.000",g_Balance.GetKg(1))+" Kg";
            Label1->Caption=g_Balance.nTmpData;
        }
        else
        {
            Label2->Caption="Wiight:"+FormatFloat("0.000",g_ModBus.dF911Data)+" Kg";
            Label1->Caption=g_ModBus.dF911Data;
        }

        Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmAD::SpeedButton1Click(TObject *Sender)
{
        if (!g_IniFile.m_bIsUseF911) g_Balance.EnableZero(1);
        else g_ModBus.InitialZero(4);
}
//---------------------------------------------------------------------------


void __fastcall TfrmAD::SpeedButton2Click(TObject *Sender)
{
        if(Application->MessageBox("�Щ�������W��m���w�|�X�M����U�T�w", "Look", MB_OKCANCEL)==IDCANCEL) return;
        
        double dWeight=editWeight->Text.ToDouble();      //g
        
        ShowMessage("�A���T�{");
        int nDiv = -1;
        if (!g_IniFile.m_bIsUseF911)
            nDiv=g_Balance.GetData(1);

        if(nDiv==-1)
        {
                return ShowMessage("������Ū�����` ���ˬd�s�u!!");
        }

        g_IniFile.m_dLoadCellX[0]=0.0;
        g_IniFile.m_dLoadCellX[1]=nDiv;
        g_IniFile.m_dLoadCellY[0]=0.0;
        g_IniFile.m_dLoadCellY[1]=dWeight;

        if (!g_IniFile.m_bIsUseF911)
        {
            g_Balance.SetXY(0.0,nDiv,0.0,dWeight);
            ShowMessage("�ե�����,�Ш��U�|�X");
        }

        ShowMessage("F911�|�L�ե��\��");


}
//---------------------------------------------------------------------------

