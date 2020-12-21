#pragma once
#include "shp.h"

void ReadPolyShapefile(Shapefile& result, std::string shpDir);
void ReadCountriesCSV(std::vector<CountryData>& countries, std::string dir);