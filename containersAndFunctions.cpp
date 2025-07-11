#include "containersAndFunctions.h"
#include "menus.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <cctype>
#include <ctime>
#include <sstream>

namespace bankSimulation {
    // ====================
    // Storage Class
    // ====================
    //Accessors
    const std::vector<Account>& Storage::getAccounts() const {
        return accounts;
    }

    std::vector<BankFunds>& Storage::getFunds() {
        return funds;
    }

    //Account Functions
    void Storage::newAccount() {
        Account account;
        std::cout << "Welcome to your new account. Let's start by getting some basic information about you." << std::endl;
        do {
            account.setHolderFirstName(stringValidator("Please enter your first name: "));
            account.setHolderLastName(stringValidator("Please enter your last name: "));
            std::string password;
            do {
                password = stringValidator("Please enter a strong password. Your password must contain at least 8 characters, a mix of upper/lowercase, a number, and a symbol): ");
                if (!passwordCheck(password)) {
                    std::cout << "Invalid Password. Please try again." << std::endl;
                }
            } while (!passwordCheck(password));
            account.setHolderPassword(password);
        } while (!userCheck("Does this look right to you [Y / N]: ",
            "Your account is now active!",
            "That's okay. Let's try again."));
        account.setHolderAccountNumber(lastAccountNumber++);
        account.setBalance(0.0);
        accounts.push_back(account);
    }

    void Storage::searchAccounts() const {
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

        out.write(reinterpret_cast<const char*>(&lastAccountNumber), sizeof(lastAccountNumber));

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

        in.read(reinterpret_cast<char*>(&lastAccountNumber), sizeof(lastAccountNumber));

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
    void Account::withdrawal(bankSimulation::BankFunds& bank) {
        float withdrawal = bankSimulation::numericValidator("Enter withdrawal amount: ", 0.01f, 5000.0f);

        if (withdrawal > balance) {
            std::cout << "Insufficient funds! ";
            printAccountBalance();
        }
        else {
            balance -= withdrawal;
            accountHistory["Withdrawal"] += withdrawal;

            double newTotal = bank.getTotalWithdrawals() + withdrawal;
            bank.setTotalWithdrawals(newTotal);

            std::cout << "Your withdrawal has been successfully processed! ";
            printAccountBalance();
        }
    }

    void Account::deposit(BankFunds& bank) {
        double depositAmount = numericValidator("Enter deposit amount: ", 0.01, 5000.0);
        balance += depositAmount;

        logTransaction("Deposit", depositAmount, balance);

        bank.setTotalDeposits(bank.getTotalDeposits() + depositAmount);

        std::cout << "Deposit successful. ";
        printAccountBalance();
    }

    void Account::logTransaction(const std::string& type, double amount, double resultingBalance) {
        if (transactionCount >= MAX_TRANSACTIONS) {
            std::cout << "Transaction log full. Oldest entry will be overwritten." << std::endl;
            // Optionally shift everything left
            for (int i = 1; i < MAX_TRANSACTIONS; ++i) {
                transactionHistory[i - 1] = transactionHistory[i];
            }
            transactionCount = MAX_TRANSACTIONS - 1;
        }

        Transaction t;
        t.type = type;
        t.amount = amount;
        t.resultingBalance = resultingBalance;

        // Get timestamp
        auto now = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
        t.timestamp = ss.str();

        transactionHistory[transactionCount++] = t;
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
        std::cout << "Transaction History:" << std::endl;
        for (int i = 0; i < transactionCount; ++i) {
            const auto& t = transactionHistory[i];
            std::cout << t.timestamp << " | "
                << t.type << " | "
                << "$" << t.amount << " | "
                << "Balance after: $" << t.resultingBalance << std::endl;
        }
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
    std::string BankFunds::getPassword() const {
        return password;
    }

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

    //Password Validation
    bool logIn(Storage& storage, bool employeeCheck) {
        short securityCounter = 0;

        //Employee Login Path
        if (employeeCheck) {
            while (securityCounter < 3) {
                std::string entered = stringValidator("Please enter the employee password: ");
                if (!storage.funds.empty() && entered == storage.funds[0].getPassword()) {
                    std::cout << "Employee login successful.\n";
                    return true;
                }
                else {
                    std::cout << "Invalid password.\n";
                    securityCounter++;
                }
            }
            std::cout << "Too many failed login attempts. Exiting program.\n";
            return false;
        }

        // User Login Path
        Account* matchedAccount = nullptr;

        while (securityCounter < 3 && !matchedAccount) {
            std::string lastName = stringValidator("Please enter your last name: ");

            for (auto& acc : storage.getAccounts()) {
                if (acc.getHolderLastName() == lastName) {
                    matchedAccount = &acc;
                    break;
                }
            }

            if (!matchedAccount) {
                std::cout << "Account not found." << std::endl;
                securityCounter++;
            }
        }

        if (!matchedAccount) {
            std::cout << "Too many failed login attempts. Exiting program." << std::endl;
            return false;
        }

        securityCounter = 0;
        while (securityCounter < 3) {
            std::string enteredPassword = stringValidator("Please enter your password: ");
            if (enteredPassword == matchedAccount->getHolderPassword()) {
                std::cout << "Login successful." << std::endl;
                return true;
            }
            else {
                std::cout << "Incorrect password." << std::endl;
                securityCounter++;
            }
        }

        std::cout << "Too many failed login attempts. Exiting program.\n";
        return false;
    }
    bool passwordCheck(const std::string& password) {
        if (password.length() < 8) return false;

        bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbol = false;
        for (char ch : password) {
            if (std::isupper(ch)) hasUpper = true;
            else if (std::islower(ch)) hasLower = true;
            else if (std::isdigit(ch)) hasDigit = true;
            else hasSymbol = true;
        }

        return hasUpper && hasLower && hasDigit && hasSymbol;
    }
}