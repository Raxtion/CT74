//---------------------------------------------------------------------------

#ifndef Sqlite3Interfaceh
#define Sqlite3Interfaceh
//---------------------------------------------------------------------------
#endif

#include "vcl.h"
#include "stdlib.h"
#include "sqlite3.h"
#include "time.h"

class SQLITE3IF
{
private:
    AnsiString m_strDBPath;
    int rows, cols, unique_id;
    sqlite3 *db;
    char *errMsg;
    char **result;
public:
    __fastcall SQLITE3IF(int DBtype, AnsiString DBPath);    //Temp=0; Action=1; Account=2;

    void __fastcall open(int DBtype);                       //Temp=0; Action=1; Account=2;
    void __fastcall free();
    void __fastcall close();
    void __fastcall insertTemp();
    void __fastcall insertAction(AnsiString Action);
    void __fastcall insertAccount(AnsiString Account, AnsiString Password);
    int __fastcall checkAccountPass(AnsiString Account, AnsiString Password);   //"准許通過"=0; "帳號錯誤"=1; "密碼錯誤"=2
    int __fastcall changeAccountPass(AnsiString Account, AnsiString OldPassword, AnsiString NewPassword);  //"修改失敗"=0; "修改成功"=1
    AnsiString __fastcall queryAccountPass();                                   //retrun select * form C74log
    AnsiString __fastcall updateAccountPass(AnsiString Input);                  //return "succeed" "falure"

    void __fastcall selectSQL(AnsiString SQL_SELECT);
    void __fastcall insertSQL(AnsiString SQL_INSERT);
    void __fastcall updateSQL(AnsiString SQL_UPDATE);

};