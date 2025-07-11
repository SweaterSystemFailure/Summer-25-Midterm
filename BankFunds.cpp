#include "BankFunds.h"
#include "Account.h"
#include "Storage.h"
#include "Validators.h"


namespace bankSimulation {
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
}