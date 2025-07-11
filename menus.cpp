#include "menus.h"
#include "containersAndFunctions.h"
#include <iostream>


namespace BankSimulation {
	void branchMenu(Storage& storage) {
		std::cout << "Welcome to Laney Bank!" << std::endl;
		std::cout << std::endl;
		std::cout << "Would you like to: " << std::endl;
		std::cout << "1. Create a new account" << std::endl;
		std::cout << "2. Log in to an existing account" << std::endl;
		std::cout << "3. Log in as an employee" << std::endl;
		std::cout << "4. Exit program" << std::endl;
		std::cout << std::endl;

		switch (numericValidator("Please enter your selection: ", 1, 4)) {
		case 1:
			storage.newAccount(Storage& storage);
			break;
		case 2:
			if logIn(Storange & storage, false) {
				userMenu(Storage & storage);
			}
			break;
		case 3:
			if logIn(Storage & storage, true) {
				employeeMenu(Storage & storage);
			}
			break;
		case 4:
			break;
		default:
			std::cout << "Critical Error! Program closing." << std::endl;
		}
	}

	void userMenu(Storage& storage) {
		//TODO customer menu
	}

	void employeeMenu(Storage& storage) {
		//TODO: employee menu
	}
}