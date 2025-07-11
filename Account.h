#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace bankSimulation {
	class Storage;
	class BankFunds;
	
	class Account {
	private:
		//Biographical Variables
		std::string holderFistName;
		std::string holderLastName;
		unsigned holderAccountNumber;
		std::string holderPassword;
		double balance;

		//Transaction Element Storage
		struct Transaction {
			std::string type;
			double amount;
			double resultingBalance;
			std::string timestamp;
		};

		//Transaction Histoy
		static const int maxTransactions = 1'000;

		Transaction transactionHistory[maxTransactions];
		int transactionCount = 0;;

	public:
		//Mutators
		void setHolderFirstName(const std::string& entry);
		void setHolderLastName(const std::string& entry);
		void setHolderAccountNumber(const unsigned& entry);
		void setHolderPassword(const std::string& entry);
		void setBalance(const double& entry);

		//Accessors
		std::string getHolderFirstName() const;
		std::string getHolderLastName() const;
		unsigned getHolderAccountNumber() const;
		std::string getHolderPassword() const;
		double getBalance() const;

		//Transaction Functions
		void deposit(BankFunds& bank, Storage& storage);
		void withdrawal(BankFunds& bank, Storage& storage);
		void logTransaction(const std::string& type, double amount, double resultingBalance, Storage& storage);

		//Serialization
		void serialize(std::ostream& out) const;
		void deserialize(std::istream& in);

		//Print Functions
		void printAccountBalance() const;
		void printAccountHistory() const;
	};
}
