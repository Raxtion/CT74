//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmMachinParam.h"
#include "fmMain.h"
#include "EQPXML.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TfrmMain *frmMain;
extern CEQPXML g_eqpXML;
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
        g_eqpXML.m_CIMStatus = "0";
    }
}
//---------------------------------------------------------------------------

