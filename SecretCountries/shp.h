#pragma once
#include <vector>
#include <string>
#include <map>

typedef char mByte;
struct Stats
{
	int32_t fileCode, fileLength, shapeType;
	double xMin, yMin, xMax, yMax, zMin, zMax, mMin, mMax;
};
struct mPoint
{
	mPoint()
	{
		x = 0;
		y = 0;
	}
	mPoint(int x_, int y_)
	{
		x = x_;
		y = y_;
	}
	double x, y;
};
struct MultiPoint
{
	double box[4]; //xMin, yMin, xMax, yMax
	int32_t nPoints;
	std::vector<mPoint> points;
};
struct PolyLine
{
	double box[4]; //xMin, yMin, xMax, yMax
	int32_t nParts, nPoints;
	std::vector<int32_t> parts;
	std::vector<mPoint> points;
};
struct mPolygon
{
	double box[4]; //xMin, yMin, xMax, yMax
	int32_t nRings, tPoints;
	std::vector<int32_t> parts;
	std::vector<mPoint> points;
};
struct Shapefile
{
	Stats fileStats;
	std::vector<mPolygon> polygons;
};
struct CountryData
{
	std::string name;
	double lat, lon;
	int area;
};