#include "Canvas.h"

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
	EVT_SIZE(Canvas::Resized)
wxEND_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent, Shapefile* datasetPtr_, std::vector<CountryData>* countriesPtr_, bool draw_) : wxPanel(parent, wxID_ANY)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	datasetPtr = datasetPtr_;
	countriesPtr = countriesPtr_;
	displayText = "";
	draw = draw_;
}

Canvas::~Canvas()
{

}

mPoint Canvas::Transform(mPoint p)
{
	Shapefile& dataset = *datasetPtr;

	double dataWidth = dataset.fileStats.xMax - dataset.fileStats.xMin;
	double dataHeight = dataset.fileStats.yMax - dataset.fileStats.yMin;
	double x_ = (p.x - dataset.fileStats.xMin) / dataWidth * w;
	double y_ = h - (p.y - dataset.fileStats.yMin) / dataHeight * h;

	return mPoint(x_, y_);
}

mPoint Canvas::Transform(double x, double y)
{
	Shapefile& dataset = *datasetPtr;

	double dataWidth = dataset.fileStats.xMax - dataset.fileStats.xMin;
	double dataHeight = dataset.fileStats.yMax - dataset.fileStats.yMin;
	double x_ = (x - dataset.fileStats.xMin) / dataWidth * w;
	double y_ = h - (y - dataset.fileStats.yMin) / dataHeight * h;

	return mPoint(x_, y_);
}

void Canvas::OnDraw(wxDC& dc)
{
	Shapefile& dataset = *datasetPtr;
	std::vector<CountryData>& countries = *countriesPtr;

	wxColour defaultLine = wxColour(0, 0, 0);
	wxColour defaultFill = wxColour(255, 255, 255);

	dc.Clear();
	wxPen pen = dc.GetPen();
	wxBrush brush = dc.GetBrush();

	brush.SetColour(wxColour(190, 190, 190));
	dc.SetBrush(brush);
	dc.DrawRectangle(-10, -10, w + 20, h + 20);

	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(defaultLine);
	brush.SetColour(defaultFill);
	dc.SetPen(pen);

	int polyIndex = 0;
	for (mPolygon poly : dataset.polygons)
	{
		bool guessed = false;
		for (int guess : guessIndicies)
		{
			if (polyIndex == guess)
			{
				guessed = true;
			}
		}
		if (guessed)
		{
			CountryData& g = countries[polyIndex];
			CountryData& s = countries[secretIndex];

			double dist = sqrt(pow(g.lat - s.lat, 2) + pow(g.lon - s.lon, 2));
			if (polyIndex == secretIndex)
			{
				pen.SetColour(wxColour(255, 0, 0));
				brush.SetColour(wxColour(0, 0, 0));
			}
			else
			{
				pen.SetColour(defaultLine);
				brush.SetColour(wxHSL(120 / (0.05 * dist + 1), 255, 255));
			}
			dc.SetBrush(brush);
			dc.SetPen(pen);
			if (g.area < 3000)
			{
				mPoint t = Transform(g.lon, g.lat);
				dc.DrawCircle(t.x, t.y, 5);
			}
		}
		else
		{
			brush.SetColour(defaultFill);
			pen.SetColour(defaultLine);
			dc.SetPen(pen);
			dc.SetBrush(brush);
		}
		for (int ringIndex = 0; ringIndex < poly.nRings; ringIndex++)
		{
			int startIndex = poly.parts[ringIndex];

			std::vector<wxPoint> pointsSource;
			wxPointList list;
			for (int pIndex = startIndex + 1; pIndex < poly.parts[ringIndex + 1]; pIndex++)
			{
				mPoint t = Transform(poly.points[pIndex].x, poly.points[pIndex].y);
				pointsSource.push_back(wxPoint(t.x, t.y));
			}

			for (wxPoint& p : pointsSource)
			{
				list.Append(&p);
			}
			dc.DrawPolygon(&list, 0, 0, wxODDEVEN_RULE);
		}
		polyIndex++;
	}
	dc.SetTextForeground(wxColour(255, 0, 0));
	dc.DrawText(wxString(displayText), wxPoint(50, 50));

	brush.SetColour(wxColour(255, 0, 0));
	dc.SetBrush(brush);
}

void Canvas::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	if (draw)
	{
		this->GetSize(&w, &h);
		this->PrepareDC(dc);
		this->OnDraw(dc);
	}
}

void Canvas::Resized(wxSizeEvent& evt)
{
	this->GetSize(&w, &h);
	this->Refresh();
	evt.Skip();
}