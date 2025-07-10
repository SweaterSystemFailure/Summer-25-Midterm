#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>

namespace bankSimulation {
	// ====================
	// Storage Class
	// ====================

	class Storage {
	private:
		std::vector<Account> accounts;
		std::vector<BankFunds> funds;
	public:
		void newAccount();
		void saveAccount();
		void loadAccount();
	};
	
	// ====================
	// Account Class
	// ====================
	class Account {
	private:
		//Biographical Containers
		std::string holderFistName;
		std::string holderLastName;
		unsigned holderAccountNumber;
		std::string holderPassword;
		double balance;

		//Transaction Histoy
		std::map <std::string, float> accountHistory;

	public:
		//Mutators
		void setHolderFirstName(const std::string& entry);
		void setHolderLastName(const std::string& entry);
		void setHolderAccountNumber(const unsigned& entry);
		void setHolderPassword(const unsigned& entry);
		void setBalance(const double& entry);

		//Accessors
		std::string getHolderFirstName() const;
		std::string getHolderLastName() const;
		unsigned getHolderAccountNumber() const;
		std::string getHolderPassword() const;
		double getBalance() const;

		//Transaction Functions
		void withdrawal();
		void deposit();

		//Print Functions
		void printAccountBalance() const;
		void printAccountHistory() const;
	};

	// ====================
	// BankFunds Class
	// ====================
	class BankFunds {
	private:
		double startingFunds = 15'000'000;
		std::string password = "ShowMeTheMoney";
		double totalHoldings;
		double totalDeposits;
		double totalWithdrawals;
	public:
		//Mutators
		double setTotalHoldings(const double& entry);
		double setTotalDeposits(const double& entry);
		double setTotalWithdrawals(const double& entry);

		//Accessors
		double getTotalHoldings() const;
		double getTotalDeposits() const;
		double getTotalWithdrawals() const;

		//Print Function
		void printBankReport() const;
	};
	
	// ====================
	// Validators
	// ====================
	template <typename T>
	T numericValidator(const std::string& prompt, T min, T max) {
		T number;
		while (true) {
			std::cout << prompt << std::endl;
			std::cin >> number;

			if (std::cin.fail()) {
				std::cout << "Enter a valid number." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else if (number < min || number > max) {
				std::cout << "Number must be between " << min << " and " << max << "." << std::endl;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else {
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
		}
		return number;
	}

	std::string stringValidator(const std::string& prompt);
	char charValidator(const std::string& prompt, const std::vector<char>& validOptions);
	bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt);
}