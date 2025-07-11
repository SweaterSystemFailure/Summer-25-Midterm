#pragma once
#include <iostream>
#include <fstream>

namespace bankSimulation {
	class BankFunds {
	private:
		double startingFunds = 15'000'000.00;
		std::string password = "ShowMeTheMoney";
		double totalHoldings;
		double totalDeposits;
		double totalWithdrawals;
	public:
		//Mutators
		double setTotalHoldings(const double& entry);
		double setTotalDeposits(const double& entry);
		double setTotalWithdrawals(const double& entry);

		//Accessors
		std::string getPassword() const;
		double getTotalHoldings() const;
		double getTotalDeposits() const;
		double getTotalWithdrawals() const;

		//Serialization
		void serialize(std::ostream& out) const;
		void deserialize(std::istream& in);

		//Print Function
		void printBankReport() const;
	};
}

