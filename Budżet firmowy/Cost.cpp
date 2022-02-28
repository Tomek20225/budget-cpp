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

	info += "Warto��: " + std::to_string(value) + " z� brutto (szt.)\n";
	info += "Ilo��: " + std::to_string(amount) + " szt.\n";
	info += "Ca�kowita warto��: " + std::to_string(wholeValue) + " z� brutto\n";

	return info;
}