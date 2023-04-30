#include "InterfaceImpl.h"
#include "CStudentDTO.h"
#include <iostream>
#include <string>
using namespace std;

InterfaceImpl::InterfaceImpl()
{
    m_pInfoManage = getInfoManageImpl();
}

InterfaceImpl::~InterfaceImpl()
{
}

void InterfaceImpl::showInterface()
{
    int ichoice;
    do
    {
        cout << "-----------InfoManageSystem-----------" << endl;
        cout << "1、输入学生信息" << endl;
        cout << "2、根据学生ID,删除学生信息" << endl;
        cout << "3、根据学生ID,修改学生信息" << endl;
        cout << "4、根据学生ID,查找学生信息" << endl;
        cout << "5、显示所有学生信息" << endl;
        cout << "6、存储所有学生信息" << endl;
        cout << "7、载入现有学生信息" << endl;
        cout << "8、退出本程序" << endl;
        cout << "请输入功能号：" << endl;
        cout << "--------------------------------------" << endl;
        cin >> ichoice;
        switch (ichoice)
        {
        case 1:
            showAddInterface();
            break;
        case 2:
            showDeleteByIDInterface();
            break;
        case 3:
            ShowEditByIDInterface();
            break;
        case 4:
            showGetByIDInterface();
            break;
        case 5:
            showAllInterface();
            break;
        case 6:
            writeInterface();
            break;
        case 7:
            readInterface();
            break;
        default:
            break;
        }
    } while (ichoice != 8);
    cout << "系统已经退出！！！" << endl;
}

void InterfaceImpl::showAddInterface()
{
    string strinput;
    do
    {
        int iID, iscore;
        string strname;
        cout << "-------------Add Student-------------" << endl;
        cout << "输入学生ID:" << endl;
        cin >> iID;
        cout << "输入学生姓名:" << endl;
        cin >> strname;
        cout << "输入学生成绩:" << endl;
        cin >> iscore;
        CStudentDTO stu(iID, iscore, strname);
        if (m_pInfoManage->addStudent(stu) == -1)
        {
            cout << "学生ID重复，请重新输入!" << endl;
        }
        cout << "是否继续输入Y/y?" << endl;
        cin >> strinput;
    } while (strinput == "Y" || strinput == "y");
}

void InterfaceImpl::showDeleteByIDInterface()
{
    string strinput;
    int iID;
    do
    {
        cout << "-----------Delete  Student-----------" << endl;
        cout << "输入学生ID:" << endl;
        cin >> iID;
        if (m_pInfoManage->deleteStudentByID(iID) == -1)
        {
            cout << "未找到学生ID，请重新输入!" << endl;
        }
        cout << "是否继续输入Y/y?" << endl;
        cin >> strinput;
    } while (strinput == "Y" || strinput == "y");
}

void InterfaceImpl::ShowEditByIDInterface()
{
    string strinput;
    do
    {
        int iID, iscore;
        string strname;
        cout << "------------Edit  Student------------" << endl;
        cout << "输入需要修改学生ID:" << endl;
        cin >> iID;
        cout << "输入需要修改学生姓名:" << endl;
        cin >> strname;
        cout << "输入需要修改学生成绩:" << endl;
        cin >> iscore;
        CStudentDTO stu(iID, iscore, strname);
        if (m_pInfoManage->editStudent(stu) == -1)
        {
            cout << "未找到学生ID，请重新输入!" << endl;
        }
        cout << "是否继续输入Y/y?" << endl;
        cin >> strinput;
    } while (strinput == "Y" || strinput == "y");
}

void InterfaceImpl::showGetByIDInterface()
{
    string strinput;
    int iID;
    CStudentDTO stu;
    do
    {
        cout << "-------------Get Student-------------" << endl;
        cout << "输入学生ID:" << endl;
        cin >> iID;

        try
        {
            stu = m_pInfoManage->getStudentByID(iID);
            cout << "学生ID: " << stu.getID() << endl;
            cout << "学生姓名: " << stu.getName() << endl;
            cout << "学生分数: " << stu.getScore() << endl;
        }
        catch (int e)
        {
            if (e == -1)
                cout << "未找到学生ID，请重新输入!" << endl;
        }

        cout << "是否继续输入Y/y?" << endl;
        cin >> strinput;
    } while (strinput == "Y" || strinput == "y");
}

void InterfaceImpl::showAllInterface()
{
    cout << "按照ID排序还是分数排序？1: ID 2: 分数" << endl;
    int iinput;
    cin >> iinput;
    vector<CStudentDTO> vec;

    switch (iinput)
    {
    case 1:
        vec = m_pInfoManage->listStudentsByID();
        break;
    case 2:
        vec = m_pInfoManage->listStudentsByScore();
        break;
    default:
        cout << "请输入正确的值！" << endl;
        return;
    }

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        cout << "--------------------------" << endl;
        cout << "ID = " << it->getID() << endl;
        cout << "Name = " << it->getName() << endl;
        cout << "Score = " << it->getScore() << endl;
        cout << "--------------------------" << endl;
    }
}

void InterfaceImpl::writeInterface()
{
	string strinput, strfilename;
	do {
		cout << "------------Write Student------------" << endl;
		cout << "新建存储文件名:" << endl;
		cin >> strfilename;
		if (m_pInfoManage->writeStudentsToFile(strfilename))
		{
			cout << "文件存储成功！" << endl;
		}
		else
		{
			cout << "文件存储失败！" << endl;
		}
		cout << "是否退出？Y/y" << endl;
		cin >> strinput;
	} while (strinput != "Y" && strinput != "y");
}

void InterfaceImpl::readInterface()
{
	string strinput, strfilename;
	do
	{
		cout << "---------------ReadStudent----------------" << endl;
		cout << "读取文件名:" << endl;
		cin >> strfilename;
		if (m_pInfoManage->readStudentsFromFile(strfilename))
		{
			cout << "文件读取成功！" << endl;
		}
		else
		{
			cout << "文件读取失败！" << endl;
		}
		cout << "是否退出？Y/y" << endl;
		cin >> strinput;
	} while (strinput != "Y" && strinput != "y");
}
