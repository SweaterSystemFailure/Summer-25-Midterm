#include "BankFunds.h"
#include "Account.h"
#include "Storage.h"
#include "Validators.h"
#include <iomanip>

namespace bankSimulation {

    /**
     * @brief Sets the total holdings for the bank.
     *
     * Updates the internal record of total holdings and returns the updated value.
     *
     * @param entry New total holdings value.
     * @return Updated total holdings.
     */
    double BankFunds::setTotalHoldings(const double& entry) {
        totalHoldings = entry;
        return totalHoldings;
    }

    /**
     * @brief Sets the total deposits for the bank.
     *
     * Updates the internal record of total deposits and returns the updated value.
     *
     * @param entry New total deposit value.
     * @return Updated total deposits.
     */
    double BankFunds::setTotalDeposits(const double& entry) {
        totalDeposits = entry;
        return totalDeposits;
    }

    /**
     * @brief Sets the total withdrawals for the bank.
     *
     * Updates the internal record of total withdrawals and returns the updated value.
     *
     * @param entry New total withdrawal value.
     * @return Updated total withdrawals.
     */
    double BankFunds::setTotalWithdrawals(const double& entry) {
        totalWithdrawals = entry;
        return totalWithdrawals;
    }

    /**
     * @brief Retrieves the stored password for employee login.
     *
     * @return Bank password string.
     */
    std::string BankFunds::getPassword() const {
        return password;
    }

    /**
     * @brief Gets the total amount of money currently held by the bank.
     *
     * @return Total holdings value.
     */
    double BankFunds::getTotalHoldings() const {
        return totalHoldings;
    }

    /**
     * @brief Gets the cumulative amount deposited into the bank.
     *
     * @return Total deposits value.
     */
    double BankFunds::getTotalDeposits() const {
        return totalDeposits;
    }

    /**
     * @brief Gets the cumulative amount withdrawn from the bank.
     *
     * @return Total withdrawals value.
     */
    double BankFunds::getTotalWithdrawals() const {
        return totalWithdrawals;
    }

    /**
     * @brief Serializes the BankFunds object to a binary output stream.
     *
     * Writes the starting funds, password, total deposits, withdrawals, and holdings
     * to the specified output stream in binary format.
     *
     * @param out Output stream to serialize into.
     */
    void BankFunds::serialize(std::ostream& out) const {
        out.write(reinterpret_cast<const char*>(&startingFunds), sizeof(startingFunds));

        size_t passwordLength = password.size();
        out.write(reinterpret_cast<const char*>(&passwordLength), sizeof(passwordLength));
        out.write(password.c_str(), passwordLength);

        out.write(reinterpret_cast<const char*>(&totalHoldings), sizeof(totalHoldings));
        out.write(reinterpret_cast<const char*>(&totalDeposits), sizeof(totalDeposits));
        out.write(reinterpret_cast<const char*>(&totalWithdrawals), sizeof(totalWithdrawals));
    }

    /**
     * @brief Deserializes the BankFunds object from a binary input stream.
     *
     * Reads in the starting funds, password, total deposits, withdrawals, and holdings
     * from the specified input stream and updates the current object accordingly.
     *
     * @param in Input stream to deserialize from.
     */
    void BankFunds::deserialize(std::istream& in) {
        in.read(reinterpret_cast<char*>(&startingFunds), sizeof(startingFunds));

        size_t passwordLength;
        in.read(reinterpret_cast<char*>(&passwordLength), sizeof(passwordLength));
        password.resize(passwordLength);
        in.read(&password[0], passwordLength);

        in.read(reinterpret_cast<char*>(&totalHoldings), sizeof(totalHoldings));
        in.read(reinterpret_cast<char*>(&totalDeposits), sizeof(totalDeposits));
        in.read(reinterpret_cast<char*>(&totalWithdrawals), sizeof(totalWithdrawals));
    }

    /**
     * @brief Prints a summary of the bank's financial state.
     *
     * Outputs the starting funds, total deposits, total withdrawals, and total holdings
     * to the standard output stream in a formatted report.
     */
    void BankFunds::printBankReport() const {
        std::cout << "--- Bank Report ---" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Starting Funds: $" << startingFunds << std::endl;
        std::cout << "Total Deposits: $" << totalDeposits << std::endl;
        std::cout << "Total Withdrawals: $" << totalWithdrawals << std::endl;
        std::cout << "Total Holdings: $" << totalHoldings << std::endl;
    }
}