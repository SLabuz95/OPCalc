#include"projectmenager.hpp"
#include"projectinfo.hpp"
#include<QFile>
#include<QDir>
#include<QTextStream>
#include"errormacroes.hpp"
#include"readingprojectfile.hpp"
#include"appclass.hpp"
#include"mydialog.hpp"
#include<QTextCodec>
#include"formatabledatafield.hpp"
#include"appwindow.hpp"
#include"promptpanel.hpp"
#include"corruptedfilesmenager.hpp"

int ProjectMenager::readProject(ProjectInfo *projectInfo){
    QFile file(*projectInfo->getPath());
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        char tempChar = '\0';
        ProjectFileStat projectFileStat = READING_PARAMS;
        ProjectMenagerParam projectMenagerParam = PROJECT_MENAGER_COMPANY_NAME;
        while(!file.atEnd()){
            file.getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(projectFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                if((projectMenagerParam = checkProjectMenagerParamName(&tempStr)) == NUMB_OF_PROJECT_MENAGER_PARAM){
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd krytyczny pliku"), QString("Plik został uszkodzony. Zostanie przeniesiony do folderu plików uszkodzonych."), false, QString("Ok"), QString(), QString(),nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    CorruptedFilesMenager::addCorruptedFile(file, *projectInfo->getParent()->getConfiguration());
                    return -2;
                }else{
                   projectFileStat = READING_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READING_VALUE:
                tempStr = QString(readValue(&file, &tempChar));
                switch(projectMenagerParam){
                case PROJECT_MENAGER_COMPANY_NAME:
                    if(nameCheck(tempStr))
                        projectInfo->setCompanyName(new QString(tempStr));
                    else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_DEFAULT:
                    projectInfo->setDefaultProj((tempStr.toUShort() == 1)? true : false);
                    break;
                case PROJECT_MENAGER_DOUBLE_SIDE_FLOW:
                {
                    bool *temp = new bool;
                    *temp = (tempStr.toUShort() == 1)? true : false;
                    projectInfo->setDoubleSideFlow(temp);
                }
                    break;
                case PROJECT_MENAGER_FLANGE_NAME:
                    if(nameCheck(tempStr))
                        projectInfo->setFlangeName(new QString(tempStr));
                    else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_FLOW:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                            projectInfo->setFlow(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_FLOW, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_FLUID_TYPE:
                    projectInfo->setFluidType([](QString str)->uint*{uint *ret = new uint; *ret = str.toUInt(); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_MAX_DIFF_PRESSURE:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setMaxDiffPressure(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_MAX_DIFF_PRESSURE, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_MAX_FLOW_VALUE:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setMaxFlowValue(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_MAX_FLOW_VALUE, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setMaxUpstreamPressure(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_MEASUREMENT_DEVICE:
                    projectInfo->setMeasurementDevice([](QString str)->uint*{uint *ret = new uint; *ret = str.toUInt(); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_MEASUREMENT_TYPE:
                    projectInfo->setMeasurementType([](QString str)->uint*{uint *ret = new uint; *ret = str.toUInt(); return ret;}(tempStr));
                    break;
               case PROJECT_MENAGER_OBJECT_NAME:
                    if(nameCheck(tempStr))
                        projectInfo->setObjectName(new QString(tempStr));
                    else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
               case PROJECT_MENAGER_PIPE_DIAMETER:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setPipeDiameter(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_PIPE_DIAMETER, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_PIPE_MATERIAL:
                    projectInfo->setPipeMaterial([](QString str)->uint*{uint *ret = new uint; *ret = str.toUInt(); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_PRESSURE_TAP_DIAMETER:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setPressureTapDiameter(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_PRESSURE_TAP_DIAMETER, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_PROJECT_NAME:
                    if(nameCheck(tempStr))
                        projectInfo->setProjectName(tempStr);
                    else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_TEMP:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, true)){
                        projectInfo->setTemp(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_TEMP, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_UPSTREAM_DEVICE:
                    projectInfo->setUpstreamDevice([](QString str)->uint*{uint *ret = new uint; *ret = str.toUInt(); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_CREATED:
                    projectInfo->setCreated(tempStr);
                    break;
                case PROJECT_MENAGER_MODIFIED:
                    projectInfo->setModified(tempStr);
                    break;
                case PROJECT_MENAGER_MAX_DIFF_PRESSURE_UNITS:
                    projectInfo->setMaxDiffPressureUnits([](QString str)->ushort*{ushort *ret = new ushort; *ret = str.toUShort(); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE_UNITS:
                    projectInfo->setMaxUpstreamPressureUnits([](QString str)->ushort*{ushort *ret = new ushort; *ret = str.toUShort(); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_FLOW_UNITS:
                    projectInfo->setFlowUnits([](QString str)->ushort*{ushort *ret = new ushort; *ret = str.toUShort(); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_TEMP_UNITS:
                    projectInfo->setTempUnits([](QString str)->ushort*{ushort *ret = new ushort; *ret = str.toUShort(); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_RESULTS:
                    projectInfo->setResults((tempStr.toUShort() == 1)? true : false);
                    break;
                case PROJECT_MENAGER_FLUID_NAME:
                    projectInfo->setFluidName(new QString(tempStr));
                    break;
                case PROJECT_MENAGER_FLUID_STAT:
                    projectInfo->setSubstanceStat(new QString(tempStr));
                    break;
                case PROJECT_MENAGER_WORK_DENSITY:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setFluidDensity(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_WORK_DENSITY, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setFluidDynamicViscosity(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setFluidHeatCapacityRatio(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_FLANGE_DIAMETER:
                    projectInfo->setFlangeDiameter(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_FLANGE_DIAMETER, false));
                    break;
                case PROJECT_MENAGER_CHOKE_POINT:
                    projectInfo->setChokePoint(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_CHOKE_POINT, false));
                    break;
                case PROJECT_MENAGER_FLOW_FACTOR:
                    projectInfo->setFlowFactor(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_FLOW_FACTOR, false));
                    break;
                case PROJECT_MENAGER_EXPANSTION_NUMBER:
                    projectInfo->setExpanstionNumber(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_EXPANSTION_NUMBER, false));
                    break;
                case PROJECT_MENAGER_REYNOLDS_NUMBER:
                    projectInfo->setReynoldsNumber(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_REYNOLDS_NUMBER, false));
                    break;
                case PROJECT_MENAGER_PRESSURE_LOSS:
                    projectInfo->setPressureLoss(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_PRESSURE_LOSS, false));
                    break;
                case PROJECT_MENAGER_DENSITY_CUSTOM:
                    projectInfo->setFluidDensityCustom([](QString str)->bool*{bool* ret = new bool; *ret = ((str.toUInt() == 1)? true : false); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_VISCO_CUSTOM:
                    projectInfo->setFluidDynamicViscosityCustom([](QString str)->bool*{bool* ret = new bool; *ret = ((str.toUInt() == 1)? true : false); return ret;}(tempStr));
                    break;
                case PROJECT_MENAGER_DENSITY_FROM_FILE:
                    projectInfo->setFluidDensityFromFile(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_DENSITY_FROM_FILE, false));
                    break;
                case PROJECT_MENAGER_VISCO_FROM_FILE:
                    projectInfo->setFluidDynamicViscosityFromFile(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_VISCO_FROM_FILE, false));
                    break;
                case PROJECT_MENAGER_FLANGE_LINEAR_EXPANSTION:
                    projectInfo->setFlangeLinearExpanstionFactor(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_FLANGE_LINEAR_EXPANSTION, false));
                    break;
                case PROJECT_MENAGER_PIPE_LINEAR_EXPANSTION:
                    projectInfo->setPipeLinearExpanstionFactor(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_PIPE_LINEAR_EXPANSTION, false));
                    break;
                case PROJECT_MENAGER_COMPRESIBILITY_FACTOR:
                    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempStr, false)){
                        projectInfo->setCompresibilityFactor(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_COMPRESIBILITY_FACTOR, false));
                    }else{
                         projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_OTHER_SUBSTANCE_NAME:
                    if(nameCheck(tempStr))
                        projectInfo->setOtherSubstanceName(new QString(tempStr));
                    else{
                        projectInfo->getParent()->getMainWin()->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_READED, projectInfo, projectMenagerParam, nullptr);
                    }
                    break;
                case PROJECT_MENAGER_OTHER_SUBSTANCE_STAT:
                    projectInfo->setOtherSubstanceStat(new QString(tempStr));
                    break;
                case PROJECT_MENAGER_UPSTREAM_DEVICE_DISTANCE:
                    projectInfo->setUpstreamDeviceDistance(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_UPSTREAM_DEVICE_DISTANCE, false));
                    break;
                case PROJECT_MENAGER_DOWNSTREAM_DISTANCE:
                    projectInfo->setDownstreamDistance(new FormatableData<ProjectMenagerParam>(tempStr, PROJECT_MENAGER_DOWNSTREAM_DISTANCE, false));
                    break;
                case PROJECT_MENAGER_PATH:
                    if(App::compareStr(tempStr, file.fileName()))
                        projectInfo->setPath(new QString(tempStr));
                    else{
                        projectInfo->setPath(new QString(QDir::toNativeSeparators(file.fileName())));
                    }
                    break;
                case PROJECT_MENAGER_FLANGE_MATERIAL_NAME:
                    projectInfo->setFlangeMaterialName(tempStr);
                    break;
                case PROJECT_MENAGER_PIPE_MATERIAL_NAME:
                    projectInfo->setPipeMaterialName(tempStr);
                    break;
                default:
                    break;
                }
                tempStr.clear();
                projectFileStat = READING_PARAMS;
           }
        }
        file.close();
        projectInfo->checkFormatableDataValidity();
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return -1;
      }
    projectInfo->setSave(false);
    return 0;
}

bool ProjectMenager::writeProject(ProjectInfo *projectInfo){
    QFile file("temp.txt");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    if(file.exists())
        file.remove();
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << ("name=\"" + *projectInfo->getProjectName() + "\"\n").toUtf8();
        stream << "created=\"" + projectInfo->getCreated() + "\"\n";
        stream << "modified=\"" + projectInfo->getModified() + "\"\n";
        stream << ("path=\"" + *projectInfo->getPath() + "\"\n").toUtf8();
        if(projectInfo->getCompanyName() != nullptr)
            stream << ("companyName=\"" + *projectInfo->getCompanyName() + "\"\n").toUtf8();
        if(projectInfo->getObjectName() != nullptr)
            stream << ("objectName=\"" + *projectInfo->getObjectName() + "\"\n").toUtf8();
        if(projectInfo->getFlangeName() != nullptr)
            stream << ("flangeName=\"" + *projectInfo->getFlangeName() + "\"\n").toUtf8();
        stream << "default=" + QString::number((projectInfo->isDefaultProj() == true)? 1 : 0) + "\n";
        stream << "result=" + QString((projectInfo->isResults())? "1" : "0") + QString("\n");
        if(projectInfo->getMeasurementType() != nullptr)
            stream << "measurementType=" + QString::number(*projectInfo->getMeasurementType()) + "\n";
        if(projectInfo->getMeasurementDevice() != nullptr)
            stream << "measurementDev=" + QString::number(*projectInfo->getMeasurementDevice()) + "\n";
        if(projectInfo->getPressureTapDiameter())
            stream << "presTapDia=" + projectInfo->getPressureTapDiameter()->useStringFormat() + "\n";
        if(projectInfo->getMaxDiffPressure())
            stream << "maxDiffPres=" + projectInfo->getMaxDiffPressure()->useStringFormat() + "\n";
        if(projectInfo->getMaxDiffPressureUnits())
            stream << "maxDiffPresU=" + QString::number(*projectInfo->getMaxDiffPressureUnits()) + "\n";
        if(projectInfo->getPipeMaterial())
            stream << "pipeMaterial=" + QString::number(*projectInfo->getPipeMaterial()) + "\n";
        if(projectInfo->getUpstreamDevice())
            stream << "upstrDev=" + QString::number(*projectInfo->getUpstreamDevice()) + "\n";
        if(projectInfo->getPipeDiameter())
            stream << "pipeDia=" + projectInfo->getPipeDiameter()->useStringFormat() + "\n";
        if(projectInfo->getMaxUpstreamPressure())
            stream << "maxUpstrPres=" + projectInfo->getMaxUpstreamPressure()->useStringFormat() + "\n";
        if(projectInfo->getMaxUpstreamPressureUnits())
            stream << "maxUpstrPresU=" + QString::number(*projectInfo->getMaxUpstreamPressureUnits()) + "\n";
        if(projectInfo->getFluidType())
            stream << "fluidType=" + QString::number(*projectInfo->getFluidType()) + "\n";
        if(projectInfo->getMaxFlowValue())
            stream << "maxFlowValue=" + projectInfo->getMaxFlowValue()->useStringFormat() + "\n";
        if(projectInfo->getFlow())
            stream << "flow=" + projectInfo->getFlow()->useStringFormat() + "\n";
        if(projectInfo->getFlowUnits())
            stream << "flowU=" + QString::number(*projectInfo->getFlowUnits()) + "\n";
        if(projectInfo->getTemp())
            stream << "temp=" + projectInfo->getTemp()->useStringFormat() + "\n";
        if(projectInfo->getTempUnits())
            stream << "tempU=" + QString::number(*projectInfo->getTempUnits()) + "\n";
        if(projectInfo->getDoubleSideFlow())
            stream << "doubSideFlow=" + QString::number((*projectInfo->getDoubleSideFlow() == true)? 1 : 0) + "\n";
        if(projectInfo->getFluidName())
            stream << "fluidName=\"" + *projectInfo->getFluidName() + "\"\n";
        if(projectInfo->getSubstanceStat())
            stream << "fluidStat=\"" + *projectInfo->getSubstanceStat() + "\"\n";
        if(projectInfo->getFluidDensity())
            stream << "fluidDen=" + projectInfo->getFluidDensity()->useStringFormat() + "\n";
        if(projectInfo->getFluidDynamicViscosity())
            stream << "fluidDynVisco=" + projectInfo->getFluidDynamicViscosity()->useStringFormat() + "\n";
        if(projectInfo->getFluidHeatCapacityRatio())
            stream << "fluidHeatCapaRat=" + projectInfo->getFluidHeatCapacityRatio()->useStringFormat() + "\n";
        if(projectInfo->getFlangeDiameter())
            stream << "flangeDia=" + projectInfo->getFlangeDiameter()->useStringFormat() + "\n";
        if(projectInfo->getChokePoint())
            stream << "chokePoint=" + projectInfo->getChokePoint()->useStringFormat() + "\n";
        if(projectInfo->getFlowFactor())
            stream << "flowFactor=" + projectInfo->getFlowFactor()->useStringFormat() + "\n";
        if(projectInfo->getExpanstionNumber())
            stream << "expanNumb=" + projectInfo->getExpanstionNumber()->useStringFormat() + "\n";
        if(projectInfo->getReynoldsNumber())
            stream << "reyNum=" + projectInfo->getReynoldsNumber()->useStringFormat() + "\n";
        if(projectInfo->getPressureLoss())
            stream << "presLose=" + projectInfo->getPressureLoss()->useStringFormat() + "\n";
        if(projectInfo->getFluidDensityCustom())
            stream << "densityC=" + ((*projectInfo->getFluidDensityCustom())? QString("1") : QString("0")) + "\n";
        if(projectInfo->getFluidDynamicViscosityCustom())
            stream << "viscoC=" + ((*projectInfo->getFluidDynamicViscosityCustom())? QString("1") : QString("0")) + "\n";
        if(projectInfo->getFluidDensityFromFile())
            stream << "densityFile=" + projectInfo->getFluidDensityFromFile()->useStringFormat() + "\n";
        if(projectInfo->getFluidDynamicViscosityFromFile())
            stream << "viscoFile=" + projectInfo->getFluidDynamicViscosityFromFile()->useStringFormat() + "\n";
        if(projectInfo->getFlangeLinearExpanstionFactor())
            stream << "flangeLinExpF=" + projectInfo->getFlangeLinearExpanstionFactor()->useStringFormat() + "\n";
        if(projectInfo->getPipeLinearExpanstionFactor())
            stream << "pipeLinExpF=" + projectInfo->getPipeLinearExpanstionFactor()->useStringFormat() + "\n";
        if(projectInfo->getCompresibilityFactor())
            stream << "compresFactor=" + projectInfo->getCompresibilityFactor()->useStringFormat() + "\n";
        if(projectInfo->getOtherSubstanceName())
            stream << "otherSubName=\"" + *projectInfo->getOtherSubstanceName() + "\"\n";
        if(projectInfo->getOtherSubstanceStat())
            stream << "otherSubStat=\"" + *projectInfo->getOtherSubstanceStat() + "\"\n";
        if(projectInfo->getUpstreamDeviceDistance())
            stream << "upstrDevDis=\"" + projectInfo->getUpstreamDeviceDistance()->useStringFormat() + "\"\n";
        if(projectInfo->getDownstreamDistance())
            stream << "downstrDis=\"" + projectInfo->getDownstreamDistance()->useStringFormat() + "\"\n";
        if(!projectInfo->getFlangeMaterialName().isEmpty())
            stream << ("flangeMatName=\"" + projectInfo->getFlangeMaterialName() + "\"\n").toUtf8();
        if(!projectInfo->getPipeMaterialName().isEmpty())
            stream << ("pipeMatName=\"" + projectInfo->getPipeMaterialName() + "\"\n").toUtf8();
        file.close();
        QFile(*projectInfo->getPath()).remove();
        file.rename(*projectInfo->getPath());
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
      }
    projectInfo->setSave(false);
    return true;
}

bool ProjectMenager::moveProject(ProjectInfo *projectInfo, QString newPath){
    if(!App::compareStr(*projectInfo->getPath(), newPath)){
        if(QFile(newPath).exists()){
            MyDialog dlg(QUESTION_DIALOG, QString(ERROR_NEW_PATH_FILE_EXIST_TITLE), QString(ERROR_NEW_PATH_FILE_EXIST_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }else{
            if(!QFile((*projectInfo->getPath())).exists()){
                MyDialog dlg(QUESTION_DIALOG, QString(ERROR_NOT_FOUND_PROJECT_FILE_TITLE), QString(ERROR_NOT_FOUND_PROJECT_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
                return false;
            }else{
                if(QFile(*projectInfo->getPath()).copy(newPath))
                    QFile(*projectInfo->getPath()).remove();
                return true;
            }
        }
    }
}

bool ProjectMenager::checkProject(ProjectInfo *projectInfo){
    return projectInfo->getSave();
 }

QString ProjectMenager::readValue(QFile *file, char *tempCharFunc){
    QString tempStrFunc = QString();
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
                  tempStrFunc.append(byteArr);
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

ProjectMenagerParam ProjectMenager::checkProjectMenagerParamName(QString *tempStr){
    QString namesStr = QString();
    uint i;
    switch(tempStr->length()){
      case 4:
        // Sprawdzanie słowa "projectName"
        namesStr.append("name");

        for(i = 0; i < 4; i++){
            if(tempStr->at(i) != namesStr.at(i))
              break;
          }
        if(i == 4){
            return PROJECT_MENAGER_PROJECT_NAME;
          }
        namesStr.clear();
        // Sprawdzanie słowa "path"
        namesStr.append("path");
        for(i = 0; i < 4; i++)
          if(tempStr->at(i) != namesStr.at(i))
            break;
        if(i == 4){
            return PROJECT_MENAGER_PATH;
          }
        namesStr.clear();
        // Sprawdzanie słowa "flow"
        namesStr.append("flow");
        for(i = 0; i < 4; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 4)
            return PROJECT_MENAGER_FLOW;
        namesStr.clear();
        // Sprawdzanie słowa "temp"
        namesStr.append("temp");
        for(i = 0; i < 4; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 4)
            return PROJECT_MENAGER_TEMP;
        break;
    case 7:
        // Sprawdzanie słowa "created"
        namesStr.append("created");
        for(i = 0; i < 7; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 7)
            return PROJECT_MENAGER_CREATED;
        namesStr.clear();
        // Sprawdzanie słowa "default"
        namesStr.append("default");
        for(i = 0; i < 7; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 7)
            return PROJECT_MENAGER_DEFAULT;
        namesStr.clear();
        // Sprawdzanie słowa "flanDia"
        namesStr.append("flanDia");
        for(i = 0; i < 7; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 7)
            return PROJECT_MENAGER_FLANGE_DIAMETER;
        namesStr.clear();
        // Sprawdzanie słowa "pipeDia"
        namesStr.append("pipeDia");
        for(i = 0; i < 7; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 7)
            return PROJECT_MENAGER_PIPE_DIAMETER;
        break;
    case 8:
        // Sprawdzanie słowa "modified"
        namesStr.append("modified");
        for(i = 0; i < 8; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 8)
            return PROJECT_MENAGER_MODIFIED;
        namesStr.clear();
        // Sprawdzanie słowa "upstrDev"
        namesStr.append("upstrDev");
        for(i = 0; i < 8; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 8)
            return PROJECT_MENAGER_UPSTREAM_DEVICE;
        namesStr.clear();
        // Sprawdzanie słowa "fluidDen"
        namesStr.append("fluidDen");
        for(i = 0; i < 8; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 8)
            return PROJECT_MENAGER_WORK_DENSITY;
        namesStr.clear();
        // Sprawdzanie słowa "presLose"
        namesStr.append("presLose");
        for(i = 0; i < 8; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 8)
            return PROJECT_MENAGER_PRESSURE_LOSS;
        namesStr.clear();
        // Sprawdzanie słowa "densityC"
        namesStr.append("densityC");
        for(i = 0; i < 8; i++)
           if(namesStr.at(i) != tempStr->at(i))
             break;
        if(i == 8)
           return PROJECT_MENAGER_DENSITY_CUSTOM;
        break;
    case 10:
        // Sprawdzanie słowa "objectName"
        namesStr.append("objectName");
        for(i = 0; i < 10; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 10)
            return PROJECT_MENAGER_OBJECT_NAME;
        namesStr.clear();
        // Sprawdzanie słowa "flangeName"
        namesStr.append("flangeName");
        for(i = 0; i < 10; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 10)
            return PROJECT_MENAGER_FLANGE_NAME;
        namesStr.clear();
        // Sprawdzenie słowa "presTapDia"
        namesStr.append("presTapDia");
        for(i = 0; i < 10; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 10)
            return PROJECT_MENAGER_PRESSURE_TAP_DIAMETER;
        namesStr.clear();
        // Sprawdzanie słowa "chokePoint"
        namesStr.append("chokePoint");
        for(i = 0; i < 10; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 10)
            return PROJECT_MENAGER_CHOKE_POINT;
        namesStr.clear();
        // Sprawdzanie słowa "flowFactor"
        namesStr.append("flowFactor");
        for(i = 0; i < 10; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 10)
            return PROJECT_MENAGER_FLOW_FACTOR;
        namesStr.clear();
        // Sprawdzanie słowa "downstrDis"
        namesStr.append("downstrDis");
        for(i = 0; i < 10; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 10)
            return PROJECT_MENAGER_DOWNSTREAM_DISTANCE;
        break;
    case 15:
        // Sprawdzanie słowa "measurementType"
        namesStr.append("measurementType");
        for(i = 0; i < 15; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 15)
            return PROJECT_MENAGER_MEASUREMENT_TYPE;
        break;
    case 14:
        // Sprawdzanie słowa "measurementDev"
        namesStr.append("measurementDev");
        for(i = 0; i < 14; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 14)
            return PROJECT_MENAGER_MEASUREMENT_DEVICE;
        namesStr.clear();
        // Sprawdzanie słowa "numbOfPresTaps"
        namesStr.append("numbOfPresTaps");
        for(i = 0; i < 14; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 14)
            return PROJECT_MENAGER_NUMBER_OF_PRESSURE_TAPS;
        break;
    case 11:
        // Sprawdzaenie słowa "maxDiffPres"
        namesStr.append("maxDiffPres");
        for(i = 0; i < 11; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 11)
            return PROJECT_MENAGER_MAX_DIFF_PRESSURE;
        namesStr.clear();
        // Sprawdzanie słowa "densityFile"
        namesStr.append("densityFile");
        for(i = 0; i < 11; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 11)
            return PROJECT_MENAGER_DENSITY_FROM_FILE;
        namesStr.clear();
        // Sprawdzaenie słowa "pipeLinExpF"
        namesStr.append("pipeLinExpF");
        for(i = 0; i < 11; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 11)
            return PROJECT_MENAGER_PIPE_LINEAR_EXPANSTION;
        namesStr.clear();
        // Sprawdzanie słowa "upstrDevDis"
        namesStr.append("upstrDevDis");
        for(i = 0; i < 11; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 11)
            return PROJECT_MENAGER_UPSTREAM_DEVICE_DISTANCE;
        namesStr.clear();
        // Sprawdzanie słowa "pipeMatName"
        namesStr.append("pipeMatName");
        for(i = 0; i < 11; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 11)
            return PROJECT_MENAGER_PIPE_MATERIAL_NAME;
        namesStr.clear();
        // Sprawdzanie słowa "pipeLinExpF"
        namesStr.append("pipeLinExpF");
        for(i = 0; i < 11; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 11)
            return PROJECT_MENAGER_PIPE_LINEAR_EXPANSTION;
        namesStr.clear();
        // Srawdzanie słowa "companyName"
        namesStr.append("companyName");
        for(i = 0; i < 11; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 11)
            return PROJECT_MENAGER_COMPANY_NAME;
        break;
    case 9:
        // Sprawdzanie słowa "fluidType"
        namesStr.append("fluidType");
        for(i = 0; i < 9; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 9)
            return PROJECT_MENAGER_FLUID_TYPE;
        namesStr.clear();
        // Sprawdzanie słowa "fluidName"
        namesStr.append("fluidName");
        for(i = 0; i < 9; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 9)
            return PROJECT_MENAGER_FLUID_NAME;
        namesStr.clear();
        // Sprawdzanie słowa "fluidStat"
        namesStr.append("fluidStat");
        for(i = 0; i < 9; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 9)
            return PROJECT_MENAGER_FLUID_STAT;
        namesStr.clear();
        // Sprawdzanie słowa "expanNumb"
        namesStr.append("expanNumb");
        for(i = 0; i < 9; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 9)
            return PROJECT_MENAGER_EXPANSTION_NUMBER;
        namesStr.clear();
        // Sprawdzanie słowa "flangeDia"
        namesStr.append("flangeDia");
        for(i = 0; i < 9; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 9)
            return PROJECT_MENAGER_FLANGE_DIAMETER;
        namesStr.clear();
        // Sprawdzanie słowa "viscoFile"
        namesStr.append("viscoFile");
        for(i = 0; i < 9; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 9)
            return PROJECT_MENAGER_VISCO_FROM_FILE;
        break;
    case 12:
        // Sprawdzanie słowa "pipeMaterial"
        namesStr.append("pipeMaterial");
        for(i = 0; i < 12; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 12)
            return PROJECT_MENAGER_PIPE_MATERIAL;
        namesStr.clear();
        // Sprawdzanie słowa "maxUpstrPres"
        namesStr.append("maxUpstrPres");
        for(i = 0; i < 12; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 12)
            return PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE;
        namesStr.clear();
        // Sprawdzanie słowa "maxFlowValue"
        namesStr.append("maxFlowValue");
        for(i = 0; i < 12; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 12)
            return PROJECT_MENAGER_MAX_FLOW_VALUE;
        namesStr.clear();
        // Sprawdzanie słowa "doubSideFlow"
        namesStr.append("doubSideFlow");
        for(i = 0; i < 12; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 12)
            return PROJECT_MENAGER_DOUBLE_SIDE_FLOW;
        namesStr.clear();
        // Sprawdzanie słowa "maxDiffPresU"
        namesStr.append("maxDiffPresU");
        for(i = 0; i < 12; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 12)
            return PROJECT_MENAGER_MAX_DIFF_PRESSURE_UNITS;
        namesStr.clear();
        // Sprawdzanie słowa "otherSubName"
        namesStr.append("otherSubName");
        for(i = 0; i < 12; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 12)
            return PROJECT_MENAGER_OTHER_SUBSTANCE_NAME;
        namesStr.clear();
        // Sprawdzanie słowa "otherSubName"
        namesStr.append("otherSubStat");
        for(i = 0; i < 12; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 12)
            return PROJECT_MENAGER_OTHER_SUBSTANCE_STAT;
        break;
    case 6:
        // Sprawdzanie słowa "result"
        namesStr.append("result");
        for(i = 0; i < 6; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 6)
            return PROJECT_MENAGER_RESULTS;
        namesStr.clear();
        // Sprawdzanie słowa "reyNum"
        namesStr.append("reyNum");
        for(i = 0; i < 6; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 6)
            return PROJECT_MENAGER_REYNOLDS_NUMBER;
        namesStr.clear();
        // Sprawdzanie słowa "viscoC"
        namesStr.append("viscoC");
        for(i = 0; i < 6; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 6)
            return PROJECT_MENAGER_VISCO_CUSTOM;
        break;
    case 13:
        // Sprawdzanie słowa "maxUpstrPresU"
        namesStr.append("maxUpstrPresU");
        for(i = 0; i < 13; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 13)
            return PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE_UNITS;
        namesStr.clear();
        // Sprawdzanie słowa "fluidDynVisco"
        namesStr.append("fluidDynVisco");
        for(i = 0; i < 13; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 13)
            return PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY;
        namesStr.clear();
        // Sprawdzanie słowa "flangeLinExpF"
        namesStr.append("flangeLinExpF");
        for(i = 0; i < 13; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 13)
            return PROJECT_MENAGER_FLANGE_LINEAR_EXPANSTION;
        namesStr.clear();
        // Sprawdzanie słowa "compresFactor"
        namesStr.append("compresFactor");
        for(i = 0; i < 13; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 13)
            return PROJECT_MENAGER_COMPRESIBILITY_FACTOR;
        namesStr.clear();
        // Sprawdzanie słowa "flangeMatName"
        namesStr.append("flangeMatName");
        for(i = 0; i < 13; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 13)
            return PROJECT_MENAGER_FLANGE_MATERIAL_NAME;
        namesStr.clear();
        // Sprawdzanie słowa "flangeLinExpF"
        namesStr.append("flangeLinExpF");
        for(i = 0; i < 13; i++)
            if(tempStr->at(i) != namesStr.at(i))
                break;
        if(i == 13)
            return PROJECT_MENAGER_FLANGE_LINEAR_EXPANSTION;
        break;
    case 5:
        // Sprawdzanie słowa "flowU"
        namesStr.append("flowU");
        for(i = 0; i < 5; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 5)
            return PROJECT_MENAGER_FLOW_UNITS;
        namesStr.clear();
        // Sprawdzanie słowa "tempU"
        namesStr.append("tempU");
        for(i = 0; i < 5; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 5)
            return PROJECT_MENAGER_TEMP_UNITS;
        break;
    case 16:
        // Sprawdzanie słowa "fluidHeatCapaRat"
        namesStr.append("fluidHeatCapaRat");
        for(i = 0; i < 16; i++)
            if(namesStr.at(i) != tempStr->at(i))
                break;
        if(i == 16)
            return PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO;
        break;
    }
    return NUMB_OF_PROJECT_MENAGER_PARAM;
}

QString ProjectMenager::readForConfigFile(QTextStream *str, QFile *proFile, bool* rewitePath){
    QString retStr;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    if(proFile->open(QIODevice::ReadOnly | QIODevice::Text)){
        QString tempStr =  QString();
        char tempChar = '\0';
        bool stop = false;
        QString projectName;
        QString path;
        QString created;
        QString modified;
        QString companyName;
        QString flangeName;
        QString objectName;
        *rewitePath = true;
        ProjectFileStat projectFileStat = READING_PARAMS;
        ProjectMenagerParam projectMenagerParam = PROJECT_MENAGER_COMPANY_NAME;
        while(!stop && !proFile->atEnd()){
            proFile->getChar(&tempChar);
            if(tempChar == '\n')
                continue;
            switch(projectFileStat){
            case READING_PARAMS:
                if(tempChar == '='){
                if((projectMenagerParam = checkProjectMenagerParamName(&tempStr)) == NUMB_OF_PROJECT_MENAGER_PARAM){


                }else {
                   projectFileStat = READING_VALUE;
                   tempStr.clear();
                }
                }else{
                    tempStr.append(tempChar);
                }
                break;
            case READING_VALUE:
                tempStr = QString(readValue(proFile, &tempChar));
                switch(projectMenagerParam){
                case PROJECT_MENAGER_COMPANY_NAME:
                    if(nameCheck(tempStr)){
                        companyName = tempStr;
                    }
                    break;
                case PROJECT_MENAGER_DEFAULT:                    
                    break;
                case PROJECT_MENAGER_FLANGE_NAME:
                    if(nameCheck(tempStr))
                        flangeName = tempStr;
                    break;
                case PROJECT_MENAGER_MEASUREMENT_TYPE:
                    stop = true;
                    break;
               case PROJECT_MENAGER_OBJECT_NAME:
                    if(nameCheck(tempStr))
                        objectName = tempStr;
                    break;
                case PROJECT_MENAGER_PATH:
                    if(App::compareStr(tempStr, QDir::toNativeSeparators(proFile->fileName())))
                        *rewitePath = false;
                    break;
               case PROJECT_MENAGER_PROJECT_NAME:
                    break;
               case PROJECT_MENAGER_CREATED:
                    created = tempStr;
                    break;
                case PROJECT_MENAGER_MODIFIED:
                    modified = tempStr;
                    break;
                case PROJECT_MENAGER_RESULTS:
                    break;
                default:
                    break;
                }
                tempStr.clear();
                projectFileStat = READING_PARAMS;
           }
        }
        QString name = QDir::toNativeSeparators(proFile->fileName());
        name.chop(QString(PROJECT_FILE_EXTENSTION).length());
        int i = name.length() - 1;
        for( ; i > -1; i--){
            if(name.at(i) == '\\'){
                name = name.right(name.length() - i - 1);
                break;
            }
        }
        projectName = name;
        path = QDir::toNativeSeparators(proFile->fileName());
        if(nameCheck(projectName) && !created.isEmpty() && !modified.isEmpty()){
            retStr = ("name=\"" + projectName + "\"\n").toUtf8() + "created=\"" + created + "\"\n" + "modified=\"" + modified + "\"\n" + ("path=\"" + path + "\"\n").toUtf8();
            if(!companyName.isEmpty())
                retStr.append(("companyName=\"" + companyName + "\"\n").toUtf8());
            if(!objectName.isEmpty())
                retStr.append(("fullscalePlantName=\"" + objectName + "\"\n").toUtf8());
            if(!flangeName.isEmpty())
                retStr.append(("flangeName=\"" + flangeName + "\"\n").toUtf8());
            retStr.append("default=1\n");
        }else{
            return QString();
        }
        proFile->close();
        delete proFile;
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString(ERROR_CANT_OPEN_CONFIG_FILE_TITLE), QString(ERROR_CANT_OPEN_CONFIG_FILE_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        delete proFile;
        return QString();
      }
    return retStr;
}

bool ProjectMenager::rewriteForConfigFile(QFile *proFile){
    QFile tempFile("tempPro.tmp");
    if(tempFile.exists())
        tempFile.remove();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextStream str(&tempFile);
    str.setCodec("UTF-8");
    if(proFile->open(QIODevice::ReadOnly | QIODevice::Text)){
        if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString tempStr =  QString();
            char tempChar = '\0';
            ProjectFileStat projectFileStat = READING_PARAMS;
            ProjectMenagerParam projectMenagerParam = PROJECT_MENAGER_COMPANY_NAME;
            while(!proFile->atEnd()){
                proFile->getChar(&tempChar);
                if(tempChar == '\n')
                    continue;
                switch(projectFileStat){
                case READING_PARAMS:
                    if(tempChar == '='){
                        if((projectMenagerParam = checkProjectMenagerParamName(&tempStr)) == NUMB_OF_PROJECT_MENAGER_PARAM){
                            return false;
                        }else {
                            projectFileStat = READING_VALUE;
                            tempStr.clear();
                        }
                    }else{
                        tempStr.append(tempChar);
                    }
                    break;
                case READING_VALUE:
                    tempStr = QString(readValue(proFile, &tempChar));
                    switch(projectMenagerParam){
                    case PROJECT_MENAGER_COMPANY_NAME:
                        str << ("companyName=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case PROJECT_MENAGER_DEFAULT:
                        str << "default=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_DOUBLE_SIDE_FLOW:
                    {
                        str << "doubSideFlow=" + tempStr + "\n";
                    }
                        break;
                    case PROJECT_MENAGER_FLANGE_NAME:
                        str << ("flangeName=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case PROJECT_MENAGER_FLOW:
                        str << "flow=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_FLUID_TYPE:
                        str << "fluidType=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_MAX_DIFF_PRESSURE:
                        str << "maxDiffPres=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_MAX_FLOW_VALUE:
                        str << "maxFlowValue=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE:
                        str << "maxUpstrPres=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_MEASUREMENT_DEVICE:
                        str << "measurementDev=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_MEASUREMENT_TYPE:
                        str << "measurementType=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_OBJECT_NAME:
                        str << ("objectName=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case PROJECT_MENAGER_PATH:
                        str << ("path=\"" + QDir::toNativeSeparators(proFile->fileName()) + "\"\n").toUtf8();
                        break;
                    case PROJECT_MENAGER_PIPE_DIAMETER:
                        str << "pipeDia=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_PIPE_MATERIAL:
                        str << "pipeMaterial=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_PRESSURE_TAP_DIAMETER:
                        str << "presTapDia=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_PROJECT_NAME:
                    {
                        QString name = QDir::toNativeSeparators(proFile->fileName());
                        name.chop(QString(PROJECT_FILE_EXTENSTION).length());
                        int i = name.length() - 1;
                         for( ; i > -1; i--){
                            if(name.at(i) == '\\'){
                                name = name.right(name.length() - i - 1);
                                break;
                            }
                        }
                        if(i > -1)
                            str << ("name=\"" + name + "\"\n").toUtf8();
                    }
                        break;
                    case PROJECT_MENAGER_TEMP:
                        str << "temp=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_UPSTREAM_DEVICE:
                        str << "upstrDev=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_CREATED:
                        str << "created=\"" + tempStr + "\"\n";
                        break;
                    case PROJECT_MENAGER_MODIFIED:
                        str << "modified=\"" + tempStr + "\"\n";
                        break;
                    case PROJECT_MENAGER_MAX_DIFF_PRESSURE_UNITS:
                        str << "maxDiffPresU=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE_UNITS:
                        str << "maxUpstrPresU=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_FLOW_UNITS:
                        str << "flowU=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_TEMP_UNITS:
                        str << "tempU=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_RESULTS:
                        str << "result=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_FLUID_NAME:
                        str << ("fluidName=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case PROJECT_MENAGER_FLUID_STAT:
                        str << "fluidStat=\"" + tempStr + "\"\n";
                        break;
                    case PROJECT_MENAGER_WORK_DENSITY:
                        str << "fluidDen=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY:
                        str << "fluidDynVisco=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO:
                        str << "fluidHeatCapaRat=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_FLANGE_DIAMETER:
                        str << "flangeDia=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_CHOKE_POINT:
                        str << "chokePoint=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_FLOW_FACTOR:
                        str << "flowFactor=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_EXPANSTION_NUMBER:
                        str << "expanNumb=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_REYNOLDS_NUMBER:
                        str << "reyNum=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_PRESSURE_LOSS:
                        str << "presLose=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_DENSITY_CUSTOM:
                        str << "densityC=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_VISCO_CUSTOM:
                        str << "viscoC=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_DENSITY_FROM_FILE:
                        str << "densityFile=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_VISCO_FROM_FILE:
                        str << "viscoFile=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_FLANGE_LINEAR_EXPANSTION:
                        str << "flangeLinExpF=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_PIPE_LINEAR_EXPANSTION:
                        str << "pipeLinExpF=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_COMPRESIBILITY_FACTOR:
                        str << "compresFactor=" + tempStr + "\n";
                        break;
                    case PROJECT_MENAGER_OTHER_SUBSTANCE_NAME:
                        str << ("otherSubName=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case PROJECT_MENAGER_OTHER_SUBSTANCE_STAT:
                        str << ("otherSubStat=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case PROJECT_MENAGER_UPSTREAM_DEVICE_DISTANCE:
                        str << "upstrDevDis=\"" + tempStr + "\"\n";
                        break;
                    case PROJECT_MENAGER_DOWNSTREAM_DISTANCE:
                        str << "downstrDis=\"" + tempStr + "\"\n";
                        break;
                    case PROJECT_MENAGER_FLANGE_MATERIAL_NAME:
                        str << ("flangeMatName=\"" + tempStr + "\"\n").toUtf8();
                        break;
                    case PROJECT_MENAGER_PIPE_MATERIAL_NAME:
                        str << ("pipeMatName=\"" + tempStr +"\"\n").toUtf8();
                        break;
                    default:
                        break;
                    }
                    tempStr.clear();
                    projectFileStat = READING_PARAMS;
                }
            }
            proFile->close();
            tempFile.close();
            QString path = QDir::toNativeSeparators(proFile->fileName());
            proFile->remove();
            tempFile.rename(path);
            delete proFile;
        }else{
            delete proFile;
            return false;
        }
    }else{
        delete proFile;
        return false;
      }
    return true;
}

bool ProjectMenager::nameCheck(QString &str){
    int len = str.length();
    QChar tempCh;
    for(int i = 0; i < len; i++){
        tempCh = str.at(i);
        if(tempCh == '\r' || tempCh == '\n' || tempCh == '\t' || tempCh == '\\'
                || tempCh == '/' || tempCh == ':' || tempCh == '*' || tempCh == '<'
                || tempCh == '>' || tempCh == '\"' || tempCh == '?' || tempCh == '|')
            return  false;
        }
    QString newStr = QString();
    if(len != 0 && str.at(0) != ' ')
        newStr.append(str.at(0));
    for(uint i = 1; i < len; i++)
        if(str.at(i).toLatin1() == str.at(i - 1).toLatin1() && str.at(i).toLatin1() == ' ')
            ;
        else
            newStr.append(str.at(i));
    str = newStr;
    if(str.isEmpty())
        return false;
    return true;
}
