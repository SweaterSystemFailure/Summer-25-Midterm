/*TODO:
* bug hunt!!
*/

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

    branchMenu(storage);
    
    return 0;
}
