#ifndef IINTERFACE_H
#define IINTERFACE_H

#include <memory>

//门面设计模式：
//界面接口类
class IInterface
{
public:
    virtual ~IInterface();
    virtual void showInterface() = 0;
};

extern std::shared_ptr<IInterface> getInterfaceImpl();
#endif