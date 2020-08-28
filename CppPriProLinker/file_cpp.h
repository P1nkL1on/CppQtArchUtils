#ifndef FILECPP_H
#define FILECPP_H

#include "file.h"
#include "ref_class.h"

class FileCpp : public File
{
public:
    explicit FileCpp(const QFileInfo &fileInfo);
    ~FileCpp() override;
    QVector<RefClass> refClasses() const;
    void addRefToClass(const RefClass &ref);
protected:
    QVector<RefClass> m_refClasses;
};

#endif // FILECPP_H
