#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <limits>
#include "Storage.h"

namespace bankSimulation {
    /**
     * @brief Template function to validate numeric input of any type T (int, double, etc.).
     *
     * Prompts the user with a message, reads input, and ensures the input is numeric and
     * falls within the specified inclusive range [min, max]. Loops until valid input is received.
     *
     * @tparam T Numeric type of input.
     * @param prompt Message to display to the user.
     * @param min Minimum acceptable value.
     * @param max Maximum acceptable value.
     * @return Validated numeric value of type T.
     */
    template <typename T>
    T numericValidator(const std::string& prompt, T min, T max) {
        T number;

        while (true) {
            std::cout << prompt << std::endl; 
            std::cin >> number; 

            if (std::cin.fail()) {
                std::cout << "Enter a valid number." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            }
            else if (number < min || number > max) { 
                std::cout << "Number must be between " << min << " and " << max << "." << std::endl;  
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            }
            else {  
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                break;  
            }
        }

        return number;  
    }

    /**
     * @brief Validates and returns a non-empty string input from the user.
     *
     * @param prompt Message to prompt the user.
     * @return Non-empty string input.
     */
    std::string stringValidator(const std::string& prompt);

    /**
     * @brief Validates a single character input against a list of valid options.
     *
     * @param prompt Message to prompt the user.
     * @param validOptions Vector of valid characters.
     * @return Validated character input.
     */
    char charValidator(const std::string& prompt, const std::vector<char>& validOptions);

    /**
     * @brief Asks the user a yes/no question and returns true for yes, false for no.
     *
     * @param prompt Question to present to the user.
     * @param yesPrompt Message to display if user answers yes.
     * @param noPrompt Message to display if user answers no.
     * @return Boolean representing user's confirmation.
     */
    bool userCheck(const std::string& prompt, const std::string& yesPrompt, const std::string& noPrompt);

    /**
     * @brief Handles user login process by validating credentials against stored accounts.
     *
     * @param storage Reference to Storage containing accounts.
     * @return Pointer to Account on successful login, nullptr otherwise.
     */
    Account* userLogIn(Storage& storage);

    /**
     * @brief Handles employee login process by validating credentials.
     *
     * @param storage Reference to Storage for necessary data access.
     * @return True if login successful, false otherwise.
     */
    bool employeeLogIn(Storage& storage);

    /**
     * @brief Validates password strength according to defined security rules.
     *
     * @param password Password string to validate.
     * @return True if password meets strength requirements, false otherwise.
     */
    bool passwordCheck(const std::string& password);
}