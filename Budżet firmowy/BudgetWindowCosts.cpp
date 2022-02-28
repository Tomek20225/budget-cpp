#include "BudgetWindowCosts.h"
#include "BudgetMain.h"

wxBEGIN_EVENT_TABLE(BudgetWindowCosts, wxMDIChildFrame)
	EVT_BUTTON(10001, AddCost)
	EVT_LISTBOX(10002, OnCostSelect)
	EVT_CLOSE(OnClose)
wxEND_EVENT_TABLE()

BudgetWindowCosts::BudgetWindowCosts(wxMDIParentFrame* parent, wxString bMonthId) : wxMDIChildFrame(parent, wxID_ANY, "Koszty z " + bMonthId)
{
	monthId = bMonthId;

	headerDesc = new wxStaticText(this, wxID_ANY, "Informacje o koszcie", wxDefaultPosition, wxSize(250, 200));
	wxStaticText* headerName = new wxStaticText(this, wxID_ANY, "Nazwa kosztu");
	wxStaticText* headerAmount = new wxStaticText(this, wxID_ANY, "Iloœæ");
	wxStaticText* headerValue = new wxStaticText(this, wxID_ANY, "Wartoœæ (z³ brutto)\n(liczba ca³kowita >=0)");
	wxStaticText* headerBotValue = new wxStaticText(this, wxID_ANY, "Suma kosztów w miesi¹cu: ");

	btn = new wxButton(this, 10001, "Dodaj koszt", wxDefaultPosition, wxSize(100, 30));
	txtName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
	txtAmount = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
	txtValue = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
	list = new wxListBox(this, 10002, wxDefaultPosition, wxSize(300, 200));
	
	txtSum = new wxStaticText(this, wxID_ANY, "0 z³ netto");
	ResetCostsSum();

	sizer = new wxBoxSizer(wxVERTICAL);
	rowSizer = new wxFlexGridSizer(3, 2, 2, 2);
	rowSizerList = new wxFlexGridSizer(1, 2, 2, 0);

	rowSizer->Add(headerName, 0, wxALL, 2);
	rowSizer->Add(txtName, 0, wxALL, 2);
	rowSizer->Add(headerAmount, 0, wxALL, 2);
	rowSizer->Add(txtAmount, 0, wxALL, 2);
	rowSizer->Add(headerValue, 0, wxALL, 2);
	rowSizer->Add(txtValue, 0, wxALL, 2);

	rowSizerList->Add(list, 1, wxEXPAND | wxALL, 0);
	rowSizerList->Add(headerDesc, 1, wxEXPAND | wxALL, 0);

	rowSizerBot = new wxFlexGridSizer(1, 2, 2, 2);
	rowSizerBot->Add(headerBotValue, 0, wxALL, 2);
	rowSizerBot->Add(txtSum, 0, wxALL, 2);

	sizer->Add(rowSizer);
	sizer->Add(btn, 0, wxALL, 2);
	sizer->Add(rowSizerList, 1, wxEXPAND | wxALL, 2);
	sizer->Add(rowSizerBot);

	SetSizerAndFit(sizer);
}

BudgetWindowCosts::~BudgetWindowCosts()
{
}

void BudgetWindowCosts::AddCost(wxCommandEvent& evt)
{
	wxString const name = txtName->GetValue();
	wxString const amount = txtAmount->GetValue();
	wxString const value = txtValue->GetValue();

	if (!name.empty() && !amount.empty() && std::all_of(amount.begin(), amount.end(), ::isdigit) && !value.empty() && std::all_of(value.begin(), value.end(), ::isdigit))
	{
		unsigned int const amountInt = std::stoi((std::string)amount);
		unsigned int const valueInt = std::stoi((std::string)value);

		BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
		if (trueParent)
		{
			if (trueParent->AddCostToMonth(monthId, name, amountInt, valueInt))
			{
				trueParent->HasChanged();

				list->Append(name);
				txtName->SetValue("");
				txtAmount->SetValue("");
				txtValue->SetValue("");

				ResetCostsSum();
			}
		}
	}
	else
	{
		wxMessageBox("Uzupe³nij dane na temat kosztu!", "Uwaga!");
	}
	evt.Skip();
}

unsigned int BudgetWindowCosts::ResetCostsSum()
{
	int sum = 0;

	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent == nullptr) return sum;

	MonthBudget* m = trueParent->GetMonthBudget(monthId);
	if (m == nullptr) return sum;

	sum = m->GetCostsValue();
	txtSum->SetLabel(std::to_string(sum) + " z³ netto");
	return sum;
}

void BudgetWindowCosts::OnCostSelect(wxCommandEvent& evt)
{
	wxString errorMsg = "Podczas pobierania informacji o koszcie wyst¹pi³ b³¹d";

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

		Cost* c = m->GetCost(list->GetStringSelection());
		if (c == nullptr)
		{
			headerDesc->SetLabel(errorMsg);
			return;
		}

		headerDesc->SetLabel(c->GetInfo());
	}
	else
	{
		headerDesc->SetLabel("Wybierz koszt");
	}
	evt.Skip();
}

void BudgetWindowCosts::OnClose(wxCloseEvent& evt)
{
	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent) trueParent->OnChildClose(evt);

	Destroy();
	evt.Skip();
}

