#include "Account.h"
#include "BankFunds.h"
#include "menus.h"
#include "Storage.h"
#include "Validators.h"
#include <iostream>

using namespace bankSimulation;

int main()
{
    //storage instantiation
    Storage storage;
    //load from relevent .dat files.
    storage.loadBank();
    storage.loadAccount();
    //login menus for clients/employee branching
    branchMenu(storage);
    
    return 0;
}
