#include "Validators.h"
#include "Account.h"
#include "Storage.h"
#include <iostream>
#include <string>
#include <vector>

namespace bankSimulation {

    /**
     * @brief Validates and returns a non-empty string input from the user.
     *
     * Prompts the user with the given message and reads a line of input.
     * Continues to prompt until the user enters a non-empty string.
     *
     * @param prompt The message to display to the user.
     * @return A validated, non-empty string input.
     */
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

    /**
     * @brief Validates a single character input against a list of valid options.
     *
     * Prompts the user with the given message and reads a character input.
     * Converts input to lowercase for case-insensitive matching.
     * Continues prompting until a valid character from validOptions is entered.
     *
     * @param prompt The message to display to the user.
     * @param validOptions Vector of valid characters to accept.
     * @return The validated character input (always lowercase).
     */
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

    /**
     * @brief Asks the user a yes/no question and returns a boolean accordingly.
     *
     * Prompts the user with the given question and expects 'y' or 'n' input (case-insensitive).
     * If 'y', outputs yesPrompt and returns true.
     * If 'n', outputs noPrompt and returns false.
     * Continues prompting until valid input is received.
     *
     * @param prompt The yes/no question to present.
     * @param yesPrompt Message to display if the user answers yes.
     * @param noPrompt Message to display if the user answers no.
     * @return True if user answers yes, false if no.
     */
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

    /**
     * @brief Handles user login by matching last name and password.
     *
     * Allows up to 3 attempts to find an account by last name.
     * If found, allows up to 3 attempts to enter the correct password.
     * Returns pointer to matched Account on success, nullptr on failure.
     *
     * @param storage Reference to Storage object containing accounts.
     * @return Pointer to logged-in Account if successful, nullptr otherwise.
     */
    Account* userLogIn(Storage& storage) {
        short securityCounter = 0;
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
                std::cout << "Account not found.\n";
                securityCounter++;
            }
        }

        if (!matchedAccount) {
            std::cout << "Too many failed login attempts. Exiting program." << std::endl;
            return nullptr;
        }

        securityCounter = 0;
        while (securityCounter < 3) {
            std::string enteredPassword = stringValidator("Please enter your password: ");
            if (enteredPassword == matchedAccount->getHolderPassword()) {
                std::cout << "Login successful." << std::endl;
                return matchedAccount;
            }
            else {
                std::cout << "Incorrect password." << std::endl;
                securityCounter++;
            }
        }

        std::cout << "Too many failed login attempts. Exiting program.\n";
        return nullptr;
    }

    /**
     * @brief Handles employee login by verifying employee password.
     *
     * Allows up to 3 attempts to enter correct employee password stored in bank funds.
     * Returns true if successful, false if too many failed attempts.
     *
     * @param storage Reference to Storage object containing bank funds.
     * @return True if employee login succeeds, false otherwise.
     */
    bool employeeLogIn(Storage& storage) {
        short securityCounter = 0;

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

    /**
     * @brief Validates the strength of a password string.
     *
     * Checks that the password is at least 8 characters long and contains at least
     * one uppercase letter, one lowercase letter, one digit, and one symbol.
     *
     * @param password The password string to validate.
     * @return True if the password meets all strength requirements, false otherwise.
     */
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