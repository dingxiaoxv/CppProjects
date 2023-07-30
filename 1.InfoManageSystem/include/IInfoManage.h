#ifndef IINFOMANAGE_H
#define IINFOMANAGE_H

#include <memory>
#include <vector>
#include "CStudentDTO.h"
using std::shared_ptr;
using std::vector;

//使用纯虚接口类的目的是为了实现逻辑算法的多态
class IInfoManage
{
public:
    virtual ~IInfoManage();
    /*
    函数名：addStudent
    功能：添加指定的学生信息
    参数：student对象
    返回值：失败返回-1，成功返回0
    */
    virtual int addStudent(const CStudentDTO &stu) = 0;
    /*
    函数名：deleteStudentByID
    功能：删除指定的学生信息
    参数：学生ID
    返回值：失败返回-1，成功返回0
    */
    virtual int deleteStudentByID(int id) = 0;
    /*
    函数名：editStudent
    功能：修改指定的学生信息
    参数：student对象
    返回值：失败返回-1，成功返回0
    */
    virtual int editStudent(CStudentDTO &stu) = 0;
    /*
    函数名：getStudentByID
    功能：通过ID获取学生信息
    参数：学生ID
    返回值：失败抛出异常-1，成功返回student对象
    */
    virtual CStudentDTO getStudentByID(int id) = 0;
    /*
    函数名：getStudentsCount
    功能：获取学生列表中信息个数
    参数：无
    返回值：列表元素数量
    */
    virtual int getStudentsCount() = 0;
    /*
    函数名：listStudentsByID
    功能：以ID顺序列出所有学生信息
    参数：无
    返回值：学生信息列表
    */
    virtual vector<CStudentDTO> listStudentsByID() = 0;
    /*
    函数名：listStudentsByScore
    功能：以分数顺序列出所有学生信息
    参数：无
    返回值：学生信息列表
    */
    virtual vector<CStudentDTO> listStudentsByScore() = 0;
    /*
    函数名：writeStudentsToFile
    功能：将所有学生信息以二进制写入文件
    参数：文件名
    返回值：失败返回false，成功返回true
    */
    virtual bool writeStudentsToFile(std::string fileName) = 0;
    /*
    函数名：readStudentsFromFile
    功能：从二进制文件中读取学生信息
    参数：文件名
    返回值：失败返回false，成功返回true
    */
    virtual bool readStudentsFromFile(std::string fileName) = 0;
};

/*
函数名：getInfoManageImpl
功能：获取接口实现的对象
参数：无
返回值：接口的共享指针
*/
extern shared_ptr<IInfoManage> getInfoManageImpl();

#endif