#include "MonthBudget.h"

MonthBudget::MonthBudget(unsigned int monthIn, unsigned int yearIn)
{
	month = monthIn;
	year = yearIn;
	id = std::to_string(yearIn) + "-" + std::to_string(monthIn);
}

MonthBudget::~MonthBudget()
{
}

unsigned int MonthBudget::GetProjectsAmount()
{
	return projects.size();
}

Project* MonthBudget::GetProject(wxString pName)
{
	for (Project& project : projects)
		if (project.name == pName)
			return &project;
	
	return nullptr;
}

wxString MonthBudget::GetInfo()
{
	wxString info = "";
	int income = GetProjectsValue(), projectCosts = GetProjectsCost(), costs = GetCostsValue(), revenue = income - projectCosts - costs;

	info += "Wartoœæ projektów: " + std::to_string(income) + " z³ netto\n";
	info += "Koszt realizacji projektów: " + std::to_string(projectCosts) + " z³ brutto\n";
	info += "Koszty operacyjne: " + std::to_string(costs) + " z³ brutto\n";
	info += "Dochód: " + std::to_string(revenue) + " z³ netto\n";
	info += "Iloœæ projektów: " + std::to_string(GetProjectsAmount()) + "\n";

	return info;
}

Cost* MonthBudget::GetCost(wxString name)
{
	for (Cost& cost : costs)
		if (cost.name == name)
			return &cost;

	return nullptr;
}

bool MonthBudget::AddCost(Cost cost)
{
	costs.push_back(cost);
	return true;
}

unsigned int MonthBudget::GetCostsValue()
{
	unsigned int sum = 0;

	for (Cost cost : costs)
		sum += cost.wholeValue;

	return sum;
}

int MonthBudget::GetRevenue()
{
	return GetProjectsValue() - GetProjectsCost() - GetCostsValue();
}

bool MonthBudget::AddProject(Project project)
{
	projects.push_back(project);
	return true;
}

unsigned int MonthBudget::GetProjectsValue()
{
	unsigned int sum = 0;

	for (Project project : projects)
		sum += project.value;
	
	return sum;
}

unsigned int MonthBudget::GetProjectsCost()
{
	unsigned int sum = 0;

	for (Project p : projects)
		for (Contract c : p.contracts)
			sum += c.taxedValue;

	return sum;
}
