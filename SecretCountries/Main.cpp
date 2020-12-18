#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_PAINT(Main::OnPaint)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Secret Countries", wxPoint(30, 30), wxSize(600, 600))
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	// ========================
	const char* shpDir = "C:\\Users\\matty\\Documents\\Maps\\Shapefile\\TM_WORLD_BORDERS-0.3.shp";
	// ========================

	ReadPolyShapefile(dataset, shpDir);
	for (mPolygon& poly : dataset.polygons)
	{
		poly.parts.push_back(poly.tPoints);
	}
}

Main::~Main()
{

}

mPoint Main::Transform(mPoint p)
{
	double dataWidth = dataset.fileStats.xMax - dataset.fileStats.xMin;
	double dataHeight = dataset.fileStats.yMax - dataset.fileStats.yMin;
	double x_ = (p.x - dataset.fileStats.xMin) / dataWidth * 600;
	double y_ = 600 - (p.y - dataset.fileStats.yMin) / dataHeight * 600;

	return mPoint(x_, y_);
}

mPoint Main::Transform(double x, double y)
{
	double dataWidth = dataset.fileStats.xMax - dataset.fileStats.xMin;
	double dataHeight = dataset.fileStats.yMax - dataset.fileStats.yMin;
	double x_ = (x - dataset.fileStats.xMin) / dataWidth * 600;
	double y_ = 600 - (y - dataset.fileStats.yMin) / dataHeight * 600;

	return mPoint(x_, y_);
}

void Main::mDrawLine(wxDC& dc, mPoint p1, mPoint p2)
{
	dc.DrawLine(p1.x, p1.y, p2.x, p2.y);
}

void Main::OnDraw(wxDC& dc)
{
	dc.Clear();
	wxPen pen = dc.GetPen();
	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(wxColour(0, 0, 0));
	dc.SetPen(pen);

	int polyCount = dataset.polygons.size();

	for (mPolygon& poly : dataset.polygons)
	{
		for (int ringIndex = 0; ringIndex < poly.nRings; ringIndex++)
		{
			int startIndex = poly.parts[ringIndex];
			mPoint ringStart, last;

			ringStart = Transform(poly.points[startIndex].x, poly.points[startIndex].y);
			last = ringStart;
			for (int pIndex = startIndex + 1; pIndex < poly.parts[ringIndex + 1]; pIndex++)
			{
				mPoint current = Transform(poly.points[pIndex].x, poly.points[pIndex].y);
				mDrawLine(dc, last, current);

				last = current;
			}
			mDrawLine(dc, last, ringStart);
		}
	}
}

void Main::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc (this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}