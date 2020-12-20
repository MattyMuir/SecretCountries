#pragma once
#include <wx\wx.h>
#include <wx\dcclient.h>
#include <wx\dcmemory.h>
#include <wx\dcbuffer.h>

#include <string>
#include <random>

#include "shp.h"
#include "iofunctions.h"
#include "colour.h"

#include "Canvas.h"

class Main : public wxFrame
{
public:
	Main();
	~Main();
	
private:
	Shapefile dataset;
	std::vector<CountryData> countries;
	int secretIndex;

	wxBoxSizer* mSizer;
	Canvas* mCanvas;
	wxPanel* topPanel;

	wxStaticText* mLabel;
	wxTextCtrl* mTextBox;
	wxButton* guessButton;

	void ButtonPressed(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};