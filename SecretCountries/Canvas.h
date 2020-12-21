#pragma once
#include <wx\wx.h>
#include <wx\dcclient.h>
#include <wx\dcmemory.h>
#include <wx\dcbuffer.h>

#include <string>
#include <vector>
#include "shp.h"
#include "iofunctions.h"
#include "colour.h"

class Canvas : public wxPanel
{
public:
	Canvas(wxWindow* parent, Shapefile* datasetPtr_, std::vector<CountryData>* countriesPtr_, bool draw);
	~Canvas();

	bool draw;
	std::string displayText;
	int secretIndex;
	std::vector<int> guessIndicies;
	void mRefresh();
protected:
	int w, h;
	Shapefile* datasetPtr;
	std::vector<CountryData>* countriesPtr;

	mPoint Transform(mPoint p);
	mPoint Transform(double x, double y);

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void Resized(wxSizeEvent& evt);

	void KeyDown(wxKeyEvent& evt);

	wxDECLARE_EVENT_TABLE();
};