#pragma once
#include "pch.h

class SET_ITEM : public QueryInput
{
public:
    string TABLE_NAME;
    string ITEM_NAME;

public:
    SET_ITEM()
    {
        SetDBType(DB_TYPE::DB_MYSQL);
        SetQueryType(QUERY_TYPE::WRITE);
    }

    virtual std::string MakeQuery() override
    {
        return ("INSERT INTO " + TABLE_NAME + " VALUE " + ITEM_NAME);
    }

    void SetParameter(string TABLE_NAME, string ITEM_NAME)
    {
        this->TABLE_NAME = TABLE_NAME;
        this->ITEM_NAME = ITEM_NAME;
    }
};
                
class GET_ITEM_FROM_USERID : public QueryInput
{
public:
    string TABLE_NAME;
    int USER_ID;
    string ITEM_NAME;

public:
    GET_ITEM_FROM_USERID()
    {
        SetDBType(DB_TYPE::DB_MYSQL);
        SetQueryType(QUERY_TYPE::READ);
    }

    virtual std::string MakeQuery() override
    {
        return ("SELECT * FROM " + TABLE_NAME + " WHERE User_Id=" + std::to_string(USER_ID) + " Item_Name=" + ITEM_NAME);
    }

    void SetParameter(string TABLE_NAME, int USER_ID, string ITEM_NAME)
    {
        this->TABLE_NAME = TABLE_NAME;
        this->USER_ID = USER_ID;
        this->ITEM_NAME = ITEM_NAME;
    }
};
                
class GET_FOO : public QueryInput
{
public:
    string foo;

public:
    GET_FOO()
    {
        SetDBType(DB_TYPE::DB_REDIS);
        SetQueryType(QUERY_TYPE::READ);
    }

    virtual std::string MakeQuery() override
    {
        return ("GET " + foo);
    }

    void SetParameter(string foo)
    {
        this->foo = foo;
    }
};
                