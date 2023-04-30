#include "IInterface.h"
#include "InterfaceImpl.h"

IInterface::~IInterface()
{

}

std::shared_ptr<IInterface> getInterfaceImpl()
{
    std::shared_ptr<IInterface> p = std::make_shared<InterfaceImpl>();
    return p;
}