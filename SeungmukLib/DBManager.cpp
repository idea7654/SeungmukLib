#include "pch.h"
#include "DBManager.h"

void DBManager::CreateDatabase(const char* configFile)
{
	Json config(configFile);
	auto dbType = config.GetValue<std::string>("dbType");

	if (dbType == "MYSQL")
	{
		MySQLDB* newDB = new MySQLDB();
		if(newDB->Initialize(configFile))
			m_MySQLs.push_back(newDB);
	}
	else
		return;
}
