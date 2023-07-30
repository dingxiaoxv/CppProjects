#include "InfoManageImpl.h"
#include <fstream>
#include <cstring>

vector<CStudentDTO>::iterator InfoManageImpl::findElem(const CStudentDTO &stu)
{
    vector<CStudentDTO>::iterator it = m_vecStu.begin();
    for (; it != m_vecStu.end(); it++)
    {
        if (*it == stu)
        {
            //搜索到相同元素返回对应位置的迭代器
            return it;
        }
    }
    //搜索不到返回尾后迭代器
    return it;
}

int InfoManageImpl::addStudent(const CStudentDTO &stu)
{
    if (findElem(stu) != m_vecStu.end())
    {
        return -1;
    }
    else
    {
        m_vecStu.push_back(stu);
        return 0;
    }
}

int InfoManageImpl::deleteStudentByID(int id)
{
    CStudentDTO stu(id);
    vector<CStudentDTO>::iterator it = findElem(stu);
    if (it == m_vecStu.end())
    {
        return -1;
    }
    else
    {
        m_vecStu.erase(it);
        return 0;
    }
}

int InfoManageImpl::editStudent(CStudentDTO &stu)
{
    vector<CStudentDTO>::iterator it = findElem(stu);
    if (it == m_vecStu.end())
    {
        return -1;
    }
    else
    {
        it->setName(stu.getName().c_str());
        it->setScore(stu.getScore());
        return 0;
    }
}

CStudentDTO InfoManageImpl::getStudentByID(int id)
{
    CStudentDTO stu(id);
    vector<CStudentDTO>::iterator it = findElem(stu);
    if (it == m_vecStu.end())
    {
        throw -1;
    }
    else
    {
        return *it;
    }
}

int InfoManageImpl::getStudentsCount()
{
    return m_vecStu.size();
}

vector<CStudentDTO> InfoManageImpl::listStudentsByID()
{
    //初始化为m_vecStu的拷贝
    vector<CStudentDTO> vec = m_vecStu;
    sortByID(vec);
    return vec;
}

vector<CStudentDTO> InfoManageImpl::listStudentsByScore()
{
    vector<CStudentDTO> vec = m_vecStu;
    sortByScore(vec);
    return vec;
}

bool InfoManageImpl::writeStudentsToFile(const std::string &fileName)
{
    std::ofstream outfile(fileName, std::ios::binary | std::ios::out);
    if (!outfile)
    {
        return false;
    }
    int len = m_vecStu.size();
    outfile.write((char *)&len, sizeof(int));
    for (auto it = m_vecStu.begin(); it != m_vecStu.end(); ++it)
    {
        int itemp = it->getID();
        outfile.write((char *)&itemp, sizeof(int));
        itemp = it->getScore();
        outfile.write((char *)&itemp, sizeof(int));
        itemp = it->getName().size() + 1; //有效字符长度+1 包含'\0'
        outfile.write((char *)&itemp, sizeof(int));
        char chname[itemp]{'\0'}; //创建全0数组 
        strcpy(chname, it->getName().c_str());
        outfile.write((char *)chname, itemp);
    }
    outfile.close();
    return true;
}

bool InfoManageImpl::readStudentsFromFile(const std::string &fileName)
{
    std::ifstream infile(fileName, std::ios::binary | std::ios::in);
    if (!infile)
    {
        return false;
    }
    m_vecStu.clear();
    int len;
    infile.read((char *)&len, sizeof(int));
    for (int i = 0; i < len; ++i)
    {
        int id = 0, score = 0, strlen = 0;
        infile.read((char *)&id, sizeof(int));
        infile.read((char *)&score, sizeof(int));
        infile.read((char *)&strlen, sizeof(int));
        char chname[strlen];
        infile.read((char *)chname, strlen);
        string strname(chname);
        CStudentDTO stu(id, score, strname);
        m_vecStu.push_back(stu);
    }
    infile.close();
    return true;
}

void InfoManageImpl::sortByID(vector<CStudentDTO> &vec)
{
    int len = vec.size();
    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - i - 1; j++)
        {
            if (vec[j].getID() > vec[j + 1].getID())
            {
                CStudentDTO temp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = temp;
            }
        }
    }
}

void InfoManageImpl::sortByScore(vector<CStudentDTO> &vec)
{
    int len = vec.size();
    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - i - 1; j++)
        {
            if (vec[j].getScore() > vec[j + 1].getScore())
            {
                CStudentDTO temp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = temp;
            }
        }
    }
}