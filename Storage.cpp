#include "Storage.h"
#include "Validators.h"

namespace bankSimulation {
    /**
     * @brief Returns a reference to the vector containing all Account objects.
     *
     * Allows external code to access or modify the stored accounts directly.
     * @return Reference to accounts vector.
     */
    std::vector<Account>& Storage::getAccounts() {
        return accounts; 
    }

    /**
     * @brief Returns a reference to the vector containing all BankFunds objects.
     *
     * Allows external code to access or modify the stored bank funds directly.
     * @return Reference to funds vector.
     */
    std::vector<BankFunds>& Storage::getFunds() {
        return funds; 
    }

    /**
     * @brief Creates a new Account object from validated user input and adds it to storage.
     *
     * Prompts user for first and last name, enforces password strength requirements,
     * confirms input with the user, assigns a unique account number, initializes balance,
     * then stores the new account.
     */
    void Storage::newAccount() {
        Account account; 

        std::cout << "Welcome to your new account. Let's start by getting some basic information about you." << std::endl;

        do {
            account.setHolderFirstName(stringValidator("Please enter your first name: "));
            account.setHolderLastName(stringValidator("Please enter your last name: "));

            std::string password; 

            do {
                password = stringValidator(
                    "Please enter a strong password. Your password must contain at least 8 characters, a mix of upper/lowercase, a number, and a symbol): ");
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

    /**
     * @brief Searches stored accounts by account number or last name and displays results.
     *
     * Allows the user to select search criteria, performs search on accounts vector,
     * and prints account balances and transaction histories for any matches found.
     */
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
                    std::cout << std::endl << "Account found:" << std::endl;
                    acc.printAccountBalance();
                    acc.printAccountHistory();
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cout << "No account found with that number." << std::endl;
            }
            break;
        }
        case '2': {
            std::string searchLastName = bankSimulation::stringValidator("Enter the account holder's last name: ");
            for (const auto& acc : accounts) {
                if (acc.getHolderLastName() == searchLastName) {
                    std::cout << std::endl << "nAccount found:" << std::endl;
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

    /**
     * @brief Serializes and writes all Account objects and last account number to a binary file.
     *
     * Opens "accounts.dat" for binary output, writes account count, serializes each account,
     * and saves the last assigned account number for data continuity.
     */
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

    /**
     * @brief Reads and deserializes Account objects and last account number from binary file.
     *
     * Attempts to open "accounts.dat", creates it with default values if missing,
     * reads the number of accounts, deserializes each account into storage,
     * and restores last assigned account number.
     */
    void Storage::loadAccount() {
        std::ifstream in("accounts.dat", std::ios::binary);

        if (!in) {
            std::cerr << "accounts.dat not found. Creating a new file." << std::endl;

            std::ofstream out("accounts.dat", std::ios::binary);
            if (!out) {
                std::cerr << "Failed to create accounts.dat" << std::endl;
                return;
            }

            size_t count = 0;
            out.write(reinterpret_cast<const char*>(&count), sizeof(count));
            out.write(reinterpret_cast<const char*>(&lastAccountNumber), sizeof(lastAccountNumber));
            out.close();

            in.open("accounts.dat", std::ios::binary);
            if (!in) {
                std::cerr << "Could not open accounts.dat after creation." << std::endl;
                return;
            }
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

        std::cout << "Successfully loaded account entries." << std::endl;
        in.close();
    }

    /**
     * @brief Serializes and writes all BankFunds objects to a binary file.
     *
     * Opens "funds.dat" for binary output, writes the number of fund records,
     * and serializes each BankFunds object.
     */
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

    /**
     * @brief Reads and deserializes BankFunds objects from a binary file.
     *
     * Opens "funds.dat", if missing or empty, creates a default funds file.
     * Reads the number of records and deserializes each BankFunds into storage.
     */
    void Storage::loadBank() {
        std::ifstream in("funds.dat", std::ios::binary);

        if (!in || in.peek() == std::ifstream::traits_type::eof()) {
            std::cerr << "funds.dat not found. Creating new file." << std::endl;

            funds.clear();

            BankFunds defaultBank;
            funds.push_back(defaultBank);

            saveBank();
            return;
        }

        size_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));

        if (!in || count == 0) {
            std::cerr << "Invalid or empty data. Recreating funds.dat with default values." << std::endl;

            funds.clear();

            BankFunds defaultBank;
            funds.push_back(defaultBank);

            saveBank();
            return;
        }

        funds.clear();

        for (size_t i = 0; i < count; ++i) {
            BankFunds temp;
            temp.deserialize(in);
            funds.push_back(temp);
        }

        std::cout << "Successfully loaded fund entries." << std::endl;
        in.close();
    }
}