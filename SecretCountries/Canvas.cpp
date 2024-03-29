#include "Canvas.h"

#define USES_CACHING 0

wxBEGIN_EVENT_TABLE(Canvas, wxPanel)
	EVT_PAINT(Canvas::OnPaint)
	EVT_SIZE(Canvas::Resized)
wxEND_EVENT_TABLE()

Canvas::Canvas(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	changelog.reserve(nCountries);
}

Canvas::~Canvas()
{
	for (Country& country : countries)
	{
		SHPDestroyObject(country.geometry);
	}
	ClearCache();
}

void Canvas::FullRefresh()
{
	ClearCache();
	Refresh();
}

wxPoint Canvas::Transform(double x, double y)
{
	double dataWidth = bounds.xMax - bounds.xMin;
	double dataHeight = bounds.yMax - bounds.yMin;
	double x_ = (x - bounds.xMin) / dataWidth * w;
	double y_ = h - (y - bounds.yMin) / dataHeight * h;

	return wxPoint(x_, y_);
}

void Canvas::ResetCountries()
{
	for (Country& c : countries)
	{
		c.col = wxColour(255, 255, 255);
		c.guessed = false;
	}
	AllChanged();
}

void Canvas::AllChanged()
{
	for (int i = 0; i < nCountries; i++)
	{
		changelog.push_back(i);
	}
}

void Canvas::CountryGuessed(int index)
{
	Country& g = countries[index];
	Country& s = countries[secretIndex];

	double dist = sqrt(pow(g.metadata.lat - s.metadata.lat, 2) + pow(g.metadata.lon - s.metadata.lon, 2));
	if (index == secretIndex)
	{
		g.col = wxColour(0, 0, 0);
	}
	else
	{
		g.col = wxHSL(120 / (0.05 * dist + 1), 255, 255);
	}
	g.guessed = true;
	changelog.push_back(index);
}

void Canvas::RevealAll()
{
	for (int i = 0; i < countries.size(); i++)
	{
		CountryGuessed(i);
	}
}

void Canvas::SetBlind(bool isBlind)
{
	blind = isBlind;
	AllChanged();
	FullRefresh();
}

void Canvas::OnDraw(wxDC& dc)
{
	if (blind) { dc.SetBackground(wxBrush(wxColour(0, 0, 0))); }
	else { dc.SetBackground(wxBrush(wxColour(200, 200, 200))); }
	if (changelog.size() == nCountries) { dc.Clear(); }

	wxBrush brush(wxColour(255, 255, 255), wxBRUSHSTYLE_SOLID);

	for (int polyI : changelog)
	{
		Country& country = countries[polyI];
		SHPObject* poly = countries[polyI].geometry;

		if (blind && !country.guessed) { continue; }

		// Set colour
		brush.SetColour(countries[polyI].col);
		dc.SetBrush(brush);

		// Draw polygons
		for (int partIndex = 0; partIndex < poly->nParts; partIndex++)
		{
			int startIndex = poly->panPartStart[partIndex];
			int endIndex = (partIndex < poly->nParts - 1) ? poly->panPartStart[partIndex + 1] : poly->nVertices;
			int partSize = endIndex - startIndex;

			wxPoint* points = new wxPoint[partSize];
			for (int pi = 0; pi < partSize; pi++)
			{
				points[pi] = Transform(poly->padfX[startIndex + pi], poly->padfY[startIndex + pi]);
			}
			dc.DrawPolygon(partSize, points);

			delete[] points;
		}

		// Check if country is very small
		if (country.metadata.area < 3000)
		{
			// Draw cirlce on country
			if (country.guessed)
			{
				dc.DrawCircle(Transform(country.metadata.lon, country.metadata.lat), 5);
			}
		}
	}

	changelog.clear();
}

void Canvas::OnPaint(wxPaintEvent& evt)
{
	wxBufferedPaintDC dc(this);
	if (ready)
	{
		GetSize(&w, &h);
		PrepareDC(dc);

		if (cache)
		{
			dc.DrawBitmap(*cache, 0, 0);
		}
		else
		{
			OnDraw(dc);
#if USES_CACHING == 1
			UpdateCache(&dc);
#endif
		}
	}
}

void Canvas::Resized(wxSizeEvent& evt)
{
	GetSize(&w, &h);
	AllChanged();
	FullRefresh();
	evt.Skip();
}

void Canvas::UpdateCache(wxDC* dc)
{
	cache = new wxBitmap(w, h);
	wxMemoryDC memDC(*cache);
	memDC.Blit(0, 0, w, h, dc, 0, 0);
	memDC.SelectObject(wxNullBitmap);
}

void Canvas::ClearCache()
{
	if (!cache) { return; }
	delete cache;
	cache = nullptr;
}