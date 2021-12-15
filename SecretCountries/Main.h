#pragma once
#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/dcbuffer.h>

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
	std::string shpDir;
	std::string csvDir;

	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> dist;

	wxBoxSizer* mSizer;
	Canvas* mCanvas;
	wxPanel* topPanel;

	wxMenuBar* mMenuBar;
	wxMenu* fileMenu;
	wxMenu* gameMenu;

	wxStaticText* mLabel;
	wxStaticText* guessNumText;
	wxStaticText* answerRevealText;
	wxTextCtrl* mTextBox;
	wxButton* guessButton;
	wxButton* giveUpButton;

	void KeyEntered(wxKeyEvent& evt);

	void ButtonPressed(wxCommandEvent& evt);
	void GuessMade();
	void GiveUp(wxCommandEvent& evt);

	void GameReset(wxCommandEvent& evt);
	void OnOpenSHP(wxCommandEvent& evt);
	void OnOpenCSV(wxCommandEvent& evt);

	void InitializeCanvas();

	wxDECLARE_EVENT_TABLE();
};