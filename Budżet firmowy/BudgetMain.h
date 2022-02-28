#pragma once
#include "wx/wx.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "BudgetWindowMonths.h"
#include "BudgetWindowCosts.h"
#include "BudgetWindowProjects.h"
#include "BudgetWindowContracts.h"
#include "MonthBudget.h"
#include "Project.h"

struct TaxStatus {
	std::vector<wxString> exemptContracts;
	unsigned int newTaxRate;
};

class BudgetMain : public wxMDIParentFrame
{
public:
	//wxToolBar* toolBar = nullptr;
	wxMenuBar* menuBar = nullptr;

	BudgetWindowMonths* w_months = nullptr;
	BudgetWindowCosts* w_costs = nullptr;
	BudgetWindowProjects* w_projects = nullptr;
	BudgetWindowContracts* w_contracts = nullptr;

	std::vector<MonthBudget> monthBudgets;

	std::map<wxString, unsigned int> const TaxRates = {
		{"UOD", 9},
		{"UZ", 54},
		{"UZ+UOP", 18},
		{"UOP", 62},
		{"CZ", 19},
		{"JDG", 19},
		{"VAT", 23}
	};
	std::map<wxString, TaxStatus> taxStatuses;

private:
	bool unsavedChanges = false;

public:
	BudgetMain();
	~BudgetMain();

	void OnMenuOpen(wxCommandEvent& evt);
	void OnMenuSave(wxCommandEvent& evt);
	void OnMenuDebug(wxCommandEvent& evt);
	void OnMenuExit(wxCommandEvent& evt);

	MonthBudget* GetMonthBudget(wxString monthId);
	bool AddMonthBudget(unsigned int month, unsigned int year);
	void EditMonthBudget(wxCommandEvent& evt, wxString monthId);

	bool CostExistsInMonth(wxString monthId, wxString name);
	bool AddCostToMonth(wxString monthId, wxString name, unsigned int amount, unsigned int value);

	bool ProjectExistsInMonth(wxString monthId, wxString name);
	bool AddProjectToMonth(wxString monthId, wxString name, unsigned int value);

	bool ContractExistsInProject(wxString monthId, wxString projectName, wxString contractName);
	void EditProjectContracts(wxCommandEvent& evt, wxString monthId, wxString projectName);
	bool AddContractToProject(wxString monthId, wxString projectName, wxString contractor, wxString type, wxString exemption, unsigned int value, unsigned int contractNum, wxString contractName);
	unsigned int TaxContract(wxString type, wxString exemption, unsigned int value);
	
	//void OnClose(wxCloseEvent& evt);
	void OnChildClose(wxCloseEvent& evt);

	void HasChanged();
	void HasChanged(bool opt);
	bool OpenUnsavedChangesDialog();
	std::vector<std::string> SplitString(const std::string& s, char delimiter);
	void ResetDescriptionData();

	wxDECLARE_EVENT_TABLE();
};

