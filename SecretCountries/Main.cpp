#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_MENU(20003, Main::GameReset)
	EVT_MENU(20001, Main::OnOpenSHP)
	EVT_MENU(20002, Main::OnOpenCSV)
	EVT_BUTTON(10001, Main::ButtonPressed)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Secret Countries", wxPoint(30, 30), wxSize(1280, 720))
{
	const char* shpDir = "";
	const char* csvDir = "";

	hasSHP = false;
	hasCSV = false;

	// Window setup
	mMenuBar = new wxMenuBar();

	fileMenu = new wxMenu();
	fileMenu->Append(20001, "Open SHP");
	fileMenu->Append(20002, "Open CSV");

	gameMenu = new wxMenu();
	gameMenu->Append(20003, "Reset");

	mMenuBar->Append(fileMenu, "File");
	mMenuBar->Append(gameMenu, "Game");

	topPanel = new wxPanel(this, wxID_ANY);
	mCanvas = new Canvas(this, &dataset, &countries, false);

	mLabel = new wxStaticText(topPanel, wxID_ANY, "Enter Any Country:", wxPoint(5, 6));
	mTextBox = new wxTextCtrl(topPanel, wxID_ANY, "", wxPoint(110, 5), wxSize(100, 20));
	guessButton = new wxButton(topPanel, 10001, "Guess", wxPoint(235, 0), wxSize(100, 30));

	mSizer = new wxBoxSizer(wxVERTICAL);
	mSizer->Add(topPanel, 0, wxEXPAND | wxALL);
	mSizer->Add(mCanvas, 1, wxEXPAND | wxALL);

	this->SetMenuBar(mMenuBar);
	this->SetSizer(mSizer);
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
	mCanvas->Refresh();
	evt.Skip();
}

void Main::GameReset(wxCommandEvent& evt)
{
	mCanvas->guessIndicies.clear();
	secretIndex = dist(mt);
	mCanvas->secretIndex = secretIndex;
	//mCanvas->displayText = countries[secretIndex].name;
	mCanvas->Refresh();
	evt.Skip();
}

void Main::OnOpenSHP(wxCommandEvent& evt)
{
	wxFileDialog dlg(this, "Open Countries Shapefile", "", "", ".shp Files|*.shp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		shpDir = dlg.GetPath().ToStdString();
		ReadPolyShapefile(dataset, shpDir);
		for (mPolygon& poly : dataset.polygons)
		{
			poly.parts.push_back(poly.tPoints);
		}
		hasSHP = true;
		InitializeCanvas();
	}
}

void Main::OnOpenCSV(wxCommandEvent& evt)
{
	wxFileDialog dlg(this, "Open Countries CSV File", "", "", ".csv Files|*.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		csvDir = dlg.GetPath().ToStdString();
		ReadCountriesCSV(countries, csvDir);
		for (CountryData& country : countries)
		{
			std::transform(country.name.begin(), country.name.end(), country.name.begin(),
				[](unsigned char c) { return std::tolower(c); });
		}
		hasCSV = true;
		InitializeCanvas();
	}
}

void Main::InitializeCanvas()
{
	if (hasSHP && hasCSV)
	{
		mt = std::mt19937(rd());
		dist = std::uniform_int_distribution<int>(0, countries.size() - 1);
		secretIndex = dist(mt);

		mCanvas->draw = true;
		mCanvas->secretIndex = secretIndex;
		//mCanvas->displayText = countries[secretIndex].name;

		mCanvas->Refresh();
	}
}