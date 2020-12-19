#pragma once
#include <wx\wx.h>
#include <wx\dcclient.h>
#include <wx\dcmemory.h>
#include <wx\dcbuffer.h>

#include <string>
#include "shp.h"
#include "iofunctions.h"
#include "colour.h"

#include "Canvas.h"

class Main : public wxFrame
{
public:
	Main();
	~Main();

	Shapefile dataset;
	std::vector<CountryData> countries;
	Canvas* mCanvas;

	wxDECLARE_EVENT_TABLE();
private:

};