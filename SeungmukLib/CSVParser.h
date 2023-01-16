#pragma once
#include "pch.h"
#include "rapidcsv/rapidcsv.h"

class CSVParser
{
public:
	template <typename T>
	std::vector<T>	GetColumns(std::string rowName)
	{
		std::vector<T> returnVec = m_File.GetColumn<T>(rowName);
		return returnVec;
	}

	template <typename T>
	std::vector<T>	GetRows(std::string columnName)
	{
		std::vector<T> returnVec = m_File.GetRow<T>(columnName);
		return returnVec;
	}

	template <typename T>
	T GetCell(std::string columnName, std::string rowName)
	{
		T returnValue = m_File.GetCell<T>(columnName, rowName);
		return returnValue;
	}

	void	SetCSV(std::string path);

private:
	rapidcsv::Document	m_File;
	Lock				m_Lock;
};

