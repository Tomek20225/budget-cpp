#pragma once
#include "wx/wx.h"

class BudgetWindowCosts : public wxMDIChildFrame
{
public:
	wxString monthId;

	wxButton* btn = nullptr;
	wxTextCtrl* txtName = nullptr;
	wxTextCtrl* txtAmount = nullptr;
	wxTextCtrl* txtValue = nullptr;
	wxListBox* list = nullptr;
	wxStaticText* txtSum = nullptr;
	wxStaticText* headerDesc = nullptr;

	wxBoxSizer* sizer = nullptr;
	wxFlexGridSizer* rowSizer = nullptr;
	wxFlexGridSizer* rowSizerBot = nullptr;
	wxFlexGridSizer* rowSizerList = nullptr;

	void AddCost(wxCommandEvent& evt);
	unsigned int ResetCostsSum();

	void OnCostSelect(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);

	wxDECLARE_EVENT_TABLE();

public:
	BudgetWindowCosts(wxMDIParentFrame* parent, wxString bMonthId);
	~BudgetWindowCosts();
};

