#include"materialsinfofiles.hpp"
#include"readingmaterialsfilesenum.hpp"
#include"flangematerialsfileparams.hpp"
#include"pipematerialsfileparams.hpp"
#include"substancesfileparams.hpp"
#include"innerwindowcep.hpp"
#include"mainmacros.hpp"
#include"errormacroes.hpp"
#include"config.hpp"
#include<QDir>
#include<QFile>
#include<QTextStream>
#include"appwindow.hpp"
#include"materialschangeswidget.hpp"
#include"appclass.hpp"
#include<QListWidget>
#include"formatabledatafield.hpp"
#include<QLineEdit>
#include<QComboBox>
#include<QCheckBox>
#include"projectinfo.hpp"
#include"formatabledata.hpp"
#include"materialschangeswidgetdataholder.hpp"
#include"materialschangeswidget.hpp"
#include<QStringList>
#include"mydialog.hpp"
#include"corruptedfilesmenager.hpp"
#include<math.h>


bool MaterialsInfoFiles::readFlangeMaterials(InnerWindowCEP *innerWindowCEP){
    QFile file(innerWindowCEP->getParent()->getApplication()->getConfiguration()->getDefaultPath() + QString(APP_NAME) + "\\" + FLANGE_MATERIALS_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        bool nameReaded = false;
        QString name = QString();
        bool linearExpReaded = false;
        bool fiducialTempReaded = false;
        char tempChar = '\0';
        MaterialsFileStat materialsFileStat = READING_PARAMS;
        FlangeMaterialsFileParams flangeMaterialFileParam = FLANGE_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((flangeMaterialFileParam = checkFlangeMaterialsFileParams(tempStr)) == NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *innerWindowCEP->getParent()->getApplication()->getConfiguration());
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    file.close();
                    return false;
                }else{
                   materialsFileStat = READ_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(flangeMaterialFileParam){
                case FLANGE_NAME:
                    nameReaded = false;
                    linearExpReaded = false;
                    fiducialTempReaded = false;
                    name = tempStr;
                    if(nameCheck(name))
                        nameReaded = true;
                    break;
                case FLANGE_LINEAR_EXPANSTION:
                    if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, false))
                        linearExpReaded = true;
                    break;
                case FLANGE_FIDUCIAL_TEMP:
                    if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, true))
                        fiducialTempReaded = true;
                    if(nameReaded && linearExpReaded && fiducialTempReaded)
                        innerWindowCEP->getMeasurementDeviceMaterialList()->addItem(name);
                    break;
                }
                materialsFileStat = READING_PARAMS;
                tempStr.clear();
          }

     }
        file.close();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
      }
    return true;
  }


bool MaterialsInfoFiles::readMaterials(MaterialsChangesWidget<FlangeMaterialsFileParams> *materialsChangesWidget){
    QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +FLANGE_MATERIALS_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        bool nameReaded = false;
        QString name = QString();
        bool linearExpReaded = false;
        bool fiducialTempReaded = false;
        char tempChar = '\0';
        MaterialsFileStat materialsFileStat = READING_PARAMS;
        FlangeMaterialsFileParams flangeMaterialFileParam = FLANGE_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((flangeMaterialFileParam = checkFlangeMaterialsFileParams(tempStr)) == NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    file.close();
                    return false;
                }else{
                   materialsFileStat = READ_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(flangeMaterialFileParam){
                case FLANGE_NAME:
                    nameReaded = false;
                    linearExpReaded = false;
                    fiducialTempReaded = false;

                    name = tempStr;
                    if(nameCheck(name))
                        nameReaded = true;
                    break;
                case FLANGE_LINEAR_EXPANSTION:
                    if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, false))
                        linearExpReaded = true;
                    break;
                case FLANGE_FIDUCIAL_TEMP:
                    if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, true))
                        fiducialTempReaded = true;
                    if(nameReaded && linearExpReaded && fiducialTempReaded)
                        materialsChangesWidget->getList()->addItem(name);
                    break;
                }
                materialsFileStat = READING_PARAMS;
                tempStr.clear();
            }
        }
        file.close();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
      }
    return true;
  }


bool MaterialsInfoFiles::addMaterial(MaterialsChangesWidget<FlangeMaterialsFileParams>* materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + FLANGE_MATERIALS_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            char tempChar = '\0';
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar > 0)
                    stream << tempChar;
                else
                    stream << App::readCharUtf8(&file, &tempChar);
            }
            stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
            stream << "lineExp=" + materialsChangesWidget->getLinearExpanstionFDF()->getDataUTextEdit()->text() + "\n";
            stream << "fiducTemp=" + materialsChangesWidget->getFiducialTemperatureFDF()->getDataUTextEdit()->text() + "\n";
            file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + FLANGE_MATERIALS_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();

   }
    return true;
  }


bool MaterialsInfoFiles::editMaterial(MaterialsChangesWidget<FlangeMaterialsFileParams>* materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + FLANGE_MATERIALS_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool edit = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            FlangeMaterialsFileParams flangeMaterialFileParam = FLANGE_NAME;
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((flangeMaterialFileParam = checkFlangeMaterialsFileParams(tempStr)) == NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych. Zedytowane dane zostały zapisane w nowym pliku."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        tempFile.close();
                        tempFile.remove();
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
                        stream << "lineExp=" + materialsChangesWidget->getLinearExpanstionFDF()->getDataUTextEdit()->text() + "\n";
                        stream << "fiducTemp=" + materialsChangesWidget->getFiducialTemperatureFDF()->getDataUTextEdit()->text() + "\n";
                        file.close();
                        return false;
                    }else{
                       materialsFileStat = READ_VALUE;
                       tempStr.clear();
                    }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(flangeMaterialFileParam){
                    case FLANGE_NAME:
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            edit = true;
                        else
                            edit = false;
                        if(!edit)
                            stream << ("name=\"" + tempStr + "\"\n").toUtf8();
                        else{
                            stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
                            stream << "lineExp=" + materialsChangesWidget->getLinearExpanstionFDF()->getDataUTextEdit()->text() + "\n";
                            stream << "fiducTemp=" + materialsChangesWidget->getFiducialTemperatureFDF()->getDataUTextEdit()->text() + "\n";
                         }
                        break;
                    case FLANGE_LINEAR_EXPANSTION:
                        if(!edit)
                            stream << "lineExp=" + tempStr + "\n";
                        break;
                    case FLANGE_FIDUCIAL_TEMP:
                        if(!edit)
                            stream << "fiducTemp=" + tempStr + "\n";
                        break;
                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;

                }
            }
                file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +FLANGE_MATERIALS_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();

   }
    return true;
  }


bool MaterialsInfoFiles::removeMaterial(MaterialsChangesWidget<FlangeMaterialsFileParams>* materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + FLANGE_MATERIALS_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool remove = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            FlangeMaterialsFileParams flangeMaterialFileParam = FLANGE_NAME;
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((flangeMaterialFileParam = checkFlangeMaterialsFileParams(tempStr)) == NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        tempFile.close();
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                    }else{
                       materialsFileStat = READ_VALUE;
                       tempStr.clear();
                    }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(flangeMaterialFileParam){
                    case FLANGE_NAME:
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            remove = true;
                        else
                            remove = false;
                        if(!remove)
                            stream << ("name=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case FLANGE_LINEAR_EXPANSTION:
                        if(!remove)
                            stream << "lineExp=" + tempStr + "\n";
                        break;
                    case FLANGE_FIDUCIAL_TEMP:
                        if(!remove)
                            stream << "fiducTemp=" + tempStr + "\n";
                        break;
                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                }
            }
                file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + FLANGE_MATERIALS_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();

   }
    return true;
  }

bool MaterialsInfoFiles::readMaterialParams(MaterialsChangesWidget<FlangeMaterialsFileParams> *materialsChangesWidget){
    QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +FLANGE_MATERIALS_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool read = false;
            bool stop = false;
            bool nameReaded = false;
            QString name = QString();
            QString fiducialTemp;
            QString linearExp;
            bool linearExpReaded = false;
            bool fiducialTempReaded = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            FlangeMaterialsFileParams flangeMaterialFileParam = FLANGE_NAME;
            while(!(file.atEnd() || stop)){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((flangeMaterialFileParam = checkFlangeMaterialsFileParams(tempStr)) == NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                    }else{
                       materialsFileStat = READ_VALUE;
                       tempStr.clear();
                    }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(flangeMaterialFileParam){
                    case FLANGE_NAME:
                        if(read == true){
                            stop = true;
                            break;
                        }
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            read = true;
                        else
                            read = false;
                        if(read){
                            name = tempStr;
                            if(nameCheck(name))
                                nameReaded = true;
                        }
                    break;
                    case FLANGE_LINEAR_EXPANSTION:
                        if(read){
                            if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, false)){
                                linearExpReaded = true;
                                linearExp = tempStr;
                            }
                         }
                        break;
                    case FLANGE_FIDUCIAL_TEMP:
                        if(read){
                            if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, true)){
                                fiducialTempReaded = true;
                                fiducialTemp = tempStr;
                            }
                            if(nameReaded && linearExpReaded && fiducialTempReaded){
                                materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->setText(name);
                                materialsChangesWidget->getFiducialTemperatureFDF()->getDataUTextEdit()->setText(fiducialTemp);
                                materialsChangesWidget->getLinearExpanstionFDF()->getDataUTextEdit()->setText(linearExp);
                            }else{
                                MyDialog dlg(QUESTION_DIALOG, QString("Błąd pliku"), QString("Dane na temat wybranej substancji zostały uszkodzone."), false, QString("Ok"), QString(), QString(),nullptr);
                                dlg.setWindowModality(Qt::ApplicationModal);
                                dlg.show();
                                dlg.exec();
                                file.close();
                                return false;
                            }
                        }
                        break;
                    }                    
                    materialsFileStat = READING_PARAMS;
                    tempStr.clear();
                }                
            }
                file.close();
            }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
                return false;
            }
    return true;
}

bool MaterialsInfoFiles::mergeFlangeMaterialsFiles(QFile &file, QFile &mergedFile, Config &cnf){
    QFile tempFile("temp");
    QStringList strList;
    if(tempFile.exists())
        tempFile.remove();
    QString tempStr =  QString();
    char tempChar = '\0';
    bool nameReaded = false;
    QString name = QString();
    QString fiducialTemp;
    QString linearExp;
    bool linearExpReaded = false;
    bool fiducialTempReaded = false;
    MaterialsFileStat materialsFileStat = READING_PARAMS;
    FlangeMaterialsFileParams flangeMaterialFileParam = FLANGE_NAME;
    // Otwarcie Pliku Tymczasowego do zapisu
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
     QTextStream stream(&tempFile);
     stream.setCodec("UTF-8");
    // Otwarcie Pliku File do odczytu
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((flangeMaterialFileParam = checkFlangeMaterialsFileParams(tempStr)) == NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, cnf);
                    tempFile.close();
                    tempFile.remove();
                    if(!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
                        return false;
                }else{
                   materialsFileStat = READ_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(flangeMaterialFileParam){
                case FLANGE_NAME:
                    nameReaded = false;
                    linearExpReaded = false;
                    fiducialTempReaded = false;
                    name = tempStr;
                    if(nameCheck(name))
                        nameReaded = true;
                   break;
                case FLANGE_LINEAR_EXPANSTION:
                    if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, false)){
                        linearExpReaded = true;
                        linearExp = tempStr;
                    }
                    break;
                case FLANGE_FIDUCIAL_TEMP:
                    if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, true)){
                        fiducialTempReaded = true;
                        fiducialTemp = tempStr;
                    }
                    if(nameReaded && linearExpReaded && fiducialTempReaded){
                        strList.append(name);
                        stream << ("name=\"" + name + "\"\n").toUtf8();
                        stream << "lineExp=" + linearExp + "\n";
                        stream << "fiducTemp=" + fiducialTemp + "\n";
                        }
                    break;
                }
                tempStr.clear();
                materialsFileStat = READING_PARAMS;
            }
        }
        file.close();
        }else{
        if(file.exists()){
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            }
        }
        file.remove();
        uint listSize = strList.count();
        materialsFileStat = READING_PARAMS;
        if(mergedFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            while(!mergedFile.atEnd()){
                mergedFile.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((flangeMaterialFileParam = checkFlangeMaterialsFileParams(tempStr)) == NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(mergedFile, cnf);
                        tempFile.close();
                        tempFile.remove();
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                        }else{
                            materialsFileStat = READ_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&mergedFile, &tempChar);
                    switch(flangeMaterialFileParam){
                    case FLANGE_NAME:
                        nameReaded = false;
                        linearExpReaded = false;
                        fiducialTempReaded = false;
                        name = tempStr;
                        if(nameCheck(name))
                            nameReaded = true;
                      break;
                    case FLANGE_LINEAR_EXPANSTION:
                        if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, false)){
                            linearExpReaded = true;
                            linearExp = tempStr;
                        }
                        break;
                    case FLANGE_FIDUCIAL_TEMP:
                        if(FormatableData<FlangeMaterialsFileParams>::checkStringFormat(tempStr, true)){
                            fiducialTempReaded = true;
                            fiducialTemp = tempStr;
                        }
                        if(nameReaded && linearExpReaded && fiducialTempReaded){
                            for(uint i = 0;  i < listSize; i++){
                                if(App::compareStr(strList.value(i), name)){
                                    name.append("_Moved_");
                                    for(uint j = 1; j < 1000; j++){
                                        if(!App::compareStr(strList.value(i), name + QString::number(j))){
                                            name = name + QString::number(j);
                                            break;
                                        }
                                    }
                                }else{
                                    break;
                                }
                            }
                            stream << ("name=\"" + name + "\"\n").toUtf8();
                            strList.append(name);
                            stream << "lineExp=" + linearExp + "\n";
                            stream << "fiducTemp=" + fiducialTemp + "\n";
                        }
                        break;
                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                }
            }
            mergedFile.close();
        }else{
            if(mergedFile.exists()){
                MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
            }
        }
        tempFile.close();
        tempFile.rename(QDir::toNativeSeparators(file.fileName()));
        }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
        }
    return true;
}

bool MaterialsInfoFiles::readPipeMaterials(InnerWindowCEP *innerWindowCEP){
    QFile file(innerWindowCEP->getParent()->getApplication()->getConfiguration()->getDefaultPath() + QString(APP_NAME) + "\\" + PIPE_MATERIALS_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        bool nameReaded = false;
        QString name = QString();
        bool linearExpReaded = false;
        bool fiducialTempReaded = false;
        char tempChar = '\0';
        MaterialsFileStat materialsFileStat = READING_PARAMS;
        PipeMaterialsFileParams pipeMaterialFileParam = PIPE_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((pipeMaterialFileParam = checkPipeMaterialsFileParams(tempStr)) == NUMB_OF_PIPE_MATERIALS_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *innerWindowCEP->getParent()->getApplication()->getConfiguration());
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    file.close();
                    return false;
                }else{
                   materialsFileStat = READ_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(pipeMaterialFileParam){
                case PIPE_NAME:
                    nameReaded = false;
                    linearExpReaded = false;
                    fiducialTempReaded = false;
                    name = tempStr;
                    if(nameCheck(name))
                        nameReaded = true;
                    break;
                case PIPE_LINEAR_EXPANSTION:
                    if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, false))
                        linearExpReaded = true;
                    break;
                case PIPE_FIDUCIAL_TEMP:
                    if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, true))
                        fiducialTempReaded = true;
                    if(nameReaded && linearExpReaded && fiducialTempReaded)
                        innerWindowCEP->getMeasurementDeviceMaterialList()->addItem(name);
                    break;
                }
                materialsFileStat = READING_PARAMS;
                tempStr.clear();
          }

     }
        file.close();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
      }
    return true;
  }


bool MaterialsInfoFiles::readMaterials(MaterialsChangesWidget<PipeMaterialsFileParams> *materialsChangesWidget){
    QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +PIPE_MATERIALS_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        bool nameReaded = false;
        QString name = QString();
        bool linearExpReaded = false;
        bool fiducialTempReaded = false;
        char tempChar = '\0';
        MaterialsFileStat materialsFileStat = READING_PARAMS;
        PipeMaterialsFileParams pipeMaterialFileParam = PIPE_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((pipeMaterialFileParam = checkPipeMaterialsFileParams(tempStr)) == NUMB_OF_PIPE_MATERIALS_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    file.close();
                    return false;
                }else{
                   materialsFileStat = READ_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(pipeMaterialFileParam){
                case PIPE_NAME:
                    nameReaded = false;
                    linearExpReaded = false;
                    fiducialTempReaded = false;
                    name = tempStr;
                    if(nameCheck(name))
                        nameReaded = true;
                    break;
                case PIPE_LINEAR_EXPANSTION:
                    if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, false))
                        linearExpReaded = true;
                    break;
                case PIPE_FIDUCIAL_TEMP:
                    if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, true))
                        fiducialTempReaded = true;
                    if(nameReaded && linearExpReaded && fiducialTempReaded)
                        materialsChangesWidget->getList()->addItem(name);
                    break;
                }
                materialsFileStat = READING_PARAMS;
                tempStr.clear();
            }
        }
        file.close();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
      }
    return true;
  }


bool MaterialsInfoFiles::addMaterial(MaterialsChangesWidget<PipeMaterialsFileParams> *materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + PIPE_MATERIALS_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            char tempChar = '\0';
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar > 0)
                    stream << tempChar;
                else
                    stream << App::readCharUtf8(&file, &tempChar);
            }
            stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
            stream << "lineExp=" + materialsChangesWidget->getLinearExpanstionFDF()->getDataUTextEdit()->text() + "\n";
            stream << "fiducTemp=" + materialsChangesWidget->getFiducialTemperatureFDF()->getDataUTextEdit()->text() + "\n";
            file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + PIPE_MATERIALS_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();

   }
    return true;
  }

bool MaterialsInfoFiles::editMaterial(MaterialsChangesWidget<PipeMaterialsFileParams> *materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + PIPE_MATERIALS_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool edit = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            PipeMaterialsFileParams pipeMaterialFileParam = PIPE_NAME;
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((pipeMaterialFileParam = checkPipeMaterialsFileParams(tempStr)) == NUMB_OF_PIPE_MATERIALS_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych. Zedytowane dane zostały zapisane w nowym pliku."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        tempFile.close();
                        tempFile.remove();
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
                        stream << "lineExp=" + materialsChangesWidget->getLinearExpanstionFDF()->getDataUTextEdit()->text() + "\n";
                        stream << "fiducTemp=" + materialsChangesWidget->getFiducialTemperatureFDF()->getDataUTextEdit()->text() + "\n";
                        file.close();
                        return false;
                    }else{
                       materialsFileStat = READ_VALUE;
                       tempStr.clear();
                    }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(pipeMaterialFileParam){
                    case PIPE_NAME:
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            edit = true;
                        else
                            edit = false;
                        if(!edit)
                            stream << ("name=\"" + tempStr + "\"\n").toUtf8();
                        else{
                            stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
                            stream << "lineExp=" + materialsChangesWidget->getLinearExpanstionFDF()->getDataUTextEdit()->text() + "\n";
                            stream << "fiducTemp=" + materialsChangesWidget->getFiducialTemperatureFDF()->getDataUTextEdit()->text() + "\n";
                         }
                        break;
                    case PIPE_LINEAR_EXPANSTION:
                        if(!edit)
                            stream << "lineExp=" + tempStr + "\n";
                        break;
                    case PIPE_FIDUCIAL_TEMP:
                        if(!edit)
                            stream << "fiducTemp=" + tempStr + "\n";
                        break;
                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;

                }
            }
                file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +PIPE_MATERIALS_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();

   }
    return true;
  }


bool MaterialsInfoFiles::removeMaterial(MaterialsChangesWidget<PipeMaterialsFileParams> *materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + PIPE_MATERIALS_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool remove = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            PipeMaterialsFileParams pipeMaterialFileParam = PIPE_NAME;
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((pipeMaterialFileParam = checkPipeMaterialsFileParams(tempStr)) == NUMB_OF_PIPE_MATERIALS_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        tempFile.close();
                        tempFile.remove();
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                    }else{
                       materialsFileStat = READ_VALUE;
                       tempStr.clear();
                    }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(pipeMaterialFileParam){
                    case PIPE_NAME:
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            remove = true;
                        else
                            remove = false;
                        if(!remove)
                            stream << ("name=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case PIPE_LINEAR_EXPANSTION:
                        if(!remove)
                            stream << "lineExp=" + tempStr + "\n";
                        break;
                    case PIPE_FIDUCIAL_TEMP:
                        if(!remove)
                            stream << "fiducTemp=" + tempStr + "\n";
                        break;
                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                }
            }
                file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + PIPE_MATERIALS_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();

   }
    return true;
  }

bool MaterialsInfoFiles::readMaterialParams(MaterialsChangesWidget<PipeMaterialsFileParams> *materialsChangesWidget){
    QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +PIPE_MATERIALS_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool read = false;
            bool stop = false;
            bool nameReaded = false;
            QString name = QString();
            QString fiducialTemp;
            QString linearExp;
            bool linearExpReaded = false;
            bool fiducialTempReaded = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            PipeMaterialsFileParams pipeMaterialFileParam = PIPE_NAME;
            while(!(file.atEnd() || stop)){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((pipeMaterialFileParam = checkPipeMaterialsFileParams(tempStr)) == NUMB_OF_PIPE_MATERIALS_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                    }else{
                       materialsFileStat = READ_VALUE;
                       tempStr.clear();
                    }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(pipeMaterialFileParam){
                    case PIPE_NAME:
                        if(read == true){
                            stop = true;
                            break;
                        }
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            read = true;
                        else
                            read = false;
                        if(read){
                            name = tempStr;
                            if(nameCheck(name))
                                nameReaded = true;
                        }
                    break;
                    case PIPE_LINEAR_EXPANSTION:
                        if(read){
                            if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, false)){
                                linearExpReaded = true;
                                linearExp = tempStr;
                            }
                         }
                        break;
                    case PIPE_FIDUCIAL_TEMP:
                        if(read){
                            if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, true)){
                                fiducialTempReaded = true;
                                fiducialTemp = tempStr;
                            }
                            if(nameReaded && linearExpReaded && fiducialTempReaded){
                                materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->setText(name);
                                materialsChangesWidget->getFiducialTemperatureFDF()->getDataUTextEdit()->setText(fiducialTemp);
                                materialsChangesWidget->getLinearExpanstionFDF()->getDataUTextEdit()->setText(linearExp);
                            }else{
                                MyDialog dlg(QUESTION_DIALOG, QString("Błąd pliku"), QString("Dane na temat wybranej substancji zostały uszkodzone."), false, QString("Ok"), QString(), QString(),nullptr);
                                dlg.setWindowModality(Qt::ApplicationModal);
                                dlg.show();
                                dlg.exec();
                                file.close();
                                return false;
                            }
                        }
                        break;
                    }
                    materialsFileStat = READING_PARAMS;
                    tempStr.clear();
                }
            }
                file.close();
            }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
                return false;
            }
    return true;
  }

bool MaterialsInfoFiles::mergePipeMaterialsFiles(QFile &file, QFile &mergedFile, Config &cnf){
    QFile tempFile("temp");
    QStringList strList;
    if(tempFile.exists())
        tempFile.remove();
    QString tempStr =  QString();
    char tempChar = '\0';
    bool nameReaded = false;
    QString name = QString();
    QString fiducialTemp;
    QString linearExp;
    bool linearExpReaded = false;
    bool fiducialTempReaded = false;
    MaterialsFileStat materialsFileStat = READING_PARAMS;
    PipeMaterialsFileParams pipeMaterialFileParam = PIPE_NAME;
    // Otwarcie Pliku Tymczasowego do zapisu
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
     QTextStream stream(&tempFile);
     stream.setCodec("UTF-8");
    // Otwarcie Pliku File do odczytu
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((pipeMaterialFileParam = checkPipeMaterialsFileParams(tempStr)) == NUMB_OF_PIPE_MATERIALS_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, cnf);
                    tempFile.close();
                    tempFile.remove();
                    if(!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
                        return false;
                }else{
                   materialsFileStat = READ_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(pipeMaterialFileParam){
                case PIPE_NAME:
                    nameReaded = false;
                    linearExpReaded = false;
                    fiducialTempReaded = false;
                    name = tempStr;
                    if(nameCheck(name))
                        nameReaded = true;
                   break;
                case PIPE_LINEAR_EXPANSTION:
                    if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, false)){
                        linearExpReaded = true;
                        linearExp = tempStr;
                    }
                    break;
                case PIPE_FIDUCIAL_TEMP:
                    if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, true)){
                        fiducialTempReaded = true;
                        fiducialTemp = tempStr;
                    }
                    if(nameReaded && linearExpReaded && fiducialTempReaded){
                        strList.append(name);
                        stream << ("name=\"" + name + "\"\n").toUtf8();
                        stream << "lineExp=" + linearExp + "\n";
                        stream << "fiducTemp=" + fiducialTemp + "\n";
                        }
                    break;
                }
                tempStr.clear();
                materialsFileStat = READING_PARAMS;
            }
        }
        file.close();
        }else{
        if(file.exists()){
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            }
        }
        file.remove();
        uint listSize = strList.count();
        materialsFileStat = READING_PARAMS;
        if(mergedFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            while(!mergedFile.atEnd()){
                mergedFile.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((pipeMaterialFileParam = checkPipeMaterialsFileParams(tempStr)) == NUMB_OF_PIPE_MATERIALS_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(mergedFile, cnf);
                        tempFile.close();
                        tempFile.remove();
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                        }else{
                            materialsFileStat = READ_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&mergedFile, &tempChar);
                    switch(pipeMaterialFileParam){
                    case PIPE_NAME:
                        nameReaded = false;
                        linearExpReaded = false;
                        fiducialTempReaded = false;
                        name = tempStr;
                        if(nameCheck(name))
                            nameReaded = true;
                      break;
                    case PIPE_LINEAR_EXPANSTION:
                        if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, false)){
                            linearExpReaded = true;
                            linearExp = tempStr;
                        }
                        break;
                    case PIPE_FIDUCIAL_TEMP:
                        if(FormatableData<PipeMaterialsFileParams>::checkStringFormat(tempStr, true)){
                            fiducialTempReaded = true;
                            fiducialTemp = tempStr;
                        }
                        if(nameReaded && linearExpReaded && fiducialTempReaded){
                            for(uint i = 0;  i < listSize; i++){
                                if(App::compareStr(strList.value(i), name)){
                                    name.append("_Moved_");
                                    for(uint j = 1; j < 1000; j++){
                                        if(!App::compareStr(strList.value(i), name + QString::number(j))){
                                            name = name + QString::number(j);
                                            break;
                                        }
                                    }
                                }else{
                                    break;
                                }
                            }
                            stream << ("name=\"" + name + "\"\n").toUtf8();
                            strList.append(name);
                            stream << "lineExp=" + linearExp + "\n";
                            stream << "fiducTemp=" + fiducialTemp + "\n";
                        }
                        break;
                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                }
            }
            mergedFile.close();
        }else{
            if(mergedFile.exists()){
                MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
            }
        }
        tempFile.close();
        tempFile.rename(QDir::toNativeSeparators(file.fileName()));
        }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
        }
    return true;
}

bool MaterialsInfoFiles::readSubstances(InnerWindowCEP* innerWindowCEP){
    QFile file(innerWindowCEP->getParent()->getApplication()->getConfiguration()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        QString substanceName = QString();
        QString substanceStat;
        bool nameR = false;
        bool statR = false;
        bool denPR = false;
        bool viscoPR = false;
        char tempChar = '\0';
        MaterialsFileStat materialsFileStat = READING_PARAMS;
        SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *innerWindowCEP->getParent()->getApplication()->getConfiguration());
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    file.close();
                    return false;
                    }else{
                        materialsFileStat = READ_VALUE;
                        tempStr.clear();
                    }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(substancesFileParam){
                case SUBSTANCE_NAME:
                    nameR = false;
                    statR = false;
                    denPR = false;
                    viscoPR = false;
                    substanceName = tempStr;
                    if(nameCheck(substanceName))
                        nameR = true;
                    break;
                case SUBSTANCE_STAT:
                    substanceStat = tempStr;
                    if(App::compareStr(substanceStat, "Ciecz") || App::compareStr(substanceStat, "Gaz"))
                        statR = true;
                    break;
                case SUBSTANCE_DENSITY_FILE_PATH:
                    if(QFile(tempStr).exists())
                        denPR = true;
                    break;
                case SUBSTANCE_VISCO_FILE_PATH:
                    if(QFile(tempStr).exists())
                        viscoPR = true;
                    if(nameR && statR && (denPR || viscoPR)){
                        innerWindowCEP->getFluidTypeList()->addItem(substanceName + " (" + substanceStat + ")");
                    }
                    break;
                }
                materialsFileStat = READING_PARAMS;
                tempStr.clear();
            }
        }
        file.close();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
      }
    return true;
  }


bool MaterialsInfoFiles::readMaterials(MaterialsChangesWidget<SubstancesParamsFileParam> *materialsChangesWidget){
    QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        QString substanceName = QString();
        QString substanceStat;
        bool nameR = false;
        bool statR = false;
        bool denPR = false;
        bool viscoPR = false;
        char tempChar = '\0';
        MaterialsFileStat materialsFileStat = READING_PARAMS;
        SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    file.close();
                    return false;
                    }else{
                        materialsFileStat = READ_VALUE;
                        tempStr.clear();
                    }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(substancesFileParam){
                case SUBSTANCE_NAME:
                    nameR = false;
                    statR = false;
                    denPR = false;
                    viscoPR = false;
                    substanceName = tempStr;
                    if(nameCheck(substanceName))
                        nameR = true;
                    break;
                case SUBSTANCE_STAT:
                    substanceStat = tempStr;
                    if(App::compareStr(substanceStat, "Ciecz") || App::compareStr(substanceStat, "Gaz"))
                        statR = true;
                    break;
                case SUBSTANCE_DENSITY_FILE_PATH:
                    if(QFile(tempStr).exists())
                        denPR = true;
                    break;
                case SUBSTANCE_VISCO_FILE_PATH:
                    if(QFile(tempStr).exists())
                        viscoPR = true;
                    if(nameR && statR && (denPR || viscoPR)){
                        materialsChangesWidget->getList()->addItem(substanceName + " (" + substanceStat + ")");
                    }
                    break;
                }
                materialsFileStat = READING_PARAMS;
                tempStr.clear();
            }
        }
        file.close();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
      }
    return true;
  }


bool MaterialsInfoFiles::addMaterial(MaterialsChangesWidget<SubstancesParamsFileParam> *materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            char tempChar = '\0';
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar > 0)
                    stream << tempChar;
                else
                    stream << App::readCharUtf8(&file, &tempChar);
            }
            stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
            if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                 stream << "stat=\"Ciecz\"\n";
            else
                 stream << "stat=\"Gaz\"\n";
            stream << ("denPath=\"" + materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +  QString("Fluids\\")  + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()).toUtf8();
            if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                stream << "Fluid";
            else
                stream << "Gase";
            stream << "Density" + QString(RESOURCES_FILE_EXTENSTION) + "\"\n";
            stream << ("viscoPath=\"" + materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()).toUtf8();
            if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                stream << "Fluid";
            else
                stream << "Gase";
            stream << "Visco" + QString(RESOURCES_FILE_EXTENSTION) + "\"\n";
            file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
      return false;
    }
    return true;
  }

bool MaterialsInfoFiles::editMaterial(MaterialsChangesWidget<SubstancesParamsFileParam> *materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool edit = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych. Zedytowane dane zostały zapisane w nowym pliku."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        tempFile.close();
                        tempFile.remove();
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
                        if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                            stream << "stat=\"Ciecz\"\n";
                        else
                            stream << "stat=\"Gaz\"\n";
                        stream << ("denPath=\"" + materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +  QString("Fluids\\") + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()).toUtf8();
                        if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                            stream << "Fluid";
                        else
                            stream << "Gase";
                        stream << "Density" + QString(RESOURCES_FILE_EXTENSTION) + "\"\n";
                        stream << ("viscoPath=\"" + materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +  QString("Fluids\\") + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()).toUtf8();
                        if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                            stream << "Fluid";
                        else
                            stream << "Gase";
                        stream << "Visco" + QString(RESOURCES_FILE_EXTENSTION) + "\"\n";
                        file.close();
                        return false;
                        }else{
                            materialsFileStat = READ_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(substancesFileParam){
                    case SUBSTANCE_NAME:
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            edit = true;
                        else
                            edit = false;
                        if(!edit)
                            stream << ("name=\"" + tempStr + "\"\n").toUtf8();
                        else{
                            stream << ("name=\"" + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text() + "\"\n").toUtf8();
                            if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                                stream << "stat=\"Ciecz\"\n";
                            else
                                stream << "stat=\"Gaz\"\n";
                            stream << ("denPath=\"" + materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +  QString("Fluids\\") + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()).toUtf8();
                            if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                                stream << "Fluid";
                            else
                                stream << "Gase";
                            stream << "Density" + QString(RESOURCES_FILE_EXTENSTION) + "\"\n";
                            stream << ("viscoPath=\"" + materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +  QString("Fluids\\") + materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()).toUtf8();
                            if(materialsChangesWidget->getFluidTypeCheckBox()->isChecked())
                                stream << "Fluid";
                            else
                                stream << "Gase";
                            stream << "Visco" + QString(RESOURCES_FILE_EXTENSTION) + "\"\n";
                           }
                        break;
                    case SUBSTANCE_STAT:
                        if(!edit)
                        {
                            stream<< "stat=\"" + tempStr + "\"\n";
                        }
                        break;
                    case SUBSTANCE_DENSITY_FILE_PATH:
                        if(!edit)
                            stream << ("denPath=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case SUBSTANCE_VISCO_FILE_PATH:
                        if(!edit)
                            stream << ("viscoPath=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                }
            }
                file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
      return false;
       }
    return true;
  }


bool MaterialsInfoFiles::removeMaterial(MaterialsChangesWidget<SubstancesParamsFileParam> *materialsChangesWidget){
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool remove = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        tempFile.close();
                        tempFile.remove();
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                        }else{
                            materialsFileStat = READ_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(substancesFileParam){
                    case SUBSTANCE_NAME:
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            remove = true;
                        else
                            remove = false;
                        if(!remove)
                            stream << ("name=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case SUBSTANCE_STAT:
                        if(!remove)
                            stream << "stat=\"" + tempStr + "\"\n";
                        break;
                    case SUBSTANCE_VISCO_FILE_PATH:
                        if(!remove)
                            stream << ("viscoPath=\"" + tempStr + "\"\n").toUtf8();
                        else
                        {
                            QFile(QDir::toNativeSeparators(tempStr)).remove();

                        }
                        break;
                    case SUBSTANCE_DENSITY_FILE_PATH:
                        if(!remove)
                            stream << ("denPath=\"" + tempStr + "\"\n").toUtf8();
                        else
                            QFile(QDir::toNativeSeparators(tempStr)).remove();
                        break;

                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                }
            }
                file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
            tempFile.close();
            file.remove();
            tempFile.rename(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" + SUBSTANCES_FILE);
        }
   else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
      return false;
       }
    return true;
  }


bool MaterialsInfoFiles::readSubstanceParams(QString substanceToRead, ProjectInfo* projectInfo){
   QFile file(projectInfo->getParent()->getConfiguration()->getDefaultPath() + QString(APP_NAME) + "\\" +FLANGE_MATERIALS_FILE);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool read = false;
            bool stop = false;
            QString substanceName = QString();
            QString substanceStat;
            QString densityPath;
            QString viscoPath;
            bool nameR = false;
            bool statR = false;
            bool denPR = false;
            bool viscoPR = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
            while(!(file.atEnd() || stop)){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *projectInfo->getParent()->getConfiguration());
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                        }else{
                            materialsFileStat = READ_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(substancesFileParam){
                    case SUBSTANCE_NAME:
                        if(read){
                            stop = true;
                            break;
                        }
                        if(App::compareStr(substanceToRead, tempStr))
                            read = true;
                        else
                            read = false;
                        if(read){
                            substanceName = tempStr;
                            if(nameCheck(substanceName))
                                nameR = true;
                        }
                        break;
                    case SUBSTANCE_STAT:
                        if(read){
                            if(App::compareStr(tempStr, "Gaz") || App::compareStr(tempStr, "Ciecz")){
                                statR = true;
                                substanceStat = tempStr;
                            }
                        }
                        break;
                    case SUBSTANCE_DENSITY_FILE_PATH:
                        if(read){
                            if(QFile(tempStr).exists()){
                                denPR = true;
                                densityPath = tempStr;
                                }
                        }
                        break;
                    case SUBSTANCE_VISCO_FILE_PATH:
                        if(read){
                            if(QFile(tempStr).exists()){
                                viscoPR = true;
                                viscoPath = tempStr;
                            }
                            if(nameR && statR && (denPR || viscoPR)){
                                projectInfo->setFluidName(new QString(substanceName));
                                projectInfo->setSubstanceStat(new QString(substanceStat));
                                projectInfo->setSubstanceDensityPath(new QString(densityPath));
                                projectInfo->setSubstanceViscoPath(new QString(viscoPath));
                               }else{
                                MyDialog dlg(QUESTION_DIALOG, QString("Błąd pliku"), QString("Dane na temat wybranej substancji zostały uszkodzone."), false, QString("Ok"), QString(), QString(),nullptr);
                                dlg.setWindowModality(Qt::ApplicationModal);
                                dlg.show();
                                dlg.exec();
                                file.close();
                                return false;
                            }
                        }
                        break;
                     }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                    break;
                }
             }
                file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
   return true;
}

bool MaterialsInfoFiles::readMaterialParams(MaterialsChangesWidget<SubstancesParamsFileParam> *materialsChangesWidget){
   QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            bool read = false;
            bool stop = false;
            QString substanceName = QString();
            QString substanceStat;
            QString densityPath;
            QString viscoPath;
            bool nameR = false;
            bool statR = false;
            bool denPR = false;
            bool viscoPR = false;
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
            while(!(file.atEnd() || stop)){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                        file.open(QIODevice::WriteOnly | QIODevice::Text);
                        file.close();
                        return false;
                        }else{
                            materialsFileStat = READ_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(substancesFileParam){
                    case SUBSTANCE_NAME:
                        if(read){
                            stop = true;
                            break;
                        }
                        if(App::compareStr(materialsChangesWidget->getSelectedFromList(), tempStr))
                            read = true;
                        else
                            read = false;
                        if(read){
                            substanceName = tempStr;
                            if(nameCheck(substanceName))
                                nameR = true;
                        }
                        break;
                    case SUBSTANCE_STAT:
                        if(read){
                            if(App::compareStr(tempStr, "Gaz") || App::compareStr(tempStr, "Ciecz")){
                                statR = true;
                                substanceStat = tempStr;
                            }

                        }
                        break;
                    case SUBSTANCE_DENSITY_FILE_PATH:
                        if(read){
                            if(QFile(tempStr).exists()){
                                denPR = true;
                                densityPath = tempStr;
                                }
                        }
                        break;
                    case SUBSTANCE_VISCO_FILE_PATH:
                        if(read){
                            if(QFile(tempStr).exists()){
                                viscoPR = true;
                                viscoPath = tempStr;
                            }
                            if(nameR && statR && (denPR || viscoPR)){
                                if(materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text().isEmpty())
                                    materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->setText(substanceName);
                                if(!materialsChangesWidget->getFirstRead())
                                {
                                    if(App::compareStr("Ciecz", substanceStat))
                                        materialsChangesWidget->getFluidTypeCheckBox()->setChecked(true);
                                    else{
                                        materialsChangesWidget->getGaseTypeCheckBox()->setChecked(true);
                                        materialsChangesWidget->getFluidTypeCheckBox()->setChecked(false);
                                     }
                                }
                                QFile fileD(densityPath);
                                materialsChangesWidget->getDensityDataHolder()->readFromFile(&fileD);
                                QFile fileV(viscoPath);
                                materialsChangesWidget->getDynamicViscosityDataHolder()->readFromFile(&fileV);
                            }else{
                                MyDialog dlg(QUESTION_DIALOG, QString("Błąd pliku"), QString("Dane na temat wybranej substancji zostały uszkodzone."), false, QString("Ok"), QString(), QString(),nullptr);
                                dlg.setWindowModality(Qt::ApplicationModal);
                                dlg.show();
                                dlg.exec();
                                file.close();
                                return false;
                            }
                        }
                        break;
                     }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                    break;
                }
             }
                file.close();
            }else{
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
                return false;
            }
   return true;
}

FlangeMaterialsFileParams MaterialsInfoFiles::checkFlangeMaterialsFileParams(QString tempStr){
    QString nameStr = QString();
    uint i = 0;
    switch(tempStr.length()){
    case 4:
        // Sprawdzanie dla słowa "name"
        nameStr.append("name");
        for( ; i < 4; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 4)
            return FLANGE_NAME;
        break;
    case 7:
        // Sprawdzanie dla słowa "lineExp"
        nameStr.append("lineExp");
        for( ; i < 7; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 7)
            return FLANGE_LINEAR_EXPANSTION;
        break;
    case 9:
        // Sprawdzanie dla słowa "fiducTemp"
        nameStr.append("fiducTemp");
        for( ; i < 9; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 9)
            return FLANGE_FIDUCIAL_TEMP;
        break;
    default:
        return NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS;
    }
    return NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS;
}

PipeMaterialsFileParams MaterialsInfoFiles::checkPipeMaterialsFileParams(QString tempStr){
    QString nameStr = QString();
    uint i = 0;
    switch(tempStr.length()){
    case 4:
        // Sprawdzanie dla słowa "name"
        nameStr.append("name");
        for( ; i < 4; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 4)
            return PIPE_NAME;
        break;
    case 7:
        // Sprawdzanie dla słowa "lineExp"
        nameStr.append("lineExp");
        for( ; i < 7; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 7)
            return PIPE_LINEAR_EXPANSTION;
        break;
    case 9:
        // Sprawdzanie dla słowa "fiducTemp"
        nameStr.append("fiducTemp");
        for( ; i < 9; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 9)
            return PIPE_FIDUCIAL_TEMP;
        break;
    default:
        return NUMB_OF_PIPE_MATERIALS_FILE_PARAMS;
    }
    return NUMB_OF_PIPE_MATERIALS_FILE_PARAMS;
}

SubstancesFileParams MaterialsInfoFiles::checkSubstancesFileParams(QString tempStr){
    QString nameStr = QString();
    uint i = 0;
    switch(tempStr.length()){
    case 4:
        // Sprawdzanie dla słowa "name"
        nameStr.append("name");
        for( ; i < 4; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 4)
            return SUBSTANCE_NAME;
        nameStr.clear();
        // Sprawdzanie dla slowa "stat"
        nameStr.append("stat");
        for(i = 0; i < 4 ; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 4)
            return SUBSTANCE_STAT;
        break;
    case 7:
        // Sprawdzanie dla słowa "denPath"
        nameStr.append("denPath");
        for(i = 0; i < 7; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 7)
            return SUBSTANCE_DENSITY_FILE_PATH;
        break;
    case 9:
        // Sprawdzanie dla słowa "viscoPath"
        nameStr.append("viscoPath");
        for(i = 0; i < 9; i++)
            if(nameStr.at(i) != tempStr.at(i))
                break;
        if(i == 9)
            return SUBSTANCE_VISCO_FILE_PATH;
        break;
    default:
        return NUMB_OF_SUBSTANCES_FILE_PARAMS;
    }
    return NUMB_OF_SUBSTANCES_FILE_PARAMS;
}

bool MaterialsInfoFiles::readSubstanceForFluidStat(InnerWindowCEP *win){
    bool densityNotReady = false;
    bool viscoNotReady = false;
    int ans = 0;
    switch(*win->getProjectInfo()->getFluidType()){
    case 0:
    {
        win->getSubstanceNameDataLabel()->setText("Woda");
        win->getSubstanceTypeDataLabel()->setText("Ciecz");
        // Odczyt i interpolacja gęstości dla wody
        QFile file(QString(RESOURCES_FOLDER_PATH) + "Application\\waterFluidDensity.rsc");
        ans = MaterialsInfoFiles::readAndInterpolateForWaterFluid(&file, win, true);
        switch(ans){
        case -1:
        case -2:
            densityNotReady = true;
            win->getProjectInfo()->setFluidDensityFromFile(nullptr);
            win->getProjectInfo()->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
        // Odczyt i interpolacja lepkości dla wody
        file.setFileName(QString(RESOURCES_FOLDER_PATH) + "Application\\waterFluidVisco.rsc");
        ans = MaterialsInfoFiles::readAndInterpolateForWaterFluid(&file, win, false);
        switch(ans){
        case -1:
        case -2:
            viscoNotReady = true;
            win->getProjectInfo()->setFluidDynamicViscosityFromFile(nullptr);
            win->getProjectInfo()->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
    }
        break;
    case 1:
    {
        win->getSubstanceNameDataLabel()->setText("Para wodna");
        win->getSubstanceTypeDataLabel()->setText("Gaz");
        // Odczyt i interpolacja gęstości dla
        QFile file(QString(RESOURCES_FOLDER_PATH) + "Application\\waterGaseDensity.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, true);
        switch(ans){
        case -1:
        case -2:
            densityNotReady = true;
            win->getProjectInfo()->setFluidDensityFromFile(nullptr);
            win->getProjectInfo()->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
        // Odczyt i interpolacja lepkości dla
        file.setFileName(QString(RESOURCES_FOLDER_PATH) + "Application\\waterGaseVisco.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, false);
        switch(ans){
        case -1:
        case -2:
            viscoNotReady = true;
            win->getProjectInfo()->setFluidDynamicViscosityFromFile(nullptr);
            win->getProjectInfo()->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
    }
        break;
    case 2:
    {
        win->getSubstanceNameDataLabel()->setText("Powietrze");
        win->getSubstanceTypeDataLabel()->setText("Gaz");
        // Odczyt i interpolacja gęstości dla powietrza
        QFile file(QString(RESOURCES_FOLDER_PATH) + "Application\\airGaseDensity.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, true);
        switch(ans){
        case -1:
        case -2:
            densityNotReady = true;
            win->getProjectInfo()->setFluidDensityFromFile(nullptr);
            win->getProjectInfo()->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
        // Odczyt i interpolacja lepkości dla powietrza
        file.setFileName(QString(RESOURCES_FOLDER_PATH) + "Application\\airGaseVisco.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, false);
        switch(ans){
        case -1:
        case -2:
            viscoNotReady = true;
            win->getProjectInfo()->setFluidDynamicViscosityFromFile(nullptr);
            win->getProjectInfo()->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
    }
        break;
    case 3:
    {
        win->getSubstanceNameDataLabel()->setText("Dwutlenek węgla");
        win->getSubstanceTypeDataLabel()->setText("Gaz");
        // Odczyt i interpolacja gęstości dla dwutlenku wegla
        QFile file(QString(RESOURCES_FOLDER_PATH) + "Application\\carbonDioxideGaseDensity.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, true);
        switch(ans){
        case -1:
        case -2:
            densityNotReady = true;
            win->getProjectInfo()->setFluidDensityFromFile(nullptr);
            win->getProjectInfo()->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
        // Odczyt i interpolacja lepkości dla dwutlenku wegla
        file.setFileName(QString(RESOURCES_FOLDER_PATH) + "Application\\carbonDioxideGaseVisco.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, false);
        switch(ans){
        case -1:
        case -2:
            viscoNotReady = true;
            win->getProjectInfo()->setFluidDynamicViscosityFromFile(nullptr);
            win->getProjectInfo()->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
    }
        break;
    case 4:
    {
        win->getSubstanceNameDataLabel()->setText("Wodór");
        win->getSubstanceTypeDataLabel()->setText("Gaz");
        // Odczyt i interpolacja gęstości dla wodoru
        QFile file(QString(RESOURCES_FOLDER_PATH) + "Application\\hydrogenGaseDensity.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, true);
        switch(ans){
        case -1:
        case -2:
            densityNotReady = true;
            win->getProjectInfo()->setFluidDensityFromFile(nullptr);
            win->getProjectInfo()->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
        // Odczyt i interpolacja lepkości dla wodoru
        file.setFileName(QString(RESOURCES_FOLDER_PATH) + "Application\\hydrogenGaseVisco.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, false);
        switch(ans){
        case -1:
        case -2:
            viscoNotReady = true;
            win->getProjectInfo()->setFluidDynamicViscosityFromFile(nullptr);
            win->getProjectInfo()->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
    }
        break;
    case 5:
    {
        win->getSubstanceNameDataLabel()->setText("Azot");
        win->getSubstanceTypeDataLabel()->setText("Gaz");
        // Odczyt i interpolacja gęstości dla azotu
        QFile file(QString(RESOURCES_FOLDER_PATH) + "Application\\nitrogenGaseDensity.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, true);
        switch(ans){
        case -1:
        case -2:
            densityNotReady = true;
            win->getProjectInfo()->setFluidDensityFromFile(nullptr);
            win->getProjectInfo()->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
        // Odczyt i interpolacja lepkości dla azotu
        file.setFileName(QString(RESOURCES_FOLDER_PATH) + "Application\\nitrogenGaseVisco.rsc");
        ans = MaterialsInfoFiles::readAndInterpolate(&file, win, false);
        switch(ans){
        case -1:
        case -2:
            viscoNotReady = true;
            win->getProjectInfo()->setFluidDynamicViscosityFromFile(nullptr);
            win->getProjectInfo()->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
            break;
        default:
            break;
        }
    }
        break;
    default:
    {
        QString fluidName = QString();
        QString fluidType = QString();
        QString substanceDensityFilePath = QString();
        QString substanceViscoFilePath = QString();
        uint number = *win->getProjectInfo()->getFluidType() - 6;
        if(!MaterialsInfoFiles::readSubstanceForInterpolate(win,&fluidName, &fluidType, &substanceDensityFilePath, &substanceViscoFilePath, number))
            return false;
        win->getSubstanceNameDataLabel()->setText(fluidName);
        win->getSubstanceTypeDataLabel()->setText(fluidType);
        // Odczyt i interpolacja gęstości dla
        QFile file(substanceDensityFilePath);
        if(substanceDensityFilePath.isEmpty() || !file.exists()){
            densityNotReady = true;
            win->getProjectInfo()->setFluidDensityFromFile(nullptr);
            win->getProjectInfo()->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
        }else{
            ans = MaterialsInfoFiles::readAndInterpolate(&file, win, true);
            switch(ans){
            case -1:
            case -2:
                densityNotReady = true;
                win->getProjectInfo()->setFluidDensityFromFile(nullptr);
                win->getProjectInfo()->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
                break;
            default:
                break;
            }
        }
        // Odczyt i interpolacja lepkości dla
        file.setFileName(substanceViscoFilePath);
        if(substanceViscoFilePath.isEmpty() || !file.exists()){
            viscoNotReady = true;
            win->getProjectInfo()->setFluidDynamicViscosityFromFile(nullptr);
            win->getProjectInfo()->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
        }else{
            ans = MaterialsInfoFiles::readAndInterpolate(&file, win, false);
            switch(ans){
            case -1:
            case -2:
                viscoNotReady = true;
                win->getProjectInfo()->setFluidDynamicViscosityFromFile(nullptr);
                win->getProjectInfo()->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
                break;
            default:
                break;
            }
        }
    }
        break;
    }
    if(densityNotReady && viscoNotReady)
        return false;
    return true;
}

int MaterialsInfoFiles::readAndInterpolate(QFile *file, InnerWindowCEP *win, bool densityRead){
    double myTemp = 0;
    double myPres = 0;
    if(win->getProjectInfo()->getTemp() != nullptr){
        switch(*win->getProjectInfo()->getTempUnits()){
        case 0 : // Stopnie Celciusza
            myTemp = *win->getProjectInfo()->getTemp()->getData();
            break;
        case 1: // Kelwiny
            myTemp = KELWIN_TO_CELCIUS_CONV(*win->getProjectInfo()->getTemp()->getData());
            break;
        default:
            break;
        }
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString("Brak temperatury płynu"), QString("Podaj temperaturę płynu."), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return -1;
    }
    if(win->getProjectInfo()->getMaxUpstreamPressure()){
        switch(*win->getProjectInfo()->getMaxUpstreamPressureUnits()){
        case 0: // MPa
            myPres = 1000000 * *win->getProjectInfo()->getMaxUpstreamPressure()->getData();
            break;
        case 1: // kPa
            myPres = 1000 * *win->getProjectInfo()->getMaxUpstreamPressure()->getData();
            break;
        case 2: // Bar
            myPres = 100000 * *win->getProjectInfo()->getMaxUpstreamPressure()->getData();
            break;
        default:
            break;
        }
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString("Brak ciśnienia dopływowego"), QString("Podaj ciśnienie strony dopływowej."), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return -1;
    }
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_MATERIALS_FILE_TITLE), QString(ERROR_CANT_OPEN_MATERIALS_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return -1;
    }else{
        double prevPres = 0;
        double pres = 0;
        SubstancesParamsFileParam fileParam = DEPENDENCY;
        bool prevTempNotFound = false;
        double prevMinTemp = 0;
        double prevMaxTemp = 0;
        double minTemp = 0;
        double maxTemp = 0;
        double prevMinTempData = 0;
        double prevMaxTempData = 0;
        double minTempData = 0;
        double maxTempData = 0;
        short k = 1;
        ushort zal = 1;
        char tempChar = '\0';
        {
        MaterialsFileStat fileStat = READING_PARAMS;
        QString tempStr = QString();
        uint pressureCounter = 0;
        uint tempCounter = 0;
        bool findT = false;
        bool dontRead = false;
        bool setDontRead = false;
        bool dataFound = false;
        bool stop = false;
        uint pressureUnit;
        while(!(file->atEnd() || stop)){
                file->getChar(&tempChar);
                if(tempChar == '\n' || tempChar == 20)
                    continue;
                switch(fileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                        if((uint)(fileParam = checkFileParam(&tempStr)) == NUMB_OF_SUBSTANCES_PARAMS_FILE_PARAM){
                            MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                            dlg.setWindowModality(Qt::ApplicationModal);
                            dlg.show();
                            dlg.exec();
                            CorruptedFilesMenager::addCorruptedFile(*file, *win->getParent()->getApplication()->getConfiguration());
                            return -1;
                        }else {
                            fileStat = READ_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = readValue(file, &tempChar);
                    switch(fileParam){
                    case PRESSURE_UNIT:
                        if(pressureUnit < 5)
                            pressureUnit = tempStr.toUShort();
                        else{
                            MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                            dlg.setWindowModality(Qt::ApplicationModal);
                            dlg.show();
                            dlg.exec();
                            CorruptedFilesMenager::addCorruptedFile(*file, *win->getParent()->getApplication()->getConfiguration());
                            return -1;
                        }
                        break;
                    case DEPENDENCY:
                        zal = tempStr.toUShort();
                        if(zal < 3){
                            if(zal == 1)
                                findT = true;
                        }else{
                            MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                            dlg.setWindowModality(Qt::ApplicationModal);
                            dlg.show();
                            dlg.exec();
                            CorruptedFilesMenager::addCorruptedFile(*file, *win->getParent()->getApplication()->getConfiguration());
                            return -1;
                        }
                        break;
                    case K_VALUE:
                        k = tempStr.toShort();
                        break;
                    case PRESSURE:
                    {
                        if(dontRead)
                            dontRead = false;
                        if(findT){
                            file->close();
                            return -2;
                        }
                        prevPres = pres;
                        prevMinTemp = minTemp;
                        prevMaxTemp = maxTemp;
                        prevMaxTempData = maxTempData;
                        prevMinTempData = minTempData;
                        minTemp = 0;
                        maxTemp = 0;
                        maxTempData = 0;
                        minTempData = 0;
                        pres = [tempStr, densityRead, pressureUnit](uint fluidType)->double{switch(fluidType){case 1: if(densityRead) return AT_TO_PASCAL_CONV(tempStr.toDouble()); else return BAR_TO_PASCAL_CONV(tempStr.toDouble()); break; case 2: if(densityRead) return PSI_TO_PASCAL_CONV(tempStr.toDouble()) + 101325; else return tempStr.toDouble(); break; case 3: if(densityRead) return BAR_TO_PASCAL_CONV(tempStr.toDouble()); else return tempStr.toDouble(); break; case 4: if(densityRead) return ATM_TO_PASCAL_CONV(tempStr.toDouble()); else return tempStr.toDouble(); case 5: if(densityRead) return ATM_TO_PASCAL_CONV(tempStr.toDouble()); else return tempStr.toDouble();default: switch(pressureUnit){case 0: return AT_TO_PASCAL_CONV(tempStr.toDouble()); case 1: return ATM_TO_PASCAL_CONV(tempStr.toDouble()); case 2: return BAR_TO_PASCAL_CONV(tempStr.toDouble()); case 3: return PSI_TO_PASCAL_CONV(tempStr.toDouble()); case 4: return KPASCAL_TO_PASCAL_CONV(tempStr.toDouble());}}return 1;}(*win->getProjectInfo()->getFluidType());
                        if(myPres <= pres){
                            if(pressureCounter != 0){
                                findT = true;
                                if(myPres == pres){
                                    prevPres = pres;
                                    findT = true;
                                    prevTempNotFound = false;
                                }
                            }else{
                                if(myPres == pres){
                                    prevPres = pres;
                                    findT = true;
                                    prevTempNotFound = false;
                                }
                                else
                                    stop = true;
                            }
                        }
                        pressureCounter++;
                        tempCounter = 0;
                    }
                        break;
                    case SUBSTANCE_TEMP:
                    {
                        if(!dontRead){
                            minTemp = maxTemp;
                            minTempData = maxTempData;
                            maxTemp = [tempStr, densityRead](uint fluidType)->double{switch(fluidType){case 4: if(densityRead) return KELWIN_TO_CELCIUS_CONV(tempStr.toDouble()); else return tempStr.toDouble(); break; case 5: if(densityRead) return KELWIN_TO_CELCIUS_CONV(tempStr.toDouble()); else return tempStr.toDouble(); default: return tempStr.toDouble();}}(*win->getProjectInfo()->getFluidType());
                            if(myTemp <= maxTemp){
                                setDontRead = true;
                                if(findT){
                                    dataFound = true;
                                }else{
                                    prevTempNotFound = false;                                    
                                }
                                if(tempCounter == 0){                                    
                                   if(myTemp == maxTemp){                                       
                                        minTemp = maxTemp;
                                        tempCounter--;
                                    }else{
                                       prevTempNotFound = true;
                                        setDontRead = false;
                                        dontRead = true;
                                    }
                                }else{
                                    if(myTemp == maxTemp){
                                         minTemp = maxTemp;
                                    }
                                }
                           }
                            tempCounter++;
                    }
                    }
                        break;
                    case DENSITY:                        
                    case VISCOSITY:
                        if(!dontRead){
                            maxTempData = tempStr.toDouble();
                            if(setDontRead){
                                dontRead = true;
                                setDontRead = false;
                                if(tempCounter == 0){
                                    minTempData = maxTempData;
                                    tempCounter++;
                                }else{
                                    if(minTemp == maxTemp)
                                        minTempData = maxTempData;
                                }
                                if(dataFound)
                                    stop = true;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                    fileStat = READING_PARAMS;
                    tempStr.clear();
                    break;
                }
        }
            if(!findT){
                file->close();
                return -2;
            }
            if(!dataFound){
                file->close();
                return -2;
            }
            if(prevTempNotFound){
                file->close();
                return -2;
            }
      }
    switch(*win->getProjectInfo()->getFluidType()){
        case 0:
            if(fileParam == VISCOSITY)
            {
                prevMaxTempData = CP_TO_PAS_CONV(prevMaxTempData);
                prevMinTempData = CP_TO_PAS_CONV(prevMinTempData);
                minTempData = CP_TO_PAS_CONV(minTempData);
                maxTempData = CP_TO_PAS_CONV(maxTempData);
            }
            break;
        case 1:
        if(fileParam == VISCOSITY){
            prevMaxTempData = CP_TO_PAS_CONV(prevMaxTempData);
            prevMinTempData = CP_TO_PAS_CONV(prevMinTempData);
            minTempData = CP_TO_PAS_CONV(minTempData);
            maxTempData = CP_TO_PAS_CONV(maxTempData);
        }
            break;
        case 3:
        if(fileParam == DENSITY){
            prevMaxTempData = prevMaxTempData * 1000;
            prevMinTempData = prevMinTempData * 1000;
            minTempData = minTempData * 1000;
            maxTempData = maxTempData *1000;
        }
        break;
        default:
            break;
        }
        prevMaxTempData *= pow(10, k);
        prevMinTempData *= pow(10, k);
        minTempData *= pow(10, k);
        maxTempData *= pow(10, k);
        double tempResult = 0;
       if(zal == 2){
           double wspPsz;
           if(pres != prevPres)
               wspPsz = (myPres - prevPres)/(pres - prevPres);
           else
               wspPsz = 0;
          double wspTsz;
          if(maxTemp != minTemp)
                wspTsz  = (myTemp - minTemp)/(maxTemp - minTemp);
          else
              wspTsz = 0;
          if(wspPsz == 0 && wspTsz == 0){
              prevMinTempData = minTempData;
          }else{
              if(wspPsz == 0){
                  prevMinTempData = minTempData;
                  prevMaxTempData = maxTempData;
              }
          }
          if(fileParam == DENSITY)
              win->getProjectInfo()->setFluidDensityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_DENSITY_FROM_FILE);}(BILINEAR_INTERPOLATION_EQ(wspPsz, wspTsz)));
          else
              win->getProjectInfo()->setFluidDynamicViscosityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_VISCO_FROM_FILE);}(BILINEAR_INTERPOLATION_EQ(wspPsz, wspTsz)));
       }else{
           double wspTsz = 0;
           if(maxTemp != minTemp)
                wspTsz = (myTemp - minTemp)/(maxTemp - minTemp);
           else
               wspTsz = 0;
           if(fileParam == DENSITY)
                win->getProjectInfo()->setFluidDensityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_DENSITY_FROM_FILE);}(LINEAR_INTERPOLATION_EQ(wspTsz)));
            else
                win->getProjectInfo()->setFluidDynamicViscosityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_VISCO_FROM_FILE);}(LINEAR_INTERPOLATION_EQ(wspTsz)));
       }
       file->close();
       return 0;
    }
}

SubstancesParamsFileParam MaterialsInfoFiles::checkFileParam(QString* tempStr){
     QString nameStr = QString();
        uint i = 0;
        switch(tempStr->length()){
        case 1:
        {
            QChar tempChar = tempStr->at(0);
            if(tempChar == QChar('k'))
                return K_VALUE;
            if(tempChar == QChar('T'))
                return SUBSTANCE_TEMP;
            if(tempChar == QChar('p'))
                return PRESSURE;
            if(tempChar == QChar('n'))
                return VISCOSITY;
            if(tempChar == QChar('r'))
                return DENSITY;
        }
            break;
        case 3:
            nameStr.append("zal");
            for(i = 0; i < 3; i++)
                if(tempStr->at(i) != nameStr.at(i))
                    break;
            if(i == 3)
                return DEPENDENCY;
            break;
        }
        return NUMB_OF_SUBSTANCES_PARAMS_FILE_PARAM;
}

QString MaterialsInfoFiles::readValue(QFile* file, char*tempCharFunc){
    QString tempStrFunc = QString();
    QString sign;
    QByteArray byteArr = QByteArray();
      if(*tempCharFunc == '\"')
          while(!file->atEnd()){
              file->getChar(tempCharFunc);
              if(*tempCharFunc > 0){
                  if(*tempCharFunc != '\"')
                      tempStrFunc.append(*tempCharFunc);
                  else
                      break;
              }else{
                  byteArr.append(*tempCharFunc);
                  if(*tempCharFunc > -96)
                  {
                      file->getChar(tempCharFunc);
                      byteArr.append(*tempCharFunc);
                  }else{
                      if(*tempCharFunc > -112){
                          for(uint i = 0; i < 2; i++){
                              file->getChar(tempCharFunc);
                              byteArr.append(*tempCharFunc);
                          }
                      }else{
                          if(*tempCharFunc > -120){
                              for(uint i = 0; i < 3; i++){
                                  file->getChar(tempCharFunc);
                                  byteArr.append(*tempCharFunc);
                              }
                          }else{
                              if(*tempCharFunc > -124){
                                  for(uint i = 0; i < 4; i++){
                                      file->getChar(tempCharFunc);
                                      byteArr.append(*tempCharFunc);
                                  }
                              }else{
                                  for(uint i = 0; i < 5; i++){
                                      file->getChar(tempCharFunc);
                                      byteArr.append(*tempCharFunc);
                                  }
                              }
                          }
                      }
                  }
                  sign.clear();
                  sign = QString(byteArr);
                  tempStrFunc.append(sign);
                  byteArr.clear();
              }
          }
      else{
          tempStrFunc.append(*tempCharFunc);
          while(!file->atEnd()){
              file->getChar(tempCharFunc);
              if(*tempCharFunc != ' ' && *tempCharFunc != '\n')
                {
                  tempStrFunc.append(*tempCharFunc);
                  }
              else
                {
                  break;
                }
          }
      }
      return tempStrFunc;
}

int MaterialsInfoFiles::readAndInterpolateForWaterFluid(QFile *file, InnerWindowCEP *win, bool findDensity){
double myTemp = 0;
double myPres = 0;
if(win->getProjectInfo()->getTemp() != nullptr){
    switch(*win->getProjectInfo()->getTempUnits()){
    case 0 : // Stopnie Celciusza
        myTemp = *win->getProjectInfo()->getTemp()->getData();
        break;
    case 1: // Kelwiny
        myTemp = KELWIN_TO_CELCIUS_CONV(*win->getProjectInfo()->getTemp()->getData());
        break;
    default:
        break;
    }
}else{
    MyDialog dlg(QUESTION_DIALOG, QString("Brak temperatury płynu"), QString("Podaj temperaturę płynu."), false, QString("Ok"), QString(), QString(), nullptr);
    dlg.setWindowModality(Qt::ApplicationModal);
    dlg.show();
    dlg.exec();
    return -1;
}
if(win->getProjectInfo()->getMaxUpstreamPressure() != nullptr){
    switch(*win->getProjectInfo()->getMaxUpstreamPressureUnits()){
    case 0: // MPa
        myPres = 1000000 * *win->getProjectInfo()->getMaxUpstreamPressure()->getData();
        break;
    case 1: // kPa
        myPres = 1000 * *win->getProjectInfo()->getMaxUpstreamPressure()->getData();
        break;
    case 2: // Bar
        myPres = 100000 * *win->getProjectInfo()->getMaxUpstreamPressure()->getData();
        break;
    default:
        break;
    }
}else{
    MyDialog dlg(QUESTION_DIALOG, QString("Brak ciśnienia dopływowego"), QString("Podaj ciśnienie strony dopływowej."), false, QString("Ok"), QString(), QString(), nullptr);
    dlg.setWindowModality(Qt::ApplicationModal);
    dlg.show();
    dlg.exec();
    return -1;
}
if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
    MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_MATERIALS_FILE_TITLE), QString(ERROR_CANT_OPEN_MATERIALS_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
    dlg.setWindowModality(Qt::ApplicationModal);
    dlg.show();
    dlg.exec();
    return -1;
}else{
    double prevPres = 0;
    double pres = 0;
    SubstancesParamsFileParam fileParam = DEPENDENCY;
    double prevMinTemp = 0;
    double prevMaxTemp = 0;
    double minTemp = 0;
    double maxTemp = 0;
    double prevMinTempData = 0;
    double prevMaxTempData = 0;
    double minTempData = 0;
    double maxTempData = 0;
    short k = 1;
    ushort zal = 1;
    char tempChar = '\0';
    bool myPresLessThenOne = false;
    if(findDensity)
    myPresLessThenOne = (myPres < AT_TO_PASCAL_CONV(1))? true : false;
    {
    MaterialsFileStat fileStat = READING_PARAMS;
    QString tempStr = QString();
    uint pressureCounter = 0;
    uint tempCounter = 0;
    bool dontRead = false;
    bool findT = false;
    bool setDontRead = false;
    bool dataFound = false;
    bool stop = false;
    bool prevTempNotFound = false;
    while(!(file->atEnd() || stop)){
            file->getChar(&tempChar);
            if(tempChar == '\n' || tempChar == 20)
                continue;
            switch(fileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                    if((uint)(fileParam = checkFileParam(&tempStr)) == NUMB_OF_SUBSTANCES_PARAMS_FILE_PARAM){
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(*file, *win->getParent()->getApplication()->getConfiguration());
                        return -1;
                    }else {
                        fileStat = READ_VALUE;
                        tempStr.clear();
                    }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = readValue(file, &tempChar);
                switch(fileParam){
                case DEPENDENCY:
                    zal = tempStr.toUShort();
                    if(zal < 3){
                        if(zal == 1)
                            findT = true;
                    }else{
                        MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                        CorruptedFilesMenager::addCorruptedFile(*file, *win->getParent()->getApplication()->getConfiguration());
                        return -1;
                    };
                    break;
                case K_VALUE:
                    k = tempStr.toShort();
                    break;
                case PRESSURE:
                {
                    if(myPresLessThenOne){
                        file->seek(339);
                        findT = true;
                        zal = 1;
                        myPresLessThenOne = false;
                    }
                    else{
                        if(pressureCounter == 0 && findDensity){
                            file->seek(339);
                            pres = AT_TO_PASCAL_CONV(tempStr.toDouble());
                            pressureCounter++;
                            break;
                        }
                        if(dontRead)
                            dontRead = false;
                    if(findT){
                        file->close();
                        return -2;
                    }
                    prevPres = pres;
                    prevMinTemp = minTemp;
                    prevMaxTemp = maxTemp;
                    prevMaxTempData = maxTempData;
                    prevMinTempData = minTempData;
                    minTemp = 0;
                    maxTemp = 0;
                    maxTempData = 0;
                    minTempData = 0;
                    pres = (findDensity)? AT_TO_PASCAL_CONV(tempStr.toDouble()) : BAR_TO_PASCAL_CONV(tempStr.toDouble());
                    if(myPres <= pres){
                        if(pressureCounter != 0){
                            findT = true;
                            if(myPres == pres){
                                prevPres = pres;
                                findT = true;
                                prevTempNotFound = false;
                            }
                        }else{
                            if(myPres == pres){
                                prevPres = pres;
                                findT = true;
                                 prevTempNotFound = false;
                            }
                            else
                                stop = true;
                        }
                    }
                    pressureCounter++;
                    tempCounter = 0;
                    }
                }
                break;
                case SUBSTANCE_TEMP:
                {
                    if(!dontRead){
                            minTemp = maxTemp;
                            minTempData = maxTempData;
                            maxTemp = tempStr.toDouble();
                            if(myTemp <= maxTemp){
                                setDontRead = true;
                                if(findT){
                                    dataFound = true;
                                }else{
                                    prevTempNotFound = false;
                                }
                                if(tempCounter == 0){
                                   if(myTemp == maxTemp){
                                        minTemp = maxTemp;
                                        tempCounter--;
                                    }else{
                                       prevTempNotFound = true;
                                        setDontRead = false;
                                        dontRead = true;
                                    }
                                }else{
                                    if(myTemp == maxTemp){
                                         minTemp = maxTemp;
                                    }
                                }
                           }
                            tempCounter++;
                    }
                 }
                 break;
                case DENSITY:                    
                case VISCOSITY:
                    if(!dontRead){
                        maxTempData = tempStr.toDouble();
                        if(setDontRead){
                            dontRead = true;
                            setDontRead = false;
                            if(tempCounter == 0){
                                minTempData = maxTempData;
                                tempCounter++;
                            }else{
                                if(minTemp == maxTemp)
                                    minTempData = maxTempData;
                            }
                            if(dataFound)
                                stop = true;
                        }
                    }
                    break;
                default:
                    break;
                }
                fileStat = READING_PARAMS;
                tempStr.clear();
                break;
            }
            }
        if(!findT){
            file->close();
            return -2;
        }
        if(!dataFound){
            file->close();
            return -2;
        }
        if(prevTempNotFound){
            file->close();
            return -2;
        }
   }
  if(fileParam == VISCOSITY)
      {
       prevMaxTempData = CP_TO_PAS_CONV(prevMaxTempData);
       prevMinTempData = CP_TO_PAS_CONV(prevMinTempData);
       minTempData = CP_TO_PAS_CONV(minTempData);
       maxTempData = CP_TO_PAS_CONV(maxTempData);
   }
    prevMaxTempData *= pow(10, k);
    prevMinTempData *= pow(10, k);
    minTempData *= pow(10, k);
    maxTempData *= pow(10, k);
    if(myPresLessThenOne){
        double wspTsz = (myTemp - minTemp)/(maxTemp - minTemp);
        if(fileParam == DENSITY)
            win->getProjectInfo()->setFluidDensityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_DENSITY_FROM_FILE);}(LINEAR_INTERPOLATION_EQ(wspTsz)));
        else
            win->getProjectInfo()->setFluidDynamicViscosityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_VISCO_FROM_FILE);}(LINEAR_INTERPOLATION_EQ(wspTsz)));

    }else{
   if(zal == 2){
       double wspPsz;
       if(pres != prevPres)
           wspPsz = (myPres - prevPres)/(pres - prevPres);
       else
           wspPsz = 0;
      double wspTsz;
      if(maxTemp != minTemp)
            wspTsz  = (myTemp - minTemp)/(maxTemp - minTemp);
      else
          wspTsz = 0;
      if(wspPsz == 0 && wspTsz == 0){
          prevMinTempData = minTempData;
      }else{
          if(wspPsz == 0){
              prevMinTempData = minTempData;
              prevMaxTempData = maxTempData;
          }
      }
      if(findDensity)
          win->getProjectInfo()->setFluidDensityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_DENSITY_FROM_FILE);}(BILINEAR_INTERPOLATION_EQ(wspPsz, wspTsz)));
      else
          win->getProjectInfo()->setFluidDynamicViscosityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_VISCO_FROM_FILE);}(BILINEAR_INTERPOLATION_EQ(wspPsz, wspTsz)));
   }else{
       double wspTsz = 0;
       if(maxTemp != minTemp)
        wspTsz = (myTemp - minTemp)/(maxTemp - minTemp);
       else
           wspTsz = 0;
        if(findDensity)
            win->getProjectInfo()->setFluidDensityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_DENSITY_FROM_FILE);}(LINEAR_INTERPOLATION_EQ(wspTsz)));
        else
            win->getProjectInfo()->setFluidDynamicViscosityFromFile([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_VISCO_FROM_FILE);}(LINEAR_INTERPOLATION_EQ(wspTsz)));
   }
  }
   file->close();
   return 0;
    }
}

int MaterialsInfoFiles::checkMaterialsNames(MaterialsChangesWidget<FlangeMaterialsFileParams> *materialsChangesWidget){
    QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +FLANGE_MATERIALS_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        char tempChar = '\0';
        MaterialsFileStat materialsFileStat = READING_PARAMS;
        FlangeMaterialsFileParams flangeMaterialFileParam = FLANGE_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((flangeMaterialFileParam = checkFlangeMaterialsFileParams(tempStr)) == NUMB_OF_FLANGE_MATERIALS_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    file.close();
                    return 1;
                }else{
                   materialsFileStat = READ_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(flangeMaterialFileParam){
                case FLANGE_NAME:
                    if(App::compareStr(tempStr, materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()))
                    {
                        file.close();
                        return 0;
                    }
                    break;
                default:
                    break;
                }
                materialsFileStat = READING_PARAMS;
                tempStr.clear();
            }
        }
        file.close();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return 3;
      }
    return 2;
}

int MaterialsInfoFiles::checkMaterialsNames(MaterialsChangesWidget<PipeMaterialsFileParams> *materialsChangesWidget){
QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +PIPE_MATERIALS_FILE);
if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
    QString tempStr =  QString();
    char tempChar = '\0';
    MaterialsFileStat materialsFileStat = READING_PARAMS;
    PipeMaterialsFileParams pipeMaterialFileParam = PIPE_NAME;
    while(!file.atEnd()){
        file.getChar(&tempChar);
        if(tempChar == '\n')
            continue;
        switch(materialsFileStat){
        case READING_PARAMS:
            if(tempChar == '='){
              if((pipeMaterialFileParam = checkPipeMaterialsFileParams(tempStr)) == NUMB_OF_PIPE_MATERIALS_FILE_PARAMS){
                MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
                CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                file.open(QIODevice::WriteOnly | QIODevice::Text);
                file.close();
                return 1;
                }else{
                    materialsFileStat = READ_VALUE;
                    tempStr.clear();
                }
            }else{
                tempStr.append(tempChar);
            }
            break;
        case READ_VALUE:
            tempStr = Config::readValue(&file, &tempChar);
            switch(pipeMaterialFileParam){
            case PIPE_NAME:
                if(App::compareStr(tempStr, materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()))
                {
                    file.close();
                    return 0;
                }
                break;
            default:
                break;
            }
            materialsFileStat = READING_PARAMS;
            tempStr.clear();
        }
    }
    file.close();
}else{
    MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
    dlg.setWindowModality(Qt::ApplicationModal);
    dlg.show();
    dlg.exec();
    return 3;
  }
return 2;
}

int MaterialsInfoFiles::checkMaterialsNames(MaterialsChangesWidget<SubstancesParamsFileParam> *materialsChangesWidget){
    QFile file(materialsChangesWidget->getConfig()->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        char tempChar = '\0';
        bool nameFound = false;
        MaterialsFileStat materialsFileStat = READING_PARAMS;
        SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(materialsFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                  if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *materialsChangesWidget->getConfig());
                    file.open(QIODevice::WriteOnly | QIODevice::Text);
                    file.close();
                    return 1;
                    }else{
                        materialsFileStat = READ_VALUE;
                        tempStr.clear();
                    }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READ_VALUE:
                tempStr = Config::readValue(&file, &tempChar);
                switch(substancesFileParam){
                case SUBSTANCE_NAME:
                   if(App::compareStr(tempStr, materialsChangesWidget->getNameTextEdit()->getDataUTextEdit()->text()))
                   {
                       nameFound = true;
                   }
                    break;
                case SUBSTANCE_STAT:
                    if(nameFound){
                        if(App::compareStr(tempStr, (materialsChangesWidget->getFluidTypeCheckBox()->isChecked())? "Ciecz" : "Gaz")){
                            file.close();
                            return 0;
                        }else{
                            file.close();
                            return 2;
                        }
                    }
                    break;
                default:
                    break;
                }
                materialsFileStat = READING_PARAMS;
                tempStr.clear();
            }
        }
        file.close();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return 3;
      }
    return 2;
}

int MaterialsInfoFiles::getNumbOfMaterials(QFile* file, Config &cnf){
    int numbOfMaterials = 0;
         if(file->open(QIODevice::ReadOnly | QIODevice::Text)){
             QString tempStr =  QString();
             char tempChar = '\0';
             MaterialsFileStat materialsFileStat = READING_PARAMS;
             SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
             while(!file->atEnd()){
                 file->getChar(&tempChar);
                 if(tempChar == '\n')
                     continue;
                 switch(materialsFileStat){
                 case READING_PARAMS:
                     if(tempChar == '='){
                       if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                         MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                         dlg.setWindowModality(Qt::ApplicationModal);
                         dlg.show();
                         dlg.exec();
                         CorruptedFilesMenager::addCorruptedFile(*file, cnf);
                         file->open(QIODevice::WriteOnly | QIODevice::Text);
                         file->close();
                         return -2;
                         }else{
                             materialsFileStat = READ_VALUE;
                             tempStr.clear();
                         }
                     }else{
                         tempStr.append(tempChar);
                     }
                     break;
                 case READ_VALUE:
                     tempStr = Config::readValue(file, &tempChar);
                     switch(substancesFileParam){
                     case SUBSTANCE_NAME:
                         numbOfMaterials++;
                         break;
                     default:
                         break;
                     }
                     tempStr.clear();
                     materialsFileStat = READING_PARAMS;
                     break;
                 }
              }
                 file->close();
                 delete file;
             }else{
                delete file;
             MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
             dlg.setWindowModality(Qt::ApplicationModal);
             dlg.show();
             dlg.exec();
                 return -1;
             }
    return numbOfMaterials;
}

bool MaterialsInfoFiles::readSubstanceForInterpolate(InnerWindowCEP* win, QString* fluidName, QString* fluidType, QString* substanceDensityFilePath, QString* substanceViscoFilePath, uint number){
    QFile file(win->getParent()->getApplication()->getConfiguration()->getDefaultPath() + QString(APP_NAME) + "\\" + SUBSTANCES_FILE);
         if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
             QString tempStr =  QString();
             uint counter = 0;
             char tempChar = '\0';
             bool read = false;
             bool stop = false;
             QString substanceName = QString();
             QString substanceStat;
             QString densityPath;
             QString viscoPath;
             bool nameR = false;
             bool statR = false;
             bool denPR = false;
             bool viscoPR = false;
             MaterialsFileStat materialsFileStat = READING_PARAMS;
             SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
             while(!(file.atEnd() || stop)){
                 file.getChar(&tempChar);
                 if(tempChar == '\n')
                     continue;
                 switch(materialsFileStat){
                 case READING_PARAMS:
                     if(tempChar == '='){
                       if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                         MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                         dlg.setWindowModality(Qt::ApplicationModal);
                         dlg.show();
                         dlg.exec();
                         CorruptedFilesMenager::addCorruptedFile(file, *win->getParent()->getApplication()->getConfiguration());
                         file.open(QIODevice::WriteOnly | QIODevice::Text);
                         file.close();
                         return false;
                         }else{
                             materialsFileStat = READ_VALUE;
                             tempStr.clear();
                         }
                     }else{
                         tempStr.append(tempChar);
                     }
                     break;
                 case READ_VALUE:
                     tempStr = Config::readValue(&file, &tempChar);
                     switch(substancesFileParam){
                     case SUBSTANCE_NAME:
                         if(read)
                             stop = true;
                         if(counter == number)
                             read = true;
                         else
                             read = false;
                         counter++;
                         if(read){
                             substanceName = tempStr;
                             if(nameCheck(substanceName))
                                 nameR = true;

                         }
                         break;
                     case SUBSTANCE_STAT:
                         if(read){
                             if(App::compareStr(tempStr, "Gaz") || App::compareStr(tempStr, "Ciecz")){
                                 statR = true;
                                 substanceStat = tempStr;
                             }

                         }
                         break;
                     case SUBSTANCE_DENSITY_FILE_PATH:
                         if(read){
                             denPR = true;
                             densityPath = tempStr;
                         }
                         break;
                     case SUBSTANCE_VISCO_FILE_PATH:
                         if(read){
                             viscoPR = true;
                             viscoPath = tempStr;
                             if(nameR && statR && (denPR || viscoPR)){
                                 *fluidName = substanceName;
                                 *fluidType = substanceStat;
                                 *substanceDensityFilePath = densityPath;
                                 *substanceViscoFilePath = viscoPath;
                             }else{
                                 MyDialog dlg(QUESTION_DIALOG, QString("Błąd pliku"), QString("Dane na temat wybranej substancji zostały uszkodzone."), false, QString("Ok"), QString(), QString(),nullptr);
                                 dlg.setWindowModality(Qt::ApplicationModal);
                                 dlg.show();
                                 dlg.exec();
                                 CorruptedFilesMenager::addCorruptedFile(file, *win->getParent()->getApplication()->getConfiguration());
                                 file.open(QIODevice::WriteOnly | QIODevice::Text);
                                 file.close();
                                 return false;
                             }
                         }
                         break;
                      }
                     tempStr.clear();
                     materialsFileStat = READING_PARAMS;
                     break;
                 }
              }
                 file.close();
             }else{
             MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
             dlg.setWindowModality(Qt::ApplicationModal);
             dlg.show();
             dlg.exec();
                 return false;
             }
    return true;
}

double* MaterialsInfoFiles::readAndInterpolateForMaterials(ProjectInfo *projectInfo, bool readPipeMaterial){
    double temp = TEMP_CONV(projectInfo);
    if(readPipeMaterial){
        switch(*projectInfo->getPipeMaterial()){
        case 0:
            if(temp >= 500) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(18 * pow(10, -6));
            else if(temp <= 20) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(0);
            else if(temp >= 400) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(400, 500, 17.5 * pow(10, -6), 18 * pow(10, -6)));
            else if(temp >= 300)return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(300, 400, 17 * pow(10, -6), 17.5 * pow(10, -6)));
            else if(temp >= 200) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(200, 300, 16.5 * pow(10, -6), 17 * pow(10, -6)));
            else if(temp >= 20) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(20, 200, 16 * pow(10, -6), 16.5 * pow(10, -6)));
            else return nullptr;
            break;
        case 1:
            return [](double set)->double*{double* ret = new double; *ret = set; return ret;}(10.8 * pow(10, -6));
          break;
        case 2:
            return [](double set)->double*{double* ret = new double; *ret = set; return ret;}(9.9 * pow(10, -6));
            break;
        case 3:
            return [](double set)->double*{double* ret = new double; *ret = set; return ret;}(14 * pow(10, -6));
            break;
        case 4:
            if(temp >= 500) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(96.1 * pow(10, -6));
            else if(temp <= -190) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(-28.4 * pow(10, -6));
            else if(temp >= 400) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(400, 500, 75.1 * pow(10, -6), 96.1 * pow(10, -6)));
            else if(temp >= 300)return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(300, 400, 55 * pow(10, -6), 75.1 * pow(10, -6)));
            else if(temp >= 200) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(200, 300, 35.8 * pow(10, -6), 55 * pow(10, -6)));
            else if(temp >= 100) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(100, 200, 17.5 * pow(10, -6), 35.8 * pow(10, -6)));
            else if(temp >= 20) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(20, 100, 0, 17.5 * pow(10, -6)));
            else if(temp > -190) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(-190, 20, -28.4 * pow(10, -6), 0));
            else return nullptr;
            break;
        case 5:
            if(temp >= 600) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(170 * pow(10, -6));
            else if(temp <= -253) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(-37.2 * pow(10, -6));
            else if(temp >= 500) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(500, 600, 137 * pow(10, -6), 170 * pow(10, -6)));
            else if(temp >= 400)return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(400, 500, 106 * pow(10, -6), 137 * pow(10, -6)));
            else if(temp >= 300) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(300, 400, 76.5 * pow(10, -6), 106 * pow(10, -6)));
            else if(temp >= 200) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(200, 300, 49 * pow(10, -6), 76.5 * pow(10, -6)));
            else if(temp >= 100) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(100, 200, 23.8, 49 * pow(10, -6)));
            else if(temp >= 50) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(50, 100, 11.7 * pow(10, -6), 23.8 * pow(10, -6)));
            else if(temp >= 0) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(0, 50, 0, 11.7 * pow(10, -6)));
            else if(temp >= -100) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(-100, 0, -22 * pow(10, -6), 0));
            else if(temp >= -190) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(-190, -100, -34.3 * pow(10, -6), -22 * pow(10, -6)));
            else if(temp > -253) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(-253, -190, -37.2 * pow(10, -6), -34.3 * pow(10, -6)));
            else return nullptr;
            break;
        default:
            break;
        }
    }else{
        switch(*projectInfo->getMeasurementDevice()){
        case 0:
            if(temp >= 500) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(18 * pow(10, -6));
            else if(temp <= 20) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(0);
            else if(temp >= 400) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(400, 500, 17.5 * pow(10, -6), 18 * pow(10, -6)));
            else if(temp >= 300)return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(300, 400, 17 * pow(10, -6), 17.5 * pow(10, -6)));
            else if(temp >= 200) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(200, 300, 16.5 * pow(10, -6), 17 * pow(10, -6)));
            else if(temp >= 20) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(20, 200, 16 * pow(10, -6), 16.5 * pow(10, -6)));
            else return nullptr;
        case 1:
            return [](double set)->double*{double* ret = new double; *ret = set; return ret;}(10.8 * pow(10, -6));
        case 2:
            return [](double set)->double*{double* ret = new double; *ret = set; return ret;}(9.9 * pow(10, -6));
        case 3:
            return [](double set)->double*{double* ret = new double; *ret = set; return ret;}(14 * pow(10, -6));
        case 4:
            if(temp >= 500) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(96.1 * pow(10, -6));
            else if(temp <= -190) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(-28.4 * pow(10, -6));
            else if(temp >= 400) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(400, 500, 75.1 * pow(10, -6), 96.1 * pow(10, -6)));
            else if(temp >= 300)return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(300, 400, 55 * pow(10, -6), 75.1 * pow(10, -6)));
            else if(temp >= 200) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(200, 300, 35.8 * pow(10, -6), 55 * pow(10, -6)));
            else if(temp >= 100) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(100, 200, 17.5 * pow(10, -6), 35.8 * pow(10, -6)));
            else if(temp >= 20) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(20, 100, 0, 17.5 * pow(10, -6)));
            else if(temp > -190) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(-190, 20, -28.4 * pow(10, -6), 0));
            else return nullptr;
        case 5:
            if(temp >= 600) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(170 * pow(10, -6));
            else if(temp <= -253) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}(-37.2 * pow(10, -6));
            else if(temp >= 500) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(500, 600, 137 * pow(10, -6), 170 * pow(10, -6)));
            else if(temp >= 400)return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(400, 500, 106 * pow(10, -6), 137 * pow(10, -6)));
            else if(temp >= 300) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(300, 400, 76.5 * pow(10, -6), 106 * pow(10, -6)));
            else if(temp >= 200) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(200, 300, 49 * pow(10, -6), 76.5 * pow(10, -6)));
            else if(temp >= 100) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(100, 200, 23.8, 49 * pow(10, -6)));
            else if(temp >= 50) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(50, 100, 11.7 * pow(10, -6), 23.8 * pow(10, -6)));
            else if(temp >= 0) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(0, 50, 0, 11.7 * pow(10, -6)));
            else if(temp >= -100) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(-100, 0, -22 * pow(10, -6), 0));
            else if(temp >= -190) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(-190, -100, -34.3 * pow(10, -6), -22 * pow(10, -6)));
            else if(temp > -253) return [](double set)->double*{double *ret = new double; *ret = set; return ret;}([temp](double minTemp, double maxTemp, double minTempData, double maxTempData)->double{double wspTemp = (temp - minTemp)/(maxTemp - minTemp); return LINEAR_INTERPOLATION_EQ(wspTemp);}(-253, -190, -37.2 * pow(10, -6), -34.3 * pow(10, -6)));
            else return nullptr;
        default:
            break;
        }
    }
    return nullptr;
}

bool MaterialsInfoFiles::rewriteSubstancesPaths(Config* cnf, bool start){
    QStringList filesList = QDir(cnf->getDefaultPath() + QString(APP_NAME + QString("\\Fluids\\"))).entryList(QStringList("*" + QString(RESOURCES_FILE_EXTENSTION)), QDir::Files, QDir::NoSort);
    uint fileListSize = filesList.count();
    bool *fileListApproved = nullptr;
    if(!filesList.isEmpty()){
        fileListApproved = new bool[filesList.count()];
        for(int i = 0; i < filesList.count(); i++)
            *(fileListApproved + i) = false;
    }
    QFile tempFile(TEMP_FILE);
    if(tempFile.exists())
        tempFile.remove();
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QString newPath = cnf->getDefaultPath() + QString(APP_NAME) + "\\Fluids\\";
        QString name;
        QString state;
        bool rewrite;
        bool fluidState = false;
        QFile file(cnf->getDefaultPath() + QString(APP_NAME) + "\\" + SUBSTANCES_FILE);
        QTextStream stream(&tempFile);
        stream.setCodec("UTF-8");
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString tempStr =  QString();
            char tempChar = '\0';
            MaterialsFileStat materialsFileStat = READING_PARAMS;
            SubstancesFileParams substancesFileParam = SUBSTANCE_NAME;
            while(!file.atEnd()){
                file.getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(materialsFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                      if((substancesFileParam = checkSubstancesFileParams(tempStr)) == NUMB_OF_SUBSTANCES_FILE_PARAMS){
                          if(!start){
                              MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                              dlg.setWindowModality(Qt::ApplicationModal);
                              dlg.show();
                              dlg.exec();
                              CorruptedFilesMenager::addCorruptedFile(file, *cnf);
                              tempFile.close();
                              tempFile.remove();
                              file.open(QIODevice::WriteOnly | QIODevice::Text);
                              file.close();
                          }
                              return false;
                        }else{
                            materialsFileStat = READ_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READ_VALUE:
                    tempStr = Config::readValue(&file, &tempChar);
                    switch(substancesFileParam){
                    case SUBSTANCE_NAME:
                        rewrite = false;
                        name = tempStr;                        
                        break;
                    case SUBSTANCE_STAT:
                    {
                        bool densityFound = false;
                        bool viscoFound = false;
                        if(App::compareStr("Ciecz", tempStr)){
                            state = "Fluid";
                            fluidState = true;
                        }
                        else{
                            state = "Gase";
                            fluidState = false;
                        }
                        for(uint i = 0; i < fileListSize; i++){
                            if(densityFound && viscoFound)
                                break;
                            if(!densityFound && App::compareStr(filesList.value(i), QString(name) + state + "Density" + RESOURCES_FILE_EXTENSTION)){
                                    rewrite = true;
                                    *(fileListApproved + i) = true;
                                    continue;
                            }
                            if(!viscoFound &&App::compareStr(filesList.value(i), QString(name) + state + "Visco" + RESOURCES_FILE_EXTENSTION)){
                                    rewrite = true;
                                    *(fileListApproved + i) = true;
                                    continue;
                            }
                        }
                       }
                        break;
                    case SUBSTANCE_DENSITY_FILE_PATH:                            
                        break;
                    case SUBSTANCE_VISCO_FILE_PATH:
                    if(rewrite){
                        stream << ("name=\"" + name + "\"\n").toUtf8();
                        stream<<  "stat=\"";
                        if(fluidState)
                            stream <<  QString("Ciecz");
                        else
                            stream << QString("Gaz");
                        stream << "\"\n";
                        stream << ("denPath=\"" + newPath + name + state + "Density" + RESOURCES_FILE_EXTENSTION + "\"\n").toUtf8();
                        stream << ("viscoPath=\"" + newPath + name + state + "Visco" + RESOURCES_FILE_EXTENSTION + "\"\n").toUtf8();
                    }
                        break;
                    }
                    tempStr.clear();
                    materialsFileStat = READING_PARAMS;
                }
            }
                file.close();
                for(uint i = 0; i < fileListSize; i++){
                    if(!(*(fileListApproved + i))){
                        QString fileStr = filesList.value(i);
                        bool fD = false;
                        bool fV = false;
                        bool gD = false;
                        bool gV = false;
                        int lenCheck = 0;
                        if(App::compareStr(fileStr.right(16), QString("FluidDensity") + RESOURCES_FILE_EXTENSTION)){
                            lenCheck = 16;
                            fD = true;
                        }else{
                            if(App::compareStr(fileStr.right(14), QString("FluidVisco") + RESOURCES_FILE_EXTENSTION)){
                                lenCheck = 14;
                                fV = true;
                            }else{
                                if(App::compareStr(fileStr.right(15),  QString("GaseDensity") + RESOURCES_FILE_EXTENSTION)){
                                    lenCheck = 15;
                                    gD = true;
                                }else{
                                    if( App::compareStr(fileStr.right(13), QString("GaseVisco") + RESOURCES_FILE_EXTENSTION)){
                                        lenCheck = 13;
                                        gV = true;
                                    }else{
                                        continue;
                                    }
                                }
                            }
                        }
                        if(fileStr.length() <= lenCheck)
                            continue;
                        fileStr.chop(4);
                        if(fD || gD){
                            fileStr.chop(7);
                        }
                        if(fV || gV){
                            fileStr.chop(5);
                        }
                        state.clear();
                        name.clear();
                        if(fD || fV){
                            state = fileStr.right(5);
                            fluidState = true;
                            fileStr.chop(5);
                        }
                        if(gD || gV){
                            state = fileStr.right(4);
                            fluidState = false;
                            fileStr.chop(4);
                        }
                        name = fileStr;
                        filesList.replace(i, name + state);
                        bool noSave = false;
                        for( uint j = 0; j < i; j++)
                            if(App::compareStr(filesList.value(i), filesList.value(j))){
                                noSave = true;
                                break;
                            }
                        if(noSave)
                            continue;
                        stream << ("name=\"" + name + "\"\n").toUtf8();
                        stream<<  "stat=\"";
                        if(fluidState)
                            stream <<  QString("Ciecz");
                        else
                            stream << QString("Gaz");
                        stream << "\"\n";
                        stream << ("denPath=\"" + newPath + name + state + "Density" + RESOURCES_FILE_EXTENSTION + "\"\n").toUtf8();
                        stream << ("viscoPath=\"" + newPath + name + state + "Visco" + RESOURCES_FILE_EXTENSTION + "\"\n").toUtf8();
                    }
                }
            }else{
            delete []fileListApproved;
            if(!start){
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            }
                return false;
            }
        delete [] fileListApproved;
            tempFile.close();
            file.remove();
            tempFile.rename(cnf->getDefaultPath() + QString(APP_NAME) + "\\" +SUBSTANCES_FILE);
        }
   else{
        if(!start){
        delete [] fileListApproved;
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_TEMP_FILE_TITLE), QString(ERROR_CANT_OPEN_TEMP_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        }
      return false;
       }
    return true;
}

bool MaterialsInfoFiles::nameCheck(QString &str){
    int len = str.length();
    QChar tempCh;
    for(int i = 0; i < len; i++){
        tempCh = str.at(i);
        if(tempCh == '\r' || tempCh == '\n' || tempCh == '\t' || tempCh == '\\'
                || tempCh == '/' || tempCh == ':' || tempCh == '*' || tempCh == '<'
                || tempCh == '>' || tempCh == '\"' || tempCh == '?' || tempCh == '|')
            return  false;
        }
    FormatableDataField<FlangeMaterialsFileParams> fdf(true, nullptr, false, nullptr, QString(), false);
    fdf.getDataUTextEdit()->setText(str);
    fdf.multiSpaceKeyDetection();
    if(fdf.getDataUTextEdit()->text().isEmpty())
        return false;
    return true;
}
