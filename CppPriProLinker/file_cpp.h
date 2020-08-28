#ifndef FILECPP_H
#define FILECPP_H

#include "file.h"
#include "ref_class.h"

class FileCpp : public File
{
public:
    explicit FileCpp(const QFileInfo &fileInfo);
    ~FileCpp() override;
protected:
};

#endif // FILECPP_H
