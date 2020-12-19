#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	// Events
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Secret Countries", wxPoint(30, 30), wxSize(1280, 720))
{
	// ========================
	const char* shpDir = "C:\\Users\\matty\\Documents\\Maps\\Shapefile\\TM_WORLD_BORDERS-0.3.shp";
	const char* csvDir = "C:\\Users\\matty\\Documents\\Maps\\Shapefile\\TM_WORLD_BORDERS-0.3.csv";
	// ========================

	ReadPolyShapefile(dataset, shpDir);
	for (mPolygon& poly : dataset.polygons)
	{
		poly.parts.push_back(poly.tPoints);
	}

	ReadCountriesCSV(countries, csvDir);
	mCanvas = new Canvas(this, &dataset, &countries);
}

Main::~Main()
{

}