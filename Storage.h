#pragma once
#include <vector>
#include "Account.h"
#include "BankFunds.h"

namespace bankSimulation {
	/**
	 * @class Storage
	 * @brief Manages storage, retrieval, and persistence of Account and BankFunds data.
	 *
	 * Provides functions for creating accounts, searching accounts, and saving/loading
	 * accounts and bank funds from binary files.
	 */
	class Storage {
	private:
		/// Container holding all account objects in memory
		std::vector<Account> accounts;

		/// Container holding bank funds data in memory
		std::vector<BankFunds> funds;

		/// Last assigned unique account number (auto-incrementing)
		unsigned lastAccountNumber = 10'000;

	public:
		/**
		 * @brief Accesses the accounts container.
		 * @return Reference to vector of Account objects.
		 */
		std::vector<Account>& getAccounts();

		/**
		 * @brief Accesses the funds container.
		 * @return Reference to vector of BankFunds objects.
		 */
		std::vector<BankFunds>& getFunds();

		/**
		 * @brief Creates and adds a new account after user input and validation.
		 *
		 * Collects user info, validates password strength, assigns unique account number,
		 * initializes balance to zero, then stores the new account.
		 */
		void newAccount();

		/**
		 * @brief Searches accounts by account number or last name and prints results.
		 *
		 * Prompts user to select search criteria, performs search, and displays
		 * account balance and transaction history if a match is found.
		 */
		void searchAccounts() const;

		/**
		 * @brief Saves all account data to a binary file ("accounts.dat").
		 *
		 * Writes the number of accounts, each serialized Account object,
		 * and the last used account number for continuity.
		 */
		void saveAccount();

		/**
		 * @brief Loads account data from a binary file ("accounts.dat").
		 *
		 * Creates the file with defaults if missing, then reads account count,
		 * deserializes accounts, and restores last assigned account number.
		 */
		void loadAccount();

		/**
		 * @brief Saves all bank fund records to a binary file ("funds.dat").
		 */
		void saveBank();

		/**
		 * @brief Loads bank fund records from a binary file ("funds.dat").
		 *
		 * If file is missing or invalid, creates a new default funds file.
		 */
		void loadBank();
	};
}