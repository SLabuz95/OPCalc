#include"materialschangeswidgetdataholder.hpp"
#include"materialschangeswidgetarray.hpp"
#include"readingmaterialsfilesenum.hpp"
#include"formatabledatafield.hpp"
#include<QDir>
#include<QTextStream>
#include"formatabledata.hpp"
#include"appclass.hpp"
#include<QLineEdit>
#include<QFileInfo>
#include"mydialog.hpp"
#include"errormacroes.hpp"

MaterialsChangesWidgetDataHolder::MaterialsChangesWidgetDataHolder(int setK, ushort setDependency, ushort setPressureUnit){
    k = setK;
    dependency = setDependency;
    pressureUnit = setPressureUnit;
}

MaterialsChangesWidgetDataHolder::~MaterialsChangesWidgetDataHolder(){
    if(dependency == 2){
        for(uint i = 0; i < numbOfPressures; i++)
        {
            if(numbOfElem){
                for(uint j = 0; j < *(numbOfElem + i); j++){
                    delete **(*(data + i) + j);
                    **(*(data + i) + j) = nullptr;
                    delete *(*(*(data + i) + j) + 1);
                    *(*(*(data + i) + j) + 1) = nullptr;
                    delete [](*(*(data + i) + j));
                    (*(*(data + i) + j)) = nullptr;
                }
            }
            delete [](*(data + i));
            (*(data + i)) = nullptr;
            delete (*(pressure + i));
            (*(pressure + i)) = nullptr;
        }
        delete []data;
        data = nullptr;
        delete []pressure;
        pressure = nullptr;
        delete []numbOfElem;
        numbOfElem = nullptr;
    }else{
        if(numbOfElem){
            for(uint i = 0; i < *numbOfElem; i++){
                delete **(dataD + i);
                **(data + i) = nullptr;
                delete *(*(dataD + i) + 1);
                *(*(dataD + i) + 1) = nullptr;
                delete [](*(dataD + i));
                (*(dataD + i)) = nullptr;
            }
        }
        if(dataD){
            delete []dataD;
            dataD = nullptr;
        }
        delete []numbOfElem;
        numbOfElem = nullptr;
    }
}

void MaterialsChangesWidgetDataHolder::readDataFromArray(MaterialsChangesWidgetArray* dataArray){
    if(dependency == 2){
        uint index = dataArray->getCurrentIndex();
            if(numbOfElem && *(numbOfElem + index) != 0){
                for(uint i = 0; i < *(numbOfElem + index); i++){
                    delete **(*(data + index) + i);
                    **(*(data + index) + i) = nullptr;
                    delete *(*(*(data + index) + i) + 1);
                    *(*(*(data + index) + i) + 1) = nullptr;
                    delete [](*(*(data + index) + i));
                    (*(*(data + index) + i)) = nullptr;
                }
                delete [](*(data + index));
                (*(data + index)) = nullptr;
                (*(numbOfElem + index)) = 0;
            }
        uint numbOfRow = dataArray->getRow();
        if(numbOfRow != 0){
            (*(data + index)) = new FormatableData<SubstancesParamsFileParam>**[numbOfRow];
            (*(numbOfElem + index)) = numbOfRow;
            for(uint i = 0; i < numbOfRow; i++){
                (*(*(data + index) + i)) = new FormatableData<SubstancesParamsFileParam>*[2]();
                (**(*(data + index) + i)) = new FormatableData<SubstancesParamsFileParam>((*(dataArray->getTemperatureTextEdit() + i))->getDataUTextEdit()->text(), SUBSTANCE_TEMP, false);
                (*(*(*(data + index) + i) + 1)) = new FormatableData<SubstancesParamsFileParam>((*(dataArray->getDataTextEdit() + i))->getDataUTextEdit()->text(), DENSITY, false);
            }
        }
    }else{
        if(numbOfElem)
        for(uint i = 0; i < *numbOfElem ; i++){
            delete **(dataD + i);
            **(dataD + i) = nullptr;
            delete *(*(dataD + i) + 1);
            *(*(dataD + i) + 1) = nullptr;
            delete [](*(dataD + i));
            (*(dataD + i)) = nullptr;
        }
        if(dataD){
            delete []dataD;
            dataD = nullptr;
        }

        if(numbOfElem == nullptr)
            numbOfElem = new uint[1];
        if((*numbOfElem = dataArray->getRow()) != 0)
            dataD = new FormatableData<SubstancesParamsFileParam>**[*numbOfElem];
        for(uint i = 0; i < *numbOfElem; i++){
            (*(dataD + i)) = new FormatableData<SubstancesParamsFileParam>*[2];
            (**(dataD + i)) = new FormatableData<SubstancesParamsFileParam>((*(dataArray->getTemperatureTextEdit() + i))->getDataUTextEdit()->text(), SUBSTANCE_TEMP, false);
            (*(*(dataD + i) + 1)) = new FormatableData<SubstancesParamsFileParam>((*(dataArray->getDataTextEdit() + i))->getDataUTextEdit()->text(), DENSITY, false);
        }
      }
    }


uint MaterialsChangesWidgetDataHolder::getNumbOfPressures(){
    return numbOfPressures;
}

FormatableData<SubstancesParamsFileParam>** MaterialsChangesWidgetDataHolder::getPressure(){
    return pressure;
}

uint* MaterialsChangesWidgetDataHolder::getNumbOfElem(){
    return numbOfElem;
}

FormatableData<SubstancesParamsFileParam>**** MaterialsChangesWidgetDataHolder::getData(){
    return data;
}

bool MaterialsChangesWidgetDataHolder::readFromFile(QFile *file){
    if(!file->exists()){
       return false;
    }else{
        if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
             return false;
        }else{
            char tempChar = '\0';
            QString tempStr = QString();
            MaterialsFileStat fileStat = READING_PARAMS;
            SubstancesParamsFileParam fileParam = DEPENDENCY;
            while(!file->atEnd() ){
                file->getChar(&tempChar);
                if(tempChar == '\n' || tempChar == 20)
                    continue;
                switch(fileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                        if((uint)(fileParam = checkFileParam(&tempStr)) == NUMB_OF_SUBSTANCES_PARAMS_FILE_PARAM){
                            // KOMUNIKAT BLEDU
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
                        dependency = tempStr.toUShort();
                        if(dependency == 1){
                            if(!numbOfElem)
                                numbOfElem = new uint;
                            *numbOfElem = 0;
                        }
                        break;
                    case K_VALUE:
                        k = tempStr.toUShort();
                        break;
                    case PRESSURE_UNIT:
                        pressureUnit = tempStr.toUShort();
                        break;
                    case PRESSURE:
                    {
                        {
                            {
                                FormatableData<SubstancesParamsFileParam> **temp = new FormatableData<SubstancesParamsFileParam>*[numbOfPressures + 1];
                                for(uint i = 0; i < numbOfPressures; i++)
                                    *(temp + i) = *(pressure + i);
                                *(temp + numbOfPressures) = new FormatableData<SubstancesParamsFileParam>(tempStr, PRESSURE, false);
                                if(pressure)
                                    delete []pressure;
                                pressure = temp;
                            }
                            uint *temp = new uint[numbOfPressures + 1];
                            for(uint i = 0; i < numbOfPressures; i++)
                                *(temp + i) = *(numbOfElem + i);
                            *(temp + numbOfPressures) = 0;
                            if(numbOfElem)
                                delete []numbOfElem;
                            numbOfElem = temp;
                        }
                        FormatableData<SubstancesParamsFileParam> ****temp = new FormatableData<SubstancesParamsFileParam>***[numbOfPressures + 1];
                        for(uint i = 0; i < numbOfPressures; i++)
                            *(temp + i) = *(data + i);
                        *(temp + numbOfPressures) = nullptr;
                        if(data)
                            delete []data;
                        data = temp;
                        numbOfPressures++;
                     }
                        break;
                    case SUBSTANCE_TEMP:
                    {
                        if(dependency == 2)
                        {
                            uint current = *(numbOfElem + numbOfPressures - 1);
                            FormatableData<SubstancesParamsFileParam> ***temp = new FormatableData<SubstancesParamsFileParam>**[current + 1];
                            for(uint i = 0; i < current; i++)
                                *(temp + i) = *(*(data + numbOfPressures - 1) + i);
                            *(temp + current) = new FormatableData<SubstancesParamsFileParam>*[2];
                            **(temp +  current) = new FormatableData<SubstancesParamsFileParam>(tempStr, SUBSTANCE_TEMP, false);
                            (*(numbOfElem + numbOfPressures - 1))++;
                            if(*(data + numbOfPressures - 1))
                                delete [](*(data + numbOfPressures - 1));
                            (*(data + numbOfPressures - 1)) = temp;
                        }else{
                            uint current = *numbOfElem ;
                            FormatableData<SubstancesParamsFileParam> ***temp = new FormatableData<SubstancesParamsFileParam>**[current + 1];
                            for(uint i = 0; i < current; i++)
                                *(temp + i) = *(dataD + i);
                            *(temp + current) = new FormatableData<SubstancesParamsFileParam>*[2];
                            **(temp +  current) = new FormatableData<SubstancesParamsFileParam>(tempStr, SUBSTANCE_TEMP, false);
                            (*numbOfElem)++;
                            if(dataD)
                                delete []dataD;
                            dataD  = temp;
                        }
                    }
                        break;
                    case DENSITY:
                        if(dependency == 2)
                            *(*(*(data + numbOfPressures - 1) + *(numbOfElem + numbOfPressures - 1) - 1) + 1) = new FormatableData<SubstancesParamsFileParam>(tempStr, DENSITY, false);
                        else
                            *(*(dataD + *numbOfElem - 1) + 1) = new FormatableData<SubstancesParamsFileParam>(tempStr, DENSITY, false);
                        break;
                    case VISCOSITY:
                        if(dependency == 2)
                            *(*(*(data + numbOfPressures - 1) + *(numbOfElem + numbOfPressures - 1) - 1) + 1) = new FormatableData<SubstancesParamsFileParam>(tempStr, VISCOSITY, false);
                       else
                            *(*(dataD + *numbOfElem - 1) + 1) = new FormatableData<SubstancesParamsFileParam>(tempStr, VISCOSITY, false);
                        break;
                    case PRESSURE_FORMAT:
                        (*(pressure + numbOfPressures - 1))->setFormat(new QString(tempStr));
                        break;
                    case SUBSTANCE_TEMP_FORMAT:
                        if(dependency == 2)
                            (**(*(data + numbOfPressures - 1) + *(numbOfElem + numbOfPressures - 1) - 1))->setFormat(new QString(tempStr));
                        else
                            (**(dataD + *numbOfElem - 1))->setFormat(new QString(tempStr));
                        break;
                    case DENSITY_FORMAT:
                        // Dokładnie tak samo zachowa się dla lepkości więc pomijam do lepkości
                    case VISCOSITY_FORMAT:
                        if(dependency == 2)
                            (*(*(*(data + numbOfPressures - 1) + *(numbOfElem + numbOfPressures - 1) - 1) + 1))->setFormat(new QString(tempStr));
                        else
                            (*(*(dataD + *numbOfElem - 1) + 1))->setFormat(new QString(tempStr));
                        break;

                    }
                    fileStat = READING_PARAMS;
                    tempStr.clear();
                    break;
                }
            }
            file->close();
        }
    }
    return true;
}

bool MaterialsChangesWidgetDataHolder::writeToFile(QFile *file, bool densityData, QString *oldFile){
    if(QFile("temp.txt").exists())
            QFile("temp.txt").remove();
    if(numbOfElem){
        QFileInfo fi;
        fi.setFile(*file);
        QString filePath = QDir::toNativeSeparators(fi.absoluteFilePath());
        file->setFileName("temp.txt");
        if(!file->open(QIODevice::WriteOnly | QIODevice::Text)){
            delete file;
            if(oldFile)
                delete oldFile;
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_TITLE), QString(ERROR_CANT_OPEN_SUBSTANCE_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
      return false;
        }else{
                QTextStream str(file);
                if(dependency == 2){
                       if(densityData){
                            str << "zal=" + QString::number(dependency) + "\n";
                            str << "k=" + QString::number(k) + "\n";
                            str << "pu=" + QString::number(pressureUnit);
                            for(uint i = 0;  i < numbOfPressures; i++){
                                str << "\np=" + (*(pressure + i))->useStringFormat() + " ";
                                str << "pf=" + *(*(pressure + i))->getFormat();
                                for(uint j = 0; j < *(numbOfElem + i); j++){
                                    str << " T=" + (**(*(data + i) + j))->useStringFormat() + " ";
                                    str << "Tf=" + *(**(*(data + i) + j))->getFormat() + " ";
                                    str << "r=" + (*(*(*(data + i) + j) + 1))->useStringFormat() + " ";
                                    str << "rf=" + *(*(*(*(data + i) + j) + 1))->getFormat();
                                }
                            }
                        }else{
                            str << "zal=" + QString::number(dependency) + "\n";
                            str << "k=" + QString::number(k) + "\n";
                            str << "pu=" + QString::number(pressureUnit);
                            for(uint i = 0;  i < numbOfPressures; i++){
                                str << "\np=" + (*(pressure + i))->useStringFormat() + " ";
                                str << "pf=" + *(*(pressure + i))->getFormat();
                                for(uint j = 0; j < *(numbOfElem + i); j++){
                                    str << " T=" + (**(*(data + i) + j))->useStringFormat() + " ";
                                    str << "Tf=" + *(**(*(data + i) + j))->getFormat() + " ";
                                    str << "n=" + (*(*(*(data + i) + j) + 1))->useStringFormat() + " ";
                                    str << "nf=" + *(*(*(*(data + i) + j) + 1))->getFormat();
                                }
                            }
                        }
                    }else{
                        if(densityData){
                            str << "zal=" + QString::number(dependency) + "\n";
                            str << "k=" + QString::number(k) + "\n";
                            for(uint j = 0; j < *numbOfElem; j++){
                                    str << "\nT=" + (*(*(dataD + j)))->useStringFormat() + " ";
                                    str << "Tf=" + *(*(*(dataD + j)))->getFormat()+  " ";
                                    str << "r=" + (*(*(dataD + j) + 1))->useStringFormat() + " ";
                                    str << "rf=" + *(*(*(dataD + j) + 1))->getFormat();
                                }
                      }else{
                            str << "zal=" + QString::number(dependency) + "\n";
                            str << "k=" + QString::number(k) + "\n";
                            for(uint j = 0; j < *numbOfElem; j++){
                                str << "\nT=" + (*(*(dataD + j)))->useStringFormat() + " ";
                                str << "Tf=" + *(*(*(dataD + j)))->getFormat()+  " ";
                                str << "n=" + (*(*(dataD + j) + 1))->useStringFormat() + " ";
                                str << "nf=" + *(*(*(dataD + j) + 1))->getFormat();
                                }
                            }
                    }
                str << "\n"; // Zapis bezpieczeństwa
                file->close();
                QFile((oldFile)? filePath : *oldFile).remove();
                file->rename(filePath);
        }
        delete file;
        if(oldFile)
            delete oldFile;
    }
        return true;
}


SubstancesParamsFileParam MaterialsChangesWidgetDataHolder::checkFileParam(QString *tempStr){
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
    case 2:
        nameStr.append("Tf");
        for(i = 0; i < 2; i++)
            if(tempStr->at(i) != nameStr.at(i))
                break;
        if(i == 2)
            return SUBSTANCE_TEMP_FORMAT;
        nameStr = QString("pf");
        for(i = 0; i < 2; i++)
            if(tempStr->at(i) != nameStr.at(i))
                break;
        if(i == 2)
            return PRESSURE_FORMAT;
        nameStr = QString("rf");
        for(i = 0; i < 2; i++)
            if(tempStr->at(i) != nameStr.at(i))
                break;
        if(i == 2)
            return DENSITY_FORMAT;
        nameStr = QString("nf");
        for(i = 0; i < 2; i++)
            if(tempStr->at(i) != nameStr.at(i))
                break;
        if(i == 2)
            return VISCOSITY_FORMAT;
        nameStr = QString("pu");
        for(i = 0; i < 2; i++)
            if(tempStr->at(i) !=  nameStr.at(i))
                break;
        if(i == 2)
            return PRESSURE_UNIT;
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

QString MaterialsChangesWidgetDataHolder::readValue(QFile *file, char *tempCharFunc){
  QTextStream stream(file);
  stream.setCodec("UTF-8");
    QString tempStrFunc = QString();
        tempStrFunc.append(*tempCharFunc);
        while(!file->atEnd()){
            stream.device()->getChar(tempCharFunc);
            if(*tempCharFunc == ' ' || *tempCharFunc == '\n')
              {
                break;
                }
            else
              {
                tempStrFunc.append(*tempCharFunc);
              }
        }
    return tempStrFunc;
}

int MaterialsChangesWidgetDataHolder::getK(){
    return k;
}

ushort MaterialsChangesWidgetDataHolder::getDependency(){
    return dependency;
}

FormatableData<SubstancesParamsFileParam>*** MaterialsChangesWidgetDataHolder::getDataD(){
    return dataD;
}

ushort MaterialsChangesWidgetDataHolder::getPressureUnit(){
    return pressureUnit;
}

uint MaterialsChangesWidgetDataHolder::findPressureToAdd(double findPre){
    if(numbOfPressures){
        for(uint i = 0; i < numbOfPressures; i++){
            if(*(*(pressure + i))->getData() > findPre)
                return i;
        }
        return numbOfPressures;
    }else{
        return 0;
    }
}

void MaterialsChangesWidgetDataHolder::addPressure(uint index, QString value){
    if(numbOfPressures != 0){
        {
            FormatableData<SubstancesParamsFileParam> **temp = new FormatableData<SubstancesParamsFileParam>*[numbOfPressures + 1];
            for(uint i = 0; i < numbOfPressures; i++){
                if(i < index){
                    *(temp + i) = *(pressure + i);
                }else{
                    *(temp + i + 1) = *(pressure + i);
                }
            }
            *(temp + index) = new FormatableData<SubstancesParamsFileParam>(value, PRESSURE, false);
            delete []pressure;
            pressure = temp;
        }
        {
            uint *temp = new uint[numbOfPressures + 1];
            for(uint i = 0; i < numbOfPressures; i++){
                if(i < index){
                    *(temp + i) = *(numbOfElem + i);
                }else{
                    *(temp + i + 1) = *(numbOfElem + i);
                }
            }
            *(temp + index) = 0;
            delete []numbOfElem;
            numbOfElem = temp;
        }
        FormatableData<SubstancesParamsFileParam> ****temp = new FormatableData<SubstancesParamsFileParam>***[numbOfPressures + 1];
        for(uint i = 0; i < numbOfPressures; i++){
            if(i < index){
                *(temp + i) = *(data + i);
            }else{
                *(temp + i + 1) = *(data + i);
            }
        }
        *(temp + index) = nullptr;
        delete []data;
        data = temp;
    }else{
        pressure = new FormatableData<SubstancesParamsFileParam>*[1];
        *pressure = new FormatableData<SubstancesParamsFileParam>(value, PRESSURE, false);
        numbOfElem = new uint[1];
        *numbOfElem = 0;
        data = new FormatableData<SubstancesParamsFileParam>***[1];
        *data = nullptr;
    }
    numbOfPressures++;
}

void MaterialsChangesWidgetDataHolder::removePressure(uint index){
    if(dependency == 2)
    {
        if(numbOfPressures > 1){
            {
                FormatableData<SubstancesParamsFileParam> ****temp = new FormatableData<SubstancesParamsFileParam>***[numbOfPressures - 1];
                for(uint i = 0; i < numbOfPressures; i++){
                    if(i < index){
                        (*(temp + i)) = (*(data + i));
                    }else{
                        if(i != index)
                            (*(temp + i - 1)) = (*(data + i));
                        else{
                            if((*(data + i))){
                                for(uint j = 0; j < *(numbOfElem + i); j++){
                                    delete **(*(data + i) + j);
                                    (**(*(data + i) + j)) = nullptr;
                                    delete *(*(*(data + i) + j) + 1);
                                    (*(*(*(data + i) + j) + 1)) = nullptr;
                                    delete [](*(*(data + i) + j));
                                    (*(*(data + i) + j)) = nullptr;
                                }
                                delete [](*(data + i));
                                (*(data + i)) = nullptr;
                            }
                        }
                    }
                }
                delete []data;
                data = temp;
            }
            {
                uint *temp = new uint[numbOfPressures - 1];
                for(uint i = 0; i < numbOfPressures; i++){
                    if(i < index){
                        (*(temp + i)) = (*(numbOfElem + i));
                    }else{
                        if(i != index){
                            (*(temp + i - 1)) = (*(numbOfElem + i));
                        }
                    }
                }
                    delete []numbOfElem;
                    numbOfElem = temp;
            }
            {
                FormatableData<SubstancesParamsFileParam> **temp = new FormatableData<SubstancesParamsFileParam>*[numbOfPressures - 1];
                for(uint i = 0; i < numbOfPressures; i++){
                    if(i < index){
                        (*(temp + i)) = (*(pressure + i));
                    }
                    else
                    {
                        if(i != index){
                            (*(temp + i - 1)) = (*(pressure + i));
                        }
                        else
                        {
                            delete (*(pressure + i));
                            (*(pressure + i)) = nullptr;
                        }
                    }
                }
                delete []pressure;
                pressure = temp;
            }

        }else{
            if(*data){
                for(uint j = 0; j < *numbOfElem; j++){
                    delete **(*data + j);
                    (**(*data + j)) = nullptr;
                    delete *(*(*data + j) + 1);
                    (*(*(*data + j) + 1)) = nullptr;
                    delete [](*(*data + j));
                    (*(*data + j)) = nullptr;
                }
                delete [](*data);
                (*data) = nullptr;
                delete []data;
                data = nullptr;
                delete *pressure;
                (*pressure) = nullptr;
            }
            delete []numbOfElem;
            numbOfElem = nullptr;
            delete []pressure;
            pressure = nullptr;
        }
        numbOfPressures--;
    }
}

uint MaterialsChangesWidgetDataHolder::getSelectedIndex(){
    return selectedIndex;
}

uint MaterialsChangesWidgetDataHolder::changePressure(QString newValue, uint index){
    uint newIndex = findPressureToAdd(newValue.toDouble());
    if(index < newIndex){
        newIndex--;
        uint oldNumbOfElem = *(numbOfElem + index);
        FormatableData<SubstancesParamsFileParam> *pressureTemp = (*(pressure + index));
        FormatableData<SubstancesParamsFileParam> ***oldData = *(data + index);
        for(uint i = index; i < newIndex; i++)
        {
            *(pressure + i) = *(pressure + i + 1);
            *(numbOfElem + i) = *(numbOfElem + i + 1);
            *(data + i) = *(data + i + 1);
        }
        (*(pressure + newIndex)) = pressureTemp;
        (*(pressure + newIndex))->setData(newValue);
        (*(numbOfElem + newIndex)) = oldNumbOfElem;
        (*(data + newIndex)) = oldData;
    }else{
        if(index > newIndex){
            uint oldNumbOfElem = *(numbOfElem + index);
            FormatableData<SubstancesParamsFileParam> *pressureTemp = (*(pressure + index));
            FormatableData<SubstancesParamsFileParam> ***oldData = *(data + index);
            for(uint i = index - 1; i > newIndex; i--)
            {
                *(pressure + i + 1) = *(pressure + i);
                *(numbOfElem + i + 1) = *(numbOfElem + i);
                *(data + i + 1) = *(data + i);
            }
            *(pressure + newIndex + 1) = *(pressure + newIndex);
            *(numbOfElem + newIndex + 1) = *(numbOfElem + newIndex);
            *(data + newIndex + 1) = *(data + newIndex);
            (*(pressure + newIndex)) = pressureTemp;
            (*(pressure + newIndex))->setData(newValue);
            *(numbOfElem + newIndex) = oldNumbOfElem;
            *(data + newIndex) = oldData;
        }else{
            (*(pressure + newIndex))->setData(newValue);
        }
    }
    if(selectedIndex == index)
        selectedIndex = newIndex;
    else{
        if(selectedIndex > index && selectedIndex <= newIndex)
            selectedIndex--;
        else
            if(selectedIndex < index && selectedIndex >= newIndex)
                selectedIndex++;
    }
    return newIndex;
}

int MaterialsChangesWidgetDataHolder::check(){
    if(dependency == 2){
        if(numbOfPressures < 2)
            return 1;
        else{
            for(uint i = 0; i < numbOfPressures; i++){
                if(*(numbOfElem + i) < 2)
                    return 2 + i;
            }
        }
    }else{
        if(*numbOfElem < 2)
            return 2;
    }
    return 0;
}

void MaterialsChangesWidgetDataHolder::sort(){
    if(dependency == 2){
        for(uint i = 0; i < numbOfPressures; i++){
            App::quickSort(*(data + i), *(numbOfElem + i));
        }
    }else{
        App::quickSort(dataD, *numbOfElem);
    }
}

void MaterialsChangesWidgetDataHolder::setK(int set){
    k = set;
}

void MaterialsChangesWidgetDataHolder::setDependency(ushort set){
    dependency = set;
}

void MaterialsChangesWidgetDataHolder::setSelectedIndex(uint index){
    selectedIndex = index;
}

void MaterialsChangesWidgetDataHolder::setPressureUnit(ushort set){
    pressureUnit = set;
}

double MaterialsChangesWidgetDataHolder::getCorrectionFactorValue(){
    switch(pressureUnit){
    case 0:
        break;
    case 1:
        return AT_TO_ATM_CORRECTION_FACTOR;
    case 2:
        return AT_TO_BAR_CORRECTION_FACTOR;
    case 3:
        return AT_TO_PSI_CORRECTION_FACTOR;
    case 4:
        return AT_TO_KPAS_CORRECTION_FACTOR;
    }
    return 1;
}

double MaterialsChangesWidgetDataHolder::getReverseCorrectionFactorValue(){
    switch(pressureUnit){
    case 0:
        break;
    case 1:
        return ATM_TO_AT_CORRECTION_FACTOR;
    case 2:
        return BAR_TO_AT_CORRECTION_FACTOR;
    case 3:
        return PSI_TO_AT_CORRECTION_FACTOR;
    case 4:
        return KPAS_TO_AT_CORRECTION_FACTOR;
    }
    return 1;
}

void MaterialsChangesWidgetDataHolder::refreshPressureArray(FormatableDataField<SubstancesParamsFileParam>** list){
    for(uint i = 0; i < numbOfPressures; i++){
        if((*(pressure + i)) != (*(list + i))->getFormatableData()){
            FormatableData<SubstancesParamsFileParam> *tempListElement =  (*(list + i))->getFormatableData();
            for(uint j = i + 1 ; j < numbOfPressures; j++){
                if(tempListElement == (*(pressure + i))){
                    FormatableData<SubstancesParamsFileParam> ***tempData = (*(data + j));
                    (*(data + j)) = (*(data + i));
                    (*(data + i)) = tempData;
                    uint tempNumbOfElem =  (*(numbOfElem + j));
                    (*(numbOfElem + j)) = (*(numbOfElem + i));
                    (*(numbOfElem + i)) = tempNumbOfElem;
                    FormatableData<SubstancesParamsFileParam> *tempPres = (*(pressure + j));
                    (*(pressure + j)) = (*(pressure + i));
                    (*(pressure + i)) = tempPres;
                    break;
                }
            }
        }

    }
}
