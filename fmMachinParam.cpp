//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmMachinParam.h"
#include "fmMain.h"
#include "EQPXML.h"
#include "fmMotorCheck.h"
#include "MyPISODIO.h"
#include "IniFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TfrmMain *frmMain;
extern CEQPXML g_eqpXML;
extern CIniFile g_IniFile;
extern CMyPISODIO g_DIO;
//TfrmMachineParam *frmMachineParam;

//---------------------------------------------------------------------------
__fastcall TfrmMachineParam::TfrmMachineParam(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TfrmMachineParam::m_bIsUseCIMClick(TObject *Sender)
{
    if (m_bIsUseCIM->Checked == true)
    {
        frmMain->ServerCIM->Active = true;
        frmMain->AddList("CIM Start!");
    }
    else
    {
        frmMain->ServerCIM->Active = false;
        frmMain->AddList("CIM Stop!");
        frmMain->Shape3->Visible = false;
        g_eqpXML.m_CIMStatus = "0";
    }
}
//---------------------------------------------------------------------------





void __fastcall TfrmMachineParam::m_dLamHeightDblClick(TObject *Sender)
{
    //pEdit->Tag X=0, Y=1, Lane Changer=2, Front Lifter=3, Rear Lifter=4

    TEdit *pEdit=(TEdit *)Sender;
    TfrmMotorCheck *pMotorCheckDlg;

    AnsiString strCap = "";
    if (pEdit->Tag == 0)
    {
        strCap = "X";
        if (!g_DIO.ReadDIBit(DI::LoadCellDown))
        {
            Application->MessageBoxA("請確認LoadCell是否在上位!!", "Confirm", MB_OK);
            return;
        }
    }
    else if (pEdit->Tag == 1)
    {
        strCap = "Y";
        if (!g_DIO.ReadDIBit(DI::LoadCellDown))
        {
            Application->MessageBoxA("請確認LoadCell是否在上位!!", "Confirm", MB_OK);
            return;
        }
    }
    else if (pEdit->Tag == 2) strCap = "Lane Changer";
    else if (pEdit->Tag == 4)
    {
        strCap = "Front Lifter";
        if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB))
        {
            Application->MessageBoxA("請確認LoadCell是否在安全位置!!", "Confirm", MB_OK);
            return;
        }
    }
    else if (pEdit->Tag == 5)
    {
        strCap = "Rear Lifter";
        if (!g_DIO.ReadDIBit(DI::YAxisSafePosA) || !g_DIO.ReadDIBit(DI::YAxisSafePosB))
        {
            Application->MessageBoxA("請確認LoadCell是否在安全位置!!", "Confirm", MB_OK);
            return;
        }
    }
    else strCap = "Error!";

    pMotorCheckDlg=new TfrmMotorCheck(this);
    pMotorCheckDlg->m_nActiveAxis=pEdit->Tag;
    pMotorCheckDlg->Caption=strCap;

    AnsiString sPath = g_IniFile.m_strApplicationPath;
	sPath = StringReplace(sPath, "\\exe\\", "", TReplaceFlags());

    if (pMotorCheckDlg->m_nActiveAxis == 0)
    {
        pMotorCheckDlg->btnFWD->Glyph->LoadFromFile(sPath + "\\bmp\\right.bmp");
        pMotorCheckDlg->btnRWD->Glyph->LoadFromFile(sPath + "\\bmp\\left.bmp");
    }
    else if (pMotorCheckDlg->m_nActiveAxis == 4 || pMotorCheckDlg->m_nActiveAxis == 5)
    {
        pMotorCheckDlg->btnRWD->Glyph->LoadFromFile(sPath + "\\bmp\\down.bmp");
        pMotorCheckDlg->btnFWD->Glyph->LoadFromFile(sPath + "\\bmp\\up.bmp");
    }

    if(pMotorCheckDlg->ShowModal()==mrOk)
    {
        TEdit *pEdit2=(TEdit *)FindComponent(pEdit->Hint);
        if(pEdit2) pEdit2->Text=pMotorCheckDlg->m_dCurrPos;
        else ShowMessage("寫入失敗");
    }

    delete pMotorCheckDlg;
}
//---------------------------------------------------------------------------


