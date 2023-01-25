#pragma once
#include "pch.h"

class QueryReturn
{
public:
	virtual void ParseQueryResult(MYSQL_RES* sql_result) {}
	virtual void ParseQueryResult(redisReply* sql_result) {}
};