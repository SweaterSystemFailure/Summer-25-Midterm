#include "Validators.h"
#include "Account.h"
#include "Storage.h"
#include <iostream>
#include <string>
#include <vector>


namespace bankSimulation {
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
                if (!storage.getFunds().empty() && entered == storage.getFunds()[0].getPassword()) {
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
        const Account* matchedAccount = nullptr;

        while (securityCounter < 3 && !matchedAccount) {
            std::string lastName = stringValidator("Please enter your last name: ");

            for (const auto& acc : storage.getAccounts()) {
                if (acc.getHolderLastName() == lastName) {
                    matchedAccount = &acc;
                    break;
                }
            }

            if (!matchedAccount) {
                std::cout << "Account not found.\n";
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

    //Strong Password Validation
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