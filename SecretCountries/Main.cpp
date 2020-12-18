#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	// Event handlers
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Secret Countries", wxPoint(30, 30), wxSize(300, 300))
{

}

Main::~Main()
{

}

void Main::OnDraw(wxDC& dc)
{

}

void Main::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc = wxBufferedPaintDC(this);
}