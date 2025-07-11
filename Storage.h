#pragma once
#include <vector>
#include "Account.h"
#include "BankFunds.h"

namespace bankSimulation {
	class Storage {
	private:
		std::vector<Account> accounts;
		std::vector<BankFunds> funds;
		unsigned lastAccountNumber = 10'000;
	public:
		//Accessors
		std::vector<Account>& getAccounts();
		std::vector<BankFunds>& getFunds();

		//Account Specific Functions
		void newAccount();
		void searchAccounts() const;
		void saveAccount();
		void loadAccount();

		//Bank Save/Load Functions
		void saveBank();
		void loadBank();
	};
}

