#include "containersAndFunctions.h"
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <cctype>

namespace bankSimulation {
    // ====================
   // Storage Class
   // ====================

    void Storage::newAccount() {
        Account account;
        std::cout << "Welcome to your new account. Let's start by getting some basic information about you." << std::endl;
        do {
            account.setHolderFirstName(stringValidator("Please enter your first name: "));
            account.setHolderLastName(stringValidator("Please enter your last name: "));
            account.setHolderPassword(stringValidator("Please enter the password for your account: "));
        } while (!userCheck("Does this look right to you [Y / N]: ", 
            "Your account is now active!", 
            "That's okay. Let's try that again."));
        account.setBalance(0.0);
        accounts.push_back(account);
        mainMenu();
    }

    void Storage::saveAccount() {
        // TODO: implement save logic
    }

    void Storage::loadAccount() {
        // TODO: implement load logic
    }

    // ====================
    // Account Class
    // ====================

    // Mutators
    void Account::setHolderFirstName(const std::string& entry) {
        holderFistName = entry;
    }

    void Account::setHolderLastName(const std::string& entry) {
        holderLastName = entry;
    }

    void Account::setHolderAccountNumber(const unsigned& entry) {
        holderAccountNumber = entry;
    }

    void Account::setHolderPassword(const unsigned& entry) {
        holderPassword = std::to_string(entry);
    }

    void Account::setBalance(const double& entry) {
        balance = entry;
    }

    // Accessors
    std::string Account::getHolderFirstName() const {
        return holderFistName;
    }

    std::string Account::getHolderLastName() const {
        return holderLastName;
    }

    unsigned Account::getHolderAccountNumber() const {
        return holderAccountNumber;
    }

    std::string Account::getHolderPassword() const {
        return holderPassword;
    }

    double Account::getBalance() const {
        return balance;
    }

    // Transaction Functions
    void Account::withdrawal(bankSimulation::BankFunds& bank) {
        float withdawal = bankSimulation::numericValidator("Please enter the amout of your withdrawal: ", 0.01, 5'000.00);
        if (withdawal > balance) {
            std::cout << "Insufficient funds! ";
            printAccountBalance();
        }
        else {
            balance -= withdawal;
            accountHistory["Withdrawal"] -= withdawal;
          
            double updatedTotal = bank.getTotalWithdrawals() - withdawal;
            bank.setTotalWithdrawals(updatedTotal);
           
            std::cout << "Your withdrawal has been successfully prossessed! ";
            printAccountBalance();
        }
    }

    void Account::deposit(bankSimulation::BankFunds& bank) {
        float deposit = bankSimulation::numericValidator("Please enter the amount of your deposit: ", 0.01, 5'000.00);
        balance += deposit;
        accountHistory["Deposit"] += deposit;
        
        double updatedTotal = bank.getTotalDeposits() + deposit;
        bank.setTotalDeposits(updatedTotal);

        
        std::cout << "Your deposit has been successfully possessed! ";
        printAccountBalance();
    }

    // Print Functions
    void Account::printAccountBalance() const {
        std::cout << "Current Balance: $" << balance << std::endl;
    }

    void Account::printAccountHistory() const {
     //TODO: Figure out how to save/track account interactions better. Map seems like the wrong container. Maybe this is where the mandatory array goes?
    }

    // ====================
    // BankFunds Class
    // ====================

    // Mutators
    double BankFunds::setTotalHoldings(const double& entry) {
        totalHoldings = entry;
        return totalHoldings;
    }

    double BankFunds::setTotalDeposits(const double& entry) {
        totalDeposits = entry;
        return totalDeposits;
    }

    double BankFunds::setTotalWithdrawals(const double& entry) {
        totalWithdrawals = entry;
        return totalWithdrawals;
    }

    // Accessors
    double BankFunds::getTotalHoldings() const {
        return totalHoldings;
    }

    double BankFunds::getTotalDeposits() const {
        return totalDeposits;
    }

    double BankFunds::getTotalWithdrawals() const {
        return totalWithdrawals;
    }

    // Print Function
    void BankFunds::printBankReport() const {
        std::cout << "--- Bank Report ---" << std::endl;
        std::cout << "Starting Funds: $" << startingFunds << std::endl;
        std::cout << "Total Deposits: $" << totalDeposits << std::endl;
        std::cout << "Total Withdrawals: $" << totalWithdrawals << std::endl;
        std::cout << "Total Holdings: $" << totalHoldings << std::endl;
    }

    // ====================
    // Validators
    // ====================
    
	//User Input: String Validation
	std::string stringValidator(const std::string& prompt) {
		std::string userInput;
		while (true) {
			std::cout << prompt;
			std::getline(std::cin, userInput);
			if (userInput.empty()) {
				std::cout << "Invalid input. This field can't be empty. Try again: " << std::endl;
			}
			else {
				return userInput;
			}
		}
	}

	//User Input: Char Validation
	char charValidator(const std::string& prompt, const std::vector<char>& validOptions) {
		char input;
		while (true) {
			std::cout << prompt << std::endl;
			std::cin >> input;
			input = static_cast<char>(std::tolower(static_cast<unsigned char>(input)));
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			for (char option : validOptions) {
				if (static_cast<char>(std::tolower(static_cast<unsigned char>(option))) == input) {
					return input;
				}
			}

			std::cout << "Invalid input. Please enter one of the following: ";
			for (char option : validOptions) {
				std::cout << option << " ";
			}
			std::cout << std::endl;
		}
	}

	//User Input Check
	bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt) {
		char choice;
		while (true) {
			std::cout << prompt << std::endl;
			std::cin >> choice;
			choice = static_cast<char>(std::tolower(static_cast<unsigned char>(choice)));
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			if (choice == 'y') {
				std::cout << yesPrompt << std::endl << std::endl;
				return true;
			}
			else if (choice == 'n') {
				std::cout << noPrompt << std::endl << std::endl;
				return false;
			}
			else {
				std::cout << "Invalid input. Please enter y or n." << std::endl << std::endl;
			}
		}
	}
}