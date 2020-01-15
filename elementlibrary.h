#ifndef ELEMENTLIBRARY_H
#define ELEMENTLIBRARY_H

#include <QString>
#include <vector>


class ElementLibrary
{  
public:
    static QString getLibraryPath();
    ElementLibrary() = default;
    std::vector<QString> getFolders() const;
    std::vector<QString> getElementsByFolder(const QString &folderName);
    QString getFileByElementName(const QString &elName);
};

#endif // ELEMENTLIBRARY_H
