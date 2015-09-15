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
    __fastcall SQLITE3IF(int DBtype, AnsiString DBPath);    //Temp=0; Action=1;

    void __fastcall open(int DBtype);                       //Temp=0; Action=1;
    void __fastcall free();
    void __fastcall close();
    void __fastcall insertTemp();
    void __fastcall insertAction(AnsiString Action);
    void __fastcall selectSQL(AnsiString SQL_SELECT);
    void __fastcall insertSQL(AnsiString SQL_INSERT);

};