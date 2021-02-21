#pragma once

#include<unordered_map>
#include<string>
#include <vector>

enum class OverlayOperationType
{
	all = 0,
	lesser,
	greater,
	average
};

static const std::unordered_map<std::string, OverlayOperationType> OVERLAY_FLAG_TO_TYPE =
{	{ "", OverlayOperationType::all },
	{ "-l", OverlayOperationType::lesser },
	{ "-g", OverlayOperationType::greater },
	{ "-a", OverlayOperationType::average }
};

static std::vector<int> GetParsedData( std::string line )
{
	std::string delimiter = " \r\n\t";

	std::vector<int> parsedData;

	size_t beg, pos = 0;
	while ( (beg = line.find_first_not_of( delimiter, pos )) != std::string::npos )
	{
		pos = line.find_first_of( delimiter, beg + 1 );
		try
		{

			parsedData.push_back( std::stoi( line.substr( beg, pos - beg ) ) );
		}
		catch ( ... )
		{
			//std::cout << "Invalid Data for Size";
		}
	}

	return std::move( parsedData );
}

static std::vector<std::string> GetParsedDataToString( std::string line )
{
	std::string delimiter = " \r\n\t";

	std::vector<std::string> parsedData;

	size_t beg, pos = 0;
	while ( (beg = line.find_first_not_of( delimiter, pos )) != std::string::npos )
	{
		pos = line.find_first_of( delimiter, beg + 1 );
		parsedData.push_back( line.substr( beg, pos - beg ) );
	}

	return std::move( parsedData );
}