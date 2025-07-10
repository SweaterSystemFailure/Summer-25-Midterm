#include "containersAndFunctions.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <cctype>

namespace bankSimulation {
   // ====================
   // Storage Class
   // ====================

    //Account Functions
    void Storage::newAccount() {
        Account account;
        std::cout << "Welcome to your new account. Let's start by getting some basic information about you." << std::endl;
        do {
            account.setHolderFirstName(stringValidator("Please enter your first name: "));
            account.setHolderLastName(stringValidator("Please enter your last name: "));
            account.setHolderPassword(stringValidator("Please enter the password for your account: "));
        } while (!userCheck("Does this look right to you [Y / N]: ", 
            "Your account is now active!", 
            "That's okay. Let's try again."));
        account.setBalance(0.0);
        accounts.push_back(account);
        mainMenu();
    }

    void Storage::searchAccounts() const{
        if (accounts.empty()) {
            std::cout << "No accounts found." << std::endl;
            return;
        }

        std::cout << "Search by:" << std::endl;
        std::cout << "1. Account Number" << std::endl;
        std::cout << "2. Last Name" << std::endl;
        char choice = bankSimulation::charValidator("Select 1 or 2: ", { '1', '2' });

        bool found = false;

        switch (choice) {
        case '1': {
            unsigned searchNumber = bankSimulation::numericValidator("Enter the account number: ", 1, 999999999);

            for (const auto& acc : accounts) {
                if (acc.getHolderAccountNumber() == searchNumber) {
                    std::cout << std::endl << "Account found: " << std::endl;
                    acc.printAccountBalance();
                    acc.printAccountHistory();
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cout << "No account found with that number.\n";
            }
            break;
        }

        case '2': {
            std::string searchLastName = bankSimulation::stringValidator("Enter the account holder's last name: ");

            for (const auto& acc : accounts) {
                if (acc.getHolderLastName() == searchLastName) {
                    std::cout << std::endl << "Account found: " << std::endl;
                    acc.printAccountBalance();
                    acc.printAccountHistory();
                    found = true;
                }
            }

            if (!found) {
                std::cout << "No accounts found with that last name." << std::endl;
            }
            break;
        }

        default:
            std::cout << "Invalid option." << std::endl;
            break;
        }
    }
    

    //Save/Load Functions
    void Storage::saveAccount() {
        std::ofstream out("accounts.dat", std::ios::binary);
        if (!out) {
            std::cerr << "Error opening accounts.dat for writing." << std::endl;
            return;
        }

        size_t count = accounts.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        for (const auto& acc : accounts) {
            acc.serialize(out);
        }

        out.close();
    }

    void Storage::loadAccount() {
        std::ifstream in("accounts.dat", std::ios::binary);
        if (!in) {
            std::cerr << "Could not open accounts.dat for reading." << std::endl;
            return;
        }

        size_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        accounts.clear();

        for (size_t i = 0; i < count; ++i) {
            Account temp;
            temp.deserialize(in);
            accounts.push_back(temp);
        }

        in.close();
    }

    void Storage::saveBank() {
        std::ofstream out("funds.dat", std::ios::binary);
        if (!out) {
            std::cerr << "Error opening funds.dat for writing." << std::endl;
            return;
        }

        size_t count = funds.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));
        for (const auto& fund : funds) {
            fund.serialize(out);
        }

        out.close();
    }

    void Storage::loadBank() {
        std::ifstream in("funds.dat", std::ios::binary);
        if (!in) {
            std::cerr << "Could not open funds.dat for reading." << std::endl;
            return;
        }

        size_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));
        funds.clear();

        for (size_t i = 0; i < count; ++i) {
            BankFunds temp;
            temp.deserialize(in);
            funds.push_back(temp);
        }

        in.close();
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

    //Serialization Functions
    void Account::serialize(std::ostream& out) const {
        size_t len;

        // First Name
        len = holderFistName.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(holderFistName.c_str(), len);

        // Last Name
        len = holderLastName.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(holderLastName.c_str(), len);

        // Account number, password, balance
        out.write(reinterpret_cast<const char*>(&holderAccountNumber), sizeof(holderAccountNumber));

        len = holderPassword.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(holderPassword.c_str(), len);

        out.write(reinterpret_cast<const char*>(&balance), sizeof(balance));

        // History size
        size_t mapSize = accountHistory.size();
        out.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

        for (const auto& [key, value] : accountHistory) {
            len = key.size();
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(key.c_str(), len);
            out.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
    }

    void Account::deserialize(std::istream& in) {
        size_t len;

        // First Name
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        holderFistName.resize(len);
        in.read(&holderFistName[0], len);

        // Last Name
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        holderLastName.resize(len);
        in.read(&holderLastName[0], len);

        in.read(reinterpret_cast<char*>(&holderAccountNumber), sizeof(holderAccountNumber));

        // Password
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        holderPassword.resize(len);
        in.read(&holderPassword[0], len);

        in.read(reinterpret_cast<char*>(&balance), sizeof(balance));

        // History
        size_t mapSize;
        in.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));
        accountHistory.clear();
        for (size_t i = 0; i < mapSize; ++i) {
            std::string key;
            float value;

            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            key.resize(len);
            in.read(&key[0], len);
            in.read(reinterpret_cast<char*>(&value), sizeof(value));
            accountHistory[key] = value;
        }
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

    //Serialization
    void BankFunds::serialize(std::ostream& out) const {
        // Write startingFunds
        out.write(reinterpret_cast<const char*>(&startingFunds), sizeof(startingFunds));

        // Write password
        size_t passwordLength = password.size();
        out.write(reinterpret_cast<const char*>(&passwordLength), sizeof(passwordLength));
        out.write(password.c_str(), passwordLength);

        // Write totals
        out.write(reinterpret_cast<const char*>(&totalHoldings), sizeof(totalHoldings));
        out.write(reinterpret_cast<const char*>(&totalDeposits), sizeof(totalDeposits));
        out.write(reinterpret_cast<const char*>(&totalWithdrawals), sizeof(totalWithdrawals));
    }

    void BankFunds::deserialize(std::istream& in) {
        // Read startingFunds
        in.read(reinterpret_cast<char*>(&startingFunds), sizeof(startingFunds));

        // Read password
        size_t passwordLength;
        in.read(reinterpret_cast<char*>(&passwordLength), sizeof(passwordLength));
        password.resize(passwordLength);
        in.read(&password[0], passwordLength);

        // Read totals
        in.read(reinterpret_cast<char*>(&totalHoldings), sizeof(totalHoldings));
        in.read(reinterpret_cast<char*>(&totalDeposits), sizeof(totalDeposits));
        in.read(reinterpret_cast<char*>(&totalWithdrawals), sizeof(totalWithdrawals));
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