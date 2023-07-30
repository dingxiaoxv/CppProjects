#ifndef CSTUDENT_H
#define CSTUDENT_H

#include <string>
using std::string;

class CStudentDTO
{
private:
    int m_ID = 0;
    int m_score = 0;
    string m_name = "";

public:
    CStudentDTO(){}
    CStudentDTO(int id) : m_ID(id), m_score(0), m_name(""){}
    CStudentDTO(int id, int score, const string &name) : m_ID(id), m_score(score), m_name(name){}
    //拷贝构造函数
    CStudentDTO(const CStudentDTO &stu);

    int getID();
    int getScore();
    string getName();
    void setID(int id);
    void setScore(int score);
    void setName(const string &name);

    //重载赋值操作符
    CStudentDTO &operator=(const CStudentDTO &stu);
    //重载相等操作符
    //双目运算符需要友元函数
    friend bool operator==(const CStudentDTO &a, const CStudentDTO &b);
};

bool operator==(const CStudentDTO &a, const CStudentDTO &b);

#endif