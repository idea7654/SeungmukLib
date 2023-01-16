#include "pch.h"
#include "CSVParser.h"

void CSVParser::SetCSV(std::string path)
{
    m_File = rapidcsv::Document(path);
}
