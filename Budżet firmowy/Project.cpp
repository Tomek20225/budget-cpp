#include "Project.h"

Project::Project(wxString nameIn, unsigned int valueIn)
{
	name = nameIn;
	value = valueIn;
}

Project::~Project()
{
}

unsigned int Project::GetContractsCosts()
{
	unsigned int sum = 0;

	for (Contract contract : contracts)
		sum += contract.taxedValue;

	return sum;
}

unsigned int Project::GetContractsAmount()
{
	return contracts.size();
}

wxString Project::GetInfo()
{
	wxString info = "";
	unsigned int contractCosts = GetContractsCosts();
	int revenue = value - contractCosts;

	info += "Warto�� netto projektu: " + std::to_string(value) + " z�\n";
	info += "Koszt wykonania projektu: " + std::to_string(contractCosts) + " z�\n";
	info += "Doch�d z projektu: " + std::to_string(revenue) + " z�\n";
	info += "Ilo�� um�w: " + std::to_string(GetContractsAmount());

	return info;
}

Contract* Project::GetContract(wxString name)
{
	for (Contract& contract : contracts)
		if (contract.name == name)
			return &contract;

	return nullptr;
}

bool Project::AddContract(Contract contract)
{
	contracts.push_back(contract);
	return true;
}
