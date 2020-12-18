#include "App.h"

#pragma warning(suppress : 28251)
wxIMPLEMENT_APP(App);

App::App()
{

}

App::~App()
{

}

bool App::OnInit()
{
	mFrame = new Main;
	mFrame->Show();

	return true;
}