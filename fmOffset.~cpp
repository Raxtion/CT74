//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmOffset.h"
#include "fmMain.h" 
#include "IniFile.h"
#include "PISODNM100.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TfrmOffser *frmOffser;
extern TfrmMain *frmMain;
extern CIniFile g_IniFile; 
extern CPISODNM100 g_DNPort0;
extern CPISODNM100 g_DNPort1;
//---------------------------------------------------------------------------
__fastcall TfrmOffser::TfrmOffser(TComponent* Owner)
        : TForm(Owner)
{
        //dTotalLayOut = g_IniFile.m_nRows * g_IniFile.m_nCols;
        AnsiString strCtl;
        dTotalLayOut = 50;
        for (int nIndex = 0; nIndex<dTotalLayOut; nIndex++)
	{
                AnsiString strGetValue = FormatFloat("0.00", 0.0);
                AnsiString strSetValue = FormatFloat("0.00", 0.0);
                ((TLabel *)this->FindComponent(strCtl.sprintf("Label%d", nIndex + 51)))->Caption = strSetValue;
                ((TLabel *)this->FindComponent(strCtl.sprintf("Label%d", nIndex + 151)))->Caption = strGetValue;
                ((TLabel *)this->FindComponent(strCtl.sprintf("Label%d", nIndex + 1)))->Caption = strSetValue;
                ((TLabel *)this->FindComponent(strCtl.sprintf("Label%d", nIndex + 101)))->Caption = strGetValue;
        }

}
//---------------------------------------------------------------------------
void __fastcall TfrmOffser::Timer1Timer(TObject *Sender)
{
        AnsiString strCtl;
        for (int nIndex = 0; nIndex<dTotalLayOut; nIndex++)
	{
                int nCol = nIndex % 10;
		int nRow = nIndex / 10;
		if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows)
		{
                        double dGetSetKgValue = g_DNPort0.GetSetKg(nIndex);
                        double dGetKgValue = g_DNPort0.GetKg(nIndex);
                        AnsiString strGetValue = FormatFloat("0.00", dGetKgValue);
                        AnsiString strSetValue = FormatFloat("0.00", dGetSetKgValue);
                        ((TLabel *)this->FindComponent(strCtl.sprintf("Label%d", nIndex + 51)))->Caption = strSetValue;
                        ((TLabel *)this->FindComponent(strCtl.sprintf("Label%d", nIndex + 151)))->Caption = strGetValue;
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TfrmOffser::Timer2Timer(TObject *Sender)
{
        AnsiString strCtl;
        for (int nIndex = 0; nIndex<dTotalLayOut; nIndex++)
	{
                int nCol = nIndex % 10;
		int nRow = nIndex / 10;
		if (nCol<g_IniFile.m_nCols && nRow< g_IniFile.m_nRows)
		{
                        double dGetSetKgValue = g_DNPort1.GetSetKg(nIndex);
                        double dGetKgValue = g_DNPort1.GetKg(nIndex);
                        AnsiString strGetValue = FormatFloat("0.00", dGetKgValue);
                        AnsiString strSetValue = FormatFloat("0.00", dGetSetKgValue);
                        ((TLabel *)this->FindComponent(strCtl.sprintf("Label%d", nIndex + 1)))->Caption = strSetValue;
                        ((TLabel *)this->FindComponent(strCtl.sprintf("Label%d", nIndex + 101)))->Caption = strGetValue;
                }
        }
}
//---------------------------------------------------------------------------


