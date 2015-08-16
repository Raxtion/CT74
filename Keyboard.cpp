//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Keyboard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TKeyboardWnd *KeyboardWnd;

//---------------------------------------------------------------------------
__fastcall TKeyboardWnd::TKeyboardWnd(TComponent* Owner)
  : TForm(Owner)
{
  m_bUseDot=TRUE;
  m_bUseNegative=FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TKeyboardWnd::OnKeyButtonClick(TObject *Sender)
{
  TBitBtn *pButton=(TBitBtn *)Sender;
  AnsiString strChar;

  if(!m_bUseDot && pButton->Tag==46)
    return;
  if(editSetValue->Text.Pos(".") && pButton->Tag==46)
    return;
  strChar.sprintf("%c",pButton->Tag);
  editSetValue->Text=editSetValue->Text+strChar;
}
//---------------------------------------------------------------------------
void __fastcall TKeyboardWnd::btnBackClick(TObject *Sender)
{
  int nLength=editSetValue->Text.Length();
  editSetValue->Text=editSetValue->Text.Delete(nLength,1);
}
//---------------------------------------------------------------------------
void __fastcall TKeyboardWnd::btnPositiveClick(TObject *Sender)
{
  if(editSetValue->Text.Length()==0)
    return;
  if(editSetValue->Text[1]==45)
    editSetValue->Text=editSetValue->Text.Delete(1,1);
}
//---------------------------------------------------------------------------
void __fastcall TKeyboardWnd::btnNegativeClick(TObject *Sender)
{
  if(!m_bUseNegative) return;
  if(editSetValue->Text.Length()==0)
    editSetValue->Text="-";

  if(editSetValue->Text[1]!=45)
    editSetValue->Text="-"+editSetValue->Text;

}
//---------------------------------------------------------------------------
void __fastcall TKeyboardWnd::btnOKClick(TObject *Sender)
{
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TKeyboardWnd::btnCancelClick(TObject *Sender)
{
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------



