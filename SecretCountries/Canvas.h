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
	Canvas(wxWindow* parent);
	~Canvas();

	void FullRefresh();

	void ResetCountries();
	void AllChanged();
	void CountryGuessed(int index);
	void RevealAll();
	void SetBlind(bool isBlind);

	bool blind = false;
	bool ready = false;
	int secretIndex = 0;
	std::vector<Country> countries;
	int nCountries;
	Bounds bounds;
	int nGuesses = 0;
	std::vector<int> changelog;
protected:
	int w, h;
	wxBitmap* cache = nullptr;

	wxPoint Transform(double x, double y);

	void OnDraw(wxDC& dc);
	void OnPaint(wxPaintEvent& evt);
	void Resized(wxSizeEvent& evt);
	void UpdateCache(wxDC* dc);
	void ClearCache();

	wxDECLARE_EVENT_TABLE();
};