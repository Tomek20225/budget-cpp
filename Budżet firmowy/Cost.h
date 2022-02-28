#pragma once
#include "wx/wx.h"

class Cost
{
public:
	wxString name;
	unsigned int value;
	unsigned int amount;
	unsigned int wholeValue;

public:
	Cost(wxString name, unsigned int amount, unsigned int value);
	~Cost();

	wxString GetInfo();
};

