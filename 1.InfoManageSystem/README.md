# InfoManageSystem
学生信息管理系统

# 主要功能

1. 对学生信息进行增删改查，文件存储、读取。
2. 界面设计与算法设计分离，算法以静态库形式加载。
3. 门面设计模式，使用者通过接口调用相应功能。
4. CMake多文件编译。
5. GTest测试算法可靠性。
6. 使用Git-GitHub进行版本管理与云端存储。

# 2022.11.07更新

1. 对CStudentDTO的规范性进行了修改，将函数中的string形参改为const string &形参，string形参只能接受非常量对象，而const string &形参可接受常量对象、字面值和非常量对象，提高了可接受形参类型的范围。
2. 将CStudentDTO由CStudentDTO.h单独实现更新为CStudentDTO.h和CStudentDTO.cpp共同实现。
3. 新增了针对文件读写函数的单元测试。
