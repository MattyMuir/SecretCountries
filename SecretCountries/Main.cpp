#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_BUTTON(10001, Main::ButtonPressed)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Secret Countries", wxPoint(30, 30), wxSize(1280, 720))
{
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
	for (CountryData& country : countries)
	{
		std::transform(country.name.begin(), country.name.end(), country.name.begin(),
			[](unsigned char c) { return std::tolower(c); });
	}

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, countries.size() - 1);

	secretIndex = dist(mt);

	// Window setup
	topPanel = new wxPanel(this, wxID_ANY);
	mCanvas = new Canvas(this, &dataset, &countries);

	mLabel = new wxStaticText(topPanel, wxID_ANY, "Enter Any Country:", wxPoint(5, 6));
	mTextBox = new wxTextCtrl(topPanel, wxID_ANY, "", wxPoint(110, 5), wxSize(100, 20));
	guessButton = new wxButton(topPanel, 10001, "Guess", wxPoint(235, 0), wxSize(100, 30));

	mSizer = new wxBoxSizer(wxVERTICAL);
	mSizer->Add(topPanel, 0, wxEXPAND | wxALL);
	mSizer->Add(mCanvas, 1, wxEXPAND | wxALL);

	this->SetSizer(mSizer);

	mCanvas->secretIndex = secretIndex;
	mCanvas->displayText = countries[secretIndex].name;
}

Main::~Main()
{

}

void Main::ButtonPressed(wxCommandEvent& evt)
{
	std::string guess = std::string(mTextBox->GetValue().c_str());
	std::transform(guess.begin(), guess.end(), guess.begin(),
		[](unsigned char c) { return std::tolower(c); });

	mTextBox->Clear();
	int index = -1;
	for (int i = 0; i < countries.size(); i++)
		if (countries[i].name == guess) { index = i; }
	mCanvas->guessIndicies.push_back(index);
	mCanvas->mRefresh();
	evt.Skip();
}