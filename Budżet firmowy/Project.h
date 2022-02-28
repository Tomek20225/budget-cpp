#pragma once
#include "wx/wx.h"
#include "Contract.h"

class Project
{
public:
	wxString name;
	unsigned int value;

	std::vector<Contract> contracts;

public:
	Project(wxString nameIn, unsigned int valueIn);
	~Project();

	wxString GetInfo();

	unsigned int GetContractsCosts();
	unsigned int GetContractsAmount();

	Contract* GetContract(wxString name);
	bool AddContract(Contract contract);
};

