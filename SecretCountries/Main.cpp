#include "Main.h"

wxBEGIN_EVENT_TABLE(Main, wxFrame)
	EVT_MENU(20003, Main::GameReset)
	EVT_MENU(20001, Main::OnOpenSHP)
	EVT_MENU(20002, Main::OnOpenCSV)
	EVT_BUTTON(10001, Main::ButtonPressed)
	EVT_BUTTON(10002, Main::GiveUp)
	EVT_CHECKBOX(10003, Main::BlindBoxClicked)
wxEND_EVENT_TABLE()

Main::Main() : wxFrame(nullptr, wxID_ANY, "Secret Countries", wxPoint(30, 30), wxSize(1280, 720))
{
	// Window setup
	mMenuBar = new wxMenuBar();

	fileMenu = new wxMenu();
	fileMenu->Append(20001, "Open SHP");
	fileMenu->Append(20002, "Open CSV");

	// Disable Open CSV option, as SHP must be opened first
	fileMenu->Enable(20002, false);

	gameMenu = new wxMenu();
	gameMenu->Append(20003, "Reset");

	mMenuBar->Append(fileMenu, "File");
	mMenuBar->Append(gameMenu, "Game");

	topPanel = new wxPanel(this, wxID_ANY);
	mCanvas = new Canvas(this);

	mLabel = new wxStaticText(topPanel, wxID_ANY, "Enter Any Country:", wxPoint(5, 6));
	mTextBox = new wxTextCtrl(topPanel, wxID_ANY, "", wxPoint(110, 5), wxSize(100, 20));
	guessButton = new wxButton(topPanel, 10001, "Guess", wxPoint(217, 0), wxSize(100, 30));
	giveUpButton = new wxButton(topPanel, 10002, "Give Up", wxPoint(320, 0), wxSize(100, 30));
	guessNumText = new wxStaticText(topPanel, wxID_ANY, "Guesses: 0", wxPoint(430, 7));
	answerRevealText = new wxStaticText(topPanel, wxID_ANY, "", wxPoint(600, 7));

	blindBox = new wxCheckBox(topPanel, 10003, "Blind", wxPoint(510, 7));

	mTextBox->Bind(wxEVT_CHAR_HOOK, &Main::KeyEntered, this);

	mSizer = new wxBoxSizer(wxVERTICAL);
	mSizer->Add(topPanel, 0, wxEXPAND | wxALL);
	mSizer->Add(mCanvas, 1, wxEXPAND | wxALL);

	wxAcceleratorEntry entries[1];
	entries[0].Set(wxACCEL_CTRL, (int)'R', 20003);
	wxAcceleratorTable accel(1, entries);
	this->SetAcceleratorTable(accel);

	this->SetMenuBar(mMenuBar);
	this->SetSizer(mSizer);
}

Main::~Main()
{

}

void Main::GuessMade()
{
	std::string guess = mTextBox->GetValue().ToStdString();

	// Make guess all lower case
	std::transform(guess.begin(), guess.end(), guess.begin(),
		[](unsigned char c) { return std::tolower(c); });

	// Look through countries for a name match and save in 'index'
	int index = -1;
	for (int i = 0; i < mCanvas->countries.size(); i++)
		if (mCanvas->countries[i].metadata.name == guess) { index = i; }

	if (index == -1) { return; }

	if (mCanvas->countries[index].guessed) { return; }

	mCanvas->nGuesses++;
	if (index == mCanvas->secretIndex)
	{
		mCanvas->RevealAll();
	}
	else
	{
		mCanvas->CountryGuessed(index);
	}

	mTextBox->Clear();
	guessNumText->SetLabelText("Guesses: " + wxString(std::to_string(mCanvas->nGuesses)));
	mCanvas->FullRefresh();
}

void Main::ButtonPressed(wxCommandEvent& evt)
{
	GuessMade();
	evt.Skip();
}

void Main::GameReset(wxCommandEvent& evt)
{
	mCanvas->ResetCountries();
	mCanvas->nGuesses = 0;
	mCanvas->secretIndex = dist(mt);
	mCanvas->FullRefresh();

	mTextBox->Clear();
	guessNumText->SetLabelText("Guesses: " + wxString(std::to_string(mCanvas->nGuesses)));
	answerRevealText->SetLabelText("");
	evt.Skip();
}

void Main::GiveUp(wxCommandEvent& evt)
{
	if (mCanvas->ready)
	{
		mCanvas->RevealAll();

		std::string answer = mCanvas->countries[mCanvas->secretIndex].metadata.name;
		answerRevealText->SetLabelText("Answer: " + wxString(answer));
		mCanvas->FullRefresh();
	}
	evt.Skip();
}

void Main::BlindBoxClicked(wxCommandEvent& evt)
{
	mCanvas->SetBlind(evt.IsChecked());
}

void Main::OnOpenSHP(wxCommandEvent& evt)
{
	wxFileDialog dlg(this, "Open Countries Shapefile", "", "", ".shp Files|*.shp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		shpDir = dlg.GetPath().ToStdString();

		// Read shapefile
		SHPHandle fileHandle = SHPOpen(shpDir.c_str(), "rb");
		SHPGetInfo(fileHandle, &mCanvas->nCountries, nullptr, &mCanvas->bounds.xMin, &mCanvas->bounds.xMax);

		auto& countries = mCanvas->countries;
		countries.reserve(mCanvas->nCountries);
		for (int p = 0; p < mCanvas->nCountries; p++)
		{
			SHPObject* geometry = SHPReadObject(fileHandle, p);
			countries.emplace_back(geometry);
		}
		SHPClose(fileHandle);

		fileMenu->Enable(20002, true);
	}
}

void Main::KeyEntered(wxKeyEvent& evt)
{
	if (evt.GetKeyCode() == WXK_RETURN)
	{
		GuessMade();
	}
	evt.Skip();
}

void Main::OnOpenCSV(wxCommandEvent& evt)
{
	wxFileDialog dlg(this, "Open Countries CSV File", "", "", ".csv Files|*.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		csvDir = dlg.GetPath().ToStdString();

		// Read CSV file
		std::vector<CountryMeta> allMetadata;
		ReadCountriesCSV(allMetadata, csvDir);
		for (int ci = 0; ci < allMetadata.size(); ci++)
		{
			CountryMeta& meta = allMetadata[ci];

			// Convert country name to lower case
			std::transform(meta.name.begin(), meta.name.end(), meta.name.begin(),
				[](unsigned char c) { return std::tolower(c); });

			mCanvas->countries[ci].metadata = meta;
		}
		InitializeCanvas();
	}
}

void Main::InitializeCanvas()
{
	mt = std::mt19937(rd());
	dist = std::uniform_int_distribution<int>(0, mCanvas->countries.size() - 1);
	mCanvas->secretIndex = dist(mt);

	mCanvas->ready = true;
	mCanvas->AllChanged();
	mCanvas->FullRefresh();
}