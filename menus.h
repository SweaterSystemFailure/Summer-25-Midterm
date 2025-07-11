#pragma once
#include "Storage.h"
#include "BankFunds.h"
#include "Account.h"

namespace bankSimulation {
	void userMenu(Account& account, BankFunds& bank);
	void employeeMenu(Storage& storage);
	void branchMenu(Storage& storage);
}