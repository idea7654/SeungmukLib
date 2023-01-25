#include "pch.h"
#include "RedisDB.h"

bool RedisDB::Initialize(const char* configFile)
{
    Json config(configFile);
    auto hostName = config.GetValue<std::string>("hostName");
    auto port = config.GetValue<int>("port");

    m_RedisContext = redisConnectWithTimeout(hostName.c_str(), port, m_Timeout);
    if (m_RedisContext == NULL || m_RedisContext->err)
    {
        if (m_RedisContext)
        {
            cout << "Connection Error: " << m_RedisContext->errstr << endl;
        }
        else
        {
            cout << "Connection Error: Can't allocate redis context" << endl;
        }
        return false;
    }
    return true;
}

void RedisDB::SetDBName(std::string dbName)
{
    m_DBName = dbName;
}

redisReply* RedisDB::SQL_QUERY(const char* query)
{
    m_RedisReply = reinterpret_cast<redisReply*>(redisCommand(m_RedisContext, query));
    //freeReplyObject(m_RedisReply);
    return m_RedisReply;
}
