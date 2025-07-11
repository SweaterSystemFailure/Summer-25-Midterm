#include "menus.h"
#include "Account.h"
#include "Storage.h"
#include "Validators.h"
#include <iostream>

namespace bankSimulation {

	/**
	 * @brief Displays the interactive user menu for account operations.
	 *
	 * Allows authenticated users to perform standard banking actions such as depositing,
	 * withdrawing, checking balances, and viewing transaction history. Changes are saved
	 * on exit.
	 *
	 * @param storage Reference to the Storage object managing persistent data.
	 * @param account Reference to the logged-in Account object.
	 * @param bank Reference to the centralized BankFunds object.
	 */
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

	/**
	 * @brief Displays the employee menu for administrative banking tasks.
	 *
	 * Enables access to bank-level data operations, including generating bank reports
	 * and searching user accounts. Saves data upon exiting the menu.
	 *
	 * @param storage Reference to the Storage object managing all accounts and funds.
	 */
	void employeeMenu(Storage& storage) {
		bool exit = false;
		while (!exit) {
			std::cout << std::endl << "Employee Menu" << std::endl;
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

	/**
	 * @brief Launches the top-level branch menu for customer and employee access.
	 *
	 * Serves as the main entry point of the application. Offers account creation,
	 * user login, employee login, and exit. On exit, all data is saved.
	 *
	 * @param storage Reference to the Storage object for managing persistent system data.
	 */
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