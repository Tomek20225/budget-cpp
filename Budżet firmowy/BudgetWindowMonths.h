#pragma once
#include "wx/wx.h"

class BudgetWindowMonths : public wxMDIChildFrame
{
public:
	wxMDIParentFrame* parentFrame = nullptr;
	
	wxBoxSizer* sizer = nullptr;
	wxFlexGridSizer* rowSizer = nullptr;
	wxFlexGridSizer* rowSizerList = nullptr;

	wxButton* btn = nullptr;
	wxButton* btnEdit = nullptr;
	wxTextCtrl* txt = nullptr;
	wxListBox* list = nullptr;

	wxStaticText* headerDesc = nullptr;

private:
	unsigned int lastMonth = -1;
	unsigned int lastYear = -1;

public:
	BudgetWindowMonths(wxMDIParentFrame* parent, wxString sName);
	~BudgetWindowMonths();

	void AddMonth(wxCommandEvent& evt);
	void EditMonth(wxCommandEvent& evt);

	void OnMonthSelect(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);

	void ResetList();

	wxDECLARE_EVENT_TABLE();
};

