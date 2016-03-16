//---------------------------------------------------------------------------

#pragma hdrstop

#include "Sqlite3Interface.h"
#include "string.h"
#include "vcl.h"
#include "dir.h"
#include "MainThread.h"
#include "DeltaPLC.h"
#include "GlobalFunction.h"

extern CMainThread *g_pMainThread;
extern CDeltaPLC g_ModBus;

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall SQLITE3IF::SQLITE3IF(int DBtype,AnsiString DBPath)
{
    m_strDBPath = DBPath;

    if (!FileExists(DBPath.c_str())) _mkdir(DBPath.c_str());

    /* ��lDB */
    open(DBtype);
    free();
    close();

    /* ���ounique_id */
    open(DBtype);
    selectSQL("SELECT idx FROM C74Log ORDER BY idx DESC;");
    if (rows != 0) unique_id = atoi(result[1])+1;
    else unique_id = 0;
    free();
    close();
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::open(int DBtype)
{
    AnsiString strFullPath;
    AnsiString strDBName; (DBtype==2) ? strDBName = "\\AccountLog.db3" :strDBName = "\\%04d_%02d_%02d.db3";
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);

    strFullPath.sprintf((m_strDBPath+strDBName).c_str(),tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday);
    /* �}�� database �� */
    if (sqlite3_open_v2(strFullPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {return;}

    /* �T�{table�O�_�s�b */
    selectSQL("SELECT name FROM SQLITE_MASTER WHERE type='table' and name='C74Log';");

    if (strcmp(result[1],"C74Log\0") != 0)
    {
        switch (DBtype)
        {
        case 0:
            /* �إ� Temp Table */
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, datetime VARCHAR(25), FrontT FLOAT, RearT FLOAT);", 0, 0, &errMsg);
            if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
            break;
        case 1:
            /* �إ� Action Table */
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, datetime VARCHAR(25), action NVARCHAR(100));", 0, 0, &errMsg);
            if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
            break;
        case 2:
            /* �إ� Account Table*/
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, account VARCHAR(25), password VARCHAR(25));", 0, 0, &errMsg);
            if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
            break;
        default:
            g_pMainThread->m_listLog.push_back("open() DBType Error!");
        }
        unique_id = 0;
    }
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::free()
{
    /* ���� */
    //�ت��O����O����,�ҥH���u�O����Ӧ�table��result
    //sqlite3_free_table(result);
    sqlite3_free(result);
    sqlite3_free(errMsg);
}
//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::close()
{
    /* ���� database */
    sqlite3_close(db);
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::insertTemp()
{
    AnsiString strDateTime;
    AnsiString strRearTemp;
    AnsiString strFrontTemp;
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);
    strDateTime.sprintf("[%4d:%02d:%02d:%2d:%02d:%02d]",
        tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday
        ,tblock->tm_hour,tblock->tm_min, tblock->tm_sec);

    AnsiString strLastrowid = IntToStr(unique_id);

    if (g_ModBus.m_bInitOK)
    {
        strRearTemp = FormatFloat("0.000", g_pMainThread->m_dRearTempRealTime);
        strFrontTemp = FormatFloat("0.000", g_pMainThread->m_dFrontTempRealTime);
    }
    else
    {
        g_ModBus.m_bInitOK = true;
        strRearTemp = "NULL";
        strFrontTemp = "NULL";
    }
    AnsiString insertsql = "INSERT INTO C74Log VALUES( "+strLastrowid
                                                    +" ,'"+strDateTime
                                                    +"', "+strFrontTemp
                                                    +" , "+strRearTemp+");";
    /* �s�W�@����� */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

    unique_id += 1;
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::insertAction(AnsiString Action)
{
    AnsiString strDateTime;
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);
    strDateTime.sprintf("[%4d:%02d:%02d:%2d:%02d:%02d]",
        tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday
        ,tblock->tm_hour,tblock->tm_min, tblock->tm_sec);

    AnsiString strLastrowid = IntToStr(unique_id);

    AnsiString insertsql = "INSERT INTO C74Log VALUES( "+strLastrowid
                                                    +" ,'"+strDateTime
                                                    +"','"+Action+"');";
    /* �s�W�@����� */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

    unique_id += 1;
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::insertAccount(AnsiString Account, AnsiString Password)
{
    AnsiString strLastrowid = IntToStr(unique_id);
    AnsiString insertsql = "INSERT INTO C74Log VALUES( "+strLastrowid
                                                    +" ,'"+Account
                                                    +"','"+Password+"');";
    /* �s�W�@����� */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

    unique_id += 1;
}

//---------------------------------------------------------------------------
int __fastcall SQLITE3IF::checkAccountPass(AnsiString Account, AnsiString Password)
{
    int IsPass = 1;

    /* ���oaccount */
    open(2);
    selectSQL("SELECT account, password FROM C74Log WHERE account='"+Account+"';");

    if (rows != 0)
    {
        if (strcmp(result[3], Password.c_str())==0) IsPass = 0;
        else IsPass = 2;
    }
    else IsPass = 1;

    free();
    close();
    return IsPass;
}

//---------------------------------------------------------------------------
int __fastcall SQLITE3IF::changeAccountPass(AnsiString Account, AnsiString OldPassword, AnsiString NewPassword)
{
    int IsPass = 0;

    /* ���oaccount */
    open(2);
    selectSQL("SELECT account, password FROM C74Log WHERE account='"+Account+"';");

    if (rows != 0)
    {
        if (strcmp(result[3], OldPassword.c_str())==0)
        {
            updateSQL("UPDATE C74Log SET password = '"+NewPassword+"' WHERE account = '"+Account+"';");
            if (errMsg == NULL) IsPass = 1;
        }
    }
    else IsPass = 0;

    free();
    close();
    return IsPass;
}

//---------------------------------------------------------------------------
AnsiString __fastcall SQLITE3IF::queryAccountPass()
{
    AnsiString Result = "";

    open(2);
    selectSQL("SELECT account, password FROM C74Log;");

    if (rows != 0)
    {
        for (int i=0;i<rows+1;i++)
        {
            Result.cat_sprintf("%s    %s\r\n", result[i*2], result[i*2+1]);
        }
    }
    else Result = "";

    free();
    close();
    return Result;
}

//---------------------------------------------------------------------------
AnsiString __fastcall SQLITE3IF::updateAccountPass(AnsiString Input)
{
    AnsiString Result = "";

    TStringList* strRowsList= SplitString(Input, "\r\n");

    open(2);
    /* �R�� Account Table*/
    sqlite3_exec(db, "DELETE FROM C74Log;", 0, 0, &errMsg);
    if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

    for (int i=1;i<(strRowsList->Count);i++)
    {
        /* Take Account and Password */
        TStringList* strDataList = SplitString(strRowsList->operator[](i) , "    ");

        try
        {
            /* INSERT Account Data */
            AnsiString strLastrowid = IntToStr(i-1);
            AnsiString insertsql = "INSERT INTO C74Log VALUES( "+strLastrowid
                                                        +" ,'"+strDataList->operator[](0)
                                                        +"','"+strDataList->operator[](1)+"');";
            /* �s�W�@����� */
            sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
            if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
        }
        catch (...)
        {
            Result = "falure";
            delete strDataList;
            break;
        }
        delete strDataList;
    }

    /* �T�{ */
    selectSQL("SELECT account, password FROM C74Log;");
    if (rows != 0 && Result != "falure")
    {
        Result = "succeed";
    }
    else Result = "falure";

    free();
    close();
    delete strRowsList;
    return Result;
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::selectSQL(AnsiString SQL_SELECT)
{
    sqlite3_get_table(db ,SQL_SELECT.c_str(), &result , &rows, &cols, &errMsg);
    if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::insertSQL(AnsiString SQL_INSERT)
{
    sqlite3_exec(db, SQL_INSERT.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::updateSQL(AnsiString SQL_UPDATE)
{
    sqlite3_exec(db, SQL_UPDATE.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
}

