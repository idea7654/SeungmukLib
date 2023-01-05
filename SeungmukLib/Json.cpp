#include "pch.h"
#include "Json.h"

Json::Json(const char* fileName)
{
	if (m_Document.Parse(fileName).HasParseError())
		m_ParseError = true;
	else m_ParseError = false;
}

bool Json::IsJsonNull()
{
	return m_ParseError;
}
