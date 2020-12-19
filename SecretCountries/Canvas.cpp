#include "Canvas.h"

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
	EVT_SIZE(Canvas::Resized)
wxEND_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent, Shapefile* datasetPtr_, std::vector<CountryData>* countriesPtr_) : wxPanel(parent, wxID_ANY)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	datasetPtr = datasetPtr_;
	countriesPtr = countriesPtr_;
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

	wxColour defaultLine = /*wxHSL(hue, 255, 255)*/wxColour(0, 0, 0);
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
		if (countries[polyIndex].name == "Indonesia")
		{
			brush.SetColour(255, 0, 0);
			dc.SetBrush(brush);
		}
		else
		{
			brush.SetColour(defaultFill);
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
}

void Canvas::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	this->GetSize(&w, &h);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}

void Canvas::Resized(wxSizeEvent& evt)
{
	this->GetSize(&w, &h);
	this->Refresh();
	evt.Skip();
}