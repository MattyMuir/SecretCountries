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

void endian(mByte* data, size_t size)
{
    mByte temp;
    for (int i = 0; i < (size >> 1); i++)
    {
        temp = data[i];
        data[i] = data[size - i - 1];
        data[size - i - 1] = temp;
    }
}

void ReadPolyShapefile(Shapefile& result, std::string shpDir)
{
    std::ifstream file = std::ifstream(shpDir, std::ios::in | std::ios::binary);

    // ===== Header =====
    file.read((mByte*)&result.fileStats.fileCode, 4);
    endian((mByte*)&result.fileStats.fileCode, 4);
    if (result.fileStats.fileCode != 9994) { throw; }

    file.ignore(5 * 4);

    file.read((mByte*)&result.fileStats.fileLength, 4);
    endian((mByte*)&result.fileStats.fileLength, 4);

    file.ignore(4);

    file.read((mByte*)&result.fileStats.shapeType, 4);

    file.read((mByte*)&result.fileStats.xMin, 8);
    file.read((mByte*)&result.fileStats.yMin, 8);
    file.read((mByte*)&result.fileStats.xMax, 8);
    file.read((mByte*)&result.fileStats.yMax, 8);
    file.read((mByte*)&result.fileStats.zMin, 8);
    file.read((mByte*)&result.fileStats.zMax, 8);
    file.read((mByte*)&result.fileStats.mMin, 8);
    file.read((mByte*)&result.fileStats.mMax, 8);
    // ==================

    // ===== Records =====
    bool reading = true;
    int32_t recordIndex, recordLength, shapeType;

    while (reading)
    {
        result.polygons.emplace_back(mPolygon());
        mPolygon& p = result.polygons[result.polygons.size() - 1];

        // Record header
        file.read((mByte*)&recordIndex, 4);
        endian((mByte*)&recordIndex, 4);
        file.read((mByte*)&recordLength, 4);
        endian((mByte*)&recordLength, 4);

        // Record body
        file.read((mByte*)&shapeType, 4);
        //std::cout << "Record Index: " << recordIndex << std::endl;

        for (int i = 0; i < 4; i++)
            file.read((mByte*)&p.box[i], 8);

        file.read((mByte*)&p.nRings, 4);
        file.read((mByte*)&p.tPoints, 4);

        p.parts = std::vector<int32_t>(p.nRings);
        for (int i = 0; i < p.nRings; i++)
            file.read((mByte*)&p.parts[i], 4);

        // Loop through points
        for (int i = 0; i < p.tPoints; i++)
        {
            p.points.emplace_back(mPoint());
            mPoint& point = p.points[p.points.size() - 1];
            //file.ignore(4);
            file.read((mByte*)&point.x, 8);
            file.read((mByte*)&point.y, 8);
        }

        reading = (file.peek() != EOF);
    }
    // ===================
    file.close();
}

void ReadCountriesCSV(std::vector<CountryData>& countries, std::string dir)
{
    countries.clear();
    std::ifstream csvFile = std::ifstream(dir);

    std::string line;
    std::getline(csvFile, line);
    while (std::getline(csvFile, line))
    {
        std::vector<std::string> splitStr;
        Split(splitStr, line, ",");
        CountryData country;

        country.name = splitStr[4];
        country.lat = std::stod(splitStr[10]);
        country.lon = std::stod(splitStr[9]);
        country.area = std::stoi(splitStr[5], nullptr, 10);
        countries.push_back(country);
    }
    csvFile.close();
}