#include"config.hpp"
#include"configdataposition.hpp"
#include<QFile>
#include"errormacroes.hpp"
#include"appclass.hpp"
#include"appwindow.hpp"
#include<QTextStream>
#include<QDir>
#include"projectmenager.hpp"
#include"innerwindowcp.hpp"
#include"recentpanel.hpp"
#include"recentlist.hpp"
#include"recentelement.hpp"
#include"projectlist.hpp"
#include"projectlistelementop.hpp"
#include"projectlistelementdp.hpp"
#include"projectinfo.hpp"
#include<QDirIterator>
#include"materialsinfofiles.hpp"

Config::Config(App *setParent){
  // Przypisanie rodzica
  parent = setParent;
  configDataPosition = new ConfigDataPosition(this);
  autocadPath = QString();
  defaultPath = QString();
}

Config::~Config(){

}

void Config::start(){
    progressBarValue = new uint;
    progressBarMax = new uint;
    progressBarMin = new uint;
    *progressBarMax = 1;
    *progressBarMin = 0;
    *progressBarValue = 0;
    // Usuwanie pliku temp
    if(QFile(TEMP_FILE).exists())
      QFile(TEMP_FILE).remove();
    // Sprawdzenie istanienia pliku konfiguracyjnego lub ewentualne jego odzyskanie
    QFile file(CONFIG_FILE);
    if(!file.exists()){
        file.setFileName(CONFIG_BACK_UP_FILE);
        if(!file.exists())
          {
            *progressBarMax = 2;
            if(!createConfigFile())
            {
                constructingFail = true;
                return;
            }else{
                file.setFileName(CONFIG_FILE);
                *progressBarMax = 2 + file.size();
                *progressBarValue = 1;
                if(!createConfigBackUpFile()){
                    QFile::remove(CONFIG_FILE);
                    constructingFail = true;
                    return;
                }
                *progressBarValue = 2;
            }
           }
        else{
            *progressBarMax = 1 + (int)file.size();
            restoreConfig(&file);
            *progressBarValue = 1;
           }
      }
    *progressBarValue = 0;
    if(*progressBarMax == 1){
        *progressBarMax = (int)file.size() + 1;
    }    
    // Jeśli jest plik konfiguracyjny
    file.setFileName(CONFIG_FILE);
    checkProjectsAtStart(&file);
    MaterialsInfoFiles::rewriteSubstancesPaths(this, true);
    readAtStart(&file);
    *progressBarMax = 1;
    *progressBarValue = 1;
    parent->getMainWin()->setProgressBarSLMaxReady(true);
    delete progressBarValue;
    progressBarValue = nullptr;
    delete progressBarMax;
    progressBarMax = nullptr;
    delete progressBarMin;
    progressBarMin = nullptr;
}

void Config::readAtStart(QFile *file){
    bool rewrite = false;
    ushort tempRecentNumb = 0;
    ushort numberOfRecentsToRemove = 0;
    ushort *recentsToRemove = nullptr;
    uint numbOfRestore = 0;
    bool stop = false;
    file->setFileName(CONFIG_FILE);
    // Otiweranie pliku konfiguracyjnego
    if(file->open(QIODevice::ReadOnly | QIODevice::Text)){
        // Iniciowanie zmiennych odczytu danych
        char tempChar = '\0';

        QString tempStr = QString();
        unsigned long long pos = 0;
        unsigned long long counter = ULONG_LONG_MAX;
        unsigned long long projectPos = 0;
        uint projectsCounter = 0;
        uint recentCounter = 0;

        // Inicjowanie zmiennych sterujących
        ConfigFileStat configFileStat = SEARCHING;
        ConfigPackName *configPackName = nullptr;
        GeneralParamNames *generalParamNames = nullptr;
        ProjectsParamNames *projectsParamNames = nullptr;
        {
         while(numbOfRestore < 2){
             stop = false;
            // Odczyt pliku
            while(!file->atEnd() && !stop){
                if(file->getChar(&tempChar))
                    if(tempChar == '\n')
                        counter += 2;
                    else
                        counter++;
                else
                    continue;
                switch((uint) configFileStat){
                case (uint) SEARCHING:
                    switch(tempChar){
                    case  '[':
                        pos = counter;
                        configFileStat = READING_PACK_NAME;
                        break;
                    case '\n':
                        configFileStat = SEARCHING;
                        break;
                    default:
                        pos = counter;
                        configFileStat = READING_PARAMS_CONFIG;
                        tempStr.append(tempChar);
                        break;
                    }
                    break;
                case (uint) READING_PACK_NAME:
                    if(tempChar == ']'){
                        if(configPackName == nullptr)
                            configPackName = new ConfigPackName;
                        if((uint)(*configPackName = checkConfigPackName(&tempStr)) == NUMB_OF_CONFIG_PACK_NAME){
                            numbOfRestore++;
                            stop = true;
                            if(resetAndRestoreConfig(file)){
                                file->open(QIODevice::ReadOnly | QIODevice::Text);
                                configFileStat = SEARCHING;
                                tempStr.clear();
                                break;
                            }else{
                                delete configPackName;
                                configPackName = nullptr;
                                delete generalParamNames;
                                generalParamNames = nullptr;
                                delete projectsParamNames;
                                projectsParamNames = nullptr;
                                return;
                            }
                        }
                        switch((uint) *configPackName){
                        case (uint) GENERAL_CONFIG:
                            configDataPosition->setPackNamePositionAt(pos, 0);
                            break;
                        case (uint) PROJECTS:
                            configDataPosition->setPackNamePositionAt(pos, 1);
                            break;
                        case (uint) OTHER_PROJECTS:
                            configDataPosition->setPackNamePositionAt(pos, 2);
                            break;
                        }
                        tempStr.clear();
                        configFileStat = SEARCHING;
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case (uint) READING_PARAMS_CONFIG:
                    if(tempChar == '='){
                        if(configPackName)
                            switch((uint) *configPackName){
                            case (uint) GENERAL_CONFIG:
                                if(generalParamNames == nullptr)
                                    generalParamNames = new GeneralParamNames;
                                if((uint)(*generalParamNames = checkGeneralParamName(&tempStr)) == NUMB_OF_GENERAL_PARAM_NAMES){
                                    numbOfRestore++;
                                    stop = true;
                                    if(resetAndRestoreConfig(file)){
                                        file->setFileName(CONFIG_FILE);
                                        file->open(QIODevice::ReadOnly | QIODevice::Text);
                                        configFileStat = SEARCHING;
                                        tempStr.clear();
                                        break;
                                    }else{
                                        delete configPackName;
                                        configPackName = nullptr;
                                        delete generalParamNames;
                                        generalParamNames = nullptr;
                                        delete projectsParamNames;
                                        projectsParamNames = nullptr;
                                        return;
                                    }
                                }
                                tempStr.clear();
                                configFileStat = READING_VALUE;
                                break;
                            case (uint) PROJECTS:
                                if(projectsParamNames == nullptr)
                                    projectsParamNames = new ProjectsParamNames;
                                if((uint)(*projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                                    numbOfRestore++;
                                    stop =true;
                                    if(resetAndRestoreConfig(file)){
                                        file->setFileName(CONFIG_FILE);
                                        file->open(QIODevice::ReadOnly | QIODevice::Text);
                                        configFileStat = SEARCHING;
                                        tempStr.clear();
                                        break;
                                    }else{
                                        delete configPackName;
                                        configPackName = nullptr;
                                        delete generalParamNames;
                                        generalParamNames = nullptr;
                                        delete projectsParamNames;
                                        projectsParamNames = nullptr;
                                        return;
                                    }
                                }
                                tempStr.clear();
                                configFileStat = READING_VALUE;
                                break;
                            case (uint) OTHER_PROJECTS:
                                if(projectsParamNames == nullptr)
                                    projectsParamNames = new ProjectsParamNames;
                                if((uint)(*projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                                    numbOfRestore++;
                                    stop = true;
                                    if(resetAndRestoreConfig(file)){
                                        file->setFileName(CONFIG_FILE);
                                        file->open(QIODevice::ReadOnly | QIODevice::Text);
                                        configFileStat = SEARCHING;
                                        tempStr.clear();
                                        break;
                                    }else{
                                        delete configPackName;
                                        configPackName = nullptr;
                                        delete generalParamNames;
                                        generalParamNames = nullptr;
                                        delete projectsParamNames;
                                        projectsParamNames = nullptr;
                                        return;
                                    }
                                }
                                tempStr.clear();
                                configFileStat = READING_VALUE;
                                break;
                            }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case (uint) READING_VALUE:
                    switch((uint)*configPackName){
                    case (uint)GENERAL_CONFIG:
                        tempStr = readValueAtStart(*file, tempChar, &counter);
                        switch((uint)*generalParamNames){
                        case (uint)DEFAULT_PATH:
                            if(QFile(defaultPath).exists()){
                                defaultPath = QString(tempStr);
                            }else{
                                numbOfRestore++;
                                stop = true;
                                if(resetAndRestoreConfig(file)){
                                    file->setFileName(CONFIG_FILE);
                                    file->open(QIODevice::ReadOnly | QIODevice::Text);
                                    configFileStat = SEARCHING;
                                    tempStr.clear();
                                    break;
                                }else{
                                    delete configPackName;
                                    configPackName = nullptr;
                                    delete generalParamNames;
                                    generalParamNames = nullptr;
                                    delete projectsParamNames;
                                    projectsParamNames = nullptr;
                                    return;
                                }
                            }
                            tempStr.clear();
                            break;
                        case (uint)AUTOCAD_PATH:
                            autocadPath = QString(tempStr);
                            tempStr.clear();
                            break;
                        case (uint)NUMB_OF_PROJ:
                            numbOfProjects = tempStr.toUInt();
                            if(numbOfProjects != 0)
                                configDataPosition->createProjectsPosition((numbOfProjects * 0.05) + 1);// 0,05 jak dzielenie przez 20 projektów
                            tempStr.clear();
                            break;
                        case (uint)NUMB_OF_REC:
                            numbOfRecentProjects = tempStr.toUShort();
                            if(numbOfRecentProjects != 0)
                                configDataPosition->createRecentProjectsPosition(numbOfRecentProjects);
                            tempStr.clear();
                            break;
                        case (uint)SORTING_TYPE:
                            sortingType = (SortingType)tempStr.toUInt();
                            tempStr.clear();
                            break;
                        }
                        configFileStat = SEARCHING;
                        break;
                    default:
                        tempStr = readValueAtStart(*file, tempChar, &counter);
                        switch((uint)*projectsParamNames){
                        case (uint)PROJECT_NUMB:
                            projectPos = pos;
                            projectsCounter++;
                            if(projectsCounter % 20 == 0 || projectsCounter == 1){
                                if(projectsCounter > numbOfProjects){
                                    uint size = (projectsCounter - numbOfProjects) * 0.05 + 1;
                                    unsigned long long *temp = new unsigned long long[size];
                                    for(uint i = 0; i < size - 1; i++)
                                        *(temp + i) = *(configDataPosition->getProjectsPosition() + i);
                                    *(temp + size - 1) = pos;
                                    configDataPosition->setProjectsPosition(temp);
                                    rewrite = true;
                                    parent->getMainWin()->setProgressBarSLMaxReady(false);
                                    temp = nullptr;
                                }else{
                                    configDataPosition->setProjectsPositionAt(pos, projectsCounter * 0.05);
                                }
                            }
                            break;
                        case (uint)RECENT_NUMB:
                        {
                            ushort number = tempStr.toUShort();
                            tempRecentNumb = number;
                            if(number != 0){
                                recentCounter++;
                                if(number > numbOfRecentProjects){
                                    unsigned long long *temp = new unsigned long long[number];
                                    for(uint i = 0; i < numbOfRecentProjects; i++)
                                        *(temp + i) = *(configDataPosition->getRecentProjectsPosition() + i);
                                    *(temp + number - 1) = projectPos;
                                    configDataPosition->setRecentProjectsPosition(temp);
                                    numbOfRecentProjects = number;
                                    rewrite = true;
                                    parent->getMainWin()->setProgressBarSLMaxReady(false);
                                    temp = nullptr;
                                }else{
                                    configDataPosition->setRecentProjectsPositionAt(projectPos, number - 1);
                                }
                            }
                        }
                            break;
                        case (uint)PATH:
                        {
                            if(!QFile(tempStr).exists()){
                                if(tempRecentNumb != 0){
                                    numberOfRecentsToRemove++;
                                    if(recentsToRemove != nullptr){
                                        ushort *temp = new ushort[numberOfRecentsToRemove];
                                        for(ushort i = 0; i < (numberOfRecentsToRemove - 1); i++)
                                            *(temp + i) = *(recentsToRemove + i);
                                        *(temp + numberOfRecentsToRemove - 1) = tempRecentNumb;
                                        delete [](recentsToRemove);
                                        recentsToRemove = temp;
                                    }else{
                                        recentsToRemove = new ushort[1];
                                        *recentsToRemove = tempRecentNumb;
                                    }
                                }
                            }
                        }
                            break;
                        default:
                            break;
                        }
                        tempStr.clear();
                        configFileStat = SEARCHING;
                        break;
                    }
                    break;
                default:
                    numbOfRestore++;
                    stop = true;
                    if(resetAndRestoreConfig(file)){
                        delete generalParamNames;
                        generalParamNames = nullptr;
                        delete projectsParamNames;
                        projectsParamNames = nullptr;
                        file->setFileName(CONFIG_FILE);
                        if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            constructingFail = true;
                            return;
                        }
                        configFileStat = SEARCHING;
                        tempStr.clear();
                        break;
                    }else{
                        delete configPackName;
                        configPackName = nullptr;
                        delete generalParamNames;
                        generalParamNames = nullptr;
                        delete projectsParamNames;
                        projectsParamNames = nullptr;
                        return;
                    }
                }
                if(tempChar == '\n')
                    *progressBarValue += 2;
                else
                    *progressBarValue += 1;

            }
            if(projectsCounter != numbOfProjects){
                rewrite = true;
                numbOfProjects = projectsCounter;
            }
            if(recentCounter != numbOfRecentProjects){
                rewrite = true;
                numbOfRecentProjects = recentCounter;
            }
        if(configPackName && *configPackName == GENERAL_CONFIG)
        {
            numbOfRestore++;
            stop = true;
            file->close();
            file->remove();
            file->setFileName(CONFIG_BACK_UP_FILE);
            if(!restoreConfig(file))
            {
                if(!createConfigFile())
                {
                    return;
                }else{
                if(!createConfigBackUpFile())
                    return;
                }
                file->setFileName(CONFIG_FILE);
                if(file->open(QIODevice::ReadOnly | QIODevice::Text)){
                    configFileStat = SEARCHING;
                }else{
                constructingFail = true;
                return;
                }
            }else{
                if(!createConfigBackUpFile())
                    return;
                configFileStat = SEARCHING;
                file->setFileName(CONFIG_FILE);
                if(file->open(QIODevice::ReadOnly | QIODevice::Text)){
                    configFileStat = SEARCHING;
                }else{
                    constructingFail = true;
                    return;
                }
            }
        }else{
           numbOfRestore = 2;
           stop = true;
                }
            }
        }
        file->close();
        }else{
            constructingFail = true;
            return;
    }
    if(numbOfRestore < 2){
        if(recentsToRemove != nullptr){
            rewriteRecents(0, recentsToRemove, numberOfRecentsToRemove, true, true);
            delete []recentsToRemove;
            recentsToRemove = nullptr;
        }
        rewriteGeneral();
    }
}

void Config::checkProjectsAtStart(QFile *file){
    if(QFile(TEMP_FILE).exists())
        QFile::remove(TEMP_FILE);
    QFile tempFile(TEMP_FILE);
    // Otiweranie pliku konfiguracyjnego
    if(file->open(QIODevice::ReadOnly | QIODevice::Text)){
        if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        // Iniciowanie zmiennych odczytu danych
        QTextStream str(&tempFile);
        str.setCodec("UTF-8");
        char tempChar = '\0';
        QString tempStr = QString();
        bool noDefaultPath = false;
        unsigned long long projectPackNamePos = 0;
        unsigned long long counter = ULONG_LONG_MAX;
        unsigned long long pos;
        bool stop = false;
        uint numbOfRestore = 0;
        bool pathToRewrite = false;
        uint projectCounter = 0;
        QString tempPath;
        QStringList filesList;
        bool *configProjectsFound = nullptr;
        bool *dirProjectsFound = nullptr;
        uint *recents = nullptr;
        ushort numbOfRecents = 0;
        uint newNumbOfProjects = 0;
        ushort readingStage = 0; // 0 - Odczyt nazw projektów dla projectsFound, 1 - Porównywanie i przepisywanie projektów istniejących
        // Inicjowanie zmiennych sterujących
        ConfigFileStat configFileStat = SEARCHING;
        ConfigPackName *configPackName = nullptr;
        GeneralParamNames *generalParamNames = nullptr;
        ProjectsParamNames *projectsParamNames = nullptr;
        {
            while(numbOfRestore < 2){
            // Odczyt pliku
                stop = false;
            while(!stop){
                if(file->atEnd())
                    switch(readingStage){
                    case 0:
                    {
                        projectCounter = 0;
                        ushort tempNumbOfRecents = 0;
                        for(uint i = 0 ; i < numbOfRecents; i++)
                            if(*(recents + i) != 0 && *(configProjectsFound + *(recents + i) - 1))
                                tempNumbOfRecents++;
                        if(tempNumbOfRecents != 0){
                            uint *temp = new uint[tempNumbOfRecents];
                            {
                                uint j = 0;
                                for(uint i = 0; i < tempNumbOfRecents; i++){
                                    while(*(recents + i + j) == 0)
                                        j++;
                                    *(temp + i) = *(recents + i + j);
                                }
                            }
                            delete []recents;
                            recents = temp;
                        }else{
                            if(numbOfRecents != 0){
                                delete []recents;
                                recents = nullptr;
                            }
                        }
                        numbOfRecents = tempNumbOfRecents;
                        newNumbOfProjects = 0;
                        for(uint i = 0; i < numbOfProjects; i++){
                            if(*(configProjectsFound + i) == true)
                                newNumbOfProjects++;
                        }
                        for(int i = 0; i < filesList.size(); i++)
                            if(*(dirProjectsFound + i) == false)
                                newNumbOfProjects++;
                        str << "[General]\ndefaultPath=\"" + ((noDefaultPath)? QDir::toNativeSeparators(QDir::homePath()) +"\\" : defaultPath).toUtf8() + "\"\n";
                        str << ("autocadPath=\"" + autocadPath + "\"\n").toUtf8();
                        str << ("numbOfProj=" + QString::number(newNumbOfProjects) + "\n").toUtf8();
                        str << ("numbOfRec=" + QString::number(numbOfRecents) + "\n").toUtf8();
                        str << "sort=" + QString::number((uint)sortingType) + "\n";
                        str << "[Projects]\n";
                        file->seek(projectPackNamePos);
                        readingStage++;
                    }
                        break;
                    case 1:
                        for(int i = 0; i < filesList.size(); i++){
                            if(*(dirProjectsFound + i) == false){
                                projectCounter++;
                                QString retStr = ProjectMenager::readForConfigFile(&str, new QFile(defaultPath + QString(APP_NAME + QString("\\Projects\\") + filesList.value(i))), &pathToRewrite);
                                if(!retStr.isEmpty()){
                                    str << "projectNumb=" + QString::number(projectCounter) + "\n";
                                    str << "recentNumb=0\n";
                                    str << retStr.toUtf8();
                                    if(pathToRewrite)
                                        ProjectMenager::rewriteForConfigFile(new QFile(defaultPath + QString(APP_NAME + QString("\\Projects\\") + filesList.value(i))));
                                }else{
                                    projectCounter--;

                                }
                            }
                        }
                        if(configProjectsFound != nullptr)
                            delete []configProjectsFound;
                        if(dirProjectsFound != nullptr)
                            delete []dirProjectsFound;
                        if(recents != nullptr)
                            delete []recents;
                        stop = true;
                        break;
                    }

                if(file->getChar(&tempChar))
                    if(tempChar == '\n')
                        counter += 2;
                    else
                        counter++;
                else
                    continue;
                switch((uint) configFileStat){
                case (uint) SEARCHING:
                    switch(tempChar){
                    case  '[':
                        pos = counter;
                        configFileStat = READING_PACK_NAME;
                        break;
                    case '\n':
                        configFileStat = SEARCHING;
                        break;
                    default:
                        pos = counter;
                        configFileStat = READING_PARAMS_CONFIG;
                        tempStr.append(tempChar);
                        break;
                    }
                    break;
                case (uint) READING_PACK_NAME:
                    if(tempChar == ']'){
                        if(configPackName == nullptr)
                            configPackName = new ConfigPackName;
                        if((uint)(*configPackName = checkConfigPackName(&tempStr)) == NUMB_OF_CONFIG_PACK_NAME){
                            numbOfRestore++;
                            stop = true;
                            if(resetAndRestoreConfig(file)){
                                file->setFileName(QString(CONFIG_FILE));
                                file->open(QIODevice::ReadOnly | QIODevice::Text);
                                configFileStat = SEARCHING;
                                tempStr.clear();
                                break;
                            }else{
                                delete configPackName;
                                configPackName = nullptr;
                                delete generalParamNames;
                                generalParamNames = nullptr;
                                delete projectsParamNames;
                                projectsParamNames = nullptr;
                                if(configProjectsFound != nullptr)
                                    delete []configProjectsFound;
                                if(dirProjectsFound != nullptr)
                                    delete []dirProjectsFound;
                                if(recents != nullptr)
                                    delete []recents;
                                return;
                            }
                        }
                        if(*configPackName == PROJECTS){
                        projectPackNamePos = pos;
                        }
                        tempStr.clear();
                        configFileStat = SEARCHING;
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case (uint) READING_PARAMS_CONFIG:
                    if(tempChar == '='){
                        if(configPackName != nullptr)
                            switch((uint) *configPackName){
                            case (uint) GENERAL_CONFIG:
                                if(generalParamNames == nullptr)
                                    generalParamNames = new GeneralParamNames;
                                if((uint)(*generalParamNames = checkGeneralParamName(&tempStr)) == NUMB_OF_GENERAL_PARAM_NAMES){
                                    numbOfRestore++;
                                    stop = true;
                                    if(resetAndRestoreConfig(file)){
                                        file->setFileName(CONFIG_FILE);
                                        file->open(QIODevice::ReadOnly | QIODevice::Text);
                                        configFileStat = SEARCHING;
                                        tempStr.clear();
                                        break;
                                    }else{
                                        delete configPackName;
                                        configPackName = nullptr;
                                        delete generalParamNames;
                                        generalParamNames = nullptr;
                                        delete projectsParamNames;
                                        projectsParamNames = nullptr;
                                        if(configProjectsFound != nullptr)
                                            delete []configProjectsFound;
                                        if(dirProjectsFound != nullptr)
                                            delete []dirProjectsFound;
                                        if(recents != nullptr)
                                            delete []recents;
                                        return;
                                    }
                                }
                                tempStr.clear();
                                configFileStat = READING_VALUE;
                                break;
                            case (uint) PROJECTS:
                                if(projectsParamNames == nullptr)
                                    projectsParamNames = new ProjectsParamNames;
                                if((uint)(*projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                                    numbOfRestore++;
                                    stop = true;
                                    if(resetAndRestoreConfig(file)){
                                        file->setFileName(CONFIG_FILE);
                                        file->open(QIODevice::ReadOnly | QIODevice::Text);
                                        configFileStat = SEARCHING;
                                        tempStr.clear();
                                        break;
                                    }else{
                                        delete configPackName;
                                        configPackName = nullptr;
                                        delete generalParamNames;
                                        generalParamNames = nullptr;
                                        delete projectsParamNames;
                                        projectsParamNames = nullptr;
                                        if(configProjectsFound != nullptr)
                                            delete []configProjectsFound;
                                        if(dirProjectsFound != nullptr)
                                            delete []dirProjectsFound;
                                        if(recents != nullptr)
                                            delete []recents;
                                        return;
                                    }
                                }
                                tempStr.clear();
                                configFileStat = READING_VALUE;
                                break;
                            case (uint) OTHER_PROJECTS:
                                if(projectsParamNames == nullptr)
                                    projectsParamNames = new ProjectsParamNames;
                                if((uint)(*projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                                    numbOfRestore++;
                                    stop = true;
                                    if(resetAndRestoreConfig(file)){
                                        file->setFileName(CONFIG_FILE);
                                        file->open(QIODevice::ReadOnly | QIODevice::Text);
                                        configFileStat = SEARCHING;
                                        tempStr.clear();
                                        break;
                                    }else{
                                        delete configPackName;
                                        configPackName = nullptr;
                                        delete generalParamNames;
                                        generalParamNames = nullptr;
                                        delete projectsParamNames;
                                        projectsParamNames = nullptr;
                                        if(configProjectsFound != nullptr)
                                            delete []configProjectsFound;
                                        if(dirProjectsFound != nullptr)
                                            delete []dirProjectsFound;
                                        if(recents != nullptr)
                                            delete []recents;
                                        return;
                                    }
                                }
                                tempStr.clear();
                                configFileStat = READING_VALUE;
                                break;
                            }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case (uint) READING_VALUE:
                    switch((uint)*configPackName){
                    case (uint)GENERAL_CONFIG:
                        tempStr = readValueAtStart(*file, tempChar, &counter);
                        switch((uint)*generalParamNames){
                        case (uint)DEFAULT_PATH:
                            defaultPath = tempStr;
                            if(defaultPath.isEmpty()){
                                noDefaultPath = true;
                                defaultPath = QDir::toNativeSeparators(QDir::homePath()) + "\\";
                            }
                            filesList = QDir(tempStr + QString(APP_NAME + QString("\\Projects\\"))).entryList(QStringList("*" + QString(PROJECT_FILE_EXTENSTION)), QDir::Files, QDir::NoSort);
                            if(!filesList.isEmpty()){
                                dirProjectsFound = new bool[filesList.count()];
                                for(int i = 0; i < filesList.count(); i++)
                                    *(dirProjectsFound + i) = false;
                            }
                            checkDefaultPathStructure();
                            if(!QFile(defaultPath + "OPCalc\\flangeMaterials.ini").exists()){
                                QFile file(defaultPath + "OPCalc\\flangeMaterials.ini");
                                file.open(QIODevice::WriteOnly | QIODevice::Text);
                                file.close();
                            }
                            if(!QFile(defaultPath + "OPCalc\\pipeMaterials.ini").exists())  {
                                QFile file(defaultPath + "OPCalc\\pipeMaterials.ini");
                                file.open(QIODevice::WriteOnly | QIODevice::Text);
                                file.close();
                            }
                            if(!QFile(defaultPath + "OPCalc\\fluids.ini").exists())
                            {
                                QFile file(defaultPath + "OPCalc\\fluids.ini");
                                file.open(QIODevice::WriteOnly | QIODevice::Text);
                                file.close();
                            }
                            tempStr.clear();
                            break;
                        case (uint)AUTOCAD_PATH:
                            autocadPath = tempStr;
                            tempStr.clear();
                            break;
                        case (uint)NUMB_OF_PROJ:
                            numbOfProjects = tempStr.toUInt();
                            if(numbOfProjects != 0){
                                configProjectsFound = new bool[numbOfProjects];
                                for(uint i = 0; i < numbOfProjects; i++)
                                    *(configProjectsFound + i) = false;
                            }
                            tempStr.clear();
                            break;
                        case (uint)NUMB_OF_REC:
                            numbOfRecents = tempStr.toUShort();
                            if(numbOfRecents != 0){
                                recents = new uint[numbOfRecents];
                                for(ushort i = 0; i < numbOfRecents; i++)
                                    *(recents + i) = 0;
                            }
                            tempStr.clear();
                            break;
                        case (uint)SORTING_TYPE:
                            sortingType = (SortingType)tempStr.toUShort();
                            tempStr.clear();
                            break;
                        }
                        configFileStat = SEARCHING;
                        break;
                    default:
                        tempStr = readValueAtStart(*file, tempChar, &counter);
                        switch((uint)*projectsParamNames){
                        case (uint)PROJECT_NUMB:
                            projectCounter++;
                            break;
                        case (uint)RECENT_NUMB:
                            switch(readingStage){
                            case 0:
                                if(tempStr.toUShort() != 0)
                                    *(recents + tempStr.toUShort() - 1) = projectCounter;
                                break;
                            }
                            break;
                       case (uint)PATH:
                            tempPath = tempStr;
                            switch(readingStage){
                            case 0:
                                for(int i = 0; i < filesList.size(); i++){
                                   if(App::compareStr(defaultPath + QString(APP_NAME + QString("\\Projects\\")) + filesList.value(i), tempStr)){
                                        *(dirProjectsFound + i) = true;
                                        *(configProjectsFound + projectCounter - 1) = true;
                                        break;
                                    }
                                }
                                break;
                            case 1:
                                if(*(configProjectsFound + projectCounter - 1) == true){
                                    tempPath = tempStr;
                                }
                                break;
                            }
                            break;
                        case (uint)DEFAULT:
                            switch(readingStage){
                            case 0:
                                if(tempStr.toUShort() == 1 && *(configProjectsFound + projectCounter - 1) == false)
                                    for(uint i = 0; i < numbOfRecents; i++)
                                        if(*(recents + i) == projectCounter)
                                            *(recents + i) = 0;
                                if(tempStr.toUShort() == 0 && QFile(tempPath).exists()){
                                    *(configProjectsFound + projectCounter - 1) = true;
                                }
                                break;
                            case 1:
                                if(*(configProjectsFound + projectCounter - 1) == true){
                                    QString retString = ProjectMenager::readForConfigFile(&str, new QFile(tempPath), &pathToRewrite);
                                    if(!retString.isEmpty())
                                    {
                                        str << "projectNumb=" + QString::number(projectCounter) + "\n";
                                        uint i = 0;
                                        for( ; i < numbOfRecents; i++)
                                            if(*(recents + i) == projectCounter){
                                                str << "recentNumb=" + QString::number(i + 1) + "\n";
                                                break;
                                            }
                                        if(i == numbOfRecents)
                                            str << "recentNumb=0\n";
                                        str << retString;
                                        if(pathToRewrite)
                                            ProjectMenager::rewriteForConfigFile(new QFile(tempPath));
                                    }
                                }else{
                                    projectCounter--;
                                }
                                break;
                            }
                            break;
                        default:
                            break;
                        }
                        tempStr.clear();
                        configFileStat = SEARCHING;
                        break;
                    }
                    break;
                default:
                    if(resetAndRestoreConfig(file)){
                        numbOfRestore++;
                        stop = true;
                        delete generalParamNames;
                        generalParamNames = nullptr;
                        delete projectsParamNames;
                        projectsParamNames = nullptr;
                        file->setFileName(CONFIG_FILE);
                        if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
                        {
                            constructingFail = true;
                            if(configProjectsFound != nullptr)
                                delete []configProjectsFound;
                            if(dirProjectsFound != nullptr)
                                delete []dirProjectsFound;
                            if(recents != nullptr)
                                delete []recents;
                            return;
                        }
                        configFileStat = SEARCHING;
                        tempStr.clear();
                        break;
                    }else{
                        delete configPackName;
                        configPackName = nullptr;
                        delete generalParamNames;
                        generalParamNames = nullptr;
                        delete projectsParamNames;
                        projectsParamNames = nullptr;
                        if(configProjectsFound != nullptr)
                            delete []configProjectsFound;
                        if(dirProjectsFound != nullptr)
                            delete []dirProjectsFound;
                        if(recents != nullptr)
                            delete []recents;
                        return;
                    }
                }
             }
            if(configPackName && *configPackName == GENERAL_CONFIG){
                file->close();
                file->remove();
                file->setFileName(CONFIG_BACK_UP_FILE);
                numbOfRestore++;
                stop = true;
                if(!restoreConfig(file))
                {
                    if(!createConfigFile())
                    {
                        return;
                    }else{
                    if(!createConfigBackUpFile())
                        return;
                    }
                    file->setFileName(CONFIG_FILE);
                    if(file->open(QIODevice::ReadOnly | QIODevice::Text)){
                        configFileStat = SEARCHING;
                        readingStage = 0;
                    }else{
                    constructingFail = true;
                    return;
                    }
                }else{
                    if(!createConfigBackUpFile())
                        return;
                    configFileStat = SEARCHING;
                    file->setFileName(CONFIG_FILE);
                    if(file->open(QIODevice::ReadOnly | QIODevice::Text)){
                        configFileStat = SEARCHING;
                        readingStage = 0;
                    }else{
                        constructingFail = true;
                        return;
                    }
                }
            }else{
               numbOfRestore = 2;
               stop = true;
            }
            }
        }
        tempFile.close();
    }else{
        constructingFail = true;
        file->close();
        return;
    }
        file->close();
        file->remove();
        tempFile.rename(CONFIG_FILE);
        createConfigBackUpFile();
        }else{
            constructingFail = true;
            return;
    }

}

bool Config::restoreConfig(QFile* file){
  uint counter = 0;
  // Próba skopiowania backupu i zmiany nazwy na config
  while(!file->copy(CONFIG_FILE)){
      if(counter < 100)
        counter++;
      else{
          constructingFail = true;
          return false;
        }
      }
  file->setFileName(CONFIG_FILE);
  return true;
}

bool Config::resetAndRestoreConfig(QFile *file){
  file->close();
  file->remove();
  file->setFileName(CONFIG_BACK_UP_FILE);
  return restoreConfig(file);
 }

bool Config::createConfigFile(){
    QFile file(CONFIG_FILE);
    if(file.exists()){
        file.remove();        
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        constructingFail = true;
        return false;
    }else{
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << (NEW_CONFIG_FILE_TEXT).toUtf8();
        file.close();
        }
    return true;
}

bool Config::createConfigBackUpFile(){
    if(QFile(CONFIG_BACK_UP_FILE).exists())
        QFile(CONFIG_BACK_UP_FILE).remove();
  if(!QFile::copy(CONFIG_FILE, CONFIG_BACK_UP_FILE))
  {
      constructingFail = true;
      return false;
  }
   return true;
}

ConfigPackName Config::checkConfigPackName(QString *tempStr){
  QString namesStr = QString();
  uint i = 0;
  switch(tempStr->length()){
    case 7:
      // Sprawdzenie dla słowa General
      namesStr.append("General");
      for(i = 0; i < 7; i++){
          if(tempStr->at(i) != namesStr.at(i))
            break;
        }
      if(i == 7){
          return GENERAL_CONFIG;
        }
      break;
    case 8:
      // Sprawdzanie dla słowa Projects
      namesStr.append("Projects");
      for(i = 0; i < 8 ; i++){
          if(tempStr->at(i) != namesStr.at(i))
            break;
        }
      if(i == 8){
          return PROJECTS;
        }
      break;
    case 14:
      // Sprawdzanie dla słowa "Other projects"
      namesStr.append("Other projects");
      for(i = 0 ; i < 14 ; i++){
          if(tempStr->at(i) != namesStr.at(i))
            break;
        }
      if(i == 14){
          return OTHER_PROJECTS;
        }
      break;
    default:
      return NUMB_OF_CONFIG_PACK_NAME;
     }
  return NUMB_OF_CONFIG_PACK_NAME;
}

GeneralParamNames Config::checkGeneralParamName(QString *tempStr){
  QString namesStr = QString();
  uint i = 0;
    switch(tempStr->length()){
      case 11:
        namesStr.append("defaultPath");
        // Sprawdzenie dla słowa "defaultPath"
        for(i = 0; i < 11 ; i++){
            if(tempStr->at(i) != namesStr.at(i))
              break;
          }
        if(i == 11){
            return DEFAULT_PATH;
          }
        namesStr.clear();
        namesStr.append("autocadPath");
        // Sprawdzenie dla słowa "defaultPath"
        for(i = 0; i < 11 ; i++){
            if(tempStr->at(i) != namesStr.at(i))
              break;
          }
        if(i == 11){
            return AUTOCAD_PATH;
          }
        break;
      case 10:
        namesStr.append("numbOfProj");
        // Sprawdzanie słowa "numbOfProj"
        for(i = 0; i < 10; i++){
            if(tempStr->at(i) != namesStr.at(i))
              break;
          }
        if(i == 10){
            return NUMB_OF_PROJ;
          }
        break;
      case 9:
        namesStr.append("numbOfRec");
        // Spradzanie słowa "numbOfRec"
        for(i = 0; i < 9; i++){
            if(tempStr->at(i) != namesStr.at(i))
              break;
          }
        if(i == 9){
            return NUMB_OF_REC;
          }
        break;
      case 4:
        // Sprawdzanie dla słowa "sort"
          namesStr.append("sort");
            for(i = 0; i < 4; i++){
                if(tempStr->at(i) != namesStr.at(i))
                  break;
              }
            if(i == 4){
                return SORTING_TYPE;
              }
          break;
      default:
        return NUMB_OF_GENERAL_PARAM_NAMES;
      }
    return NUMB_OF_GENERAL_PARAM_NAMES;
}

ProjectsParamNames Config::checkProjectParamName(QString *tempStr){
  QString namesStr = QString();
  uint i;
  switch(tempStr->length()){
    case 4:
      // Sprawdzanie słowa "name"
      namesStr.append("name");
      for(i = 0; i < 4; i++){
          if(tempStr->at(i) != namesStr.at(i))
            break;
        }
      if(i == 4){
          return NAME;
        }
      namesStr.clear();
      // Sprawdzanie słowa "path"
      namesStr.append("path");
      for(i = 0; i < 4; i++)
        if(tempStr->at(i) != namesStr.at(i))
          break;
      if(i == 4){
          return PATH;
        }
      break;
  case 7:
      // Sprawdzanie słowa "default"
      namesStr.append("default");
      for(i = 0; i < 7; i++)
        if(tempStr->at(i) != namesStr.at(i))
          break;
      if(i == 7){
          return DEFAULT;
        }
      // Sprawdzanie słowa "created"
      namesStr.clear();
      namesStr.append("created");
      for(i = 0; i < 7; i++)
          if(tempStr->at(i) != namesStr.at(i))
              break;
      if(i == 7)
          return CREATED;
      break;
    case 10:
      // Sprawdzanie słowa "recentNumb"
      namesStr.append("recentNumb");
      for(i = 0; i < 10; i++)
        if(tempStr->at(i) != namesStr.at(i))
          break;
      if(i == 10){
          return RECENT_NUMB;
        }
      namesStr.clear();
      namesStr.append("flangeName");
      // Sprawdzanie słowa "flangeName"
      for(i = 0; i < 10; i++)
        if(tempStr->at(i) != namesStr.at(i))
          break;
      if(i == 10){
          return CONFIG_FLANGE_NAME;
        }
      break;
    case 11:
      // Sprawdzanie słowa "projectNumb"
      namesStr.append("projectNumb");
      for(i = 0; i < 11; i++)
        if(tempStr->at(i) != namesStr.at(i))
          break;
      if(i == 11){
          return PROJECT_NUMB;
        }
      namesStr.clear();
      // Sprawdzanie słowa "companyName"
      namesStr.append("companyName");
      for(i = 0; i < 11; i++)
        if(tempStr->at(i) != namesStr.at(i))
          break;
      if(i == 11){
          return COMPANY_NAME;
        }
      break;
    case 18:
      // Sprawdzanie słowa "fullscalePlantName"
      namesStr.append("fullscalePlantName");
      for(i = 0; i < 18; i++)
        if(tempStr->at(i) != namesStr.at(i))
          break;
      if(i == 18){
          return FULLSCALE_PLANT_NAME;
        }
      break;
  case 8:
      // Sprawdzanie słowa "modified"
      namesStr.append("modified");
      for(i = 0; i < 8; i++)
          if(tempStr->at(i) != namesStr.at(i))
              break;
      if(i == 8)
          return MODIFIED;
      break;
    default:
      return NUMB_OF_PROJECTS_PARAM_NAMES;
    }
  return NUMB_OF_PROJECTS_PARAM_NAMES;
}

QString Config::readValueAtStart(QFile &f, char &c, unsigned long long *counter){
  QTextStream s(&f);
  s.setCodec("UTF-8");
  QByteArray byte;
  QString sign;
  QString tempStrFunc = QString();
    if(c == '\"')
        while(!f.atEnd()){
            s.device()->getChar(&c);
            if(c > 0)
            {
                if(c == '\n')
                    (*counter) += 2;
                else
                    (*counter)++;
                if(parent->getMainWin()->getProgressBarSL() != nullptr)
                    *progressBarValue += 1;
                if(c != '\"')
                    tempStrFunc.append(c);
                else
                    break;
            }else{
                ushort i = 1;
                byte.append(c);
                if(c > -96){
                    s.device()->getChar(&c);
                    byte.append(c);
                    i++;
                }else{
                    if(c > -112)
                    {
                        for( ; i < 4; i++)
                        {
                            s.device()->getChar(&c);
                            byte.append(c);
                        }

                    }else{
                        if(c > -120){
                            for( ; i < 5; i++){
                                s.device()->getChar(&c);
                                byte.append(c);
                            }
                        }else{
                            if(c > -124){
                                for( ; i < 6; i++){
                                    s.device()->getChar(&c);
                                    byte.append(c);
                                }
                            }else{
                                for( ; i < 7; i++){
                                    s.device()->getChar(&c);
                                    byte.append(c);
                                }
                            }
                        }
                    }
                }
                sign.clear();
                sign = QString(byte);
                tempStrFunc.append(sign);
                *counter += i;
                if(parent->getMainWin()->getProgressBarSL() != nullptr)
                   *progressBarValue += i;
                byte.clear();
            }
        }
    else{
        tempStrFunc.append(c);
        while(!f.atEnd()){
            s.device()->getChar(&c);
            if(c != '\n'){
                tempStrFunc.append(c);
                if(parent->getMainWin()->getProgressBarSL() != nullptr)
                *progressBarValue += 1;
                (*counter)++;
              }
            else{
              (*counter) += 2;
                if(parent->getMainWin()->getProgressBarSL() != nullptr)
                *progressBarValue += 2;
                break;
              }
        }
      }
    return tempStrFunc;
}

bool Config::isConstructingFail(){
  return constructingFail;
}

QString Config::getDefaultPath(){
  return defaultPath;
}

App* Config::getParent(){
  return parent;
}

uint Config::getNumbOfProjects(){
  return numbOfProjects;
}

ushort Config::getNumbOfRecentProjects(){
  return numbOfRecentProjects;
}

void Config::setConstructingFail(bool set){
  constructingFail = set;
}

void Config::setDefaultPath(QString set){
  QString oldDefaultPath = defaultPath;
  defaultPath = set;
  if(parent->getProjectInfo()!= nullptr)
      parent->getMainWin()->saveAllProjects();
  checkDefaultPathStructure();
  moveDefaultPath(oldDefaultPath);
  MaterialsInfoFiles::rewriteSubstancesPaths(this, false);
  QDir(oldDefaultPath + "OPCalc").removeRecursively();
  parent->getConfiguration()->rewriteGeneral();
  exit(0);
}

void Config::setNumbOfProjects(uint set){
  numbOfProjects = set;
}

void Config::setNumbOfRecentProjects(ushort set){
  numbOfRecentProjects = set;
}

bool Config::readForRecentList(){
  QFile file(CONFIG_FILE);
  if(!checkConfigFile(&file))
  {
      parent->safeCloseApp();
      }
   if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        char tempChar = '\0';
        QString tempStr = QString();
        bool read = true;
        ConfigFileStat configFileStat = SEARCHING;
        ProjectsParamNames projectsParamNames = RECENT_NUMB;
        for(uint i = 0; i < numbOfRecentProjects; i++){
            if(configDataPosition->getRecentProjectsPosition() !=  nullptr){
                file.seek(*(configDataPosition->getRecentProjectsPosition() + i));
            }
            read = true;
            configFileStat = SEARCHING;
            while(read != false && !file.atEnd()){
                file.getChar(&tempChar);
                switch((uint)configFileStat){
                case (uint) SEARCHING:
                    switch(tempChar){
                    case '[':
                        while(tempChar != '\n')
                            file.getChar(&tempChar);
                        break;
                    case '\n':
                        break;
                    default:
                        tempStr.append(tempChar);
                        configFileStat = READING_PARAMS_CONFIG;
                        break;
                    }
                    break;
                case (uint)READING_PARAMS_CONFIG:
                    switch(tempChar){
                    case '=':
                        if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                            if(resetAndRestoreConfig(&file)){
                             file.setFileName(CONFIG_FILE);
                             file.open(QIODevice::ReadOnly | QIODevice::Text);
                             configFileStat = SEARCHING;
                             tempStr.clear();
                             break;
                          }else{
                                 return false;
                          }
                        }
                        tempStr.clear();
                        configFileStat =  READING_VALUE;
                        break;
                    default:
                        tempStr.append(tempChar);
                        break;
                    }
                    break;
                case (uint)READING_VALUE:
                    switch((uint)projectsParamNames){
                    case (uint)PROJECT_NUMB:
                        tempStr = readValue(&file, &tempChar);
                        while(tempChar != '\n')
                            file.getChar(&tempChar);
                         break;
                    case (uint)RECENT_NUMB:
                         (*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->setRecentNumber(readValue(&file, &tempChar).toUShort());
                        break;
                    case (uint)NAME:
                        (*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->getUpInfoLabel()->setText(readValue(&file, &tempChar));
                        break;
                    case (uint)FULLSCALE_PLANT_NAME:
                        (*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->getUpInfoLabel()->setText((*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->getUpInfoLabel()->text() + QString(" ") + readValue(&file, &tempChar));
                        break;
                    case (uint)COMPANY_NAME:
                        (*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->getUpInfoLabel()->setText((*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->getUpInfoLabel()->text() + QString(" ") + readValue(&file, &tempChar));
                        break;
                    case (uint)CONFIG_FLANGE_NAME:
                        (*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->getUpInfoLabel()->setText((*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->getUpInfoLabel()->text() + QString(" ") + readValue(&file, &tempChar));
                        break;
                    case (uint)PATH:
                        (*parent->getMainWin()->getInnerWindowCP()->getRecentPanel()->getRecentList()->getRecentElement() + i)->getDownInfoLabel()->setText(readValue(&file, &tempChar));
                        break;
                    case (uint)DEFAULT:
                        read = false;
                        break;
                    default:
                        tempStr = readValue(&file, &tempChar);
                        break;
                          }
                    configFileStat = SEARCHING;
                    if(!tempStr.isEmpty())
                        tempStr.clear();
                    break;
                }

            }
            tempChar = '\0';
            projectsParamNames = RECENT_NUMB;
          }
        file.close();
        }else{
           return false;
        }
  return true;
}

bool Config::checkConfigFile(QFile *file){
  // Sprawdzenie istanienia pliku konfiguracyjnego lub ewentualne jego odzyskanie
  if(!file->exists()){
      file->setFileName(QString(CONFIG_BACK_UP_FILE));
      if(!file->exists())
        {
          if(!createConfigFile())
          {
              return false;
          }else{
              file->setFileName(CONFIG_FILE);
              if(!createConfigBackUpFile()){
                  QFile::remove(CONFIG_FILE);                  
              }
              return false;
             }
         }
      else{
          if(!restoreConfig(file)){
              return false;
            }
          }
    }
  return true;
  }

QString Config::readValue(QFile *file, char *tempCharFunc){
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
            if(*tempCharFunc != '\n')
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

bool Config::rewriteGeneral(){
    QFile file(CONFIG_BACK_UP_FILE);
    if(!checkConfigFile(&file))
    {
        parent->safeCloseApp();
    }
    QFile tempFile(TEMP_FILE);
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            file.remove();
            QFile::copy(CONFIG_FILE, CONFIG_BACK_UP_FILE);
            file.setFileName(CONFIG_FILE);
            if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
                unsigned long long counter = 0;
                char tempChar ='\0';
                QTextStream streamO(&tempFile);
                QTextStream sI(&file);
                QByteArray byte;
                QString sign;
                sI.setCodec("UTF-8");
                streamO.setCodec("UTF-8");
                unsigned long long diff = 0;
                // Szacowanie zmian rozmiaru pliku
                streamO << "[General]\n";
                counter += 11;
                streamO << "defaultPath=\"";
                counter += 13;
                streamO << (defaultPath + "\"\n").toUtf8();
                counter += defaultPath.length() + 3;
                streamO << ("autocadPath=\"" + autocadPath + "\"\n").toUtf8();
                counter += autocadPath.length() + 16;
                streamO << ("numbOfProj=" + QString::number(numbOfProjects ) + "\n");
                counter += 11 + QString::number(numbOfProjects).length() + 2;
                streamO << ("numbOfRec=" + QString::number(numbOfRecentProjects) + "\n");
                counter += 10 + QString::number(numbOfRecentProjects).length() + 2;
                streamO << ("sort=" + QString::number((uint)sortingType) + "\n");
                counter += 7 + QString::number((uint) sortingType).length();
                diff = counter - *(configDataPosition->getPackNamePosition() + 1);
                if(diff != 0){
                   configDataPosition->setPackNamePositionAt(*(configDataPosition->getPackNamePosition() + 2) + diff, 2);
                for(uint i = 0; i < numbOfProjects; i++)
                   configDataPosition->setProjectsPositionAt(*(configDataPosition->getProjectsPosition() + i) + diff, i);
                for(ushort i = 0; i < numbOfRecentProjects; i++)
                   configDataPosition->setRecentProjectsPositionAt(*(configDataPosition->getRecentProjectsPosition()) + diff, i);
                configDataPosition->setPackNamePositionAt(*(configDataPosition->getPackNamePosition() + 1) + diff, 1);
               }
                file.seek(*(configDataPosition->getPackNamePosition() + 1) - diff);
                while(!file.atEnd()){
                    sI.device()->getChar(&tempChar);
                    if(tempChar > 0)
                    {
                        if(tempChar == '\n')
                            counter += 2;
                        else
                            counter++;
                        streamO << tempChar;
                    }else{
                        ushort i = 1;
                        byte.append(tempChar);
                        if(tempChar > -96){
                            sI.device()->getChar(&tempChar);
                            byte.append(tempChar);
                            i++;
                        }else{
                            if(tempChar > -112)
                            {
                                for( ; i < 4; i++)
                                {
                                    sI.device()->getChar(&tempChar);
                                    byte.append(tempChar);
                                }

                            }else{
                                if(tempChar > -120){
                                    for( ; i < 5; i++){
                                        sI.device()->getChar(&tempChar);
                                        byte.append(tempChar);
                                    }
                                }else{
                                    if(tempChar > -124){
                                        for( ; i < 6; i++){
                                            sI.device()->getChar(&tempChar);
                                            byte.append(tempChar);
                                        }
                                    }else{
                                        for( ; i < 7; i++){
                                            sI.device()->getChar(&tempChar);
                                            byte.append(tempChar);
                                        }
                                    }
                                }
                            }
                        }
                        sign.clear();
                        sign = QString(byte);
                        streamO << sign;
                        counter += i;
                        byte.clear();
                    }
                }
                file.close();
                tempFile.close();
                file.remove();
                tempFile.rename(CONFIG_FILE);
                return true;
          }else{
                constructingFail = true;
                return false;
            }
      }else{
            constructingFail = true;
            return false;
    }
    return true;
}

bool Config::readForProjectList(bool isOpenProject){
  QFile file(CONFIG_FILE);
  if(!checkConfigFile(&file))
  {
      parent->safeCloseApp();
  }
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
      char tempChar = '\n';
      QString tempStr = QString();
      ConfigFileStat configFileStat = SEARCHING;
      ProjectsParamNames projectsParamNames = RECENT_NUMB;
      uint actualProject = 0;
      configFileStat = SEARCHING;
      file.seek(*(configDataPosition->getPackNamePosition() + 1));
      while(!file.atEnd()){
          file.getChar(&tempChar);
          switch((uint) configFileStat){
            case (uint) SEARCHING:
              switch(tempChar){
                case '[':
                  while(tempChar != ']')
                    file.getChar(&tempChar);
                  break;
                case '\n':
                  break;
                default:
                  tempStr.append(tempChar);
                  configFileStat = READING_PARAMS_CONFIG;
                  break;
                }
              break;
            case (uint)READING_PARAMS_CONFIG:
              switch(tempChar){
              case '=':
                  if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                      if(resetAndRestoreConfig(&file)){
                       file.setFileName(CONFIG_FILE);
                       file.open(QIODevice::ReadOnly | QIODevice::Text);
                       configFileStat = SEARCHING;
                       tempStr.clear();
                       break;
                    }else{
                           return false;
                    }
                  }
                  tempStr.clear();
                  configFileStat =  READING_VALUE;
                  break;
              default:
                  tempStr.append(tempChar);
                  break;
              }
              break;
            case (uint)READING_VALUE:
              tempStr = readValue(&file, &tempChar);
                switch((uint)projectsParamNames){
                case (uint)PROJECT_NUMB:
                     actualProject = tempStr.toUInt();
                     if(isOpenProject){
                          (*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getNumberLabel()->setText(QString::number(actualProject));
                          (*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->setNumber(actualProject);
                       }else{
                          (*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getNumberLabel()->setText(QString::number(actualProject));
                          (*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->setNumber(actualProject);
                       }
                     break;
                case (uint)NAME:
                    if(isOpenProject)
                      (*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getUpInfoLabel()->setText(tempStr);
                    else
                      (*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getUpInfoLabel()->setText(tempStr);
                    break;
                case (uint)FULLSCALE_PLANT_NAME:
                    if(isOpenProject)
                    (*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getUpInfoLabel()->setText((*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getUpInfoLabel()->text() + QString(" ") + tempStr);
                    else
                      (*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getUpInfoLabel()->setText((*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getUpInfoLabel()->text()+ QString(" ") + tempStr);
                break;
                case (uint)COMPANY_NAME:
                    if(isOpenProject)
                    (*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getUpInfoLabel()->setText((*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getUpInfoLabel()->text()+ QString(" ") + tempStr);
                    else
                      (*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getUpInfoLabel()->setText((*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getUpInfoLabel()->text()+ QString(" ") + tempStr);
                    break;
                case (uint)CONFIG_FLANGE_NAME:
                    if(isOpenProject)
                    (*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getUpInfoLabel()->setText((*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getUpInfoLabel()->text()+ QString(" ") + tempStr);
                    else
                      (*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getUpInfoLabel()->setText((*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getUpInfoLabel()->text() + QString(" ")+ tempStr);
                    break;
                case (uint)PATH:
                    if(isOpenProject)
                    (*(parent->getMainWin()->getInnerWindowCP()->getProjectListOP()->getProjectListElementOP()) + actualProject - 1)->getDownInfoLabel()->setText(tempStr);
                    else
                      (*(parent->getMainWin()->getInnerWindowCP()->getProjectListDP()->getProjectListElementDP()) + actualProject - 1)->getDownInfoLabel()->setText(tempStr);
                    break;
                }
                tempStr.clear();
                configFileStat = SEARCHING;
                break;
            }
         }
      file.close();
      }else{
      return false;
    }
  return true;
}

bool Config::addNewProject(){
    QFile configFile(CONFIG_FILE);
    if(!checkConfigFile(&configFile))
    {
        parent->safeCloseApp();
        }
    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QFile tempFile(TEMP_FILE);
        if(tempFile.exists())
            tempFile.remove();
        if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            unsigned long long counter = 0;
            QTextStream stream(&tempFile);

            stream.setCodec("UTF-8");
            // Zapisywanie paczki danych [General]
            stream << "[General]\n";
            stream << "defaultPath=\"";
            stream << (defaultPath + "\"\n").toUtf8();
            stream << "autocadPath=\"";
            stream << (autocadPath + "\"\n").toUtf8();
            stream << "numbOfProj=" + QString::number(numbOfProjects + 1) + "\n";
            stream << "numbOfRec=" + QString::number((numbOfRecentProjects != 20)? (numbOfRecentProjects + 1) : numbOfRecentProjects) + "\n";
            stream << "sort=" + QString::number((uint) sortingType) + "\n";
            counter += CALCULATE_GENERAL_PACK_NAME_SIZE(1, ((numbOfRecentProjects != 20)? 1 : 0));

            // Ustawianie pliku konfiguracyjnego (odczytywanego) na pierwszy projekt
            configFile.seek(*(configDataPosition->getPackNamePosition() + 1) + 12);

            // Dostosowanie pozycji paczek danych + Dostosowanie pozycji projektów oraz recentów
            {
            uint diff = counter - *(configDataPosition->getPackNamePosition() + 1);
            if(diff != 0){
            // Dostosowanie pozycji paczek danych
            configDataPosition->setPackNamePositionAt(*(configDataPosition->getPackNamePosition() + 1) + diff, 1);

            diff = 0;
             }
            // Zapisywanie nowego projektu z paczki danych [Projects]
            stream << "[Projects]\n";
            stream << "projectNumb=1\n";
            configDataPosition->addNewProjectPos(counter + 12);
            configDataPosition->addNewRecentPos(counter + 12);
            stream << "recentNumb=1\n";
            stream << ("name=\"" + *(*(parent->getProjectInfo()))->getProjectName() + "\"\n");
            if((*(parent->getProjectInfo()))->getCompanyName() != nullptr)
                stream << ("companyName=\"" + *((*(parent->getProjectInfo()))->getCompanyName()) + "\"\n").toUtf8();
            if((*(parent->getProjectInfo()))->getObjectName() != nullptr)
                stream << ("fullscalePlantName=\"" + *((*(parent->getProjectInfo()))->getObjectName()) + "\"\n").toUtf8();
            if((*(parent->getProjectInfo()))->getFlangeName() != nullptr)
                stream << ("flangeName=\"" + *((*(parent->getProjectInfo()))->getFlangeName()) + "\"\n").toUtf8();
            stream << ("path=\"" + *((*parent->getProjectInfo())->getPath()) + "\"\n");
            stream << "created=\"" + (*parent->getProjectInfo())->getCreated() + "\"\n";
            stream << "modified=\"" + (*parent->getProjectInfo())->getModified() + "\"\n";
            stream << "default=" + QString::number(((*parent->getProjectInfo())->isDefaultProj())? 1 : 0) + "\n";
            counter += 95;
            if(((*parent->getProjectInfo())->getObjectName() != nullptr))
                counter += (23 + App::lengthForFileCounter(*(*parent->getProjectInfo())->getObjectName()));
            if(((*parent->getProjectInfo())->getCompanyName() != nullptr))
                counter += (16 + App::lengthForFileCounter(*(*parent->getProjectInfo())->getCompanyName()));
            if(((*parent->getProjectInfo())->getFlangeName() != nullptr))
                counter += (15 + App::lengthForFileCounter(*(*parent->getProjectInfo())->getFlangeName()));
            counter += App::lengthForFileCounter(*(*parent->getProjectInfo())->getProjectName()) + App::lengthForFileCounter(*(*parent->getProjectInfo())->getPath()) + 2 * (*parent->getProjectInfo())->getCreated().length();
            }
            for(uint i = 1; i < (*parent->getProjectInfo())->getPosition(); i++)
                (*(parent->getProjectInfo() + i))->setProjectNumb(*(*(parent->getProjectInfo() + i))->getProjectNumb() + 1);

            // Zwiększenie ilości projektów o 1
            numbOfProjects++;

            // Zwiększenie ilości recentów o 1, jeśli mniej niż 20
            if(numbOfRecentProjects < 20)
              numbOfRecentProjects++;

            char tempChar = '\0';
            QString tempStr = QString();
            unsigned long long pos = 0;
            unsigned long long projectPos = 0;
            uint projectsCounter = 1;
            bool rewrite = false;
            ushort recentCounter = 1;

            // Inicjowanie zmiennych sterujących
            ConfigFileStat configFileStat = SEARCHING;
            ProjectsParamNames projectsParamNames = PROJECT_NUMB;

            // Odczyt pliku
            while(!configFile.atEnd()){
            if(configFile.getChar(&tempChar))
              if(tempChar == '\n')
              counter += 2;
              else
              counter++;
            else
             continue;
            switch((uint) configFileStat){
                case (uint) SEARCHING:
                    switch((uint) tempChar){
                    case(uint) '\n':
                        configFileStat = SEARCHING;
                        stream << '\n';
                        break;
                    default:
                        pos = counter - 1;
                        configFileStat = READING_PARAMS_CONFIG;
                        tempStr.append(tempChar);
                        break;
                    }
                    break;
              case (uint) READING_PARAMS_CONFIG:
                if(tempChar == '='){
                     if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                              if(resetAndRestoreConfig(&configFile)){
                               configFile.setFileName(CONFIG_FILE);
                               configFile.open(QIODevice::ReadOnly | QIODevice::Text);
                               configFileStat = SEARCHING;
                               tempStr.clear();
                               break;
                            }else{
                                  return false;
                            }
                          }
                          tempStr.append(tempChar);
                          stream << (tempStr).toUtf8();
                          tempStr.clear();
                          configFileStat = READING_VALUE;


                  }else{
                        tempStr.append(tempChar);
                      }
                break;
              case (uint) READING_VALUE:
                    tempStr = readValueAtStart(configFile, tempChar, &counter);
                    switch((uint)projectsParamNames){
                    case (uint)PROJECT_NUMB:
                        {
                        projectPos = pos;
                          projectsCounter++;
                        if(projectsCounter % 20 == 0 || projectsCounter == 1){
                          if(projectsCounter > numbOfProjects){
                                uint size = (projectsCounter - numbOfProjects) * 0.05 + 1;
                                unsigned long long *temp = new unsigned long long[size];
                                for(uint i = 0; i < size - 1; i++)
                                  *(temp + i) = *(configDataPosition->getProjectsPosition() + i);
                                *(temp + size - 1) = pos;
                                configDataPosition->setProjectsPosition(temp);
                                rewrite = true;
                                parent->getMainWin()->setProgressBarSLMaxReady(false);
                                temp = nullptr;
                             }else{
                              configDataPosition->setProjectsPositionAt(pos, projectsCounter * 0.05);
                            }
                          }
                        counter -= tempStr.length();
                        stream << projectsCounter;
                        counter += QString::number(projectsCounter).length();
                        stream << tempChar;
                        }
                        break;
                      case (uint)RECENT_NUMB:
                            {
                          ushort number = tempStr.toUShort();
                            if(number == 20){
                                counter -= tempStr.length();
                                stream << "0";
                                counter++;
                                stream << tempChar;
                             }else {
                                if(number != 0){
                                    recentCounter++;
                                    if(number > numbOfRecentProjects){
                                        unsigned long long *temp = new unsigned long long[number];
                                        for(uint i = 0; i < numbOfRecentProjects; i++)
                                          *(temp + i) = *(configDataPosition->getRecentProjectsPosition() + i);
                                        *(temp + number - 1) = pos;
                                        configDataPosition->setRecentProjectsPosition(temp);
                                        numbOfRecentProjects = number;
                                        rewrite = true;
                                        temp = nullptr;
                                      }else{
                                        number++;
                                        counter -= tempStr.length();
                                        stream << number;
                                        counter += QString::number(number).length();
                                        stream << tempChar;
                                        configDataPosition->setRecentProjectsPositionAt(projectPos, number - 1);
                                      }

                                }else{
                                    stream << tempStr.toUtf8();
                                    stream << tempChar;
                                    }
                            }

                        }
                       break;
                    case NAME:
                        stream <<( "\"" + tempStr + "\"").toUtf8();
                        break;
                    case FULLSCALE_PLANT_NAME:
                        stream <<("\"" + tempStr + "\"").toUtf8();
                        break;
                    case PATH:
                        stream <<("\"" + tempStr + "\"").toUtf8();
                        break;
                    case DEFAULT:
                        stream << tempStr + "\n";
                        break;
                    case CREATED:
                        stream << "\"" + tempStr + "\"";
                        break;
                    case MODIFIED:
                        stream << "\"" + tempStr + "\"";
                        break;
                    case CONFIG_FLANGE_NAME:
                        stream <<( "\"" + tempStr + "\"").toUtf8();
                        break;
                    case COMPANY_NAME:
                        stream << ("\"" + tempStr + "\"").toUtf8();
                        break;
                    }
                    configFileStat = SEARCHING;
                    tempStr.clear();
                  }
                }
            configFileStat = SEARCHING;
            if(projectsCounter > numbOfProjects){
                rewrite = true;
                numbOfProjects = projectsCounter;
            }
            tempFile.close();
            configFile.close();
      if(rewrite)
        rewriteGeneral();
      QFile backUpFile(CONFIG_BACK_UP_FILE);
      backUpFile.remove();
      configFile.copy(CONFIG_BACK_UP_FILE);
      }else{
            configFile.close();
            return false;
        }
        configFile.remove();
        tempFile.rename(CONFIG_FILE);
        }else{
        return false;
    }
    return true;
   }

bool Config::readRecentProject(ushort number){
  QFile file(CONFIG_FILE);
  if(!checkConfigFile(&file))
  {
      parent->safeCloseApp();
      }
  if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        char tempChar = '\0';
        QString tempStr = QString();
        ConfigFileStat configFileStat = SEARCHING;
        ProjectsParamNames projectsParamNames = PROJECT_NUMB;
        file.seek(*(configDataPosition->getRecentProjectsPosition() + number - 1));
        while(!file.atEnd()){
           file.getChar(&tempChar);
                switch((uint)configFileStat){
                case (uint) SEARCHING:
                    switch(tempChar){
                    case '[':
                        while(tempChar != '\n')
                            file.getChar(&tempChar);
                        break;
                    case '\n':
                        break;
                    default:
                        tempStr.append(tempChar);
                        configFileStat = READING_PARAMS_CONFIG;
                        break;
                    }
                    break;
                case (uint)READING_PARAMS_CONFIG:
                    switch(tempChar){
                    case '=':
                        if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                            if(resetAndRestoreConfig(&file)){
                             file.setFileName(CONFIG_FILE);
                             file.open(QIODevice::ReadOnly | QIODevice::Text);
                             configFileStat = SEARCHING;
                             tempStr.clear();
                             break;
                          }else{
                                 return false;
                          }
                        }
                        tempStr.clear();
                        configFileStat = READING_VALUE;
                        break;
                    default:
                        tempStr.append(tempChar);
                        break;
                    }
                    break;
                case (uint)READING_VALUE:
                    tempStr = readValue(&file, &tempChar);
                    switch((uint)projectsParamNames){
                    case (uint)PROJECT_NUMB:
                          (*parent->getProjectInfo())->setProjectNumb(tempStr.toUInt());
                          break;
                    case (uint)NAME:
                        (*parent->getProjectInfo())->setProjectName(tempStr);
                        break;
                    case (uint)FULLSCALE_PLANT_NAME:
                        (*parent->getProjectInfo())->setObjectName(new QString(tempStr));
                        break;
                    case (uint)PATH:
                        (*parent->getProjectInfo())->setPath(new QString(tempStr));
                        break;
                    case (uint)DEFAULT:
                        (*parent->getProjectInfo())->setDefaultProj((tempStr.toUShort() == 1)? true : false);
                        file.seek(file.size());
                        break;
                    case (uint)CREATED:
                        (*parent->getProjectInfo())->setCreated(tempStr);
                        break;
                    case (uint)MODIFIED:
                        (*parent->getProjectInfo())->setModified(tempStr);
                        break;
                    case (uint)COMPANY_NAME:
                        (*parent->getProjectInfo())->setCompanyName(new QString(tempStr));
                        break;
                    case (uint)CONFIG_FLANGE_NAME:
                        (*parent->getProjectInfo())->setFlangeName(new QString(tempStr));
                        break;
                          }
                    tempStr.clear();
                    configFileStat = SEARCHING;
                    break;
                }
            }
        file.close();
        }else{
      return false;
   }
  return true;
}

bool Config::readProject(uint number, ushort **recentNumb){
  QFile file(CONFIG_FILE);
  if(!checkConfigFile(&file))
  {
      parent->safeCloseApp();
  }
      if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        char tempChar = '\0';
        QString tempStr = QString();
        ConfigFileStat configFileStat = SEARCHING;
        ProjectsParamNames projectsParamNames = RECENT_NUMB;
        bool read = false;
        bool finish = false;
        uint projectNumb = 0;
        file.seek(*(configDataPosition->getProjectsPosition() + (uint)(number * 0.05)));
        while(!file.atEnd() && finish != true){
           file.getChar(&tempChar);
                switch((uint)configFileStat){
                case (uint) SEARCHING:
                    switch(tempChar){
                    case '[':
                        while(tempChar != '\n')
                            file.getChar(&tempChar);
                        break;
                    case '\n':
                        break;
                    default:
                        tempStr.append(tempChar);
                        configFileStat = READING_PARAMS_CONFIG;
                        break;
                    }
                    break;
                case (uint)READING_PARAMS_CONFIG:
                    switch(tempChar){
                    case '=':
                        if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                            if(resetAndRestoreConfig(&file)){
                             file.setFileName(CONFIG_FILE);
                             file.open(QIODevice::ReadOnly | QIODevice::Text);
                             configFileStat = SEARCHING;
                             tempStr.clear();
                             break;
                          }else{
                                return false;
                          }
                        }
                        tempStr.clear();
                        configFileStat = READING_VALUE;
                        break;
                    default:
                        tempStr.append(tempChar);
                        break;
                    }
                    break;
                case (uint)READING_VALUE:
                    tempStr = readValue(&file, &tempChar);
                    switch((uint)projectsParamNames){
                    case (uint)PROJECT_NUMB:
                          if(read)
                            finish = true;
                          else{
                        if((projectNumb = tempStr.toUInt()) == number){
                          read = true;
                          (*parent->getProjectInfo())->setProjectNumb(projectNumb);
                          }
                        if(number * 0.05 < projectNumb * 0.05)
                          finish = true;
                          }
                          break;
                    case (uint)RECENT_NUMB:
                        if(read){
                            ushort recent = tempStr.toUShort();
                            if(recent != 0){
                              *recentNumb = new ushort[1];
                              **recentNumb = tempStr.toUShort();
                              }
                        }
                        break;
                    case (uint)NAME:
                        if(read)
                        (*parent->getProjectInfo())->setProjectName(tempStr);
                        break;
                    case (uint)FULLSCALE_PLANT_NAME:
                        if(read)
                        (*parent->getProjectInfo())->setObjectName(new QString(tempStr));
                        break;
                    case (uint)PATH:
                        if(read)
                        (*parent->getProjectInfo())->setPath(new QString(tempStr));
                        break;
                    case (uint)DEFAULT:
                        if(read)
                        (*parent->getProjectInfo())->setDefaultProj((tempStr.toUShort() == 1)? true : false );
                        break;
                    case (uint)CREATED:
                        if(read)
                            (*parent->getProjectInfo())->setCreated(tempStr);
                        break;
                    case (uint)MODIFIED:
                        if(read)
                            (*parent->getProjectInfo())->setModified(tempStr);
                        break;
                    case (uint)COMPANY_NAME:
                        if(read)
                            (*parent->getProjectInfo())->setCompanyName(new QString(tempStr));
                        break;
                    case (uint)CONFIG_FLANGE_NAME:
                        if(read)
                            (*parent->getProjectInfo())->setFlangeName(new QString(tempStr));
                        break;
                      }
                    tempStr.clear();
                    configFileStat = SEARCHING;
                    break;
                }
            }
        file.close();
        }else{
          return false;
   }
  return true;
}

bool Config::rewriteRecents(uint projectNumb, ushort *recentNum, short numbOfRec, bool afterRemovedProject, bool removeAtStart)
{

  QFile configFile(CONFIG_FILE);
  if(!checkConfigFile(&configFile))
  {
      parent->safeCloseApp();
   }
  if(configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
      QFile tempFile(TEMP_FILE);
      if(tempFile.exists())
          tempFile.remove();
      if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
          unsigned long long counter = 0;
          QTextStream stream(&tempFile);
          stream.setCodec("UTF-8");
          // Zapisywanie paczki danych [General]
          stream << "[General]\n";
          stream << "defaultPath=\"";
          stream << (defaultPath + "\"\n").toUtf8();
          stream << ("autocadPath=\"" + autocadPath + "\"\n").toUtf8();
          stream << "numbOfProj=" + QString::number(numbOfProjects) + "\n";
          stream << "numbOfRec=" + QString::number((afterRemovedProject)? (numbOfRecentProjects - numbOfRec) : ((numbOfRecentProjects < 20 && projectNumb != 0 && recentNum == nullptr)? (numbOfRecentProjects + 1) : numbOfRecentProjects)) + "\n";
          stream << "sort=" + QString::number((uint) sortingType) + "\n";
          stream <<"[Projects]\n";
          if(afterRemovedProject)
              counter += CALCULATE_GENERAL_PACK_NAME_SIZE(0, (-1 * numbOfRec));
          else
            if((numbOfRecentProjects < 20 && projectNumb != 0 && recentNum == nullptr)? 1 : 0)
                counter += CALCULATE_GENERAL_PACK_NAME_SIZE(0, 1);
            else
                counter += CALCULATE_GENERAL_PACK_NAME_SIZE(0, 0);

          // Ustawianie pliku konfiguracyjnego (odczytywanego) na pierwszy projekt
          configFile.seek(*(configDataPosition->getPackNamePosition() + 1) + 12);

          // Dostosowanie pozycji paczek danych + Dostosowanie pozycji projektów oraz recentów
          {
          int diff = counter - *(configDataPosition->getPackNamePosition() + 1);

          // Dostosowanie pozycji paczek danych
          configDataPosition->setPackNamePositionAt(*(configDataPosition->getPackNamePosition() + 1) + diff, 1);
          diff = 0;
          }
          counter += 12;

          if(afterRemovedProject)
              configDataPosition->removeRecentPos(numbOfRec);
          else
              if(numbOfRecentProjects < 20 && projectNumb != 0 && recentNum == nullptr)
                  configDataPosition->addNewRecentPos(counter);

          // Zwiększenie ilości recentów o 1, jeśli mniej niż 20
          if(afterRemovedProject)
             numbOfRecentProjects -= numbOfRec;
          else
            if(numbOfRecentProjects < 20 && projectNumb != 0 && recentNum == nullptr)
              (numbOfRecentProjects)++;

          char tempChar = '\0';
          QString tempStr = QString();
          unsigned long long pos = 0;
          unsigned long long projectPos = 0;
          bool rewrite = false;
          uint projectsCounter = 0;

          // Inicjowanie zmiennych sterujących
          ConfigFileStat configFileStat = SEARCHING;
          ProjectsParamNames *projectsParamNames = nullptr;

          // Odczyt pliku
          while(!configFile.atEnd()){
          if(configFile.getChar(&tempChar))
            if(tempChar == '\n')
            counter += 2;
            else
            counter++;
          else
           continue;
          switch((uint) configFileStat){
              case (uint) SEARCHING:
                  switch((uint) tempChar){
                  case '\n':
                      configFileStat = SEARCHING;
                      stream << '\n';
                      break;
                  default:
                      pos = counter - 1;
                      configFileStat = READING_PARAMS_CONFIG;
                      tempStr.append(tempChar);
                      break;
                  }
                  break;
            case (uint) READING_PARAMS_CONFIG:
              if(tempChar == '='){
                  if(projectsParamNames == nullptr)
                        projectsParamNames = new ProjectsParamNames;
                        if((uint)(*projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                            if(resetAndRestoreConfig(&configFile)){
                             configFile.setFileName(CONFIG_FILE);
                             configFile.open(QIODevice::ReadOnly | QIODevice::Text);
                             configFileStat = SEARCHING;
                             tempStr.clear();
                             break;
                          }else{
                                 delete projectsParamNames;
                                 projectsParamNames = nullptr;
                                 return false;
                          }
                        }
                        tempStr.append(tempChar);
                        stream << tempStr.toUtf8();
                        tempStr.clear();
                        configFileStat = READING_VALUE;


                }else{
                      tempStr.append(tempChar);
                    }
              break;
            case (uint) READING_VALUE:
                  tempStr = readValueAtStart(configFile, tempChar, &counter);
                  switch((uint)*projectsParamNames){
                  case (uint)PROJECT_NUMB:
                      {
                      projectPos = pos;
                        projectsCounter = tempStr.toUInt();
                      if(projectsCounter % 20 == 0 || projectsCounter == 1){
                        if(projectsCounter > numbOfProjects){
                              uint size = (projectsCounter - numbOfProjects) * 0.05 + 1;
                              unsigned long long *temp = new unsigned long long[size];
                              for(uint i = 0; i < size - 1; i++)
                                *(temp + i) = *(configDataPosition->getProjectsPosition() + i);
                              *(temp + size - 1) = pos;
                              configDataPosition->setProjectsPosition(temp);
                              rewrite = true;
                              parent->getMainWin()->setProgressBarSLMaxReady(false);
                              temp = nullptr;
                           }else{
                            configDataPosition->setProjectsPositionAt(pos, projectsCounter * 0.05);
                          }
                        }
                      stream << projectsCounter;
                      stream << tempChar;
                      }
                      break;
                    case (uint)RECENT_NUMB:
                      {
                      ushort number = tempStr.toUShort();
                      if(removeAtStart)
                      {
                          if(number != 0){
                              ushort counterRec = 0;
                              ushort i = 0;
                              for( ; i < numbOfRec; i++)
                                  if(*(recentNum + i) != number)
                                  {
                                      if(*(recentNum + i) < number)
                                          counterRec++;
                                  }else{
                                      break;
                                  }
                              if(i == numbOfRec){
                                  stream << (number - counterRec);
                                  stream << tempChar;
                                  if(QString::number(number).length() != QString::number(number - counterRec).length())
                                      counter--;
                                  configDataPosition->setRecentProjectsPositionAt(projectPos, number - counterRec - 1);
                              }else{
                                  stream << "0\n";
                                  if(QString::number(number).length() != 1)
                                      counter--;
                              }
                          }else{
                              stream << "0\n";
                          }
                      }else{
                          if(afterRemovedProject){
                              if(number != 0){
                                  ushort counterRec = 0;
                                  for(ushort i = 0; i < numbOfRec; i++)
                                      if(*(recentNum + i) < number)
                                          counterRec++;
                                  stream << (number - counterRec);
                                  stream << tempChar;
                                  if(QString::number(number).length() != QString::number(number - counterRec).length())
                                      counter--;
                                  configDataPosition->setRecentProjectsPositionAt(projectPos, number - counterRec - 1);
                              }else{
                                  stream << number;
                                  stream << tempChar;
                              }
                          }else{
                              if(recentNum != nullptr){
                                  if(number != 0){
                                      if(number > numbOfRecentProjects){
                                          unsigned long long *temp = new unsigned long long[number];
                                          for(uint i = 0; i < numbOfRecentProjects; i++)
                                              *(temp + i) = *(configDataPosition->getRecentProjectsPosition() + i);
                                          *(temp + number - 1) = pos;
                                          configDataPosition->setRecentProjectsPosition(temp);
                                          numbOfRecentProjects = number;
                                          rewrite = true;
                                          temp = nullptr;
                                      }else{
                                          if(number < *recentNum){
                                              if(QString::number(number).length() != QString::number(number + 1).length())
                                                  counter++;
                                              stream << (number + 1);
                                              stream << tempChar;
                                              configDataPosition->setRecentProjectsPositionAt(projectPos, number);

                                          }else{
                                              if(number == *recentNum){
                                                  if(QString::number(number).length() != QString::number(1).length())
                                                      counter--;
                                                  stream << 1;
                                                  stream << tempChar;
                                                  configDataPosition->setRecentProjectsPositionAt(projectPos, 0);
                                              }else{
                                                  stream << number;
                                                  stream << tempChar;
                                                  configDataPosition->setRecentProjectsPositionAt(projectPos, number - 1);
                                              }
                                          }
                                      }
                                  }else{
                                      stream << number;
                                      stream << tempChar;
                                  }
                              }else{
                                  if(projectNumb != projectsCounter){
                                      if(number != 0){
                                          if(number < 20){
                                              if(QString::number(number).length() != QString::number(number + 1).length())
                                                  counter++;
                                              stream << (number + 1);
                                              stream << tempChar;
                                              configDataPosition->setRecentProjectsPositionAt(projectPos, number);
                                          }else{
                                              if(number == 20){
                                                  counter--;
                                                  stream << 0;
                                                  stream << tempChar;
                                              }
                                          }
                                      }else{
                                          stream << 0;
                                          stream << tempChar;
                                      }
                                  }else{
                                      stream << 1;
                                      stream << tempChar;
                                      configDataPosition->setRecentProjectsPositionAt(projectPos, 0);
                                  }
                              }
                          }
                      }
                      }
                     break;
                  case NAME:
                      stream << ("\"" + tempStr + "\"").toUtf8();
                      break;
                  case FULLSCALE_PLANT_NAME:
                      stream <<("\"" + tempStr + "\"").toUtf8();
                      break;
                  case PATH:
                      stream <<("\"" + tempStr + "\"").toUtf8();
                      break;
                  case DEFAULT:
                      stream << tempStr + "\n";
                      break;
                  case CREATED:
                      stream <<"\"" + tempStr + "\"";
                      break;
                  case MODIFIED:
                      stream <<"\"" + tempStr + "\"";
                      break;
                  case COMPANY_NAME:
                      stream <<("\"" + tempStr + "\"").toUtf8();
                      break;
                  case CONFIG_FLANGE_NAME:
                      stream <<("\"" + tempStr + "\"").toUtf8();
                      break;
                  }
                  configFileStat = SEARCHING;
                  tempStr.clear();
                }
              }
          configFileStat = SEARCHING;
          if(projectsCounter > numbOfProjects){
              rewrite = true;
              numbOfProjects = projectsCounter;
          }

          tempFile.close();
          configFile.close();
    if(rewrite)
      rewriteGeneral();
    QFile(CONFIG_BACK_UP_FILE).remove();
    configFile.copy(CONFIG_BACK_UP_FILE);
    delete projectsParamNames;
    projectsParamNames = nullptr;
  }else{
          configFile.close();
          return false;
      }
      configFile.remove();
      tempFile.rename(CONFIG_FILE);
     }else{
      return false;
  }
  return true;
}

int Config::removeFromRecentList(ushort removeNumb, bool removeProject){
   int removedProjectNumb = -1;
  QFile configFile(CONFIG_FILE);
  if(!checkConfigFile(&configFile))
  {
      parent->safeCloseApp();
   }
  if(configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
      QFile tempFile(TEMP_FILE);
      if(tempFile.exists())
        tempFile.remove();
      if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
          unsigned long long counter = 0;
          QTextStream stream(&tempFile);
          stream.setCodec("UTF-8");
          // Zapisywanie paczki danych [General]
          stream << "[General]\n";
          stream << "defaultPath=\"";
          stream << (defaultPath + "\"\n").toUtf8();
          stream << ("autocadPath=\"" +autocadPath + "\"\n").toUtf8();
          stream << "numbOfProj=" + QString::number(numbOfProjects) + "\n";
          stream << "numbOfRec=" + QString::number(numbOfRecentProjects - 1)  + "\n";
          stream << "sort=" + QString::number((uint) sortingType) + "\n";
          stream <<"[Projects]\n";
          counter += CALCULATE_GENERAL_PACK_NAME_SIZE(0, -1);
          configDataPosition->removeForRecentList(removeNumb);

          // Ustawianie pliku konfiguracyjnego (odczytywanego) na pierwszy projekt
          configFile.seek(*(configDataPosition->getPackNamePosition() + 1) + 12);

          // Dostosowanie pozycji paczek danych + Dostosowanie pozycji projektów oraz recentów
          {
          int diff = counter - *(configDataPosition->getPackNamePosition() + 1);
          for(ushort i = 0; i < (numbOfRecentProjects - 1)  ; i++)
            configDataPosition->setRecentProjectsPositionAt(*(configDataPosition->getRecentProjectsPosition() + i) + diff, i);
          // Dostosowanie pozycji paczek danych
          configDataPosition->setPackNamePositionAt(*(configDataPosition->getPackNamePosition() + 1) + diff, 1);
          diff = 0;
          }

          counter += 12;
          // Zmniejszenie ilości recentów o 1
          (numbOfRecentProjects)--;

          char tempChar = '\0';
          QString tempStr = QString();
          unsigned long long pos = 0;
          unsigned long long projectPos = 0;
          bool rewrite = false;
          uint projectsCounter = 0;

          // Inicjowanie zmiennych sterujących
          ConfigFileStat configFileStat = SEARCHING;
          ProjectsParamNames projectsParamNames = PROJECT_NUMB;

          // Odczyt pliku
          while(!configFile.atEnd()){
          if(configFile.getChar(&tempChar))
            if(tempChar == '\n')
            counter += 2;
            else
            counter++;
          else
           continue;
          switch((uint) configFileStat){
              case (uint) SEARCHING:
                  switch((uint) tempChar){
                  case '\n':
                      configFileStat = SEARCHING;
                      stream << '\n';
                      break;
                  default:
                      pos = counter - 1;
                      configFileStat = READING_PARAMS_CONFIG;
                      tempStr.append(tempChar);
                      break;
                  }
                  break;
            case (uint) READING_PARAMS_CONFIG:
              if(tempChar == '='){
                  if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                            if(resetAndRestoreConfig(&configFile)){
                             configFile.setFileName(CONFIG_FILE);
                             configFile.open(QIODevice::ReadOnly | QIODevice::Text);
                             configFileStat = SEARCHING;
                             tempStr.clear();
                             break;
                          }else{
                                 return false;
                          }
                        }
                        tempStr.append(tempChar);
                        stream << tempStr.toUtf8();
                        tempStr.clear();
                        configFileStat = READING_VALUE;


                }else{
                      tempStr.append(tempChar);
                    }
              break;
            case (uint) READING_VALUE:
                  tempStr = readValueAtStart(configFile, tempChar, &counter);
                  switch((uint)projectsParamNames){
                  case (uint)PROJECT_NUMB:
                      {
                      projectPos = pos;
                        projectsCounter = tempStr.toUInt();
                      if(projectsCounter % 20 == 0 || projectsCounter == 1){
                        if(projectsCounter > numbOfProjects){
                              uint size = (projectsCounter - numbOfProjects) * 0.05 + 1;
                              unsigned long long *temp = new unsigned long long[size];
                              for(uint i = 0; i < size - 1; i++)
                                *(temp + i) = *(configDataPosition->getProjectsPosition() + i);
                              *(temp + size - 1) = pos;
                              configDataPosition->setProjectsPosition(temp);
                              rewrite = true;
                              parent->getMainWin()->setProgressBarSLMaxReady(false);
                              temp = nullptr;
                           }else{
                            configDataPosition->setProjectsPositionAt(pos, projectsCounter * 0.05);
                          }
                        }
                      stream << projectsCounter;
                      stream << tempChar;
                      }
                      break;
                    case (uint)RECENT_NUMB:
                      {
                      ushort number = tempStr.toUShort();
                      if(number != 0){
                              if(number > removeNumb){
                                         if(QString::number(number).length() != QString::number(number - 1).length())
                                             counter--;
                                         stream << (number - 1);
                                         stream << tempChar;
                                         configDataPosition->setRecentProjectsPositionAt(projectPos, number - 2);

                                      }else{
                                         if(number == removeNumb){
                                             removedProjectNumb = projectsCounter;
                                             if(QString::number(number).length() != QString::number(0).length())
                                             counter--;
                                            stream << 0;
                                            stream << tempChar;
                                         }else{
                                             stream << number;
                                             stream << tempChar;
                                             configDataPosition->setRecentProjectsPositionAt(projectPos, number - 1);
                                             }
                                    }
                                 }else{
                                  stream << number;
                                  stream << tempChar;
                                 }
  }
                     break;
                  case NAME:
                      stream <<( "\"" + tempStr + "\"").toUtf8();
                      break;
                  case FULLSCALE_PLANT_NAME:
                      stream <<("\"" + tempStr + "\"").toUtf8();
                      break;
                  case PATH:
                      stream <<("\"" + tempStr + "\"").toUtf8();
                      break;
                  case DEFAULT:
                      stream << tempStr + "\n";
                      break;
                  case CREATED:
                      stream << "\"" + tempStr + "\"";
                      break;
                  case MODIFIED:
                      stream << "\"" + tempStr + "\"";
                      break;
                  case CONFIG_FLANGE_NAME:
                      stream <<("\"" + tempStr + "\"").toUtf8();
                      break;
                  case COMPANY_NAME:
                      stream <<("\"" + tempStr + "\"").toUtf8();
                      break;
                  }
                  configFileStat = SEARCHING;
                  tempStr.clear();
                }
              }
          configFileStat = SEARCHING;
          if(projectsCounter > numbOfProjects){
              rewrite = true;
              numbOfProjects = projectsCounter;
          }
          tempFile.close();
          configFile.close();
    if(rewrite)
      rewriteGeneral();
    QFile(CONFIG_BACK_UP_FILE).remove();
    configFile.copy(CONFIG_BACK_UP_FILE);
    }else{
          configFile.close();
          return false;
        }
      configFile.remove();
      tempFile.rename(CONFIG_FILE);
      }else{
      return -1;
    }
  return removedProjectNumb;
}

bool Config::removeProjects(uint *projectNumbs, uint numbToRemove, ushort **recentNumb, ushort *numbOfRecents){
    QFile configFile(CONFIG_FILE);

    if(!checkConfigFile(&configFile))
    {
        parent->safeCloseApp();
        }
    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QFile tempFile(TEMP_FILE);
        if(tempFile.exists())
          tempFile.remove();
        if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            unsigned long long counter = 0;
            QTextStream stream(&tempFile);
            stream.setCodec("UTF-8");
            // Zapisywanie paczki danych [General]
            stream << "[General]\n";
            stream << "defaultPath=\"";
            stream << (defaultPath + "\"\n").toUtf8();
            stream << ("autocadPath=\"" + autocadPath + "\"\n").toUtf8();
            stream << "numbOfProj=" + QString::number(numbOfProjects - numbToRemove) + "\n";
            stream << "numbOfRec=" + QString::number(numbOfRecentProjects)  + "\n";
            stream << "sort=" + QString::number((uint) sortingType) + "\n";
            stream <<"[Projects]\n";
            counter += CALCULATE_GENERAL_PACK_NAME_SIZE(-1 * numbToRemove, 0);


            // Ustawianie pliku konfiguracyjnego (odczytywanego) na pierwszy projekt
            configFile.seek(*(configDataPosition->getPackNamePosition() + 1) + 12);

            // Dostosowanie pozycji paczek danych + Dostosowanie pozycji projektów oraz recentów
            {
            int diff = counter - *(configDataPosition->getPackNamePosition() + 1);
            // Dostosowanie pozycji paczek danych
            configDataPosition->setPackNamePositionAt(*(configDataPosition->getPackNamePosition() + 1) + diff, 1);
            diff = 0;
            }

            counter += 12;

            // Zmniejszenie ilości projektów
            numbOfProjects -= numbToRemove;

            char tempChar = '\0';
            QString tempStr = QString();
            unsigned long long pos = 0;
            uint projectsCounter = 0;
            uint actualNumbToRemove = 0;
            uint projectNumberFromFile = 0;
            bool rewrite = false;
            // Inicjowanie zmiennych sterujących
            ConfigFileStat configFileStat = SEARCHING;
            ProjectsParamNames projectsParamNames = PROJECT_NUMB;

            // Odczyt pliku
            while(!configFile.atEnd()){
            if(configFile.getChar(&tempChar))
              if(tempChar == '\n')
              counter += 2;
              else
              counter++;
            else
             continue;
            switch((uint) configFileStat){
                case (uint) SEARCHING:
                    switch((uint) tempChar){
                    case '\n':
                        configFileStat = SEARCHING;
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                        stream << '\n';
                        break;
                    default:
                        pos = counter - 1;
                        configFileStat = READING_PARAMS_CONFIG;
                        tempStr.append(tempChar);
                        break;
                    }
                    break;
              case (uint) READING_PARAMS_CONFIG:
                if(tempChar == '='){
                    if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                              if(resetAndRestoreConfig(&configFile)){
                               configFile.setFileName(CONFIG_FILE);
                               configFile.open(QIODevice::ReadOnly | QIODevice::Text);
                               configFileStat = SEARCHING;
                               tempStr.clear();
                               break;
                            }else{
                                   return false;
                            }
                          }

                          tempStr.clear();
                          configFileStat = READING_VALUE;

                  }else{
                        tempStr.append(tempChar);
                      }
                break;
              case (uint) READING_VALUE:
                    tempStr = readValueAtStart(configFile, tempChar, &counter);
                    switch((uint)projectsParamNames){
                    case (uint)PROJECT_NUMB:
                        {
                          projectNumberFromFile = tempStr.toUInt();
                          projectsCounter = projectNumberFromFile - actualNumbToRemove;
                          if(actualNumbToRemove != numbToRemove){
                          if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove)){
                              if(QString::number(projectNumberFromFile).length() != QString::number(projectsCounter).length())
                                  counter--;
                              if(projectsCounter % 20 == 0 || projectsCounter == 1){
                                if(projectsCounter > numbOfProjects){
                                    uint size = (projectsCounter - numbOfProjects) * 0.05 + 1;
                                    unsigned long long *temp = new unsigned long long[size];
                                    for(uint i = 0; i < size - 1; i++)
                                        *(temp + i) = *(configDataPosition->getProjectsPosition() + i);
                                    *(temp + size - 1) = pos;
                                    configDataPosition->setProjectsPosition(temp);
                                    rewrite = true;
                                    parent->getMainWin()->setProgressBarSLMaxReady(false);
                                    temp = nullptr;
                                }else{
                                    configDataPosition->setProjectsPositionAt(pos, projectsCounter * 0.05);
                                }
                          }
                        stream << "projectNumb=";
                        stream << projectsCounter;
                        stream << tempChar;
                          }
                        }else{
                              if(QString::number(projectNumberFromFile).length() != QString::number(projectsCounter).length())
                                counter -= (QString::number(projectNumberFromFile).length() - QString::number(projectsCounter).length());
                              if(projectsCounter % 20 == 0 || projectsCounter == 1){
                                if(projectsCounter > numbOfProjects){
                                    uint size = (projectsCounter - numbOfProjects) * 0.05 + 1;
                                    unsigned long long *temp = new unsigned long long[size];
                                    for(uint i = 0; i < size - 1; i++)
                                        *(temp + i) = *(configDataPosition->getProjectsPosition() + i);
                                    *(temp + size - 1) = pos;
                                    configDataPosition->setProjectsPosition(temp);
                                    rewrite = true;
                                    parent->getMainWin()->setProgressBarSLMaxReady(false);
                                    temp = nullptr;
                                }else{
                                    configDataPosition->setProjectsPositionAt(pos, projectsCounter * 0.05);
                                }
                          }
                              stream << "projectNumb=";
                              stream << projectsCounter;
                              stream << tempChar;
                            }
                        }
                        break;
                      case (uint)RECENT_NUMB:
                        {
                        ushort number = tempStr.toUShort();
                         if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove)){
                                  stream << "recentNumb=";
                                  stream << number;
                                  stream << tempChar;
                         }else{
                         if(number != 0){
                                (*numbOfRecents)++;
                                if(*recentNumb != nullptr){
                                     ushort *temp = new ushort[*numbOfRecents];
                                    for(ushort i = 0; i < (*numbOfRecents - 1); i++)
                                    *(temp + i) = *(*recentNumb + i);
                                     *(temp + *numbOfRecents - 1) = number;
                                    delete [](*recentNumb);
                                    *recentNumb = temp;
                                }else{
                                    *recentNumb = new ushort[1];
                                    **recentNumb = number;
                                }
                         }
                        }
                    }
                       break;
                    case NAME:
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                        stream << ("name=\"" + tempStr + "\"").toUtf8();
                        break;
                    case FULLSCALE_PLANT_NAME:
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                        stream <<("fullscalePlantName=\"" + tempStr + "\"").toUtf8();
                        break;
                    case PATH:
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                            stream <<("path=\"" + tempStr + "\"").toUtf8();
                        else
                           QFile(tempStr).remove();
                        break;
                    case DEFAULT:
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                        stream << ("default=" + tempStr + "\n").toUtf8();
                        else
                          if(actualNumbToRemove != numbToRemove)
                             actualNumbToRemove++;
                        break;
                    case CREATED:
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                            stream << "created=\"" + tempStr + "\"";
                        break;
                    case MODIFIED:
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                            stream << "modified=\"" + tempStr + "\"";
                        break;
                    case COMPANY_NAME:
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                            stream << ("companyName=\"" + tempStr + "\"").toUtf8();
                        break;
                    case CONFIG_FLANGE_NAME:
                        if(projectNumberFromFile != *(projectNumbs + actualNumbToRemove))
                            stream << ("flangeName=\"" + tempStr + "\"").toUtf8();
                        break;
                    }
                    configFileStat = SEARCHING;
                    tempStr.clear();
                  }
                }
            configFileStat = SEARCHING;
            tempFile.close();
            configFile.close();
            QFile(CONFIG_BACK_UP_FILE).remove();
      configFile.copy(CONFIG_BACK_UP_FILE);
      }else{
            configFile.close();
            return false;
          }
        configFile.remove();
        tempFile.rename(CONFIG_FILE);
        }else{
        return false;
      }
    return true;
}

bool Config::rewriteProject(uint numberOfProjectInfo){

    QFile configFile(CONFIG_FILE);
    if(!checkConfigFile(&configFile))
    {
        parent->safeCloseApp();
        }
    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QFile tempFile(TEMP_FILE);
        if(tempFile.exists())
            tempFile.remove();
        if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            unsigned long long counter = 0;
            QTextStream stream(&tempFile);
            stream.setCodec("UTF-8");
            // Zapisywanie paczki danych [General]
            stream << "[General]\n";
            stream << "defaultPath=\"";
            stream << (defaultPath + "\"\n").toUtf8();
            stream << ("autocadPath=\"" + autocadPath + "\"\n").toUtf8();
            stream << "numbOfProj=" + QString::number(numbOfProjects) + "\n";
            stream << "numbOfRec=" + QString::number(numbOfRecentProjects) + "\n";
            stream << "sort=" + QString::number((uint) sortingType) + "\n";
            counter += CALCULATE_GENERAL_PACK_NAME_SIZE(0, 0);
            stream << "[Projects]\n";

            // Ustawianie pliku konfiguracyjnego (odczytywanego) na pierwszy projekt
            configFile.seek(*(configDataPosition->getPackNamePosition() + 1) + 12);

            // Dostosowanie pozycji paczek danych + Dostosowanie pozycji projektów oraz recentów
            // Dostosowanie pozycji paczek danych
            configDataPosition->setPackNamePositionAt(counter, 1);
            counter += 12;
            char tempChar = '\0';
            QString tempStr = QString();
            unsigned long long pos = 0;
            unsigned long long projectPos = 0;
            uint projectsCounter = 0;
            bool rewrite = false;
            bool edit = false;
            // Inicjowanie zmiennych sterujących
            ConfigFileStat configFileStat = SEARCHING;
            ProjectsParamNames projectsParamNames = PROJECT_NUMB;

            // Odczyt pliku
            while(!configFile.atEnd()){
            if(configFile.getChar(&tempChar))
              if(tempChar == '\n' )
              counter += 2;
              else
              counter++;
            else
             continue;
            switch((uint) configFileStat){
                case (uint) SEARCHING:
                    switch((uint) tempChar){
                    case(uint) '\n':
                        if(!edit)
                            stream << "\n";
                        configFileStat = SEARCHING;
                        break;
                    default:
                        pos = counter - 1;
                        configFileStat = READING_PARAMS_CONFIG;
                        tempStr.append(tempChar);
                        break;
                    }
                    break;
              case (uint) READING_PARAMS_CONFIG:
                if(tempChar == '='){
                     if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                              if(resetAndRestoreConfig(&configFile)){
                               configFile.setFileName(CONFIG_FILE);
                               configFile.open(QIODevice::ReadOnly | QIODevice::Text);
                               configFileStat = SEARCHING;
                               tempStr.clear();
                               break;
                            }else{
                                  return false;
                            }
                          }
                          tempStr.clear();
                          configFileStat = READING_VALUE;
                  }else{
                        tempStr.append(tempChar);
                      }
                break;
              case (uint) READING_VALUE:
                    tempStr = readValueAtStart(configFile, tempChar, &counter);
                    switch((uint)projectsParamNames){
                    case (uint)PROJECT_NUMB:
                        {
                        projectPos = pos;
                          projectsCounter++;
                        if(projectsCounter % 20 == 0 || projectsCounter == 1){
                          if(projectsCounter > numbOfProjects){
                                uint size = (projectsCounter - numbOfProjects) * 0.05 + 1;
                                unsigned long long *temp = new unsigned long long[size];
                                for(uint i = 0; i < size - 1; i++)
                                  *(temp + i) = *(configDataPosition->getProjectsPosition() + i);
                                *(temp + size - 1) = pos;
                                configDataPosition->setProjectsPosition(temp);
                                rewrite = true;
                                parent->getMainWin()->setProgressBarSLMaxReady(false);
                                temp = nullptr;
                             }else{
                              configDataPosition->setProjectsPositionAt(pos, projectsCounter * 0.05);
                            }
                          }
                        if(*(*(parent->getProjectInfo() + numberOfProjectInfo))->getProjectNumb() != projectsCounter)
                            edit = false;
                        else
                            edit = true;
                        stream <<("projectNumb=" + tempStr).toUtf8();
                        stream << tempChar;
                        }
                        break;
                      case (uint)RECENT_NUMB:
                            {
                        if(tempStr.toUShort() != 0)
                            configDataPosition->setRecentProjectsPositionAt(projectPos, tempStr.toUShort() - 1);

                            stream <<("recentNumb=" + tempStr).toUtf8();
                            stream << tempChar;
                            if(edit)
                            {  // Wpisanie danych projektu
                            stream << QString("name=\"" + *(*(parent->getProjectInfo() + numberOfProjectInfo))->getProjectName() + "\"\n").toUtf8();
                            if((*(parent->getProjectInfo() + numberOfProjectInfo))->getCompanyName() != nullptr){
                                stream << QString("companyName=\"" + *(*(parent->getProjectInfo() + numberOfProjectInfo))->getCompanyName() + "\"\n").toUtf8();
                                counter += 16 + App::lengthForFileCounter(*(*(parent->getProjectInfo() + numberOfProjectInfo))->getCompanyName());
                             }
                            if((*(parent->getProjectInfo() + numberOfProjectInfo))->getObjectName() != nullptr)
                            {
                                stream << QString("fullscalePlantName=\"" + *(*(parent->getProjectInfo() + numberOfProjectInfo))->getObjectName() + "\"\n").toUtf8();
                                counter += 23 + App::lengthForFileCounter(*(*(parent->getProjectInfo() + numberOfProjectInfo))->getObjectName());
                            }
                            if((*(parent->getProjectInfo() + numberOfProjectInfo))->getFlangeName() != nullptr){
                                stream << QString("flangeName=\"" + *(*(parent->getProjectInfo() + numberOfProjectInfo))->getFlangeName() + "\"\n").toUtf8();
                                counter += 15 + App::lengthForFileCounter(*(*(parent->getProjectInfo() + numberOfProjectInfo))->getFlangeName());
                            }
                            stream << QString("path=\"" + *(*(parent->getProjectInfo() + numberOfProjectInfo))->getPath() + "\"\n").toUtf8();
                            stream << ("created=\"" + (*(parent->getProjectInfo() + numberOfProjectInfo))->getCreated() + "\"\n").toUtf8();
                            stream << ("modified=\"" + (*(parent->getProjectInfo() + numberOfProjectInfo))->getModified() + "\"\n").toUtf8();
                            stream << ("default=" + (((*(parent->getProjectInfo() + numberOfProjectInfo))->isDefaultProj())? QString("1") : QString("0")) + "\n").toUtf8();
                            counter += App::lengthForFileCounter((*(*(parent->getProjectInfo() + numberOfProjectInfo))->getProjectName())) + App::lengthForFileCounter(*(*(parent->getProjectInfo() + numberOfProjectInfo))->getPath()) + (*(parent->getProjectInfo() + numberOfProjectInfo))->getModified().length();
                        }
                    }
                        break;
                    case NAME:
                        if(!edit)
                        stream << ("name=\"" + tempStr + "\"").toUtf8();
                        else
                            counter -= App::lengthForFileCounter(tempStr);
                        break;
                    case FULLSCALE_PLANT_NAME:
                        if(!edit)
                           stream << ("fullscalePlantName=\"" + tempStr + "\"").toUtf8();
                        else
                            counter -= App::lengthForFileCounter(tempStr) + 23;
                        break;
                    case PATH:
                        if(!edit)
                            stream <<("path=\"" + tempStr + "\"").toUtf8();
                        else
                            counter -= App::lengthForFileCounter(tempStr);
                        break;
                    case DEFAULT:
                        if(!edit)
                        stream <<("default=" + tempStr + "\n").toUtf8();
                        break;
                    case CREATED:
                        if(!edit)
                            stream << ("created=\"" + tempStr + "\"").toUtf8();
                        break;
                    case MODIFIED:
                        if(!edit)
                        stream << ("modified=\"" + tempStr + "\"").toUtf8();
                        else
                            counter -= tempStr.length();
                        break;
                    case CONFIG_FLANGE_NAME:
                        if(!edit)
                        stream << ("flangeName=\"" + tempStr + "\"").toUtf8();
                        else
                            counter -= tempStr.length() + 15;
                        break;
                    case COMPANY_NAME:
                        if(!edit)
                        stream << ("companyName=\"" + tempStr + "\"").toUtf8();
                        else
                            counter -= tempStr.length() + 16;
                        break;
                    }
                    configFileStat = SEARCHING;
                    tempStr.clear();
                  }
                }
            configFileStat = SEARCHING;
            if(projectsCounter > numbOfProjects){
                rewrite = true;
                numbOfProjects = projectsCounter;
            }
            tempFile.close();
            configFile.close();
      if(rewrite)
        rewriteGeneral();
      QFile backUpFile(CONFIG_BACK_UP_FILE);
      backUpFile.remove();
      configFile.copy(CONFIG_BACK_UP_FILE);
      }else{
            configFile.close();
            return false;
        }
        configFile.remove();
        tempFile.rename(CONFIG_FILE);
        }else{
        return false;
    }
    return true;
}

void Config::setAutocadPath(QString set){
    autocadPath = set;
}

QString Config::getAutocadPath(){
    return autocadPath;
}

bool Config::findAutoCadExe(QString* path){
    QFileInfoList drives = QDir::drives();
    QString pathToCheck = QString();
    bool found = false;
    QString strPattern = QString("\\Autodesk\\AutoCAD\?\\acad.exe");
    for(uint i = 0; i < (uint)drives.size(); i++){
        if(drives.value(i).absolutePath().at(0) == 'A')
            continue;
        QDirIterator di(drives.value(i).absolutePath(),QStringList(QString::fromUtf8("acad.exe")), QDir::Files | QDir::Executable, QDirIterator::Subdirectories);
        while(di.hasNext()){
            pathToCheck = QDir::toNativeSeparators(di.next());
            uint pathLen = pathToCheck.length();
            for(uint j = 0; j < pathLen; j++){
                if(pathToCheck.at(j) == '\\'){
                    uint k = 0;
                    for( ; j < pathLen && k < (uint)strPattern.length(); k++)
                    {
                        if(strPattern.at(k) != pathToCheck.at(j))
                        {
                            if(strPattern.at(k) == '\?')
                            {
                                while(j < pathLen && pathToCheck.at(j) != '\\')
                                    j++;
                                j--;
                            }else{
                                break;
                            }
                        }
                        j++;
                    }
                    if(k == (uint)strPattern.length())
                        found = true;
                }
                if(found)
                    break;
            }
            if(found)
                break;
        }
        if(found)
            break;
    }
    if(found)
        *path = pathToCheck;
   return found;
}

bool Config::readForProjectInfoPanel(uint number, QString *infoStr, int& maxWidth, QFontMetrics& fontM){
    QFile file(CONFIG_FILE);
    if(!checkConfigFile(&file))
    {
        parent->safeCloseApp();
        }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
          char tempChar = '\0';
          QString tempStr = QString();
          ConfigFileStat configFileStat = SEARCHING;
          ProjectsParamNames projectsParamNames = RECENT_NUMB;
          bool read = false;
          bool finish = false;
          uint projectNumb = 0;
          int tempWidth = maxWidth;
          file.seek(*(configDataPosition->getProjectsPosition() + (uint)(number * 0.05)));
          while(!file.atEnd() && finish != true){
             file.getChar(&tempChar);
                  switch((uint)configFileStat){
                  case (uint) SEARCHING:
                      switch(tempChar){
                      case '[':
                          while(tempChar != '\n')
                              file.getChar(&tempChar);
                          break;
                      case '\n':
                          break;
                      default:
                          tempStr.append(tempChar);
                          configFileStat = READING_PARAMS_CONFIG;
                          break;
                      }
                      break;
                  case (uint)READING_PARAMS_CONFIG:
                      switch(tempChar){
                      case '=':
                          if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                              if(resetAndRestoreConfig(&file)){
                               file.setFileName(CONFIG_FILE);
                               file.open(QIODevice::ReadOnly | QIODevice::Text);
                               configFileStat = SEARCHING;
                               tempStr.clear();
                               break;
                            }else{
                                  return false;
                            }
                          }
                          tempStr.clear();
                          configFileStat = READING_VALUE;
                          break;
                      default:
                          tempStr.append(tempChar);
                          break;
                      }
                      break;
                  case (uint)READING_VALUE:
                      tempStr = readValue(&file, &tempChar);
                      switch((uint)projectsParamNames){
                      case (uint)PROJECT_NUMB:
                            if(read)
                              finish = true;
                            else{
                          if((projectNumb = tempStr.toUInt()) == number){
                            read = true;
                            }
                          if(number * 0.05 < projectNumb * 0.05)
                            finish = true;
                            }
                            break;
                      case (uint)RECENT_NUMB:
                          break;
                      case (uint)NAME:
                          if(read){
                              infoStr->append(QString("Nazwa projektu:\t") + tempStr + "<br><br>");
                              tempWidth = fontM.horizontalAdvance(QString("Nazwa projektu:  ") + tempStr);
                              maxWidth = (tempWidth > maxWidth)? tempWidth : maxWidth;
                          }
                          break;
                      case (uint)FULLSCALE_PLANT_NAME:
                          if(read){
                              infoStr->append(QString("Nazwa obiektu:\t") + tempStr + "<br><br>");
                              tempWidth = fontM.horizontalAdvance(QString("Nazwa obiektu:  ") + tempStr );
                              maxWidth = (tempWidth > maxWidth)? tempWidth : maxWidth;
                          }
                          break;
                      case (uint)PATH:
                          if(read){
                              infoStr->append(QString("Ścieżka dostępu:\t") + tempStr + "<br><br>");
                              tempWidth = fontM.horizontalAdvance(QString("Ścieżka dostępu:  ") + tempStr );
                              maxWidth = (tempWidth > maxWidth)? tempWidth : maxWidth;
                          }
                          break;
                      case (uint)DEFAULT:
                          if(read)
                              if(tempStr.toInt() == 1)
                                infoStr->append("<b>Projekt Domyślny<br><\b>");
                          break;
                      case (uint)CREATED:
                          if(read)
                              infoStr->append(QString("Data utworzenia:\t") + tempStr + "<br><br>");
                          break;
                      case (uint)MODIFIED:
                          if(read)
                              infoStr->append(QString("Data modyfikacji:\t") + tempStr + "<br><br>");
                          break;
                      case (uint)COMPANY_NAME:
                          if(read){
                              infoStr->append(QString("Nazwa firmy:\t") + tempStr + "<br><br>");
                              tempWidth = fontM.horizontalAdvance(QString("Nazwa firmy:  ") + tempStr);
                              maxWidth = (tempWidth > maxWidth)? tempWidth : maxWidth;
                          }
                          break;
                      case (uint)CONFIG_FLANGE_NAME:
                          if(read){
                              infoStr->append(QString("Nazwa zwężki:\t") + tempStr + "<br><br>");
                              tempWidth = fontM.horizontalAdvance(QString("Nazwa zwężki:  ") + tempStr );
                              maxWidth = (tempWidth > maxWidth)? tempWidth : maxWidth;
                          }
                          break;
                        }
                      tempStr.clear();
                      configFileStat = SEARCHING;
                      break;
                  }
              }
          file.close();
          }else{
        return false;
     }
   return true;
}


uint Config::getProgressBarValue(){
    return (progressBarValue!=nullptr? *progressBarValue : 0);
}

uint Config::getProgressBarMin(){
    return (progressBarMin != nullptr? *progressBarMin : 0);
}

uint Config::getProgressBarMax(){
    return (progressBarMax != nullptr? *progressBarMax : 0);
}

void Config::checkDefaultPathStructure(){
    QDir dir(defaultPath);
     if(!QDir(defaultPath + "OPCalc").exists()){
        dir.mkdir("OPCalc");
    }
    dir.setPath(defaultPath + "OPCalc");
    if(!QDir(defaultPath + "OPCalc\\Projects").exists())
        dir.mkdir("Projects");
    if(!QDir(defaultPath + "OPCalc\\Fluids").exists())
        dir.mkdir("Fluids");
    if(!QDir(defaultPath + "OPCalc\\Corrupted").exists())
        dir.mkdir("Corrupted");
}

void Config::moveDefaultPath(QString oldPath){
    QString tempPath;
    QString newPath;
    QString fileName;
    {
        QDirIterator di(oldPath + "OPCalc\\Projects", QDir::Files);
        while(di.hasNext()){
            tempPath = QDir::toNativeSeparators(di.next());
            QFile tempFile(tempPath);
            for(int i = tempPath.length() - 1; i >= 0; i--){
                if(tempPath.at(i) == '\\'){
                    fileName = tempPath.right(tempPath.length() - i - 1);
                    break;
                }
            }
            newPath = defaultPath + "OPCalc\\Projects\\" + fileName;
            if(QFile(newPath).exists()){
                for(uint i = 1; i < 1000; i++){
                    newPath = defaultPath + "OPCalc\\Projects\\" + fileName.left(fileName.length() - QString(PROJECT_FILE_EXTENSTION).length()) + "_Moved_" + QString::number(i) + PROJECT_FILE_EXTENSTION;
                    if(!QFile(newPath).exists())
                        break;
                }
            }
            QFile(tempPath).rename(newPath);
            QFile(tempPath).remove();
        }
    }
    QDirIterator di(oldPath + "OPCalc\\Fluids", QDir::Files);
    while(di.hasNext()){
        tempPath = QDir::toNativeSeparators(di.next());
        QFile tempFile(tempPath);
        for(int i = tempPath.length() - 1; i >= 0; i--){
            if(tempPath.at(i) == '\\'){
                fileName = tempPath.right(tempPath.length() - i - 1);
                break;
            }
        }
        newPath = defaultPath + "OPCalc\\Fluids\\" + fileName;
        if(QFile(newPath).exists()){
            for(uint i = 1; i < 1000; i++){
                newPath = defaultPath + "OPCalc\\Fluids\\" + fileName.left(fileName.length() - QString(RESOURCES_FILE_EXTENSTION).length()) + "_Moved_" + QString::number(i) + RESOURCES_FILE_EXTENSTION;
                if(!QFile(newPath).exists())
                    break;
            }
        }
        QFile(tempPath).rename(newPath);
        QFile(tempPath).remove();
    }
    QDirIterator dc(oldPath + "OPCalc\\Corrupted", QDir::Files);
    while(dc.hasNext()){
        tempPath = QDir::toNativeSeparators(dc.next());
        QFile tempFile(tempPath);
        for(int i = tempPath.length() - 1; i >= 0; i--){
            if(tempPath.at(i) == '\\'){
                fileName = tempPath.right(tempPath.length() - i - 1);
                break;
            }
        }
        newPath = defaultPath + "OPCalc\\Corrupted\\" + fileName;
        QFile(tempPath).rename(newPath);
        QFile(tempPath).remove();
    }
    // Tworze jak nie ma
    if(!QFile(oldPath + "OPCalc\\flangeMaterials.ini").exists()){
        QFile file(defaultPath + "OPCalc\\flangeMaterials.ini");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
    }else{
        QFile newFile(defaultPath + "OPCalc\\flangeMaterials.ini");
        QFile mergedFile(oldPath + "OPCalc\\flangeMaterials.ini");
        MaterialsInfoFiles::mergeFlangeMaterialsFiles(newFile, mergedFile, *this);
    }
    if(!QFile(oldPath + "OPCalc\\pipeMaterials.ini").exists())  {
        QFile file(defaultPath + "OPCalc\\pipeMaterials.ini");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
    }else{
        QFile newFile(defaultPath + "OPCalc\\pipeMaterials.ini");
        QFile mergedFile(oldPath + "OPCalc\\pipeMaterials.ini");
        MaterialsInfoFiles::mergePipeMaterialsFiles(newFile, mergedFile, *this);
    }
    if(!QFile(oldPath + "OPCalc\\fluids.ini").exists())
    {
        QFile file(defaultPath + "OPCalc\\fluids.ini");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();
    }else{
        QFile(oldPath + "OPCalc\\fluids.ini").rename(defaultPath + "OPCalc\\fluids.ini");
    }
}

int Config::findPath(QString checkPath){
QFile file(CONFIG_FILE);
if(!checkConfigFile(&file))
{
    parent->safeCloseApp();
}
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
      char tempChar = '\0';
      QString tempStr = QString();
      ConfigFileStat configFileStat = SEARCHING;
      ProjectsParamNames projectsParamNames = RECENT_NUMB;
      bool read = false;
      bool finish = false;
      uint projectNumb = 0;
      file.seek(*(configDataPosition->getPackNamePosition() + 1));
      while(!file.atEnd() && finish != true){
         file.getChar(&tempChar);
              switch((uint)configFileStat){
              case (uint) SEARCHING:
                  switch(tempChar){
                  case '[':
                      while(tempChar != '\n')
                          file.getChar(&tempChar);
                      break;
                  case '\n':
                      break;
                  default:
                      tempStr.append(tempChar);
                      configFileStat = READING_PARAMS_CONFIG;
                      break;
                  }
                  break;
              case (uint)READING_PARAMS_CONFIG:
                  switch(tempChar){
                  case '=':
                      if((uint)(projectsParamNames = checkProjectParamName(&tempStr)) == NUMB_OF_PROJECTS_PARAM_NAMES){
                          if(resetAndRestoreConfig(&file)){
                           file.setFileName(CONFIG_FILE);
                           file.open(QIODevice::ReadOnly | QIODevice::Text);
                           configFileStat = SEARCHING;
                           tempStr.clear();
                           break;
                        }else{
                              return -1;
                        }
                      }
                      tempStr.clear();
                      configFileStat = READING_VALUE;
                      break;
                  default:
                      tempStr.append(tempChar);
                      break;
                  }
                  break;
              case (uint)READING_VALUE:
                  tempStr = readValue(&file, &tempChar);
                  switch((uint)projectsParamNames){
                  case (uint)PROJECT_NUMB:
                        if(read)
                          finish = true;
                        else{
                            projectNumb = tempStr.toUInt();
                        }
                      break;
                  case (uint)PATH:
                      if(!read)
                          if(App::compareStr(tempStr, checkPath)){
                              read = true;
                              file.close();
                              return projectNumb;
                          }
                      break;
                  }
                  tempStr.clear();
                  configFileStat = SEARCHING;
                  break;
              }
          }
      file.close();
      if(!read)
          return 0;
      }else{
        return -1;
 }
    return 0;
}
