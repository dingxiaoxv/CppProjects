#include "CStudentDTO.h"

CStudentDTO::CStudentDTO(const CStudentDTO &stu)
{
    this->m_ID = stu.m_ID;
    this->m_score = stu.m_score;
    this->m_name = stu.m_name;
}

int CStudentDTO::getID()
{
    return this->m_ID;
}

int CStudentDTO::getScore()
{
    return this->m_score;
}

string CStudentDTO::getName()
{
    return this->m_name;
}

void CStudentDTO::setID(int id)
{
    m_ID = id;
}

void CStudentDTO::setScore(int score)
{
    m_score = score;
}

void CStudentDTO::setName(const string &name)
{
    m_name = name;
}

//重载赋值操作符
CStudentDTO &CStudentDTO::operator=(const CStudentDTO &stu)
{
    this->m_ID = stu.m_ID;
    this->m_score = stu.m_score;
    this->m_name = stu.m_name;
    return *this;
}

bool operator==(const CStudentDTO &a, const CStudentDTO &b)
{
    if (a.m_ID == b.m_ID)
        return true;
    else
        return false;
}