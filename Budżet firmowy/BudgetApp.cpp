#include "BudgetApp.h"

wxIMPLEMENT_APP(BudgetApp);

BudgetApp::BudgetApp()
{

}

BudgetApp::~BudgetApp()
{

}

bool BudgetApp::OnInit()
{
	main = new BudgetMain();
	main->Show();

	return true;
}