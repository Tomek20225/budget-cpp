#pragma once
#include "wx/wx.h"
#include <iostream>
#include <cmath>

class Contract
{
public:
	wxString name;
	wxString contractor;
	wxString type;
	wxString exemption;
	unsigned int num;
	unsigned int value;
	unsigned int taxedValue;
	unsigned int taxRate;

public:
	Contract(wxString type, wxString contractor, unsigned int value, unsigned int taxedValue, wxString exemption, unsigned int num, wxString name);
	~Contract();

	wxString GetInfo();
};

