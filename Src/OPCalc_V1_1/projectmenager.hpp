#ifndef PROJECTMENAGER_HPP
#define PROJECTMENAGER_HPP
#include<QTextStream>
#include"projectmenagerparamenum.hpp"
class QFile;
class ProjectInfo;
class QString;
class ProjectMenager{
public:
    static int readProject(ProjectInfo*);
    static bool writeProject(ProjectInfo*);
    static bool moveProject(ProjectInfo*, QString);
    static bool checkProject(ProjectInfo*);
    static QString readForConfigFile(QTextStream*, QFile*, bool*);
    static bool rewriteForConfigFile(QFile*);

private:
    static QString readValue(QFile*, char*);
    static ProjectMenagerParam checkProjectMenagerParamName(QString*);
    static bool nameCheck(QString&);
};

#endif // PROJECTMENAGER_HPP
