#include "BudgetWindowContracts.h"
#include "BudgetMain.h"

wxBEGIN_EVENT_TABLE(BudgetWindowContracts, wxMDIChildFrame)
	EVT_BUTTON(10001, AddContract)
	EVT_LISTBOX(10002, OnContractSelect)
	EVT_CLOSE(OnClose)
wxEND_EVENT_TABLE()

BudgetWindowContracts::BudgetWindowContracts(wxMDIParentFrame* parent, wxString bMonthId, wxString pName) : wxMDIChildFrame(parent, wxID_ANY, "Umowy w ramach projektu " + pName + " z " + bMonthId)
{
	monthId = bMonthId;
	projectName = pName;
	
	headerDesc = new wxStaticText(this, wxID_ANY, "Informacje o umowie", wxDefaultPosition, wxSize(250, 200));
	wxStaticText* headerContractor = new wxStaticText(this, wxID_ANY, "Wykonawca umowy");
	wxStaticText* headerContractType = new wxStaticText(this, wxID_ANY, "Rodzaj umowy");
	wxStaticText* headerExemption = new wxStaticText(this, wxID_ANY, "Rodzaj ulgi (opcjonalne)");
	wxStaticText* headerContractNum = new wxStaticText(this, wxID_ANY, "Numer umowy (liczba ca³kowita >=0)");
	wxStaticText* headerValue = new wxStaticText(this, wxID_ANY, "Wartoœæ umowy \"na rêkê\" (z³ netto)\n(liczba ca³kowita >=0)");
	wxStaticText* headerBotValue = new wxStaticText(this, wxID_ANY, "Suma kosztów projektu: ");

	btn = new wxButton(this, 10001, "Dodaj umowê", wxDefaultPosition, wxSize(100, 30));
	txtContractor = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
	txtValue = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
	txtContractNum = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
	list = new wxListBox(this, 10002, wxDefaultPosition, wxSize(300, 200));

	selectContractType = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
	selectExemption = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 30));
	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent != nullptr)
	{
		for (auto s : trueParent->TaxRates)
		{
			if (s.first.empty()) break;
			selectContractType->Append(s.first);
		}
		
		for (auto s : trueParent->taxStatuses)
		{
			if (s.first.empty()) break;
			selectExemption->Append(s.first);
		}
	}

	txtSum = new wxStaticText(this, wxID_ANY, "0 z³ netto");
	ResetCostsSum();

	sizer = new wxBoxSizer(wxVERTICAL);
	rowSizer = new wxFlexGridSizer(5, 2, 2, 2);
	rowSizerList = new wxFlexGridSizer(1, 2, 2, 0);

	rowSizer->Add(headerContractor, 0, wxALL, 2);
	rowSizer->Add(txtContractor, 0, wxALL, 2);
	rowSizer->Add(headerContractType, 0, wxALL, 2);
	rowSizer->Add(selectContractType, 0, wxALL, 2);
	rowSizer->Add(headerExemption, 0, wxALL, 2);
	rowSizer->Add(selectExemption, 0, wxALL, 2);
	rowSizer->Add(headerContractNum, 0, wxALL, 2);
	rowSizer->Add(txtContractNum, 0, wxALL, 2);
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

BudgetWindowContracts::~BudgetWindowContracts()
{
}

void BudgetWindowContracts::AddContract(wxCommandEvent& evt)
{
	wxString const contractor = txtContractor->GetValue();
	wxString const type = selectContractType->GetStringSelection();
	wxString const exemption = selectExemption->GetStringSelection();
	wxString const num = txtContractNum->GetValue();
	wxString const value = txtValue->GetValue();

	if (!contractor.empty() && !type.empty() && !value.empty() && std::all_of(value.begin(), value.end(), ::isdigit) && !num.empty() && std::all_of(num.begin(), num.end(), ::isdigit))
	{
		unsigned int const valueInt = std::stoi((std::string)value);
		unsigned int const numInt = std::stoi((std::string)num);

		BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
		if (trueParent)
		{
			wxString monthData[2] = {
				monthId.substr(0, monthId.find('-')),
				monthId.substr(monthId.find('-') + 1, monthId.length())
			};
			wxString contractName = contractor + " | " + type + " " + num + "/" + monthData[1] + "/" + monthData[0];
			
			if (trueParent->AddContractToProject(monthId, projectName, contractor, type, exemption, valueInt, numInt, contractName))
			{
				trueParent->HasChanged();

				list->Append(contractName);
				txtContractor->SetValue("");
				selectContractType->SetValue("");
				selectExemption->SetValue("");
				txtValue->SetValue("");
				txtContractNum->SetValue("");

				ResetCostsSum();
			}
		}
	}
	else
	{
		wxMessageBox("Uzupe³nij dane na temat umowy!", "Uwaga!");
	}
	evt.Skip();
}

unsigned int BudgetWindowContracts::ResetCostsSum()
{
	int sum = 0;

	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent == nullptr) return sum;

	MonthBudget* m = trueParent->GetMonthBudget(monthId);
	if (m == nullptr) return sum;

	Project* p = m->GetProject(projectName);
	if (p == nullptr) return sum;

	sum = p->GetContractsCosts();
	txtSum->SetLabel(std::to_string(sum) + " z³ netto");
	return sum;
}

void BudgetWindowContracts::OnContractSelect(wxCommandEvent& evt)
{
	wxString errorMsg = "Podczas pobierania informacji o umowie wyst¹pi³ b³¹d";

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

		Project* p = m->GetProject(projectName);
		if (p == nullptr)
		{
			headerDesc->SetLabel(errorMsg);
			return;
		}

		Contract* c = p->GetContract(list->GetStringSelection());
		if (p == nullptr)
		{
			headerDesc->SetLabel(errorMsg);
			return;
		}

		headerDesc->SetLabel(c->GetInfo());
	}
	else
	{
		headerDesc->SetLabel("Wybierz umowê");
	}
	evt.Skip();
}

void BudgetWindowContracts::OnClose(wxCloseEvent& evt)
{
	BudgetMain* trueParent = wxDynamicCast(this->GetParent(), BudgetMain);
	if (trueParent) trueParent->OnChildClose(evt);

	Destroy();
	evt.Skip();
}

