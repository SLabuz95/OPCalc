#include"corruptedfilesmenager.hpp"
#include"config.hpp"
#include<QFile>
#include<QDir>
#include<QString>

void CorruptedFilesMenager::addCorruptedFile(QFile &file, Config &cnf){
    if(!QDir(cnf.getDefaultPath() + "OPCalc\\Corrupted\\").exists())
        QDir(cnf.getDefaultPath()).mkdir("Corrupted");
    file.close();
    QString fileName = QDir::toNativeSeparators(file.fileName());
    int  i = fileName.length() - 1;
    for( ; i > - 1; i--)
        if(fileName.at(i) == '\\'){
            fileName = fileName.right(fileName.length() - i - 1);
            break;
        }
    file.copy(cnf.getDefaultPath() + "OPCalc\\Corrupted\\" + fileName);
    file.remove();
}
