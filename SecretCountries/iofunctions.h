#pragma once
#include "shp.h"

void ReadPolyShapefile(Shapefile& result, const char* shpDir);
void ReadCountriesCSV(std::vector<CountryData>& countries, const char* dir);