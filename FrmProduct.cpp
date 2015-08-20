//---------------------------------------------------------------------------

#include <vcl.h>
#include "IniFile.h"
#pragma hdrstop

#include "FrmProduct.h"
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
        }
        else
        {
                //Create radioPosOption
                m_nRailOption->Items->Clear();
                m_nRailOption->Items->Add("全部都開啟");
                m_nRailOption->Items->Add("前流道");
                m_nRailOption->Items->Add("後流道");
        }
}
