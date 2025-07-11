#pragma once
#include <iostream>
#include <fstream>

namespace bankSimulation {

    /**
     * @class BankFunds
     * @brief Manages the overall financial data of the bank, including totals for deposits, withdrawals, and holdings.
     *
     * This class stores and tracks aggregate financial statistics used in reporting and administrative tasks.
     * It also includes support for binary serialization and deserialization of its internal state.
     */
    class BankFunds {
    private:
        double startingFunds = 15'000'000.00;   ///< Initial seed capital of the bank
        std::string password = "ShowMeTheMoney"; ///< Password for employee access
        double totalHoldings = 15'000'000.00;   ///< Current total funds held by the bank
        double totalDeposits = 0.0;             ///< Total of all deposits made
        double totalWithdrawals = 0.0;          ///< Total of all withdrawals made

    public:
        /**
         * @brief Updates the total holdings value.
         *
         * @param entry New total holdings amount.
         * @return Updated total holdings.
         */
        double setTotalHoldings(const double& entry);

        /**
         * @brief Updates the total deposits value.
         *
         * @param entry New total deposit amount.
         * @return Updated total deposits.
         */
        double setTotalDeposits(const double& entry);

        /**
         * @brief Updates the total withdrawals value.
         *
         * @param entry New total withdrawal amount.
         * @return Updated total withdrawals.
         */
        double setTotalWithdrawals(const double& entry);

        /**
         * @brief Retrieves the stored bank password.
         *
         * @return The current password string.
         */
        std::string getPassword() const;

        /**
         * @brief Retrieves the current total holdings value.
         *
         * @return Total holdings as a double.
         */
        double getTotalHoldings() const;

        /**
         * @brief Retrieves the cumulative total of all deposits.
         *
         * @return Total deposits as a double.
         */
        double getTotalDeposits() const;

        /**
         * @brief Retrieves the cumulative total of all withdrawals.
         *
         * @return Total withdrawals as a double.
         */
        double getTotalWithdrawals() const;

        /**
         * @brief Serializes the bank fund data to a binary output stream.
         *
         * @param out Output stream to which data will be written.
         */
        void serialize(std::ostream& out) const;

        /**
         * @brief Deserializes the bank fund data from a binary input stream.
         *
         * @param in Input stream from which data will be read.
         */
        void deserialize(std::istream& in);

        /**
         * @brief Prints a formatted report of the bank’s financial summary.
         */
        void printBankReport() const;
    };

}