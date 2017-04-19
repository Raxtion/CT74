//---------------------------------------------------------------------------

#pragma hdrstop

#include "Sqlite3Interface.h"
#include "string.h"
#include "vcl.h"
#include "dir.h"
#include "MainThread.h"
#include "DeltaPLC.h"
#include "GlobalFunction.h"
#include "IniFile.h"
#include <stdio.h>

extern CMainThread *g_pMainThread;
extern CIniFile g_IniFile;
extern CDeltaPLC g_ModBus;

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall SQLITE3IF::SQLITE3IF(int DBtype,AnsiString DBPath)
{
    m_strDBPath = DBPath;
    errMsg = NULL;

    if (!FileExists(DBPath.c_str())) _mkdir(DBPath.c_str());

    /* 初始DB */
    open(DBtype);
    free();
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
__fastcall SQLITE3IF::SQLITE3IF(int DBtype, AnsiString DBPath, AnsiString strTableName)
{
	m_strDBPath = DBPath;
    errMsg = NULL;

	if (!FileExists(DBPath.c_str())) _mkdir(DBPath.c_str());

	/* 初始DB */
	open(DBtype, strTableName);
	free();
	close();

	/* 取得unique_id */
	open(DBtype, strTableName);
	selectSQL("SELECT idx FROM '" + strTableName + "' ORDER BY idx DESC;");
	if (rows != 0) unique_id = atoi(result[1]) + 1;
	else unique_id = 0;
	free();
	close();
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::open(int DBtype)
{
    AnsiString strFullPath;
	AnsiString strDBName; (DBtype == 2) ? strDBName = "\\AccountLog.db3" : strDBName = "\\%04d_%02d_%02d.db3";
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);

    strFullPath.sprintf((m_strDBPath+strDBName).c_str(),tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday);
    AnsiString strTime = IntToStr(tblock->tm_hour)+IntToStr(tblock->tm_min)+IntToStr(tblock->tm_sec);

    /* 確認資料庫是否已有缺失 */
    if (FileExists(m_strDBPath+"\\Note"))
    {
        RenameFile(strFullPath, strFullPath+strTime);
        DeleteFile(m_strDBPath+"\\Note");
        errMsg = NULL;
    }

    /* 開啟 database 檔 */
    if (sqlite3_open_v2(strFullPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) {return;}

    /* 若db開啟有錯誤 Create Note File*/
    if (errMsg != NULL)
    {
        FILE *pFile;
        pFile=fopen((m_strDBPath+"\\Note").c_str(),"a+");
        if(pFile!=NULL)
        {
            fclose(pFile);
        }
    }

    result = 0;
    /* 確認table是否存在 */
    selectSQL("SELECT name FROM SQLITE_MASTER WHERE type='table' and name='C74Log';");

    if (result != 0){
    if (strcmp(result[1],"C74Log\0") != 0)
    {
        switch (DBtype)
        {
        case 0:
            /* 建立 Temp Table */
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, datetime VARCHAR(25), FrontT FLOAT, RearT FLOAT);", 0, 0, &errMsg);
            if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
            break;
        case 1:
            /* 建立 Action Table */
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, datetime VARCHAR(25), action NVARCHAR(100));", 0, 0, &errMsg);
            if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
            break;
        case 2:
            /* 建立 Account Table*/
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, account VARCHAR(25), password VARCHAR(25));", 0, 0, &errMsg);
            if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
            break;
        case 3:
            /* 建立 Change Table*/
            sqlite3_exec(db, "CREATE TABLE C74Log(idx INTEGER PRIMARY KEY, datetime VARCHAR(25), ParamChange NVARCHAR(50));", 0, 0, &errMsg);
            if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
            break;
        default:
            g_pMainThread->m_listLog.push_back("open() DBType Error!");
        }
        unique_id = 0;
    }}
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::open(int DBtype, AnsiString strTableName)
{
	AnsiString strFullPath;
	AnsiString strDBName = "\\OffsetTable.db3";
    time_t timer = time(NULL);
    struct tm *tblock = localtime(&timer);

    strFullPath = m_strDBPath + strDBName;
    AnsiString strTime = IntToStr(tblock->tm_hour)+IntToStr(tblock->tm_min)+IntToStr(tblock->tm_sec);
    
    /* 確認資料庫是否已有缺失 */
    if (FileExists(m_strDBPath+"\\Note"))
    {
        RenameFile(strFullPath, strFullPath+strTime);
        DeleteFile(m_strDBPath+"\\Note");
        errMsg = NULL;
    }

	/* 開啟 database 檔 */
	if (sqlite3_open_v2(strFullPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)) { return; }

    /* 若db開啟有錯誤 Create Note File*/
    if (errMsg != NULL)
    {
        FILE *pFile;
        pFile=fopen((m_strDBPath+"\\Note").c_str(),"a+");
        if(pFile!=NULL)
        {
            fclose(pFile);
        }
    }

    result = 0;
	/* 確認table是否存在 */
	selectSQL("SELECT name FROM SQLITE_MASTER WHERE type='table' and name='" + strTableName + "';");

    if (result != 0){
	if (strcmp(result[1], strTableName.c_str()) != 0)
	{
		/* 建立 Offset Table */
		sqlite3_exec(db, ("CREATE TABLE '" + strTableName + "'(idx INTEGER PRIMARY KEY, isfront BOOLEAN, location INTERGER, setKg FLOAT, offsetKg FLOAT, statistic INTERGER, Note TEXT);").c_str(), 0, 0, &errMsg);
		if (errMsg != NULL && g_pMainThread != NULL) { g_pMainThread->m_listLog.push_back(errMsg); }
		unique_id = 0;
	}}
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::free()
{
    /* 釋放 */
    //目的是釋放記憶體,也要釋放來自table的result
    sqlite3_free_table(result);
    sqlite3_free(result);
    sqlite3_free(errMsg);
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
    strDateTime.sprintf("[%4d:%02d:%02d:%2d:%02d:%02d]",
        tblock->tm_year+1900,tblock->tm_mon+1,tblock->tm_mday
        ,tblock->tm_hour,tblock->tm_min, tblock->tm_sec);

    AnsiString strLastrowid = IntToStr(unique_id);

    if (g_ModBus.m_bInitOK)
    {
        strRearTemp = FormatFloat("0.000", g_pMainThread->m_dRearTempRealTime);
        strFrontTemp = FormatFloat("0.000", g_pMainThread->m_dFrontTempRealTime);
        g_pMainThread->m_bIsTempMonitorFail = false;
    }
    else
    {
        g_ModBus.m_bInitOK = true;
        strRearTemp = "NULL";
        strFrontTemp = "NULL";
        g_pMainThread->m_bIsTempMonitorFail = true;
    }
    AnsiString insertsql = "INSERT INTO C74Log VALUES( "+strLastrowid
                                                    +" ,'"+strDateTime
                                                    +"', "+strFrontTemp
                                                    +" , "+strRearTemp+");";

    /* 新增一筆資料 */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

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
    /* 新增一筆資料 */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

    unique_id += 1;
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::insertChange(AnsiString Change)
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
                                                    +"','"+Change+"');";
    /* 新增一筆資料 */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

    unique_id += 1;
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::insertAccount(AnsiString Account, AnsiString Password)
{
    AnsiString strLastrowid = IntToStr(unique_id);
    AnsiString insertsql = "INSERT INTO C74Log VALUES( "+strLastrowid
                                                    +" ,'"+Account
                                                    +"','"+Password+"');";
    /* 新增一筆資料 */
    sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

    unique_id += 1;
}

//---------------------------------------------------------------------------
int __fastcall SQLITE3IF::checkAccountPass(AnsiString Account, AnsiString Password)
{
    int IsPass = 1;

    /* 取得account */
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

    /* 取得account */
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
    /* 刪除 Account Table*/
    sqlite3_exec(db, "DELETE FROM C74Log;", 0, 0, &errMsg);
    if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}

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
            /* 新增一筆資料 */
            sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
            if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
        }
        catch (...)
        {
            Result = "falure";
            delete strDataList;
            break;
        }
        delete strDataList;
    }

    /* 確認 */
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
AnsiString __fastcall SQLITE3IF::queryOffsetTable(AnsiString strTableName, bool bIsFront, double setKg)
{
	AnsiString Result = "";

	open(4, strTableName);
	
	AnsiString strIsFront; (bIsFront) ? strIsFront = "1" : strIsFront = "0";
	AnsiString strSetKg = FormatFloat("0.0", setKg);

	//selectSQL("SELECT idx, isfront, location, setKg, offsetKg FROM " + strTableName
    //    + " WHERE isfront='" + strIsFront + "' AND setKg='" + strSetKg + "' ORDER BY location ASC;");
    selectSQL("SELECT idx, offsetKg FROM '" + strTableName + "'"
        + " WHERE isfront='" + strIsFront + "' AND setKg='" + strSetKg + "' ORDER BY location ASC;");

	if (rows != 0)
	{
        int j = 1;
        for (int i = 0; i < 50; i++)
        {
            if ((i % 10) < g_IniFile.m_nCols && (i / 10) < g_IniFile.m_nRows)
		    {
                AnsiString strTemp = result[j * 2 + 1];
                double dTemp = 0.0;

                try
                {
                    dTemp = strTemp.ToDouble();
                }
                catch (...)
                {
                    g_pMainThread->m_listLog.push_back(strTemp+" Can Not Trans To double type.");
                }

                if (bIsFront) g_IniFile.m_dScaleOffsetFront[i] = dTemp;
                else g_IniFile.m_dScaleOffsetRear[i] = dTemp;

                Result += strTemp + " ";
                j++;
            }
        }
	}
	else Result = "";

	free();
	close();
	return Result;
}

//---------------------------------------------------------------------------
AnsiString __fastcall SQLITE3IF::updateOffsetTable(AnsiString strTableName, bool bIsFront, double setKg)
{
	AnsiString Result = "";
	bool bIsUpdateMode = false;

	int pnidx[1000];
	memset(pnidx, 0, sizeof(int) * 1000);

	double *pOffset;
	if (bIsFront) pOffset = g_IniFile.m_dScaleOffsetFront;
	else pOffset = g_IniFile.m_dScaleOffsetRear;

	open(4, strTableName);

	/* 確認 TableName, bIsFront, setKg 已經存在 */
	selectSQL("SELECT idx, isfront, location, setKg, offsetKg FROM '" + strTableName + "' WHERE isfront='" + IntToStr(bIsFront) + "' AND setKg='" + setKg + "' ORDER BY location ASC;");
	if (rows != 0)
	{
		for (int i = 1; i<rows + 1; i++)
		{
			pnidx[i-1] = StrToInt(result[i * 5]);
            //g_pMainThread->m_listLog.push_back(result[i * 5]);	//Debug code
		}
		bIsUpdateMode = true;
	}
	else bIsUpdateMode = false;

    int j = 0;
	for (int i = 0; i < 50; i++)
	{
		if ((i % 10) < g_IniFile.m_nCols && (i / 10) < g_IniFile.m_nRows)
		{
			/* Update or Insert */
			if (bIsUpdateMode)	//Update
			{
				try
				{
					/* Update OffsetTable */
					unique_id++;	//must enshure unique_id always added.
					AnsiString updatesql = "UPDATE '" + strTableName + "' SET"
						+ " offsetKg=" + FormatFloat("0.000", pOffset[i])
						+ " WHERE idx = " + IntToStr(pnidx[j]) + " ;";

                    j++;
                    //g_pMainThread->m_listLog.push_back(updatesql);	//Debug code

					/* 更新一筆資料 */
					sqlite3_exec(db, updatesql.c_str(), 0, 0, &errMsg);
					if (errMsg != NULL && g_pMainThread != NULL) { g_pMainThread->m_listLog.push_back(errMsg); }
				}
				catch (...)
				{
					unique_id--;
					Result = "falure";
					break;
				}
			}
			else				//Insert
			{
				try
				{
					/* INSERT OffsetTable */
					AnsiString strLastrowid = unique_id;
					unique_id++;	//must enshure unique_id always added.
					AnsiString insertsql = "INSERT INTO '" + strTableName + "' VALUES( " + strLastrowid
						+ " , " + IntToStr(bIsFront)
						+ " , " + IntToStr(i)
						+ " , " + FormatFloat("0.000", setKg)
						+ " , " + FormatFloat("0.000", pOffset[i])
						+ " , " + "NULL"
						+ " , " + "NULL" + " );";

                    //g_pMainThread->m_listLog.push_back(insertsql);	//Debug code

					/* 新增一筆資料 */
					sqlite3_exec(db, insertsql.c_str(), 0, 0, &errMsg);
					if (errMsg != NULL && g_pMainThread != NULL) { g_pMainThread->m_listLog.push_back(errMsg); }
				}
				catch (...)
				{
					unique_id--;
					Result = "falure";
					break;
				}
			}
		}
	}
	

	/* 確認 */
	selectSQL("SELECT idx, isfront, location, setKg, offsetKg FROM '" + strTableName + "' WHERE isfront='" + IntToStr(bIsFront) + "' AND setKg='" + setKg + "' ORDER BY location ASC;");
	if (rows != 0 && Result != "falure")
	{
		Result = "succeed";
	}
	else Result = "falure";

	free();
	close();
	return Result;
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::selectSQL(AnsiString SQL_SELECT)
{
    sqlite3_get_table(db ,SQL_SELECT.c_str(), &result , &rows, &cols, &errMsg);
    if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::insertSQL(AnsiString SQL_INSERT)
{
    sqlite3_exec(db, SQL_INSERT.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
}

//---------------------------------------------------------------------------
void __fastcall SQLITE3IF::updateSQL(AnsiString SQL_UPDATE)
{
    sqlite3_exec(db, SQL_UPDATE.c_str(), 0, 0, &errMsg);
    if (errMsg != NULL && g_pMainThread != NULL) {g_pMainThread->m_listLog.push_back(errMsg);}
}





