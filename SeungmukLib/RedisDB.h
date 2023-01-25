#pragma once
#include "Database.h"

class RedisDB : public Database
{
public:
	RedisDB() {}
	~RedisDB()
	{
		redisFree(m_RedisContext);
		m_RedisContext = nullptr;
		m_RedisReply = nullptr;
	}

	virtual bool Initialize(const char* configFile) override;
	virtual void SetDBName(std::string dbName) override;

	redisReply* SQL_QUERY(const char* query);

private:
	redisContext*	m_RedisContext;
	redisReply*		m_RedisReply;
	struct timeval	m_Timeout = { 1, 500000 }; //1.5sec
};

