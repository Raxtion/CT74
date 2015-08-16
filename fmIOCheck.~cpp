//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "fmIOCheck.h"
#include "math.h"
//#include "iniFile.h"
#include "MyPISODIO.h"

#define MAX_IN 64
#define MAX_OUT 64
//---------------------------------------------------------------------
#pragma resource "*.dfm" 

extern CMyPISODIO g_DIO;
//--------------------------------------------------------------------- 
__fastcall TIOCheckDlg::TIOCheckDlg(TComponent* AOwner)
	: TForm(AOwner)
{
  SetInputCaption();
  SetOutputCaption();
}
//---------------------------------------------------------------------

void __fastcall TIOCheckDlg::timerInputTimer(TObject *Sender)
{
  TPanel *pInputPanel;
  AnsiString strComponentName;

 TPanel *pPanel;

 timerInput->Enabled=false;

 for(int nIndex=0;nIndex<MAX_IN;nIndex++)
 {
         pPanel=(TPanel *)FindComponent("InputPanel"+FormatFloat("0",nIndex));
         if(g_DIO.ReadDIBit(nIndex)) pPanel->Color=clLime;
         else pPanel->Color=clGreen;
 }

  timerInput->Enabled=true;

}
//---------------------------------------------------------------------------

void __fastcall TIOCheckDlg::timerOutputTimer(TObject *Sender)
{
  TSpeedButton *pOutputButton;
  AnsiString strComponentName;

  TSpeedButton *pBtn;

 timerOutput->Enabled=false;

 for(int nIndex=0;nIndex<MAX_OUT;nIndex++)
 {
         pBtn=(TSpeedButton *)FindComponent("OutputButton"+FormatFloat("0",nIndex));
         pBtn->Down=g_DIO.ReadDOBit(nIndex);
 }

  timerOutput->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TIOCheckDlg::OutputButton0Click(TObject *Sender)
{
  bool bSetValue;

  TSpeedButton *pOutputButton=(TSpeedButton *)Sender;
  if(pOutputButton->Down)
    bSetValue=true;
  else
    bSetValue=false;

  g_DIO.SetDO(pOutputButton->GroupIndex-1,bSetValue);  

}
//---------------------------------------------------------------------------
void __fastcall TIOCheckDlg::SetOutputCaption()
{ 
  char * strCaption[]={
  "	啟動鍵	"	,
"	停止鍵	"	,
"	復歸鍵	"	,
"	塔燈 紅	"	,
"	塔燈 綠	"	,
"	塔燈 黃	"	,
"	塔燈 蜂鳴器	"	,
"	Y007	"	,
"	入料夾持汽缸1	"	,
"	入料夾持汽缸2	"	,
"	入料靠邊汽缸	"	,
"	收料汽缸(上)	"	,
"	收料汽缸(中)	"	,
"	台車頂升汽缸(上)     "	,
"	載台靠邊汽缸	"	,
"	台車頂升汽缸(下)     "	,
"	LED 光源	"	,
"	Y017	"	,
"	Y018	"	,
"	Y019	"	,
"	Y020	"	,
"	Y021	"	,
"	Y022	"	,
"	Y023	"	,
"	Y024	"	,
"	Y025	"	,
"	Y026	"	,
"	Y027	"	,
"	Y028	"	,
"	Y029	"	,
"	Y030	"	,
"	Y031	"	,
"	吸嘴真空1	"	,
"	吸嘴真空2	"	,
"	吸嘴真空3	"	,
"	吸嘴真空4	"	,
"	吸嘴真空5	"	,
"	吸嘴真空6	"	,
"	吸嘴真空7	"	,
"	吸嘴真空8	"	,
"	吸嘴真空9	"	,
"	吸嘴真空10	"	,
"	吸嘴真空11	"	,
"	吸嘴真空12	"	,
"	吸嘴真空13	"	,
"	吸嘴真空14	"	,
"	吸嘴真空15	"	,
"	吸嘴真空16	"	,
"	吸嘴破真空1	"	,
"	吸嘴破真空2	"	,
"	吸嘴破真空3	"	,
"	吸嘴破真空4	"	,
"	吸嘴破真空5	"	,
"	吸嘴破真空6	"	,
"	吸嘴破真空7	"	,
"	吸嘴破真空8	"	,
"	吸嘴破真空9	"	,
"	吸嘴破真空10	"	,
"	吸嘴破真空11	"	,
"	吸嘴破真空12	"	,
"	吸嘴破真空13	"	,
"	吸嘴破真空14	"	,
"	吸嘴破真空15	"	,
"	吸嘴破真空16	"	
};

AnsiString strMsg;

  for(int nIndex=0;nIndex<MAX_OUT;nIndex++)
  {
         strMsg.sprintf("OutputButton%d",nIndex);
        ((TSpeedButton *)FindComponent(strMsg))->Caption=strCaption[nIndex];
   }
}
//---------------------------------------------------------------------------
void __fastcall TIOCheckDlg::SetInputCaption()
{
  char * strCaption[]={
"	啟動鍵	"	,
"	停止鍵	"	,
"	復歸鍵	"	,
"	緊急停止	"	,
"	主氣壓表	"	,
"	安全門	"	,
"	收料頂昇有無TRAY	"	,
"	X007	"	,
"	入料區有無TRAY	"	,
"	入料夾持(縮)	"	,
"	入料夾持(伸)	"	,
"	入料靠邊(縮)	"	,
"	入料靠邊(伸)	"	,
"	載台靠邊(縮)	"	,
"	載台靠邊(伸)	"	,
"	平台有無TRAY	"	,
"	台車定位1	"	,
"	台車定位2	"	,
"	收料汽缸(上)	"	,
"	收料汽缸(中)	"	,
"	收料汽缸(下)	"	,
"	台車頂升汽缸(上)	"	,
"	台車頂升汽缸(下)	"	,
"	散熱片準備位置	"	,
"	收料滿料	"	,
"	台車定位3	"	,
"	台車定位4	"	,
"	X027	"	,
"	X028	"	,
"	X029	"	,
"	X030	"	,
"	X031	"	,
"	吸嘴真空表1	"	,
"	吸嘴真空表2	"	,
"	吸嘴真空表3	"	,
"	吸嘴真空表4	"	,
"	吸嘴真空表5	"	,
"	吸嘴真空表6	"	,
"	吸嘴真空表7	"	,
"	吸嘴真空表8	"	,
"	吸嘴真空表9	"	,
"	吸嘴真空表10	"	,
"	吸嘴真空表11	"	,
"	吸嘴真空表12	"	,
"	吸嘴真空表13	"	,
"	吸嘴真空表14	"	,
"	吸嘴真空表15	"	,
"	吸嘴真空表16	"	,
"	X048	"	,
"	X049	"	,
"	X050	"	,
"	X051	"	,
"	X052	"	,
"	X053	"	,
"	X054	"	,
"	X055	"	,
"	X056	"	,
"	X057	"	,
"	X058	"	,
"	X059	"	,
"	X060	"	,
"	X061	"	,
"	X062	"	,
"	X063	"

};

  AnsiString strMsg;

  for(int nIndex=0;nIndex<MAX_IN;nIndex++)
  {
         strMsg.sprintf("InputPanel%d",nIndex);
        ((TPanel *)FindComponent(strMsg))->Caption=strCaption[nIndex];
   }

}
//---------------------------------------------------------------------------

void __fastcall TIOCheckDlg::FormCreate(TObject *Sender)
{
    AnsiString strMsg,strStatic;

    for(int nIndex=0;nIndex<64;nIndex++)
    {
        strMsg.sprintf("X%03d",nIndex);
        strStatic.sprintf("StaticText%d",nIndex+1);
        ((TStaticText *)FindComponent(strStatic))->Caption=strMsg;
    }

     for(int nIndex=0;nIndex<64;nIndex++)
    {
        strMsg.sprintf("Y%03d",nIndex);
        strStatic.sprintf("StaticText%d",nIndex+129);
        ((TStaticText *)FindComponent(strStatic))->Caption=strMsg;
     }

}
//---------------------------------------------------------------------------


