#include <iostream>
#include <fstream>

#include "shp.h"

void Split(std::vector<std::string>& result, std::string s, std::string delim)
{
    result.clear();

    int start = 0;
    int end = s.find(delim);

    while (end != std::string::npos)
    {
        result.push_back(s.substr(start, end - start));
        start = end + delim.length();
        end = s.find(delim, start);
    }
    if (end != s.length()) { result.push_back(s.substr(start, s.length() - start)); }
}

void ReadCountriesCSV(std::vector<CountryMeta>& countries, std::string dir)
{
    countries.clear();
    std::ifstream csvFile = std::ifstream(dir);

    std::string line;
    std::getline(csvFile, line);
    while (std::getline(csvFile, line))
    {
        std::vector<std::string> splitStr;
        Split(splitStr, line, ",");
        CountryMeta country;

        country.name = splitStr[4];
        country.lat = std::stod(splitStr[10]);
        country.lon = std::stod(splitStr[9]);
        country.area = std::stoi(splitStr[5], nullptr, 10);
        countries.push_back(country);
    }
    csvFile.close();
}