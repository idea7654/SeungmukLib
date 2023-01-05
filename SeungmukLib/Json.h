#pragma once
#include "pch.h"
#include "rapidjson/document.h"
#include <cstdio>

using namespace rapidjson;
using namespace std;

class Json
{
private:
	rapidjson::Document m_Document;
	bool				m_ParseError;
public:
	Json(const char* fileName);

	bool			IsJsonNull();

	template <typename T>
	T GetValue(std::string row)
	{
		assert(m_Document.IsObject());
		assert(m_Document.HasMember(row.c_str()));

		if (m_Document[row.c_str()].IsBool())
		{
			return m_Document[row.c_str()].GetBool();
		}
		else if (m_Document[row.c_str()].IsDouble)
		{
			return m_Document[row.c_str()].GetDouble();
		}
		else if (m_Document[row.c_str()].IsFloat)
		{
			return m_Document[row.c_str()].GetFloat();
		}
		else if (m_Document[row.c_str()].IsInt)
		{
			return m_Document[row.c_str()].GetInt();
		}
		else if (m_Document[row.c_str()].IsString)
		{
			return m_Document[row.c_str()].GetString();
		}
	}
};

