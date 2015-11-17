//---------------------------------------------------------------------------

#include <vcl.h>
#include "IniFile.h"
#pragma hdrstop

#include "FrmProduct.h"
#include "fmHeadScalModify.h"
#include "fmMachinParam.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern CIniFile g_IniFile;

//TfmProduct *fmProduct;
//---------------------------------------------------------------------------
__fastcall TfmProduct::TfmProduct(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfmProduct::FormCreate(TObject *Sender)
{
    RenewRadioGroup();
    RenewCmbScal("HeadScal");
    RenewCmbScal("ModuleScal");
}
//---------------------------------------------------------------------------
void __fastcall TfmProduct::RenewRadioGroup()
{
        if (g_IniFile.m_nLanguageMode>0)
        {
                //Create radioPosOption
                m_nRailOption->Items->Clear();
                m_nRailOption->Items->Add("All Lane");
                m_nRailOption->Items->Add("Only Front Lane");
                m_nRailOption->Items->Add("Only Rear Lane");
                btnHeadScalModify->Caption = "HeadScal Modify";
        }
        else
        {
                //Create radioPosOption
                m_nRailOption->Items->Clear();
                m_nRailOption->Items->Add("全部都開啟");
                m_nRailOption->Items->Add("前流道");
                m_nRailOption->Items->Add("後流道");
                btnHeadScalModify->Caption = "壓頭尺寸修改";
        }
}

//---------------------------------------------------------------------------
void __fastcall TfmProduct::RenewCmbScal(AnsiString Hint)
{
    TComboBox* pCmb;
    if (Hint == "HeadScal") pCmb = this->m_cmbHeadScal;
    else if (Hint == "ModuleScal") pCmb = this->m_cmbModuleScal;
    else return;

    pCmb->Clear();

    TStringList* strList;
    if (Hint == "HeadScal") strList = SplitString(g_IniFile.m_strHeadScals, "/");
    else if (Hint == "ModuleScal") strList = SplitString(g_IniFile.m_strModuleScals, "/");
    else return;

    for (int i=0;i<strList->Count;i++)
    {
        pCmb->AddItem(strList->Strings[i], NULL);
    }
    pCmb->ItemIndex = 0;
    delete strList;
}

//---------------------------------------------------------------------------
void __fastcall TfmProduct::btnScalModifyClick(TObject *Sender)
{
    TButton* pbtn = (TButton*)Sender;
    TfrmHeadScalModify *pHeadScalModifyDlg = new TfrmHeadScalModify(this);

    TStringList* strList;
    if (pbtn->Hint == "HeadScal") strList = SplitString(g_IniFile.m_strHeadScals, "/");
    else if (pbtn->Hint == "ModuleScal") strList = SplitString(g_IniFile.m_strModuleScals, "/");
    else return;

    pHeadScalModifyDlg->cmbSelectData->AddItem("", NULL);
    for (int i=0;i<strList->Count;i++)
    {
        pHeadScalModifyDlg->cmbSelectData->AddItem(strList->Strings[i], NULL);
    }
    pHeadScalModifyDlg->cmbSelectData->ItemIndex = 0;
    Shortint result = pHeadScalModifyDlg->ShowModal();
	if (result == mrOk)                                                 //新增
	{
        bool bIsInputINstrList = false;
        for (int i=0;i<strList->Count;i++)
        {
            if (pHeadScalModifyDlg->cmbSelectData->Text == strList->Strings[i])
            {
                bIsInputINstrList = true;
                break;
            }
            else bIsInputINstrList = false;
        }
        if (bIsInputINstrList == false && pHeadScalModifyDlg->cmbSelectData->Text != "")
        {
            if (pbtn->Hint == "HeadScal") g_IniFile.m_strHeadScals += (pHeadScalModifyDlg->cmbSelectData->Text+"/");
            else if (pbtn->Hint == "ModuleScal") g_IniFile.m_strModuleScals += (pHeadScalModifyDlg->cmbSelectData->Text+"/");
            g_IniFile.MachineFile(false);
        }
	}
    else if (result == mrYes)                                           //刪除
    {
        bool bIsInputINstrList = false;
        for (int i=0;i<strList->Count;i++)
        {
            if (pHeadScalModifyDlg->cmbSelectData->Text == strList->Strings[i])
            {
                bIsInputINstrList = true;
                break;
            }
            else bIsInputINstrList = false;
        }
        if (bIsInputINstrList == true && pHeadScalModifyDlg->cmbSelectData->Text != "")
        {
            if (pbtn->Hint == "HeadScal") ReplaceString(&g_IniFile.m_strHeadScals, pHeadScalModifyDlg->cmbSelectData->Items->Strings[pHeadScalModifyDlg->cmbSelectData->ItemIndex]+"/", "");
            else if (pbtn->Hint == "ModuleScal") ReplaceString(&g_IniFile.m_strModuleScals, pHeadScalModifyDlg->cmbSelectData->Items->Strings[pHeadScalModifyDlg->cmbSelectData->ItemIndex]+"/", "");
            g_IniFile.MachineFile(false);
        }
    }
    RenewCmbScal(pbtn->Hint);
    delete strList;
    delete pHeadScalModifyDlg;
}
//---------------------------------------------------------------------------


