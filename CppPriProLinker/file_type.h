#ifndef FILE_TYPE_H
#define FILE_TYPE_H

#include <QString>

enum class FileType {
    H,
    Cpp,
    Pri,
    Pro
};

inline bool isProjectFormat(FileType type){
    return type == FileType::Pri or type == FileType::Pro;
}

inline bool isCodeFormate(FileType type){
    return type == FileType::H or type == FileType::Cpp;
}

inline FileType fileType(const QString &filePath){
    if (filePath.endsWith(".h"))
        return FileType::H;
    if (filePath.endsWith(".cpp"))
        return FileType::Cpp;
    if (filePath.endsWith(".pro"))
        return FileType::Pro;
    if (filePath.endsWith(".pri"))
        return FileType::Pri;
    Q_ASSERT(false);
}


#endif // FILE_TYPE_H
