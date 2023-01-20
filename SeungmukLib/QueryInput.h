#pragma once
#include "pch.h"

class QueryInput
{
private:
    DB_TYPE     m_DBType;
    QUERY_TYPE  m_QueryType;

public:
	virtual std::string MakeQuery() { return ""; }

    void SetDBType(DB_TYPE DBType)
    {
        m_DBType = DBType;
    }

    DB_TYPE GetDBType()
    {
        return m_DBType;
    }

    void SetQueryType(QUERY_TYPE QueryType)
    {
        m_QueryType = QueryType;
    }

    QUERY_TYPE GetQueryType()
    {
        return m_QueryType;
    }
};