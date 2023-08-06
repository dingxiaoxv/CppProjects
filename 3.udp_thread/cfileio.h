#ifndef _CFILEIO_H
#define _CFILEIO_H

#include <string>
#include <fstream>
#include <unordered_set>

class CFileIO
{
public:
    CFileIO();
    ~CFileIO();
    bool open(const std::string &fileName);
    void close();
    void write(const std::string &data);
    std::string read();

private:
    std::fstream m_file;
    std::unordered_set<std::string> m_setFileNames;
};

#endif