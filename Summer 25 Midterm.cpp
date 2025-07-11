#include "Account.h"
#include "BankFunds.h"
#include "menus.h"
#include "Storage.h"
#include "Validators.h"
#include <iostream>

using namespace bankSimulation;

int main()
{
    Storage storage;
    storage.loadBank();
    storage.loadAccount();
    if (storage.getFunds().empty()) {
        std::cerr << "WARNING: Bank funds failed to load.\n";
    }
    else {
        std::cout << "Bank password loaded: " << storage.getFunds()[0].getPassword() << std::endl;
    }

    branchMenu(storage);
    
    return 0;
}
