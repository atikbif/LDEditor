#ifndef COMPILER_H
#define COMPILER_H

#include <QString>
#include <vector>
#include <set>

class Compiler
{
public:
    Compiler() = default;
    static void makeProgFile(const std::vector<QString> &vars, const std::vector<QString> &prog, const std::vector<QString> &funcBody, int delay=10);
    static void makeLibraryfiles(const std::set<QString> &header, const std::set<QString> &body);
    static std::vector<QString> compile(const QString &plcName, const QString &dirPath);
    static std::vector<QString> getInternalVars();
    static QString getCorePath();
    static QString getBuildPath();
};

#endif // COMPILER_H
