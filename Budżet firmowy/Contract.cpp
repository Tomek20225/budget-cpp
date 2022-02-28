#include "Contract.h"

Contract::Contract(wxString cType, wxString cContractor, unsigned int cValue, unsigned int cTaxedValue, wxString cExemption, unsigned int cNum, wxString cName)
{
	name = cName;
	type = cType;
	contractor = cContractor;
	num = cNum;
	value = cValue;
	taxedValue = cTaxedValue;
	taxRate = ceil((((double)taxedValue / value) - 1) * 100);
	exemption = cExemption;
}

Contract::~Contract()
{
}

wxString Contract::GetInfo()
{
	wxString info = "";

	info += "Wynagrodzenie netto: " + std::to_string(value) + " z³\n";
	info += "Wynagrodzenie brutto: " + std::to_string(taxedValue) + " z³\n";
	info += "Rodzaj umowy: " + type + "\n";
	info += "Ulga podatkowa: " + exemption + "\n";
	info += "Stawka podatku: " + std::to_string(taxRate) + "%\n";

	return info;
}