#include "pch.h"
#include "MySQLDB.h"

bool MySQLDB::Initialize(const char* configFile)
{
	mysql_init(&m_Conn);

	Json config(configFile);
	auto host = config.GetValue<std::string>("host");
	auto user = config.GetValue<std::string>("user");
	auto password = config.GetValue<std::string>("password");
	auto dbName = config.GetValue<std::string>("dbName");
	auto port = config.GetValue<int>("port");

	this->SetDBName(dbName);

	m_ConnPtr = mysql_real_connect(&m_Conn, host.c_str(), user.c_str(), password.c_str(), /*접속할 데이터베이스*/dbName.c_str(), port, (char*)NULL, 0);
	if (m_ConnPtr == NULL)
	{
		g_Log.error("MySQL Connect Error\n");
		return false;
	}
	cout << "MySQL Connect Success!" << endl;
	return true;
}

void MySQLDB::SetDBName(std::string dbName)
{
	m_DBName = dbName;
}

MYSQL_RES* MySQLDB::SQL_QUERY(const char* query)
{
	m_Stat = mysql_query(m_ConnPtr, query);
	if (m_Stat != 0)
	{
		m_Result = nullptr;
		return m_Result;
	}

	m_Result = mysql_store_result(m_ConnPtr);
	if (m_Result)
	{
		if (m_Result->row_count == 0)
		{
			return nullptr;
		}
	}

	mysql_free_result(m_Result);
	return m_Result;
}
