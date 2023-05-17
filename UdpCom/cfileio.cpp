#include "cfileio.h"

CFileIO::CFileIO()
{
    m_file = nullptr;
}

CFileIO::~CFileIO()
{
    Close();
}

bool CFileIO::Open(const std::string &filename)
{
    Close();
    m_file = fopen(filename.c_str(), "a+");
    return (m_file != nullptr);
}

void CFileIO::Close()
{
    if (m_file != nullptr)
    {
        fclose(m_file);
        m_file = nullptr;
    }
}

void CFileIO::Write(const std::string &data)
{
    if (m_file != nullptr)
    {
        fwrite(data.c_str(), 1, data.length(), m_file);
        fflush(m_file);
    }
}