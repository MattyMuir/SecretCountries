#pragma once
   
#include <wx\wx.h>
#include <wx\dcclient.h>
#include <wx\dcmemory.h>
#include <wx\dcbuffer.h>

#include "shp.h"
#include "iofunctions.h"
#include "colour.h"
#include <string>

class Main : public wxFrame
{
public:
	Main();
	~Main();

	// Add components here
	Shapefile dataset;
	std::vector<CountryData> countries;

	wxDECLARE_EVENT_TABLE();
private:
	mPoint Transform(mPoint p);
	mPoint Transform(double x, double y);
	void mDrawLine(wxDC& dc, mPoint p1, mPoint p2);
	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void Resized(wxSizeEvent& evt);
	//void KeyDown(wxKeyEvent& evt);
	int width, height;
	std::string franceName;
};