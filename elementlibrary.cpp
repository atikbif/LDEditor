#include "elementlibrary.h"
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <set>

QString ElementLibrary::getLibraryPath()
{
    return QCoreApplication::applicationDirPath() + "/CORE/Library";
}

std::vector<QString> ElementLibrary::getFolders() const
{
    std::set<QString> folders;
    QDir dir(getLibraryPath());
    dir.setNameFilters({"*.lbr"});

    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
       QFileInfo fileInfo = list.at(i);
       QFile file(fileInfo.absoluteFilePath());
       if (file.open(QIODevice::ReadOnly)) {
           QByteArray saveData = file.readAll();
           QJsonDocument elDoc(QJsonDocument::fromJson(saveData));
           auto elOb = elDoc.object();
           if(elOb.contains("Folder") && elOb["Folder"].isString()) {
               folders.insert(elOb["Folder"].toString());
           }
       }
    }
    std::vector<QString> result;
    std::copy(folders.begin(),folders.end(),std::back_inserter(result));
    std::sort(result.begin(),result.end());
    return result;
}

std::vector<QString> ElementLibrary::getElementsByFolder(const QString &folderName)
{
    std::vector<QString> result;
    QDir dir(getLibraryPath());
    dir.setNameFilters({"*.lbr"});

    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
       QFileInfo fileInfo = list.at(i);
       QFile file(fileInfo.absoluteFilePath());
       if (file.open(QIODevice::ReadOnly)) {
           QByteArray saveData = file.readAll();
           QJsonDocument elDoc(QJsonDocument::fromJson(saveData));
           auto elOb = elDoc.object();
           qDebug() << folderName << elOb["Folder"].toString();
           if(elOb.contains("Folder") && elOb["Folder"].isString() && elOb["Folder"].toString()==folderName) {
               if(elOb.contains("Name") && elOb["Name"].isString()) {
                    result.push_back(elOb["Name"].toString());
               }
           }
       }
    }
    std::sort(result.begin(),result.end());
    qDebug()<<result.size();
    return result;
}

QString ElementLibrary::getFileByElementName(const QString &elName)
{
    std::vector<QString> result;
    QDir dir(getLibraryPath());
    dir.setNameFilters({"*.lbr"});

    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
       QFileInfo fileInfo = list.at(i);
       QFile file(fileInfo.absoluteFilePath());
       if (file.open(QIODevice::ReadOnly)) {
           QByteArray saveData = file.readAll();
           QJsonDocument elDoc(QJsonDocument::fromJson(saveData));
           auto elOb = elDoc.object();
           if(elOb.contains("Name") && elOb["Name"].isString()) {
                if(elOb["Name"]==elName) return fileInfo.absoluteFilePath();
           }
       }
    }
    return QString();
}
