#include "Account.h"
#include "BankFunds.h"
#include "Validators.h"
#include "Storage.h"
#include <iomanip>
#include <ctime>
#include <sstream>

namespace bankSimulation {

    // === Mutators ===

    /**
     * @brief Sets the account holder's first name.
     * @param entry New first name to assign.
     */
    void Account::setHolderFirstName(const std::string& entry) {
        holderFistName = entry;
    }

    /**
     * @brief Sets the account holder's last name.
     * @param entry New last name to assign.
     */
    void Account::setHolderLastName(const std::string& entry) {
        holderLastName = entry;
    }

    /**
     * @brief Assigns a unique account number to this account.
     * @param entry Account number to set.
     */
    void Account::setHolderAccountNumber(const unsigned& entry) {
        holderAccountNumber = entry;
    }

    /**
     * @brief Sets the password for this account.
     * @param entry Password string.
     */
    void Account::setHolderPassword(const std::string& entry) {
        holderPassword = entry;
    }

    /**
     * @brief Sets the current balance for this account.
     * @param entry Balance amount.
     */
    void Account::setBalance(const double& entry) {
        balance = entry;
    }

    // === Accessors ===

    /**
     * @brief Retrieves the first name of the account holder.
     * @return First name as a string.
     */
    std::string Account::getHolderFirstName() const {
        return holderFistName;
    }

    /**
     * @brief Retrieves the last name of the account holder.
     * @return Last name as a string.
     */
    std::string Account::getHolderLastName() const {
        return holderLastName;
    }

    /**
     * @brief Gets the unique account number.
     * @return Account number.
     */
    unsigned Account::getHolderAccountNumber() const {
        return holderAccountNumber;
    }

    /**
     * @brief Retrieves the account password.
     * @return Password string.
     */
    std::string Account::getHolderPassword() const {
        return holderPassword;
    }

    /**
     * @brief Gets the current account balance.
     * @return Balance as a double.
     */
    double Account::getBalance() const {
        return balance;
    }

    // === Transaction Functions ===

    /**
     * @brief Withdraws funds from the account and updates bank-wide totals.
     *
     * Prompts the user to enter a withdrawal amount within a valid range. Checks if
     * sufficient funds are available; if so, reduces the account balance, logs the transaction,
     * and updates the bank's total withdrawals and holdings accordingly.
     *
     * @param bank Reference to the BankFunds object to update global totals.
     * @param storage Reference to the Storage object used for persisting data.
     */
    void Account::withdrawal(BankFunds& bank, Storage& storage) {
        double amount = numericValidator("Enter withdrawal amount: ", 0.01, 5000.00);

        if (amount > balance) {
            std::cout << "Insufficient funds. ";
            printAccountBalance();
            return;
        }

        balance -= amount;
        logTransaction("Withdrawal", amount, balance, storage);

        double updatedWithdrawals = bank.getTotalWithdrawals() + amount;
        bank.setTotalWithdrawals(updatedWithdrawals);

        double updatedHoldings = bank.getTotalHoldings() - amount;
        bank.setTotalHoldings(updatedHoldings);

        std::cout << "Withdrawal successful. ";
        printAccountBalance();
    }

    /**
     * @brief Deposits funds into the account and updates bank-wide totals.
     *
     * Prompts the user to enter a deposit amount within a valid range. Increases the account balance,
     * logs the transaction, and updates the bank's total deposits and holdings accordingly.
     *
     * @param bank Reference to the BankFunds object to update global totals.
     * @param storage Reference to the Storage object used for persisting data.
     */
    void Account::deposit(BankFunds& bank, Storage& storage) {
        double amount = numericValidator("Enter deposit amount: ", 0.01, 5000.00);

        balance += amount;
        logTransaction("Deposit", amount, balance, storage);

        double updatedDeposits = bank.getTotalDeposits() + amount;
        bank.setTotalDeposits(updatedDeposits);

        double updatedHoldings = bank.getTotalHoldings() + amount;
        bank.setTotalHoldings(updatedHoldings);

        std::cout << "Deposit successful. ";
        printAccountBalance();
    }

    /**
     * @brief Records a financial transaction in the account’s history.
     *
     * Maintains a fixed-size log of the most recent transactions, recording type,
     * amount, resulting balance, and a timestamp. If the log is full, it discards
     * the oldest entry to make room for new transactions.
     *
     * Saves the updated account and bank data after logging.
     *
     * @param type Type of transaction (e.g., "Deposit", "Withdrawal").
     * @param amount Amount of the transaction.
     * @param resultingBalance Account balance after the transaction.
     * @param storage Reference to the Storage object to save changes.
     */
    void Account::logTransaction(const std::string& type, double amount, double resultingBalance, Storage& storage) {
        if (transactionCount >= maxTransactions) {
            std::cout << "Transaction log full. Oldest entry will be overwritten." << std::endl;
            for (int i = 1; i < maxTransactions; ++i) {
                transactionHistory[i - 1] = transactionHistory[i];
            }
            transactionCount = maxTransactions - 1;
        }

        Transaction t;
        t.type = type;
        t.amount = amount;
        t.resultingBalance = resultingBalance;

        auto now = std::time(nullptr);
        std::tm timeInfo;
        localtime_s(&timeInfo, &now);

        std::stringstream ss;
        ss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S");
        t.timestamp = ss.str();

        transactionHistory[transactionCount++] = t;

        storage.saveAccount();
        storage.saveBank();
    }

    // === Serialization ===

    /**
     * @brief Serializes account data and writes it to a binary output stream.
     *
     * Stores personal info, balance, and transaction history in binary format
     * for persistent storage.
     *
     * @param out Output stream to write the serialized data.
     */
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

        // Write transaction count
        out.write(reinterpret_cast<const char*>(&transactionCount), sizeof(transactionCount));

        // Write each transaction
        for (int i = 0; i < transactionCount; ++i) {
            const Transaction& t = transactionHistory[i];

            // Type
            len = t.type.size();
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(t.type.c_str(), len);

            // Amount and postBalance
            out.write(reinterpret_cast<const char*>(&t.amount), sizeof(t.amount));
            out.write(reinterpret_cast<const char*>(&t.resultingBalance), sizeof(t.resultingBalance));

            // Timestamp
            len = t.timestamp.size();
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(t.timestamp.c_str(), len);
        }
    }

    /**
     * @brief Deserializes account data by reading from a binary input stream.
     *
     * Restores personal info, account balance, and transaction history from persisted storage.
     *
     * @param in Input stream to read the serialized data.
     */
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

        // Account number
        in.read(reinterpret_cast<char*>(&holderAccountNumber), sizeof(holderAccountNumber));

        // Password
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        holderPassword.resize(len);
        in.read(&holderPassword[0], len);

        // Balance
        in.read(reinterpret_cast<char*>(&balance), sizeof(balance));

        // Read transaction count
        in.read(reinterpret_cast<char*>(&transactionCount), sizeof(transactionCount));

        // Read each transaction
        for (int i = 0; i < transactionCount; ++i) {
            Transaction& t = transactionHistory[i];

            // Type
            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            t.type.resize(len);
            in.read(&t.type[0], len);

            // Amount and postBalance
            in.read(reinterpret_cast<char*>(&t.amount), sizeof(t.amount));
            in.read(reinterpret_cast<char*>(&t.resultingBalance), sizeof(t.resultingBalance));

            // Timestamp
            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            t.timestamp.resize(len);
            in.read(&t.timestamp[0], len);
        }
    }

    // === Display Functions ===

    /**
     * @brief Prints the current account balance formatted to two decimal places.
     */
    void Account::printAccountBalance() const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Current Balance: $" << balance << std::endl;
    }

    /**
     * @brief Prints a detailed transaction history including timestamps and balances.
     */
    void Account::printAccountHistory() const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Account #" << this->getHolderAccountNumber() << std::endl;
        std::cout << "Transaction History:" << std::endl;
        for (int i = 0; i < transactionCount; ++i) {
            const auto& t = transactionHistory[i];
            std::cout << t.timestamp << " | "
                << t.type << " | "
                << "$" << t.amount << " | "
                << "Balance after: $" << t.resultingBalance << std::endl;
        }
    }
}