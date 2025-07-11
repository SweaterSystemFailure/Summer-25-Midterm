#pragma once
#include "containersAndFunctions.h" 

namespace bankSimulation {
	void userMenu(Account& account, BankFunds& bank);
	void employeeMenu(Storage& storage);
	void branchMenu(Storage& storage);
}