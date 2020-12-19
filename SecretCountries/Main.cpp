#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_PAINT(Main::OnPaint)
	EVT_SIZE(Main::Resized)
	EVT_KEY_DOWN(Main::KeyDown)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Secret Countries", wxPoint(30, 30), wxSize(1280, 720))
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	width = this->GetSize().x;
	height = this->GetSize().y;

	// ========================
	const char* shpDir = "C:\\Users\\matty\\Documents\\Maps\\Shapefile\\TM_WORLD_BORDERS-0.3.shp";
	const char* csvDir = "C:\\Users\\matty\\Documents\\Maps\\Shapefile\\TM_WORLD_BORDERS-0.3.csv";
	// ========================

	ReadPolyShapefile(dataset, shpDir);
	for (mPolygon& poly : dataset.polygons)
	{
		poly.parts.push_back(poly.tPoints);
	}

	ReadCountriesCSV(countries, csvDir);

	hue = 0;
	highlight = 0;
}

Main::~Main()
{

}

mPoint Main::Transform(mPoint p)
{
	double dataWidth = dataset.fileStats.xMax - dataset.fileStats.xMin;
	double dataHeight = dataset.fileStats.yMax - dataset.fileStats.yMin;
	double x_ = (p.x - dataset.fileStats.xMin) / dataWidth * width;
	double y_ = height - (p.y - dataset.fileStats.yMin) / dataHeight * height;

	return mPoint(x_, y_);
}

mPoint Main::Transform(double x, double y)
{
	double dataWidth = dataset.fileStats.xMax - dataset.fileStats.xMin;
	double dataHeight = dataset.fileStats.yMax - dataset.fileStats.yMin;
	double x_ = (x - dataset.fileStats.xMin) / dataWidth * width;
	double y_ = height - (y - dataset.fileStats.yMin) / dataHeight * height;

	return mPoint(x_, y_);
}

void Main::mDrawLine(wxDC& dc, mPoint p1, mPoint p2)
{
	dc.DrawLine(p1.x, p1.y, p2.x, p2.y);
}

void Main::OnDraw(wxDC& dc)
{
	wxColour defaultLine = /*wxHSL(hue, 255, 255)*/wxColour(0, 0, 0);
	wxColour defaultFill = wxColour(255, 255, 255);

	dc.Clear();
	wxPen pen = dc.GetPen();
	wxBrush brush = dc.GetBrush();
	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(defaultLine);
	brush.SetColour(defaultFill);
	dc.SetPen(pen);

	int polyCount = dataset.polygons.size();
	int polyIndex = 0;
	//for (mPolygon& poly : dataset.polygons)
	//{
	//	if (countries[polyIndex].name == "Russia")
	//	{
	//		pen.SetColour(255, 0, 0);
	//		dc.SetPen(pen);
	//	}
	//	else
	//	{
	//		pen.SetColour(defaultCol);
	//		dc.SetPen(pen);
	//	}
	//	//wxPoint* pointArr = new wxPoint[poly.parts[1] + 1];
	//	/*std::vector<wxPoint> pointsSource;

	//	wxPointList pointArr;
	//	for (int i = 0; i < poly.parts[1]; i++)
	//	{
	//		mPoint p = mPoint(poly.points[i].x, poly.points[i].y);
	//		mPoint screen = Transform(p);
	//		pointsSource.push_back(wxPoint(screen.x, screen.y));
	//		pointArr.Append(&pointsSource[i]);
	//	}
	//	mPoint p = mPoint(poly.points[0].x, poly.points[0].y);
	//	mPoint screen = Transform(p);
	//	pointsSource.push_back(wxPoint(screen.x, screen.y));
	//	pointArr.Append(&pointsSource[poly.parts[1]]);

	//	dc.DrawLines(&pointArr);
	//	//dc.DrawPolygon(&pointArr, 0, 0, wxODDEVEN_RULE);*/
	//	for (int ringIndex = 0; ringIndex < poly.nRings; ringIndex++)
	//	{
	//		int startIndex = poly.parts[ringIndex];
	//		mPoint ringStart, last;

	//		ringStart = Transform(poly.points[startIndex].x, poly.points[startIndex].y);
	//		last = ringStart;
	//		for (int pIndex = startIndex + 1; pIndex < poly.parts[ringIndex + 1]; pIndex++)
	//		{
	//			mPoint current = Transform(poly.points[pIndex].x, poly.points[pIndex].y);
	//			mDrawLine(dc, last, current);

	//			last = current;
	//		}
	//		mDrawLine(dc, last, ringStart);
	//	}

	//	polyIndex++;
	//}
	//dc.DrawText(wxString(std::to_string(hue)), wxPoint(50, 50));

	for (mPolygon poly : dataset.polygons)
	{
		if (polyIndex == highlight)
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
			dc.DrawPolygon(&list, 0, 0, wxWINDING_RULE);
		}
		polyIndex++;
	}
}

void Main::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc (this);
	this->PrepareDC(dc);
	this->OnDraw(dc);
}

void Main::Resized(wxSizeEvent& evt)
{
	width = evt.GetSize().x;
	height = evt.GetSize().y;

	this->Refresh();
}

void Main::KeyDown(wxCommandEvent& evt)
{
	highlight++;
	this->Refresh();
	evt.Skip();
}