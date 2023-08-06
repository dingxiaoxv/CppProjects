#include "cfileio.h"

CFileIO::CFileIO()
{
}

CFileIO::~CFileIO()
{
    if (m_file.is_open())
    {
        m_file.close();
        m_setFileNames.clear();
    }
}

bool CFileIO::open(const std::string &fileName)
{
    if (m_file.is_open())
    {
        m_file.close();
    }

    if (m_setFileNames.find(fileName) == m_setFileNames.end())
    {
        m_setFileNames.insert(fileName);
        m_file.open(fileName, std::ios::in | std::ios::out);
        return m_file.is_open();
    }
    else
    {
        m_file.open(fileName, std::ios::in | std::ios::app);
        return m_file.is_open();
    }
}

void CFileIO::close()
{
    if (m_file.is_open())
    {
        m_file.close();
        m_setFileNames.clear();
    }
}

void CFileIO::write(const std::string &data)
{
    if (m_file.is_open())
    {
        m_file << data;
    }
}

std::string CFileIO::read()
{
    std::string data;
    if (m_file.is_open())
    {
        m_file >> data;
    }
    return data;
}
