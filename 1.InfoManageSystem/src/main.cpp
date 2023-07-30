#include "IInterface.h"

int main()
{
    std::shared_ptr<IInterface> pIInterface = getInterfaceImpl();
    pIInterface->showInterface();

    return 0;
}