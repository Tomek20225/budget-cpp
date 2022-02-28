#pragma once
#include "wx/wx.h"
#include "BudgetMain.h"

class BudgetApp : public wxApp
{
public:
	BudgetApp();
	~BudgetApp();

private:
	BudgetMain* main = nullptr;

public:
	virtual bool OnInit();
};

