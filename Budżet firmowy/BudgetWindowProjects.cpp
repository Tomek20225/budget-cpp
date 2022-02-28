#include "BudgetWindowProjects.h"
#include "BudgetMain.h"

wxBEGIN_EVENT_TABLE(BudgetWindowProjects, wxMDIChildFrame)
	EVT_BUTTON(10001, AddProject)
	EVT_BUTTON(10002, EditProject)
	EVT_LISTBOX(10003, OnProjectSelect)
	EVT_CLOSE(OnClose)
wxEND_EVENT_TABLE()

BudgetWindowProjects::BudgetWindowProjects(wxMDIParentFrame* parent, wxString sName) : wxMDIChildFrame(parent, wxID_ANY, "Projekty z " + sName)
{
	monthId = sName;

	btn = new wxButton(this, 10001, "Dodaj projekt", wxDefaultPosition, wxSize(100, 30));
	btnEdit = new wxButton(this, 10002, "Edytuj projekt", wxDefaultPosition, wxSize(100, 30));
	txtName = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
	txtValue = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(200, 30));
	list = new wxListBox(this, 10003, wxDefaultPosition, wxSize(300, 200));
	headerDesc = new wxStaticText(this, wxID_ANY, "Informacje o projekcie", wxDefaultPosition, wxSize(250, 200));
	wxStaticText* headerName = new wxStaticText(this, wxID_ANY, "Nazwa projektu");
	wxStaticText* headerValue = new wxStaticText(this, wxID_ANY, "Wartoœæ projektu (z³ netto)\n(liczba ca³kowita >= 0)");
	wxStaticText* headerBotValue = new wxStaticText(this, wxID_ANY, "Suma wartoœci projektów: ");
	wxStaticText* headerBotCosts = new wxStaticText(this, wxID_ANY, "Suma kosztów projektów: ");
	wxStaticText* headerBotFinal = new wxStaticText(this, wxID_ANY, "£¹czny dochód z projektów: ");
	
	txtSum = new wxStaticText(this, wxID_ANY, "0 z³ netto");
	txtCosts = new wxStaticText(this, wxID_ANY, "0 z³ netto");
	txtFinal = new wxStaticText(this, wxID_ANY, "0 z³ netto");
	ResetSummary();

	sizer = new wxBoxSizer(wxVERTICAL);
	rowSizer = new wxFlexGridSizer(3, 2, 2, 2);
	rowSizerList = new wxFlexGridSizer(1, 2, 2, 0);

	rowSizer->Add(headerName, 0, wxALL, 2);
	rowSizer->Add(txtName, 0, wxALL, 2);
	rowSizer->Add(headerValue, 0, wxALL, 2);
	rowSizer->Add(txtValue, 0, wxALL, 2);
	rowSizer->Add(btn, 0, wxALL, 2);
	rowSizer->Add(btnEdit, 0, wxALL, 2);

	rowSizerList->Add(list, 1, wxEXPAND | wxALL, 0);
	rowSizerList->Add(headerDesc, 1, wxEXPAND | wxALL, 0);
	
	rowSizerBot = new wxFlexGridSizer(3, 2, 2, 2);
	rowSizerBot->Add(headerBotValue, 0, wxALL, 2);
	rowSizerBot->Add(txtSum, 0, wxALL, 2);
	rowSizerBot->Add(headerBotCosts, 0, wxALL, 2);
	rowSizerBot->Add(txtCosts, 0, wxALL, 2);
	rowSizerBot->Add(headerBotFinal, 0, wxALL, 2);
	rowSizerBot->Add(txtFinal, 0, wxALL, 2);
	
	sizer->Add(rowSizer);
	sizer->Add(rowSizerList, 1, wxEXPAND | wxALL, 2);
	sizer->Add(rowSizerBot);
	SetSizerAndFit(sizer);
}

BudgetWindowProjects::~BudgetWindowProjects()
{
}

void BudgetWindowProjects::AddProject(wxCommandEvent& evt)
{
	wxString const name = txtName->GetValue();
	wxString const value = txtValue->GetValue();

	if (!name.empty() && !value.empty() && std::all_of(value.begin(), value.end(), ::isdigit))
	{
		unsigned int const valueInt = std::stoi((std::string)value);

		BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
		if (trueParent)
		{
			if (trueParent->AddProjectToMonth(monthId, name, valueInt))
			{
				trueParent->HasChanged();

				list->Append(name);
				txtName->SetValue("");
				txtValue->SetValue("");

				ResetSummary();
			}
		}
	}
	else
	{
		wxMessageBox("Uzupe³nij dane na temat projektu!", "Uwaga!");
	}
	evt.Skip();
}

void BudgetWindowProjects::EditProject(wxCommandEvent& evt)
{
	if (list->GetSelection() != -1)
	{
		BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
		if (trueParent)
			trueParent->EditProjectContracts(evt, monthId, list->GetStringSelection());
	}
	else
	{
		wxMessageBox("Wybierz projekt który chcesz uzupe³niæ!", "Uwaga!");
	}
	evt.Skip();
}

void BudgetWindowProjects::OnProjectSelect(wxCommandEvent& evt)
{
	wxString errorMsg = "Podczas pobierania informacji o projekcie wyst¹pi³ b³¹d";

	if (list->GetSelection() != -1)
	{
		BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
		if (trueParent == nullptr)
		{
			headerDesc->SetLabel(errorMsg);
			return;
		}

		MonthBudget* m = trueParent->GetMonthBudget(monthId);
		if (m == nullptr)
		{
			headerDesc->SetLabel(errorMsg);
			return;
		}

		Project* p = m->GetProject(list->GetStringSelection());
		if (p == nullptr)
		{
			headerDesc->SetLabel(errorMsg);
			return;
		}

		headerDesc->SetLabel(p->GetInfo());
	}
	else
	{
		headerDesc->SetLabel("Wybierz projekt");
	}
	evt.Skip();
}

void BudgetWindowProjects::ResetSummary()
{
	int income = 0, costs = 0, profit = 0;
	
	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent == nullptr) return;

	MonthBudget* m = trueParent->GetMonthBudget(monthId);
	if (m == nullptr) return;

	income = m->GetProjectsValue();
	costs = m->GetProjectsCost();
	profit = income - costs;

	txtSum->SetLabel(std::to_string(income) + " z³ netto");
	txtCosts->SetLabel(std::to_string(costs) + " z³ netto");
	txtFinal->SetLabel(std::to_string(profit) + " z³ netto");
}

void BudgetWindowProjects::OnClose(wxCloseEvent& evt)
{
	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent) trueParent->OnChildClose(evt);
	
	Destroy();
	evt.Skip();
}

