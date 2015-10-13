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
    RenewCmbHeadScal();
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
void __fastcall TfmProduct::RenewCmbHeadScal()
{
    this->m_cmbHeadScal->Clear();

    TStringList* strList = SplitString(g_IniFile.m_strHeadScal, "/");
    for (int i=0;i<strList->Count;i++)
    {
        this->m_cmbHeadScal->AddItem(strList->Strings[i], NULL);
    }
    this->m_cmbHeadScal->ItemIndex = 0;

    delete strList;
}

//---------------------------------------------------------------------------
void __fastcall TfmProduct::btnHeadScalModifyClick(TObject *Sender)
{
    TfrmHeadScalModify *pHeadScalModifyDlg = new TfrmHeadScalModify(this);

    TStringList* strList = SplitString(g_IniFile.m_strHeadScal, "/");

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
            g_IniFile.m_strHeadScal += (pHeadScalModifyDlg->cmbSelectData->Text+"/");
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
            ReplaceString(&g_IniFile.m_strHeadScal, pHeadScalModifyDlg->cmbSelectData->Items->Strings[pHeadScalModifyDlg->cmbSelectData->ItemIndex]+"/", "");
            g_IniFile.MachineFile(false);
        }
    }
    RenewCmbHeadScal();
    delete strList;
    delete pHeadScalModifyDlg;
}
//---------------------------------------------------------------------------

