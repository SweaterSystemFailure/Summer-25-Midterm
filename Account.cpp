#include "Account.h"
#include "BankFunds.h"
#include "Validators.h"
#include "Storage.h"
#include <iomanip>
#include <ctime>
#include <sstream>

namespace bankSimulation {
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

    void Account::setHolderPassword(const std::string& entry) {
        holderPassword = entry;
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
    void Account::withdrawal(BankFunds& bank, Storage& storage) {
        double amount = numericValidator("Enter withdrawal amount: ", 0.01, 5000.00);

        if (amount > balance) {
            std::cout << "Insufficient funds. ";
            printAccountBalance();
            return;
        }

        balance -= amount;
        logTransaction("Withdrawal", amount, balance, storage);

        // Update withdrawal total
        double updatedWithdrawals = bank.getTotalWithdrawals() + amount;
        bank.setTotalWithdrawals(updatedWithdrawals);

        // Update total holdings
        double updatedHoldings = bank.getTotalHoldings() - amount;
        bank.setTotalHoldings(updatedHoldings);

        std::cout << "Withdrawal successful. ";
        printAccountBalance();
    }


    void Account::deposit(BankFunds& bank, Storage& storage) {
        double amount = numericValidator("Enter deposit amount: ", 0.01, 5000.00);

        balance += amount;
        logTransaction("Deposit", amount, balance, storage);

        // Update deposit total
        double updatedDeposits = bank.getTotalDeposits() + amount;
        bank.setTotalDeposits(updatedDeposits);

        // Update total holdings
        double updatedHoldings = bank.getTotalHoldings() + amount;
        bank.setTotalHoldings(updatedHoldings);

        std::cout << "Deposit successful. ";
        printAccountBalance();
    }



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

        // Save updated data
        storage.saveAccount();
        storage.saveBank();
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

    // Print Functions
    void Account::printAccountBalance() const {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Current Balance: $" << balance << std::endl;
    }

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
