#pragma once
#include "pch.h"
#include "MySQLDB.h"

class DBManager
{
public:
	void CreateDatabase(const char* configFile);
	Database* FindDatabase();

private:
	std::vector<MySQLDB*> m_MySQLs;
	//std::vector<MySQLDB*> m_Databases;
};

