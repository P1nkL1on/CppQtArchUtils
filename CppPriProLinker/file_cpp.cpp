#include "file_cpp.h"

FileCpp::FileCpp(const QFileInfo &fileInfo) :
    File(fileInfo)
{
}

FileCpp::~FileCpp()
{
}

QVector<RefClass> FileCpp::refClasses() const
{
    return m_refClasses;
}

void FileCpp::addRefToClass(const RefClass &ref)
{
    if (not m_refClasses.contains(ref))
        m_refClasses << ref;
}
