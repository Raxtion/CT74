//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmDIO.h"
#include "MyPISODIO.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern CMyPISODIO g_DIO;
//---------------------------------------------------------------------------
__fastcall TfmDIO::TfmDIO(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmDIO::Timer1Timer(TObject *Sender)
{
        Timer1->Enabled=false;
        TCheckBox *pCheck;

        for(int nIndex=0;nIndex<64;nIndex++)
        {
                pCheck=(TCheckBox *)FindComponent("CheckBox"+FormatFloat("0",nIndex+1));
                pCheck->Checked=g_DIO.ReadDIBit(nIndex);

                pCheck=(TCheckBox *)FindComponent("CheckBox"+FormatFloat("0",nIndex+65));
                pCheck->Checked=g_DIO.ReadDOBit(nIndex);
                
        }
        Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TfmDIO::CheckBox65Click(TObject *Sender)
{
        TCheckBox *pCheck=(TCheckBox *)Sender;
        pCheck->Checked ?g_DIO.WriteDOBitON(pCheck->Tag) : g_DIO.WriteDOBitOFF(pCheck->Tag);
}
//---------------------------------------------------------------------------

void __fastcall TfmDIO::Button1Click(TObject *Sender)
{
        g_DIO.WriteDOPort(0,0);
        g_DIO.WriteDOPort(1,0);
}
//---------------------------------------------------------------------------

void __fastcall TfmDIO::Button2Click(TObject *Sender)
{
        g_DIO.WriteDOPort(0,0xffffffff);
        g_DIO.WriteDOPort(1,0xffffffff);
}
//---------------------------------------------------------------------------

