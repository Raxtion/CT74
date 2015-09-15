//---------------------------------------------------------------------------

#pragma hdrstop

#include "Sqlite3Interface.h"
#include "string.h"
#include "vcl.h"
#include "dir.h"
#include "MainThread.h"
#include "DeltaPLC.h"

extern CMainThread *g_pMainThread;
extern CDeltaPLC g_ModBus;

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall SQLITE3IF::SQLITE3IF(int DBtype,AnsiString DBPath)
{
    m_strDBPath = DBPath;

    if (!FileExists(DBPath.c_str())) _mkdir(DBPath.c_str());

    /* 初始DB */
    open(DBtype);
    close();

    /* 取得unique_id */
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
    AnsiString strDBName = "\\%04d_%02d_%02d.db3";
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);

    strFullPath.sprintf((m_strDBPath+strDBName).c_str(),tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday);
    /* 開啟 database 檔 */
    if (sqlite3_open_v2(strFullPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) { return;}
    if (errMsg != NULL) g_pMainThread->m_listLog.push_back(errMsg);

    /* 確認table是否存在 */
    selectSQL("SELECT name FROM SQLITE_MASTER WHERE type='table' and name='C74Log';");

    if (strcmp(result[1],"C74Log\0") != 0)
    {
        switch (DBtype)
        {
        case 0:
            /* 建立 Temp Table */
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, datetime VARCHAR(10), FrontT FLOAT, RearT FLOAT);", 0, 0, &errMsg);
            if (errMsg != NULL) g_pMainThread->m_listLog.push_back(errMsg);
            break;
        case 1:
            /* 建立 Action Table */
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, datetime VARCHAR(10), action VARCHAR(20));", 0, 0, &errMsg);
            if (errMsg != NULL) g_pMainThread->m_listLog.push_back(errMsg);
            break;
        default:
            g_pMainThread->m_listLog.push_back("open() DBType Error!");
            sqlite3_free(errMsg);
        }

    }
    free();
}
//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::free()
{
    /* 釋放 */
    //目的是釋放記憶體,所以不只是釋放來自table的result
    //sqlite3_free_table(result);
    sqlite3_free(result);
}
//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::close()
{
    /* 關閉 database */
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
    strDateTime.sprintf("[%2d:%02d:%02d]",tblock->tm_hour,tblock->tm_min, tblock->tm_sec);

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
    /* 新增一筆資料 */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL) g_pMainThread->m_listLog.push_back(errMsg);

    unique_id += 1;
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::insertAction(AnsiString Action)
{
    AnsiString strDateTime;
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);
    strDateTime.sprintf("[%2d:%02d:%02d]",tblock->tm_hour,tblock->tm_min, tblock->tm_sec);

    AnsiString strLastrowid = IntToStr(unique_id);

    AnsiString insertsql = "INSERT INTO C74Log VALUES( "+strLastrowid
                                                    +" ,'"+strDateTime
                                                    +"','"+Action+"');";
    /* 新增一筆資料 */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL) g_pMainThread->m_listLog.push_back(errMsg);

    unique_id += 1;
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::selectSQL(AnsiString SQL_SELECT)
{
    sqlite3_free_table(result);
    sqlite3_get_table(db ,SQL_SELECT.c_str(), &result , &rows, &cols, &errMsg);
    if (errMsg != NULL) g_pMainThread->m_listLog.push_back(errMsg);
}
