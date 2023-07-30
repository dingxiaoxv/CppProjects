#include "IInfoManage.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

TEST(addStudent_Test, BasicTest)
{
    shared_ptr<IInfoManage> pInfoManage = getInfoManageImpl();
    CStudentDTO stu(1, 80, "jack");

    EXPECT_EQ(pInfoManage->addStudent(stu), 0);
    EXPECT_EQ(pInfoManage->addStudent(stu), -1);
}

TEST(deleteStudentByID_Test, BasicTest)
{
    shared_ptr<IInfoManage> pInfoManage = getInfoManageImpl();
    CStudentDTO stu1(1, 80, "jack");
    CStudentDTO stu2(2, 90, "lucy");
    CStudentDTO stu3(3, 70, "hennry");
    pInfoManage->addStudent(stu1);
    pInfoManage->addStudent(stu2);
    pInfoManage->addStudent(stu3);

    EXPECT_EQ(pInfoManage->deleteStudentByID(2), 0);
    EXPECT_EQ(pInfoManage->deleteStudentByID(4), -1);
}

TEST(listStudentsByScore_Test, BasicTest)
{
    shared_ptr<IInfoManage> pInfoManage = getInfoManageImpl();
    CStudentDTO stu1(1, 80, "jack");
    CStudentDTO stu2(2, 90, "lucy");
    CStudentDTO stu3(3, 70, "hennry");
    pInfoManage->addStudent(stu1);
    pInfoManage->addStudent(stu2);
    pInfoManage->addStudent(stu3);
    std::vector<CStudentDTO> vec_ori;
    vec_ori.push_back(stu3);
    vec_ori.push_back(stu1);
    vec_ori.push_back(stu2);
    std::vector<CStudentDTO> vec_res;
    vec_res = pInfoManage->listStudentsByScore();

    EXPECT_EQ(vec_ori.size(), vec_res.size());
    for (size_t i = 0; i < vec_ori.size(); ++i)
    {
        EXPECT_EQ(vec_ori[i], vec_res[i]);
    }
}

TEST(readStudentsFromFile_Test, BasicTest)
{
    shared_ptr<IInfoManage> pInfoManage = getInfoManageImpl();
    CStudentDTO stu1(1, 80, "jack");
    CStudentDTO stu2(2, 90, "lucy");
    CStudentDTO stu3(3, 70, "hennry");
    pInfoManage->addStudent(stu1);
    pInfoManage->addStudent(stu2);
    pInfoManage->addStudent(stu3);
    std::vector<CStudentDTO> vec_ori;
    vec_ori.push_back(stu1);
    vec_ori.push_back(stu2);
    vec_ori.push_back(stu3);
    EXPECT_TRUE(pInfoManage->writeStudentsToFile("test.data"));
    EXPECT_TRUE(pInfoManage->readStudentsFromFile("test.data"));

    std::vector<CStudentDTO> vec_res;
    vec_res = pInfoManage->listStudentsByID();
    EXPECT_EQ(vec_ori.size(), vec_res.size());
    for (size_t i = 0; i < vec_ori.size(); ++i)
    {
        EXPECT_EQ(vec_ori[i], vec_res[i]);
    }
}