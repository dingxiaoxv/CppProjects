#ifndef INFOMANAGEIMPL_H
#define INFOMANAGEIMPL_H

#include "IInfoManage.h"
using std::vector;

class InfoManageImpl : public IInfoManage
{
private:
    vector<CStudentDTO> m_vecStu;

public:
    int addStudent(const CStudentDTO &stu) override;
    int deleteStudentByID(int id) override;
    int editStudent(CStudentDTO &stu) override;
    CStudentDTO getStudentByID(int id) override;
    int getStudentsCount() override;
    vector<CStudentDTO> listStudentsByID() override;
    vector<CStudentDTO> listStudentsByScore() override;
    bool writeStudentsToFile(const std::string &fileName) override;
    bool readStudentsFromFile(const std::string &fileName) override;

private:
    vector<CStudentDTO>::iterator findElem(const CStudentDTO &stu);
    void sortByID(vector<CStudentDTO> &vec);
    void sortByScore(vector<CStudentDTO> &vec);
};

#endif