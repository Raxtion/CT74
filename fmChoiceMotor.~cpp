//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmChoiceMotor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmChoiceMotor *frmChoiceMotor;
//---------------------------------------------------------------------------
__fastcall TfrmChoiceMotor::TfrmChoiceMotor(TComponent* Owner)
    : TForm(Owner)
{
    m_nSelectAxis=0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmChoiceMotor::btnPickAxisClick(TObject *Sender)
{
    m_nSelectAxis=((TBitBtn *)Sender)->Tag;
    m_strCaption=((TBitBtn *)Sender)->Caption;
}
//---------------------------------------------------------------------------
