#include "menus.h"
#include "Account.h"
#include "Storage.h"
#include "Validators.h"
#include <iostream>

namespace bankSimulation {

	void userMenu(Storage& storage, Account& account, BankFunds& bank) {
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
				account.deposit(bank, storage);
				break;
			case 2:
				account.withdrawal(bank, storage);
				break;
			case 3:
				account.printAccountBalance();
				break;
			case 4:
				account.printAccountHistory();
				break;
			case 5:
				storage.saveAccount();
				storage.saveBank();
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
				storage.saveAccount();
				storage.saveBank();
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
				if (storage.getAccounts().empty()) {
					std::cout << "No accounts are currently stored. Please create a new account first.\n";
					break;
				}

				if (Account* acc = userLogIn(storage)) {
					if (!storage.getFunds().empty()) {
						userMenu(storage, *acc, storage.getFunds()[0]);
					}
					else {
						std::cout << "Bank funds are not initialized. Please contact an employee.\n";
					}
				}
				else {
					std::cout << "User login failed or cancelled.\n";
				}
				break;
			}

			case 3:
				if (employeeLogIn(storage)) {
					employeeMenu(storage);
				}
				else {
					std::cout << "Employee login failed.\n";
				}
				break;
			case 4:
				exit = true;
				storage.saveAccount();
				storage.saveBank();
				break;
			default:
				std::cout << "Invalid selection." << std::endl;
			}
		}
	}
}