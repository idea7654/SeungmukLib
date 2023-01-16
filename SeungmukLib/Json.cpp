#include "pch.h"
#include "Json.h"

#pragma warning(disable : 4996)

Json::Json(const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");

	char readBuffer[65536];
	FileReadStream readStream(fp, readBuffer, sizeof(readBuffer));

	if (m_Document.ParseStream(readStream).HasParseError())
		m_ParseError = true;
	else m_ParseError = false;
}

bool Json::IsJsonNull()
{
	return m_ParseError;
}
