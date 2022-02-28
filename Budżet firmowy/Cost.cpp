#include "Cost.h"

Cost::Cost(wxString cName, unsigned int cAmount, unsigned int cValue)
{
	name = cName;
	amount = cAmount;
	value = cValue;
	wholeValue = value * amount;
}

Cost::~Cost()
{
}

wxString Cost::GetInfo()
{
	wxString info = "";

	info += "Wartoœæ: " + std::to_string(value) + " z³ brutto (szt.)\n";
	info += "Iloœæ: " + std::to_string(amount) + " szt.\n";
	info += "Ca³kowita wartoœæ: " + std::to_string(wholeValue) + " z³ brutto\n";

	return info;
}