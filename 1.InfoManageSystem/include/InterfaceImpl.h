#ifndef INTERFACEIMPL_H
#define INTERFACEIMPL_H

#include "IInterface.h"
#include "IInfoManage.h"
#include <memory>

using std::shared_ptr;

class InterfaceImpl : public IInterface
{
private:
    shared_ptr<IInfoManage> m_pInfoManage;

public:
    InterfaceImpl();
    ~InterfaceImpl();
    void showInterface() override;

private:
    void showAddInterface();
    void showDeleteByIDInterface();
    void ShowEditByIDInterface();
    void showGetByIDInterface();
    void showAllInterface();
    void writeInterface();
    void readInterface();
};

#endif