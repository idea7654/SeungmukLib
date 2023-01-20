#pragma once
#include "pch.h"
#include "QueryReturn.h"
#include "QueryInput.h"

class Database
{
public:
	virtual bool		Initialize(const char* configFile) { return false; }
	virtual void		SetDBName(std::string dbName){}
	virtual std::string SetDBName() { return m_DBName; }
protected:
	Lock			m_Lock;
	std::string		m_DBName;
};

