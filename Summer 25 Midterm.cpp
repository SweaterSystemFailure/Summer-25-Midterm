/*TODO:
* bug hunt!!
*/

#include "containersAndFunctions.h"
#include "menus.h"
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
