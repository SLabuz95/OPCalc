#ifndef CORRUPTEDFILESMENAGER_HPP
#define CORRUPTEDFILESMENAGER_HPP

class QFile;
class Config;
class CorruptedFilesMenager{
public:
    static void addCorruptedFile(QFile &file, Config &cnf);

};

#endif // CORRUPTEDFILESMENAGER_HPP
