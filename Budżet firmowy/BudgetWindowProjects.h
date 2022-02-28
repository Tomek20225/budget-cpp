#pragma once
#include "wx/wx.h"

class BudgetWindowProjects : public wxMDIChildFrame
{
public:
	wxString monthId;
	
	wxButton* btn = nullptr;
	wxButton* btnEdit = nullptr;
	wxTextCtrl* txtName = nullptr;
	wxTextCtrl* txtValue = nullptr;
	wxListBox* list = nullptr;
	wxStaticText* txtSum = nullptr;
	wxStaticText* txtCosts = nullptr;
	wxStaticText* txtFinal = nullptr;
	wxStaticText* headerDesc = nullptr;

	wxBoxSizer* sizer = nullptr;
	wxFlexGridSizer* rowSizer = nullptr;
	wxFlexGridSizer* rowSizerBot = nullptr;
	wxFlexGridSizer* rowSizerList = nullptr;
	

	void AddProject(wxCommandEvent& evt);
	void EditProject(wxCommandEvent& evt);
	void OnProjectSelect(wxCommandEvent& evt);

	void ResetSummary();

	void OnClose(wxCloseEvent& evt);

	wxDECLARE_EVENT_TABLE();

public:
	BudgetWindowProjects(wxMDIParentFrame* parent, wxString sName);
	~BudgetWindowProjects();
};

