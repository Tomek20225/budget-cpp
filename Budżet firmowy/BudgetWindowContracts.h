#pragma once
#include "wx/wx.h"

class BudgetWindowContracts : public wxMDIChildFrame
{
public:
	wxString monthId;
	wxString projectName;

	wxButton* btn = nullptr;
	wxTextCtrl* txtContractor = nullptr;
	wxTextCtrl* txtValue = nullptr;
	wxTextCtrl* txtContractNum = nullptr;
	wxComboBox* selectContractType = nullptr;
	wxComboBox* selectExemption = nullptr;
	wxListBox* list = nullptr;
	wxStaticText* txtSum = nullptr;
	wxStaticText* headerDesc = nullptr;

	wxBoxSizer* sizer = nullptr;
	wxFlexGridSizer* rowSizer = nullptr;
	wxFlexGridSizer* rowSizerBot = nullptr;
	wxFlexGridSizer* rowSizerList = nullptr;

	void AddContract(wxCommandEvent& evt);
	unsigned int ResetCostsSum();

	void OnContractSelect(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);

	wxDECLARE_EVENT_TABLE();

public:
	BudgetWindowContracts(wxMDIParentFrame* parent, wxString bMonthId, wxString pName);
	~BudgetWindowContracts();
};

