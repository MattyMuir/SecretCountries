#pragma once
#include <string>

#include <shapefil.h>
#include <wx/wx.h>

struct Bounds
{
	double xMin, yMin, zMin, mMin, xMax, yMax, zMax, mMax;
};
struct CountryMeta
{
	CountryMeta() {}

	std::string name = "";
	double lat = 0.0, lon = 0.0;
	int area = 0;
};
struct Country
{
	Country(SHPObject* geometry_)
		: geometry(geometry_) {}
	Country(SHPObject* geometry_, const CountryMeta& metadata_, const wxColour& col_)
		: geometry(geometry_), metadata(metadata_), col(col_) {}

	SHPObject* geometry = nullptr;
	CountryMeta metadata;
	wxColour col = wxColour(255, 255, 255);
	bool guessed = false;
};