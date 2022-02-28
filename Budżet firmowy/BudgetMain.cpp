#include "BudgetMain.h"
#include "BudgetWindowProjects.h"

wxBEGIN_EVENT_TABLE(BudgetMain, wxMDIParentFrame)
	EVT_MENU(10001, BudgetMain::OnMenuOpen)
	EVT_MENU(10002, BudgetMain::OnMenuSave)
	//EVT_MENU(10003, BudgetMain::OnMenuDebug)
	EVT_MENU(10004, BudgetMain::OnMenuExit)
	//EVT_BUTTON(wxID_ANY, BudgetMain::OnEditAction)
	//EVT_CLOSE(BudgetMain::OnClose)
wxEND_EVENT_TABLE()

BudgetMain::BudgetMain() : wxMDIParentFrame(nullptr, wxID_ANY, "Bud¿et firmowy - Tomasz Kapcia, A2, informatyka 2020/2021", wxPoint(60, 60), wxSize(1200, 700))
{
	// Menu aplikacji
	menuBar = new wxMenuBar();
	this->SetMenuBar(menuBar);
	wxMenu* menuFile = new wxMenu();
	menuFile->Append(10001, "Wczytaj");
	menuFile->Append(10002, "Zapisz");
	//menuFile->Append(10003, "Debuguj");
	menuFile->Append(10004, "WyjdŸ");
	menuBar->Append(menuFile, "File");

	// Uzupe³nienie mapy statusów podatkowych
	TaxStatus zus0status;
	zus0status.exemptContracts.push_back("UOP");
	zus0status.exemptContracts.push_back("UZ");
	zus0status.newTaxRate = 0;
	taxStatuses.insert({"ZUS-0", zus0status});

	// Okno z list¹ miesiêcy
	w_months = new BudgetWindowMonths(this, "Lista miesiêcy");
	w_months->Show();
}

BudgetMain::~BudgetMain()
{
}

void BudgetMain::HasChanged()
{
	unsavedChanges = true;
}

void BudgetMain::HasChanged(bool opt)
{
	unsavedChanges = opt;
}

bool BudgetMain::OpenUnsavedChangesDialog()
{
	wxMessageDialog* saveReminderBox = new wxMessageDialog(NULL, wxT("Czêœæ zmian nie zosta³a zapisana. Czy chcesz zapisaæ je teraz?"), wxT("Zapisz zmiany w bud¿ecie"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	int saveReminderBoxReturn = wxNO;
	saveReminderBoxReturn = saveReminderBox->ShowModal();

	if (saveReminderBoxReturn == wxID_YES)
	{
		OnMenuSave(*new wxCommandEvent(wxEVT_NULL, 0));
		return true;
	}

	return false;
}

std::vector<std::string> BudgetMain::SplitString(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);

	while (std::getline(tokenStream, token, delimiter))
		tokens.push_back(token);

	return tokens;
}

void BudgetMain::OnMenuOpen(wxCommandEvent& evt)
{
	if (unsavedChanges == true)
		OpenUnsavedChangesDialog();

	std::ifstream file("ZAPIS BUD¯ETU.txt", std::ios::in);
	std::string line;
	char delim = ';';

	if (file.is_open())
	{
		std::vector<MonthBudget> newMonthBudgets;
		bool isBudgeted = false, isProjected = false;
		int budgetIterator = -1, projectIterator = -1;

		while (std::getline(file, line))
		{
			if (line.rfind("----", 0) == 0)
			{
				if (!isBudgeted) continue;
				
				std::vector<std::string> cData = SplitString(line, delim);
				cData[0] = cData[0].substr(4, cData[0].length() - 1);
				Cost* cost = new Cost(cData[0], std::stoi(cData[2]), std::stoi(cData[1]));
				cost->wholeValue = std::stoi(cData[3]);
				newMonthBudgets[budgetIterator].costs.push_back(*cost);

				isProjected = false;
			}
			else if (line.rfind("---", 0) == 0)
			{
				if (!isProjected) continue;

				std::vector<std::string> cData = SplitString(line, delim);
				cData[0] = cData[0].substr(3, cData[0].length() - 1);
				Contract* contract = new Contract(cData[2], cData[1], std::stoi(cData[5]), std::stoi(cData[6]), cData[3], std::stoi(cData[4]), cData[0]);
				contract->taxRate = std::stoi(cData[7]);
				newMonthBudgets[budgetIterator].projects[projectIterator].contracts.push_back(*contract);
			}
			else if (line.rfind("--", 0) == 0)
			{
				if (!isBudgeted) continue;
			
				std::vector<std::string> pData = SplitString(line, delim);
				pData[0] = pData[0].substr(2, pData[0].length() - 1);
				Project* project = new Project(pData[0], std::stoi(pData[1]));
				newMonthBudgets[budgetIterator].projects.push_back(*project);

				isProjected = true;
				projectIterator++;
			}
			else if (line.rfind("-", 0) == 0)
			{
				std::vector<std::string> mData = SplitString(line, delim);
				mData[0] = mData[0].substr(1, mData[0].length() - 1);
				MonthBudget* budget = new MonthBudget(std::stoi(mData[0]), std::stoi(mData[1]));
				budget->id = mData[2];
				newMonthBudgets.push_back(*budget);

				isBudgeted = true;
				isProjected = false;
				budgetIterator++;
				projectIterator = -1;
			}
			else
				break;
		}

		monthBudgets = newMonthBudgets;
		w_months->ResetList();

		wxMessageBox("Wczytano plan bud¿etowy!", "Sukces!");
	}
	else
		wxMessageBox("Podczas odczytywania pliku wyst¹pi³ b³¹d", "B³¹d!");

	evt.Skip();
}

void BudgetMain::OnMenuSave(wxCommandEvent& evt)
{
	std::ofstream file("ZAPIS BUD¯ETU.txt", std::ios::out);
	std::string dataStr = "";
	char delim = ';';

	for (MonthBudget m : monthBudgets)
	{
		dataStr += "-";
		dataStr += std::to_string(m.month) + delim;
		dataStr += std::to_string(m.year) + delim;
		dataStr += m.id;
		dataStr += "\n";

		for (Project p : m.projects)
		{
			dataStr += "--";
			dataStr += p.name + delim;
			dataStr += std::to_string(p.value);
			dataStr += "\n";

			for (Contract c : p.contracts)
			{
				dataStr += "---";
				dataStr += c.name + delim;
				dataStr += c.contractor + delim;
				dataStr += c.type + delim;
				dataStr += c.exemption + delim;
				dataStr += std::to_string(c.num) + delim;
				dataStr += std::to_string(c.value) + delim;
				dataStr += std::to_string(c.taxedValue) + delim;
				dataStr += std::to_string(c.taxRate);
				dataStr += "\n";
			}
		}

		for (Cost c : m.costs)
		{
			dataStr += "----";
			dataStr += c.name + delim;
			dataStr += std::to_string(c.value) + delim;
			dataStr += std::to_string(c.amount) + delim;
			dataStr += std::to_string(c.wholeValue);
			dataStr += "\n";
		}
	}

	if (file.is_open())
	{
		file << dataStr;
		wxMessageBox("Zapisano zmiany!", "Sukces!");
	}
	else
		wxMessageBox("Podczas zapisu do pliku wyst¹pi³ b³¹d", "B³¹d!");

	file.close();
	evt.Skip();
}

void BudgetMain::OnMenuDebug(wxCommandEvent& evt)
{
	wxString debugString = "";
	for (MonthBudget m : monthBudgets)
	{
		debugString += m.id + "\n";

		for (Project p : m.projects)
		{
			debugString += p.name + "\n";

			for (Contract c : p.contracts)
				debugString += c.name + "\n";
		}

		debugString += "-------";

		for (Cost c : m.costs)
			debugString += c.name + "\n";

		debugString += "=============";
	}
	wxMessageBox(debugString);
}

void BudgetMain::OnMenuExit(wxCommandEvent& evt)
{
	if (unsavedChanges == true)
		OpenUnsavedChangesDialog();

	Close();
	evt.Skip();
}

MonthBudget* BudgetMain::GetMonthBudget(wxString monthId)
{
	for (MonthBudget &month : monthBudgets)
		if (month.id == monthId)
			return &month;

	return nullptr;
}

bool BudgetMain::AddMonthBudget(unsigned int month, unsigned int year)
{
	MonthBudget* budget = new MonthBudget(month, year);
	monthBudgets.push_back(*budget);
	return true;
}

void BudgetMain::EditMonthBudget(wxCommandEvent& evt, wxString monthId)
{
	if (w_projects == nullptr && w_costs == nullptr && w_contracts == nullptr)
	{
		w_projects = new BudgetWindowProjects(this, monthId);
		w_costs = new BudgetWindowCosts(this, monthId);

		MonthBudget* correspMonth = GetMonthBudget(monthId);

		if (correspMonth != nullptr)
		{
			for (Project project : (*correspMonth).projects)
				w_projects->list->Append(project.name);

			for (Cost cost : (*correspMonth).costs)
				w_costs->list->Append(cost.name);
		}
			
		w_projects->Show();
		w_costs->Show();
	}
	else
	{
		wxMessageBox("Zakoñcz edycjê obecnego miesi¹ca!", "Uwaga!");
	}
}

bool BudgetMain::CostExistsInMonth(wxString monthId, wxString name)
{
	MonthBudget* correspMonth = GetMonthBudget(monthId);
	if (correspMonth == nullptr) return false;

	Cost* correspCost = correspMonth->GetCost(name);
	if (correspCost == nullptr) return false;

	return true;
}

bool BudgetMain::AddCostToMonth(wxString monthId, wxString name, unsigned int amount, unsigned int value)
{
	if (CostExistsInMonth(monthId, name))
	{
		wxMessageBox("Koszt o podanej nazwie ju¿ istnieje w tym miesi¹cu!", "Uwaga!");
		return false;
	}

	Cost* cost = new Cost(name, amount, value);
	MonthBudget* correspMonth = GetMonthBudget(monthId);

	if ((*correspMonth).AddCost(*cost))
	{
		ResetDescriptionData();
		return true;
	}

	return false;
}

bool BudgetMain::ProjectExistsInMonth(wxString monthId, wxString name)
{
	MonthBudget* correspMonth = GetMonthBudget(monthId);
	if (correspMonth == nullptr) return false;

	Project* correspProject = correspMonth->GetProject(name);
	if (correspProject == nullptr) return false;

	return true;
}

bool BudgetMain::AddProjectToMonth(wxString monthId, wxString name, unsigned int value)
{
	if (ProjectExistsInMonth(monthId, name))
	{
		wxMessageBox("Projekt o podanej nazwie ju¿ istnieje w tym miesi¹cu!", "Uwaga!");
		return false;
	}

	Project* project = new Project(name, value);
	MonthBudget* correspMonth = GetMonthBudget(monthId);
	
	if ((*correspMonth).AddProject(*project))
	{
		ResetDescriptionData();
		return true;
	}

	return false;
}

bool BudgetMain::ContractExistsInProject(wxString monthId, wxString projectName, wxString contractName)
{
	MonthBudget* correspMonth = GetMonthBudget(monthId);
	if (correspMonth == nullptr) return false;

	Project* correspProject = correspMonth->GetProject(projectName);
	if (correspProject == nullptr) return false;

	Contract* correspContract = correspProject->GetContract(contractName);
	if (correspContract == nullptr) return false;

	return true;
}

void BudgetMain::EditProjectContracts(wxCommandEvent& evt, wxString monthId, wxString projectName)
{
	if (w_contracts == nullptr)
	{
		w_contracts = new BudgetWindowContracts(this, monthId, projectName);

		MonthBudget* correspMonth = GetMonthBudget(monthId);

		if (correspMonth != nullptr)
		{
			Project* correspProject = correspMonth->GetProject(projectName);
			
			if (correspProject != nullptr)
				for (Contract contract : (*correspProject).contracts)
					w_contracts->list->Append(contract.name);
		}

		w_contracts->Show();
	}
	else
	{
		wxMessageBox("Zakoñcz edycjê obecnego miesi¹ca!", "Uwaga!");
	}
}

unsigned int BudgetMain::TaxContract(wxString type, wxString exemption, unsigned int value)
{
	if (!exemption.empty() && taxStatuses.count(exemption) > 0)
	{
		std::map<wxString, TaxStatus>::iterator it;
		it = taxStatuses.find(exemption);
		TaxStatus status = it->second;

		for (wxString t : status.exemptContracts)
			if (t == type)
				return value * (1 + ((double)status.newTaxRate / 100));
	}

	if (TaxRates.count(type) > 0)
		return value * (1 + ((double)TaxRates.at(type) / 100));

	return 0;
}

bool BudgetMain::AddContractToProject(wxString monthId, wxString projectName, wxString contractor, wxString type, wxString exemption, unsigned int value, unsigned int contractNum, wxString contractName)
{
	if (ContractExistsInProject(monthId, projectName, contractName))
	{
		wxMessageBox("Umowa o wskazanych danych ju¿ istnieje w tym miesi¹cu!", "Uwaga!");
		return false;
	}

	Contract* contract = new Contract(type, contractor, value, TaxContract(type, exemption, value), exemption, contractNum, contractName);
	Project* correspProject = GetMonthBudget(monthId)->GetProject(projectName);
	
	if ((*correspProject).AddContract(*contract))
	{
		ResetDescriptionData();
		return true;
	}
	
	return false;
}

//void BudgetMain::OnClose(wxCloseEvent& evt)
//{
//	if (evt.GetId() == this->GetId()) {
//		Destroy();
//		evt.Skip();
//	}
//	else
//		OnChildClose(evt);
//}

void BudgetMain::ResetDescriptionData()
{
	if (w_projects != nullptr)
	{
		w_projects->ResetSummary();
		w_projects->OnProjectSelect(*new wxCommandEvent(wxEVT_NULL, 0));
	}

	if (w_costs != nullptr)
	{
		w_costs->ResetCostsSum();
		w_costs->OnCostSelect(*new wxCommandEvent(wxEVT_NULL, 0));
	}
	
	if (w_months != nullptr)
		w_months->OnMonthSelect(*new wxCommandEvent(wxEVT_NULL, 0));
}

void BudgetMain::OnChildClose(wxCloseEvent& evt)
{
	if (evt.GetEventObject() == w_projects)
	{
		w_projects = nullptr;
	}
	else if (evt.GetEventObject() == w_contracts)
	{
		w_contracts = nullptr;
	}
	else if (evt.GetEventObject() == w_costs)
	{
		w_costs = nullptr;
	}
	evt.Skip();
}