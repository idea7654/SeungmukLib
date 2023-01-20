#pragma once
#include "pch.h"
#include "Database.h"

class MySQLDB : public Database
{
public:
	MySQLDB() {}
	~MySQLDB() {}

public:
	virtual bool Initialize(const char* configFile) override;
	virtual void SetDBName(std::string dbName) override;

public:
	MYSQL_RES* SQL_QUERY(const char* query);
	
private:
	MYSQL m_Conn;
	MYSQL* m_ConnPtr = NULL;
	MYSQL_RES* m_Result;
	MYSQL_ROW m_Row;
	int m_Stat;
};

