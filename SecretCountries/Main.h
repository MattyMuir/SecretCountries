#pragma once
   
#include <wx\wx.h>
#include <wx\dcclient.h>
#include <wx\dcmemory.h>
#include <wx\dcbuffer.h>

#include "shp.h"
#include "iofunctions.h"

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
	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	/*void Resized(wxSizeEvent& evt);
	void KeyDown(wxKeyEvent& evt);*/
	int width, height;
	double zoom;
};