#pragma once
#include "wx/wx.h"
#include "Project.h"
#include "Cost.h"

class MonthBudget
{
public:
	unsigned int month;
	unsigned int year;
	wxString id;

	std::vector<Project> projects;
	std::vector<Cost> costs;

public:
	MonthBudget(unsigned int monthIn, unsigned int yearIn);
	~MonthBudget();

	wxString GetInfo();

	Cost* GetCost(wxString name);
	bool AddCost(Cost cost);

	Project* GetProject(wxString pName);
	bool AddProject(Project project);

	unsigned int GetProjectsAmount();
	unsigned int GetProjectsValue();
	unsigned int GetProjectsCost();
	unsigned int GetCostsValue();
	int GetRevenue();
};

