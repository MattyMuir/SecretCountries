#pragma once
#include <string>
#include <vector>

#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>

#include "shp.h"
#include "iofunctions.h"
#include "colour.h"

class Canvas : public wxPanel
{
public:
	Canvas(wxWindow* parent, bool blind_);
	~Canvas();

	void ResetCountries();
	void CountryGuessed(int index);
	void RevealAll();

	bool ready = false;
	int secretIndex = 0;
	std::vector<Country> countries;
	int nCountries;
	Bounds bounds;
	int nGuesses = 0;
protected:
	int w, h;
	bool blind;

	wxPoint Transform(double x, double y);

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void Resized(wxSizeEvent& evt);

	wxDECLARE_EVENT_TABLE();
};