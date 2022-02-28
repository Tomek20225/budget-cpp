#include "BudgetWindowMonths.h"
#include "BudgetMain.h"
#include <ctime>

wxBEGIN_EVENT_TABLE(BudgetWindowMonths, wxMDIChildFrame)
	EVT_BUTTON(10001, AddMonth)
	EVT_BUTTON(10002, EditMonth)
	EVT_LISTBOX(10003, OnMonthSelect)
	EVT_CLOSE(OnClose)
wxEND_EVENT_TABLE()

BudgetWindowMonths::BudgetWindowMonths(wxMDIParentFrame* parent, wxString sName) : wxMDIChildFrame(parent, wxID_ANY, sName)
{
	parentFrame = parent;
	
	btn = new wxButton(this, 10001, "Dodaj nastêpny miesi¹c", wxDefaultPosition, wxSize(200, 30));
	btnEdit = new wxButton(this, 10002, "Edytuj miesi¹c", wxDefaultPosition, wxSize(200, 30));
	list = new wxListBox(this, 10003, wxDefaultPosition, wxSize(300, 200));
	headerDesc = new wxStaticText(this, wxID_ANY, "Informacje o miesi¹cu", wxDefaultPosition, wxSize(250, 200));

	sizer = new wxBoxSizer(wxVERTICAL);
	rowSizer = new wxFlexGridSizer(2, 2, 2, 2);
	rowSizerList = new wxFlexGridSizer(1, 2, 2, 0);

	rowSizer->Add(btn, 0, wxALL, 2);
	rowSizer->Add(btnEdit, 0, wxALL, 2);

	rowSizerList->Add(list, 1, wxEXPAND | wxALL, 0);
	rowSizerList->Add(headerDesc, 1, wxEXPAND | wxALL, 0);

	sizer->Add(rowSizer);
	sizer->Add(rowSizerList, 1, wxEXPAND | wxALL, 2);
	
	SetSizerAndFit(sizer);
}

BudgetWindowMonths::~BudgetWindowMonths()
{
}

void BudgetWindowMonths::AddMonth(wxCommandEvent& evt)
{
	time_t t = time(NULL);
	tm* pTime = localtime(&t);

	if (lastMonth == -1 || lastYear == -1)
	{
		lastMonth = pTime->tm_mon;
		lastYear = pTime->tm_year + 1900;
	}
	else
	{
		lastMonth++;

		if (lastMonth == 12)
		{
			lastMonth = 0;
			lastYear++;
		}
	}

	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent)
	{
		if (trueParent->AddMonthBudget(lastMonth + 1, lastYear))
		{
			trueParent->HasChanged();
			list->AppendString(std::to_string(lastYear) + "-" + std::to_string((lastMonth + 1)));
		}
	}
	
	evt.Skip();
}

void BudgetWindowMonths::EditMonth(wxCommandEvent& evt)
{
	if (list->GetSelection() != -1)
	{
		BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
		if (trueParent)
			trueParent->EditMonthBudget(evt, list->GetString(list->GetSelection()));
	}
	else
	{
		wxMessageBox("Wybierz miesi¹c który chcesz uzupe³niæ!", "Uwaga!");
	}
	evt.Skip();
}

void BudgetWindowMonths::OnMonthSelect(wxCommandEvent& evt)
{
	wxString errorMsg = "Podczas pobierania informacji o miesi¹cu wyst¹pi³ b³¹d";

	if (list->GetSelection() != -1)
	{
		BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
		if (trueParent == nullptr)
		{
			headerDesc->SetLabel(errorMsg);
			return;
		}

		MonthBudget* m = trueParent->GetMonthBudget(list->GetStringSelection());
		if (m == nullptr)
		{
			headerDesc->SetLabel(errorMsg);
			return;
		}

		headerDesc->SetLabel(m->GetInfo());
	}
	else
	{
		headerDesc->SetLabel("Wybierz miesi¹c");
	}
	evt.Skip();
}

void BudgetWindowMonths::OnClose(wxCloseEvent& evt)
{
	//parentFrame->GetEventHandler()->ProcessEvent(evt);
	if (evt.CanVeto())
	{
		wxMessageBox("Tego okna nie mo¿na zamkn¹æ!", "Uwaga!");
		evt.Veto();
		return;
	}
	Destroy();
	evt.Skip();
}

void BudgetWindowMonths::ResetList()
{
	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent)
	{
		list->Clear();
		for (MonthBudget m : trueParent->monthBudgets)
		{
			list->AppendString(m.id);
			lastMonth = m.month - 1;
			lastYear = m.year;
		}
	}
}
