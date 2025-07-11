#include "containersAndFunctions.h"
#include "menus.h"
#include <iostream>

namespace bankSimulation {

	void userMenu(Account& account, BankFunds& bank) {
		bool exit = false;
		while (!exit) {
			std::cout << std::endl << "Welcome to your account!" << std::endl;
			std::cout << "1. Deposit funds" << std::endl;
			std::cout << "2. Withdraw funds" << std::endl;
			std::cout << "3. Check account balance" << std::endl;
			std::cout << "4. View transaction history" << std::endl;
			std::cout << "5. Exit" << std::endl;

			switch (numericValidator("Your selection: ", 1, 5)) {
			case 1:
				account.deposit(bank);
				break;
			case 2:
				account.withdrawal(bank);
				break;
			case 3:
				account.printAccountBalance();
				break;
			case 4:
				account.printAccountHistory();
				break;
			case 5:
				exit = true;
				break;
			default:
				std::cout << "Invalid selection." << std::endl;
			}
		}
	}

	void employeeMenu(Storage& storage) {
		bool exit = false;
		while (!exit) {
			std::cout << std::endl <<  "Employee Menu" << std::endl;
			std::cout << "1. Print bank report" << std::endl;
			std::cout << "2. Search accounts" << std::endl;
			std::cout << "3. Exit" << std::endl;

			switch (numericValidator("Your selection: ", 1, 3)) {
			case 1:
				if (!storage.getFunds().empty())
					storage.getFunds()[0].printBankReport();
				else
					std::cout << "Bank fund data unavailable." << std::endl;
				break;
			case 2:
				storage.searchAccounts();
				break;
			case 3:
				exit = true;
				break;
			default:
				std::cout << "Invalid selection." << std::endl;
			}
		}
	}

	void branchMenu(Storage& storage) {
		bool exit = false;

		while (!exit) {
			std::cout << std::endl << "Welcome to Laney Bank!" << std::endl;
			std::cout << "1. Create a new account" << std::endl;
			std::cout << "2. Log in to an existing account" << std::endl;
			std::cout << "3. Log in as an employee" << std::endl;
			std::cout << "4. Exit" << std::endl;

			switch (numericValidator("Your selection: ", 1, 4)) {
			case 1:
				storage.newAccount();
				break;
			case 2: {
				if (logIn(storage, false)) {
					std::string lastName = stringValidator("Re-enter your last name to access your account: ");
					for (auto& acc : storage.getAccounts()) {
						if (acc.getHolderLastName() == lastName) {
							userMenu(acc, storage.getFunds()[0]);
							break;
						}
					}
				}
				break;
			}
			case 3:
				if (logIn(storage, true)) {
					employeeMenu(storage);
				}
				break;
			case 4:
				exit = true;
				break;
			default:
				std::cout << "Invalid selection." << std::endl;
			}
		}
	}
}