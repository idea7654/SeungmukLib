#pragma once
#include "pch.h"
#include <sstream>

template <typename T>
T GetAs(const std::string& s) {
    std::stringstream ss{s};
    T t;
    ss >> t;
    return t;
}

class GET_ITEM_FROM_USERID_Return : public QueryReturn
{
public:
    
    std::vector<int> ITEM_AMOUNT;

    virtual void ParseQueryResult(MYSQL_RES* sql_result) override
    {
        MYSQL_ROW row;

        while((row = mysql_fetch_row(sql_result)))
        {
            ITEM_AMOUNT.push_back(GetAs<int>(row[0]));
        }
    }
};
