#ifndef _CFILEIO_H
#define _CFILEIO_H

#include <string>

class CFileIO
{
public:
    CFileIO();
    ~CFileIO();

    bool Open(const std::string &filename);
    void Close();
    void Write(const std::string &data);

private:
    FILE *m_file;
};

#endif