#pragma once
#include "Storage.h"
#include "BankFunds.h"
#include "Account.h"

namespace bankSimulation {

    /**
     * @brief Launches the main user menu for an authenticated account holder.
     *
     * Presents user-level banking options such as checking balance, making transactions,
     * and viewing account history. Operates on the passed Account and BankFunds references.
     *
     * @param account Reference to the currently logged-in Account object.
     * @param bank Reference to the BankFunds object used for centralized banking operations.
     */
    void userMenu(Account& account, BankFunds& bank);

    /**
     * @brief Launches the employee operations menu for bank staff.
     *
     * Enables privileged actions such as account searching and data inspection.
     * Accepts a reference to the Storage object to interact with system-wide data.
     *
     * @param storage Reference to the Storage object holding account and bank data.
     */
    void employeeMenu(Storage& storage);

    /**
     * @brief Launches the branch-level menu for administrative actions.
     *
     * Offers broader access and control features like system-wide fund management
     * and advanced administrative utilities.
     *
     * @param storage Reference to the Storage object managing all persistent data.
     */
    void branchMenu(Storage& storage);
}