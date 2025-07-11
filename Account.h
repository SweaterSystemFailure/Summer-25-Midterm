#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace bankSimulation {

	class Storage;
	class BankFunds;

	/**
	 * @class Account
	 * @brief Represents a user’s bank account, including personal details, balance, and transaction history.
	 *
	 * This class manages individual account information such as name, password, balance, and a fixed-size
	 * transaction log. It also supports deposit and withdrawal operations that update the bank’s central funds.
	 */
	class Account {
	private:
		// Biographical Variables
		std::string holderFistName;              ///< First name of the account holder
		std::string holderLastName;              ///< Last name of the account holder
		unsigned holderAccountNumber;            ///< Unique account number
		std::string holderPassword;              ///< Password for authentication
		double balance;                          ///< Current account balance

		/**
		 * @struct Transaction
		 * @brief Stores details of a single transaction.
		 *
		 * Contains the type of transaction, amount, resulting balance, and timestamp.
		 */
		struct Transaction {
			std::string type;
			double amount;
			double resultingBalance;
			std::string timestamp;
		};

		// Transaction History
		static const int maxTransactions = 1'000;           ///< Maximum number of stored transactions
		Transaction transactionHistory[maxTransactions];    ///< Fixed-size array of past transactions
		int transactionCount = 0;                           ///< Number of transactions recorded

	public:
		// Mutators

		/**
		 * @brief Sets the first name of the account holder.
		 * @param entry New first name.
		 */
		void setHolderFirstName(const std::string& entry);

		/**
		 * @brief Sets the last name of the account holder.
		 * @param entry New last name.
		 */
		void setHolderLastName(const std::string& entry);

		/**
		 * @brief Sets the unique account number.
		 * @param entry Account number to assign.
		 */
		void setHolderAccountNumber(const unsigned& entry);

		/**
		 * @brief Sets the account password.
		 * @param entry Password to assign.
		 */
		void setHolderPassword(const std::string& entry);

		/**
		 * @brief Sets the account balance.
		 * @param entry New balance value.
		 */
		void setBalance(const double& entry);

		// Accessors

		/**
		 * @brief Gets the first name of the account holder.
		 * @return First name string.
		 */
		std::string getHolderFirstName() const;

		/**
		 * @brief Gets the last name of the account holder.
		 * @return Last name string.
		 */
		std::string getHolderLastName() const;

		/**
		 * @brief Gets the unique account number.
		 * @return Unsigned account number.
		 */
		unsigned getHolderAccountNumber() const;

		/**
		 * @brief Gets the account password.
		 * @return Password string.
		 */
		std::string getHolderPassword() const;

		/**
		 * @brief Gets the current account balance.
		 * @return Balance as a double.
		 */
		double getBalance() const;

		// Transaction Functions

		/**
		 * @brief Processes a deposit operation.
		 *
		 * Updates the account balance, logs the transaction, and updates bank-wide totals.
		 *
		 * @param bank Reference to the central BankFunds object.
		 * @param storage Reference to the data storage object for persistence.
		 */
		void deposit(BankFunds& bank, Storage& storage);

		/**
		 * @brief Processes a withdrawal operation.
		 *
		 * Deducts funds from the account balance (if sufficient), logs the transaction,
		 * and updates bank-wide totals.
		 *
		 * @param bank Reference to the central BankFunds object.
		 * @param storage Reference to the data storage object for persistence.
		 */
		void withdrawal(BankFunds& bank, Storage& storage);

		/**
		 * @brief Logs a financial transaction in the account’s transaction history.
		 *
		 * Records the type, amount, resulting balance, and timestamp.
		 *
		 * @param type Type of transaction ("Deposit" or "Withdrawal").
		 * @param amount Amount of the transaction.
		 * @param resultingBalance Balance after the transaction.
		 * @param storage Reference to the storage system for persistence.
		 */
		void logTransaction(const std::string& type, double amount, double resultingBalance, Storage& storage);

		// Serialization

		/**
		 * @brief Writes the account data to a binary output stream.
		 * @param out Output stream to serialize to.
		 */
		void serialize(std::ostream& out) const;

		/**
		 * @brief Reads and reconstructs account data from a binary input stream.
		 * @param in Input stream to deserialize from.
		 */
		void deserialize(std::istream& in);

		// Print Functions

		/**
		 * @brief Displays the current balance of the account.
		 */
		void printAccountBalance() const;

		/**
		 * @brief Displays the account’s transaction history.
		 */
		void printAccountHistory() const;
	};

}