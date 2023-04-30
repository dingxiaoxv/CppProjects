#include "IInfoManage.h"
#include "InfoManageImpl.h"
using std::make_shared;

IInfoManage::~IInfoManage()
{

}

shared_ptr<IInfoManage> getInfoManageImpl()
{
    shared_ptr<IInfoManage> p = make_shared<InfoManageImpl>();
    return p;
}
