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
struct Point
{
	Point()
	{
		x = 0;
		y = 0;
	}
	Point(int x_, int y_)
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
	std::vector<Point> points;
};
struct PolyLine
{
	double box[4]; //xMin, yMin, xMax, yMax
	int32_t nParts, nPoints;
	std::vector<int32_t> parts;
	std::vector<Point> points;
};
struct mPolygon
{
	double box[4]; //xMin, yMin, xMax, yMax
	int32_t nRings, tPoints;
	std::vector<int32_t> parts;
	std::vector<Point> points;
};
struct Shapefile
{
	Stats fileStats;
	std::vector<mPolygon> polygons;
};
struct CountryData
{
	const char* name;
	double lat, lon;
};