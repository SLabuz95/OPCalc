#include"projectinfo.hpp"
#include"projectmenager.hpp"
#include<QDateTime>
#include"appclass.hpp"
#include"appwindow.hpp"
#include"innerwindowcp.hpp"
#include"config.hpp"
#include"optionalprojectdatapanel.hpp"
#include<QLineEdit>
#include<QCheckBox>
#include"formatabledatafield.hpp"
#include"formatabledata.hpp"

ProjectInfo::ProjectInfo(){
  // Brak
}

ProjectInfo::ProjectInfo(App *setParent, uint setPosition){
    parent = setParent;
    position = setPosition;
    measurementType = new uint;
    *measurementType = 0;
    measurementDevice = new uint;
    *measurementDevice = 0;
    pipeMaterial = new uint;
    *pipeMaterial = 0;
    upstreamDevice = new uint;
    *upstreamDevice = 0;
    fluidType = new uint;
    *fluidType = 0;
    fluidDensityCustom = new bool;
    *fluidDensityCustom = false;
    fluidDynamicViscosityCustom = new bool;
    *fluidDynamicViscosityCustom = false;
    maxDiffPressureUnits = new ushort;
    *maxDiffPressureUnits = 0;
    maxUpstreamPressureUnits = new ushort;
    *maxUpstreamPressureUnits = 0;
    flowUnits = new ushort;
    *flowUnits = 0;
    tempUnits = new ushort;
    *tempUnits = 0;
}

ProjectInfo::~ProjectInfo(){
    delete path;
    path = nullptr;
    delete projectNumb;
    projectNumb = nullptr;
    delete projectName;
    projectName = nullptr;
    delete companyName;
    companyName = nullptr;
    delete objectName;
    objectName = nullptr;
    delete flangeName;
    flangeName = nullptr;
    delete measurementType;
    measurementType = nullptr;
    delete measurementDevice;
    measurementDevice = nullptr;
    delete pressureTapDiameter;
    pressureTapDiameter = nullptr;
    delete maxDiffPressure;
    maxDiffPressure = nullptr;
    delete pipeMaterial;
    pipeMaterial = nullptr;
    delete upstreamDevice;
    upstreamDevice = nullptr;
    delete pipeDiameter;
    pipeDiameter = nullptr;
    delete maxUpstreamPressure;
    maxUpstreamPressure = nullptr;
    delete fluidType;
    fluidType = nullptr;
    delete maxFlowValue;
    maxFlowValue = nullptr;
    delete flow;
    flow = nullptr;
    delete temp;
    temp = nullptr;
    delete doubleSideFlow;
    doubleSideFlow = nullptr;
    delete innerWinStatusCEP;
    innerWinStatusCEP = nullptr;
    delete fluidName;
    fluidName = nullptr;
    delete fluidDensity;
    fluidDensity = nullptr;
    delete fluidDynamicViscosity;
    fluidDynamicViscosity = nullptr;
    delete fluidHeatCapacityRatio;
    fluidHeatCapacityRatio = nullptr;
    delete substanceStat;
    substanceStat = nullptr;
    delete flangeDiameter;
    flangeDiameter = nullptr;
    delete chokePoint;
    chokePoint = nullptr;
    delete flowFactor;
    flowFactor = nullptr;
    delete expanstionNumber;
    expanstionNumber = nullptr;
    delete reynoldsNumber;
    reynoldsNumber = nullptr;
    delete pressureLoss;
    pressureLoss = nullptr;
    delete flangeLinearExpanstionFactor;
    flangeLinearExpanstionFactor = nullptr;
    delete upstreamDeviceDistance;
    upstreamDeviceDistance = nullptr;
    delete pipeLinearExpanstionFactor;
    pipeLinearExpanstionFactor = nullptr;
    delete fluidDensityFromFile;
    fluidDensityFromFile = nullptr;
    delete fluidDensityCustom;
    fluidDensityCustom = nullptr;
    delete fluidDynamicViscosityCustom;
    fluidDynamicViscosityCustom = nullptr;
    delete fluidDynamicViscosityFromFile;
    fluidDynamicViscosityFromFile = nullptr;
    delete substanceDensityPath;
    substanceDensityPath = nullptr;
    delete substanceViscoPath;
    substanceViscoPath = nullptr;
    delete compresibilityFactor;
    compresibilityFactor = nullptr;
    delete otherSubstanceName;
    otherSubstanceName = nullptr;
    delete otherSubstanceStat;
    otherSubstanceStat = nullptr;
    delete downstreamDistance;
    downstreamDistance = nullptr;
    delete maxDiffPressureUnits;
    maxDiffPressureUnits = nullptr;
    delete maxUpstreamPressureUnits;
    maxUpstreamPressureUnits = nullptr;
    delete flowUnits;
    flowUnits = nullptr;
    delete tempUnits;
    tempUnits = nullptr;    
}

App* ProjectInfo::getParent(){
    return parent;
}

uint ProjectInfo::getPosition(){
    return position;
}

bool ProjectInfo::getSave(){
    return save;
}

void ProjectInfo::setSave(bool set){
    save = set;
}

InnerWinStatusCEP* ProjectInfo::getInnerWinStatusCEP(){
    return innerWinStatusCEP;
}

QString* ProjectInfo::getPath(){
    return path;
}

bool ProjectInfo::isDefaultProj(){
    return defaultProj;
}

uint* ProjectInfo::getProjectNumb(){
    return projectNumb;
}

QString* ProjectInfo::getProjectName(){
    return projectName;
}

QString* ProjectInfo::getCompanyName(){
    return companyName;
}

QString* ProjectInfo::getObjectName(){
    return objectName;
}

QString* ProjectInfo::getFlangeName(){
    return flangeName;
}

bool ProjectInfo::isResults(){
    return results;
}

uint* ProjectInfo::getMeasurementType(){
    return measurementType;
}

uint* ProjectInfo::getMeasurementDevice(){
    return measurementDevice;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getPressureTapDiameter(){
  return pressureTapDiameter;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getMaxDiffPressure(){
  return maxDiffPressure;
}

ushort* ProjectInfo::getMaxDiffPressureUnits(){
    return maxDiffPressureUnits;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFlangeLinearExpanstionFactor(){
    return flangeLinearExpanstionFactor;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getPipeDiameter(){
    return pipeDiameter;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getMaxUpstreamPressure(){
    return maxUpstreamPressure;
}

ushort* ProjectInfo::getMaxUpstreamPressureUnits(){
    return maxUpstreamPressureUnits;
}

uint* ProjectInfo::getPipeMaterial(){
    return pipeMaterial;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getPipeLinearExpanstionFactor(){
    return pipeLinearExpanstionFactor;
}

uint* ProjectInfo::getUpstreamDevice(){
    return upstreamDevice;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getUpstreamDeviceDistance(){
    return upstreamDeviceDistance;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getDownstreamDistance(){
    return downstreamDistance;
}

uint* ProjectInfo::getFluidType(){
    return fluidType;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getMaxFlowValue(){
    return maxFlowValue;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFlow(){
    return flow;
}

ushort* ProjectInfo::getFlowUnits(){
    return flowUnits;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getTemp(){
    return temp;
}

ushort* ProjectInfo::getTempUnits(){
    return tempUnits;
}

bool* ProjectInfo::getDoubleSideFlow(){
    return doubleSideFlow;
}

QString ProjectInfo::getCreated(){
    return created;
}

QString ProjectInfo::getModified(){
    return modified;
}

QString* ProjectInfo::getFluidName(){
    return fluidName;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFluidDensity(){
    return fluidDensity;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFluidDensityFromFile(){
    return fluidDensityFromFile;
}

bool* ProjectInfo::getFluidDensityCustom(){
    return fluidDensityCustom;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFluidDynamicViscosity(){
    return fluidDynamicViscosity;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFluidDynamicViscosityFromFile(){
    return fluidDynamicViscosityFromFile;
}

bool* ProjectInfo::getFluidDynamicViscosityCustom(){
    return fluidDynamicViscosityCustom;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFluidHeatCapacityRatio(){
    return fluidHeatCapacityRatio;
}

QString* ProjectInfo::getSubstanceStat(){
    return substanceStat;
}

QString* ProjectInfo::getSubstanceDensityPath(){
    return substanceDensityPath;
}

QString* ProjectInfo::getSubstanceViscoPath(){
    return substanceViscoPath;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getCompresibilityFactor(){
    return compresibilityFactor;
}

QString* ProjectInfo::getOtherSubstanceName(){
    return otherSubstanceName;
}

QString* ProjectInfo::getOtherSubstanceStat(){
    return otherSubstanceStat;
}

void ProjectInfo::setParent(App *set){
    parent = set;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFlangeDiameter(){
    return flangeDiameter;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getChokePoint(){
    return chokePoint;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getFlowFactor(){
    return flowFactor;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getExpanstionNumber(){
    return expanstionNumber;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getReynoldsNumber(){
    return reynoldsNumber;
}

FormatableData<ProjectMenagerParam>* ProjectInfo::getPressureLoss(){
    return pressureLoss;
}

void ProjectInfo::setPosition(uint set){
    position = set;
}

void ProjectInfo::setInnerWinStatusCEP(InnerWinStatusCEP* set){
    if(innerWinStatusCEP != nullptr)
        delete innerWinStatusCEP;
    innerWinStatusCEP = set;
}

void ProjectInfo::removeInnerWinStatusCEP(){
    delete innerWinStatusCEP;
    innerWinStatusCEP = nullptr;
}

void ProjectInfo::setPath(QString* set){
    if(path != nullptr)
        delete path;
    path = set;
}

void ProjectInfo::setProjectNumb(uint set){
    if(projectNumb == nullptr)
        projectNumb = new uint;
    *projectNumb = set;
}

void ProjectInfo::setDefaultProj(bool set){
    defaultProj = set;
}

void ProjectInfo::setProjectName(QString set){
    if(projectName == nullptr)
        projectName = new QString();
    if(!projectName->isEmpty())
        projectName->clear();
    projectName->append(set);
}

void ProjectInfo::setCompanyName(QString* set){
    if(companyName != nullptr)
        delete companyName;
    companyName = set;
}

void ProjectInfo::setObjectName(QString* set){
    if(objectName != nullptr)
        delete objectName;
    objectName = set;
}

void ProjectInfo::setFlangeName(QString* set){
    if(flangeName != nullptr)
        delete flangeName;
    flangeName = set;
}

void ProjectInfo::setResults(bool set){
    results = set;
}

void ProjectInfo::setMeasurementType(uint* set){
    if((measurementType && !set) || (!measurementType && set) ||(set && measurementType && *set != *measurementType))
        save = true;
    if(measurementType != nullptr)
        delete measurementType;
    measurementType = set;
}

void ProjectInfo::setMeasurementDevice(uint* set){
    if((measurementDevice && !set) || (!measurementDevice && set) || (set && measurementDevice && *set != *measurementDevice))
        save = true;
    if(measurementDevice != nullptr)
        delete measurementDevice;
    measurementDevice = set;
}

void ProjectInfo::setPipeMaterialName(QString str){
    pipeMaterialName = str;
}

void ProjectInfo::setFlangeMaterialName(QString str){
    flangeMaterialName = str;
}

QString ProjectInfo::getFlangeMaterialName(){
    return flangeMaterialName;
}

QString ProjectInfo::getPipeMaterialName(){
    return pipeMaterialName;
}

void ProjectInfo::setPressureTapDiameter(FormatableData<ProjectMenagerParam>* set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(pressureTapDiameter);
    if((pressureTapDiameter && !set) || (!pressureTapDiameter && set) || (set && pressureTapDiameter && !App::compareStr(pressureTapDiameter->useStringFormat(), set->useStringFormat())))
         save = true;
    if(pressureTapDiameter != nullptr)
        delete pressureTapDiameter;
    pressureTapDiameter = set;
}

void ProjectInfo::setMaxDiffPressure(FormatableData<ProjectMenagerParam>* set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(maxDiffPressure);
    if((maxDiffPressure && !set) || (!maxDiffPressure && set) || (set && maxDiffPressure && !App::compareStr(maxDiffPressure->useStringFormat(), set->useStringFormat()))){
        save = true;
        setFluidDensityFromFile(nullptr);
        setFluidDynamicViscosityFromFile(nullptr);
    }
    if(maxDiffPressure != nullptr)
        delete maxDiffPressure;
    maxDiffPressure = set;
}

void ProjectInfo::setMaxDiffPressureUnits(ushort *set){
    if((maxDiffPressureUnits && !set) || (!maxDiffPressureUnits && set) || (set && maxDiffPressureUnits && *set != *maxDiffPressureUnits)){
        save = true;
        setFluidDensityFromFile(nullptr);
        setFluidDynamicViscosityFromFile(nullptr);
    }
    if(maxDiffPressureUnits)
        delete maxDiffPressureUnits;
    maxDiffPressureUnits = set;
}

void ProjectInfo::setFlangeLinearExpanstionFactor(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(flangeLinearExpanstionFactor);
    if((flangeLinearExpanstionFactor && !set) || (!flangeLinearExpanstionFactor && set) || (set && flangeLinearExpanstionFactor && !App::compareStr(flangeLinearExpanstionFactor->useStringFormat(), set->useStringFormat())))
         save = true;
    if(flangeLinearExpanstionFactor != nullptr)
        delete flangeLinearExpanstionFactor;
    flangeLinearExpanstionFactor = set;
}

void ProjectInfo::setPipeMaterial(uint* set){
    if((pipeMaterial && !set) || (!pipeMaterial && set) || (set && pipeMaterial && *set != *pipeMaterial))
        save = true;
    if(pipeMaterial != nullptr)
        delete pipeMaterial;
    pipeMaterial = set;
}

void ProjectInfo::setPipeLinearExpanstionFactor(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(pipeLinearExpanstionFactor);
    if((pipeLinearExpanstionFactor && !set) || (!pipeLinearExpanstionFactor && set) || (set && pipeLinearExpanstionFactor && !App::compareStr(pipeLinearExpanstionFactor->useStringFormat(), set->useStringFormat())))
         save = true;
    if(pipeLinearExpanstionFactor != nullptr)
        delete pipeLinearExpanstionFactor;
    pipeLinearExpanstionFactor = set;
}

void ProjectInfo::setUpstreamDevice(uint* set){
    if((upstreamDevice && !set) || (!upstreamDevice && set) || (set && upstreamDevice && *set != *upstreamDevice))
        save = true;
    if(upstreamDevice != nullptr)
        delete upstreamDevice;
    upstreamDevice = set;
}

void ProjectInfo::setUpstreamDeviceDistance(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(upstreamDeviceDistance);
    if((upstreamDeviceDistance && !set) || (!upstreamDeviceDistance && set) || (set && upstreamDeviceDistance && !App::compareStr(upstreamDeviceDistance->useStringFormat(), set->useStringFormat())))
         save = true;
    if(upstreamDeviceDistance != nullptr)
        delete upstreamDeviceDistance;
    upstreamDeviceDistance = set;
}

void ProjectInfo::setDownstreamDistance(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(downstreamDistance);
    if((downstreamDistance && !set) || (!downstreamDistance && set) || (set && downstreamDistance && !App::compareStr(downstreamDistance->useStringFormat(), set->useStringFormat())))
         save = true;
    if(downstreamDistance != nullptr)
        delete downstreamDistance;
    downstreamDistance = set;
}

void ProjectInfo::setPipeDiameter(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(pipeDiameter);
    if((pipeDiameter && !set) || (!pipeDiameter && set) || (set && pipeDiameter && !App::compareStr(pipeDiameter->useStringFormat(), set->useStringFormat())))
         save = true;
    if(pipeDiameter != nullptr)
        delete pipeDiameter;
    pipeDiameter = set;
}

void ProjectInfo::setMaxUpstreamPressure(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(maxUpstreamPressure);
    if((maxUpstreamPressure && !set) || (!maxUpstreamPressure && set) || (set && maxUpstreamPressure && !App::compareStr(maxUpstreamPressure->useStringFormat(), set->useStringFormat()))){
        save = true;
        setFluidDensityFromFile(nullptr);
        setFluidDynamicViscosityFromFile(nullptr);
    }
    if(maxUpstreamPressure != nullptr)
        delete maxUpstreamPressure;
    maxUpstreamPressure = set;
}

void ProjectInfo::setMaxUpstreamPressureUnits(ushort *set){
    if((maxDiffPressureUnits && !set) || (!maxUpstreamPressureUnits && set) || (set && maxUpstreamPressureUnits && *set != *maxUpstreamPressureUnits)){
        save = true;
        setFluidDensityFromFile(nullptr);
        setFluidDynamicViscosityFromFile(nullptr);
    }
    if(maxUpstreamPressureUnits)
        delete maxUpstreamPressureUnits;
    maxUpstreamPressureUnits = set;
}

void ProjectInfo::setFluidType(uint *set){
    if((fluidType && !set) || (!fluidType && set) || (set && fluidType && *set != *fluidType)){
        save = true;
        setFluidDensityFromFile(nullptr);
        setFluidDynamicViscosityFromFile(nullptr);
    }
    if(fluidType != nullptr)
        delete fluidType;
    fluidType = set;
}

void ProjectInfo::setMaxFlowValue(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(maxFlowValue);
    if((maxFlowValue && !set) || (!maxFlowValue && set) || (set && maxFlowValue && !App::compareStr(maxFlowValue->useStringFormat(), set->useStringFormat())))
         save = true;
    if(maxFlowValue != nullptr)
        delete maxFlowValue;
    maxFlowValue = set;
}

void ProjectInfo::setFlow(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(flow);
    if((flow && !set) || (!flow && set) || (set && flow && !App::compareStr(flow->useStringFormat(), set->useStringFormat())))
         save = true;
    if(flow != nullptr)
        delete flow;
    flow = set;
}

void ProjectInfo::setFlowUnits(ushort *set){
    if((flowUnits && !set) || (!flowUnits && set) || (set && flowUnits && *set != *flowUnits))
        save = true;
    if(flowUnits)
        delete flowUnits;
    flowUnits = set;
}

void ProjectInfo::setTemp(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(temp);
    if((temp && !set) || (!temp && set) || (set && temp && !App::compareStr(temp->useStringFormat(), set->useStringFormat()))){
        save = true;
        setFluidDensityFromFile(nullptr);
        setFluidDynamicViscosityFromFile(nullptr);
    }
    if(temp != nullptr)
        delete temp;
    temp = set;
}

void ProjectInfo::setTempUnits(ushort *set){
    if((tempUnits && !set) || (!tempUnits && set) || (set && tempUnits && *set != *tempUnits)){
        save = true;
        setFluidDensityFromFile(nullptr);
        setFluidDynamicViscosityFromFile(nullptr);
    }
    if(tempUnits)
        delete tempUnits;
    tempUnits = set;
}

void ProjectInfo::setDoubleSideFlow(bool *set){
    if((doubleSideFlow && !set) || (!doubleSideFlow && set) || (set && doubleSideFlow && *set != *doubleSideFlow))
        save = true;
    if(doubleSideFlow != nullptr)
        delete doubleSideFlow;
    doubleSideFlow = set;
}

void ProjectInfo::setCreated(QString set){
    created = set;
}

void ProjectInfo::setModified(QString set){
    modified = set;
}

void ProjectInfo::setFluidName(QString *set){
    if((fluidName && !set) || (!fluidName && set) || (fluidName && set && !App::compareStr(*fluidName, *set)))
        save = true;
    if(fluidName != nullptr)
        delete fluidName;
    fluidName = set;
}

void ProjectInfo::setFluidDensity(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(fluidDensity);
    if((fluidDensity && !set) || (!fluidDensity && set) || (set && fluidDensity && !App::compareStr(fluidDensity->useStringFormat(), set->useStringFormat())))
         save = true;
    if(fluidDensity != nullptr)
        delete fluidDensity;
    fluidDensity = set;
}

void ProjectInfo::setFluidDensityCustom(bool *set){
    if(fluidDensityCustom != nullptr)
        delete fluidDensityCustom;
    fluidDensityCustom = set;
}

void ProjectInfo::setFluidDensityFromFile(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(fluidDensityFromFile);
    if((fluidDensityFromFile && !set) || (!fluidDensityFromFile && set) || (set && fluidDensityFromFile && !App::compareStr(fluidDensityFromFile->useStringFormat(), set->useStringFormat())))
         save = true;
    if(fluidDensityFromFile != nullptr)
        delete fluidDensityFromFile;
    fluidDensityFromFile = set;
}

void ProjectInfo::setFluidDynamicViscosity(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(fluidDynamicViscosity);
    if((fluidDynamicViscosity && !set) || (!fluidDynamicViscosity && set) || (set && fluidDynamicViscosity && !App::compareStr(fluidDynamicViscosity->useStringFormat(), set->useStringFormat())))
         save = true;
    if(fluidDynamicViscosity != nullptr)
        delete fluidDynamicViscosity;
    fluidDynamicViscosity = set;
}

void ProjectInfo::setFluidDynamicViscosityCustom(bool* set){
    if(fluidDynamicViscosityCustom != nullptr)
        delete fluidDynamicViscosityCustom;
    fluidDynamicViscosityCustom = set;
}

void ProjectInfo::setFluidDynamicViscosityFromFile(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(fluidDynamicViscosityFromFile);
    if((fluidDynamicViscosityFromFile && !set) || (!fluidDynamicViscosityFromFile && set) || (set && fluidDynamicViscosityFromFile && !App::compareStr(fluidDynamicViscosityFromFile->useStringFormat(), set->useStringFormat())))
         save = true;
    if(fluidDynamicViscosityFromFile != nullptr)
        delete fluidDynamicViscosityFromFile;
    fluidDynamicViscosityFromFile = set;
}

void ProjectInfo::setFluidHeatCapacityRatio(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(fluidHeatCapacityRatio);
    if((fluidHeatCapacityRatio && !set) || (!fluidHeatCapacityRatio && set) || (set && fluidHeatCapacityRatio && !App::compareStr(fluidHeatCapacityRatio->useStringFormat(), set->useStringFormat())))
         save = true;
    if(fluidHeatCapacityRatio != nullptr)
        delete fluidHeatCapacityRatio;
    fluidHeatCapacityRatio = set;
}

void ProjectInfo::setSubstanceStat(QString *set)
{
    if((substanceStat && !set) || (!substanceStat && set) || (set && substanceStat && !App::compareStr(*substanceStat, *set)))
        save = true;
    if(substanceStat != nullptr)
        delete substanceStat;
    substanceStat = set;
}

void ProjectInfo::setSubstanceDensityPath(QString *set){
    if(substanceDensityPath != nullptr)
        delete substanceDensityPath;
    substanceDensityPath = set;
}

void ProjectInfo::setSubstanceViscoPath(QString *set){
    if(substanceViscoPath != nullptr)
        delete substanceViscoPath;
    substanceViscoPath = set;
}

void ProjectInfo::setCompresibilityFactor(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(compresibilityFactor);
    if((compresibilityFactor && !set) || (!compresibilityFactor && set) || (set && compresibilityFactor && !App::compareStr(compresibilityFactor->useStringFormat(), set->useStringFormat())))
         save = true;
    if(compresibilityFactor != nullptr)
        delete compresibilityFactor;
    compresibilityFactor = set;
}

void ProjectInfo::setOtherSubstanceName(QString *set){
    if((otherSubstanceName && !set) || (!otherSubstanceName && set) || (set && otherSubstanceName && !App::compareStr(*otherSubstanceName, *set)))
         save = true;
    if(otherSubstanceName != nullptr)
        delete otherSubstanceName;
    otherSubstanceName = set;
}

void ProjectInfo::setOtherSubstanceStat(QString *set){
    if((otherSubstanceStat && !set) || (!otherSubstanceStat && set) || (set && otherSubstanceStat && !App::compareStr(*otherSubstanceStat, *set)))
         save = true;
    if(otherSubstanceStat != nullptr)
        delete otherSubstanceStat;
    otherSubstanceStat = set;
}

void ProjectInfo::setFlangeDiameter(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(flangeDiameter);
    if((flangeDiameter && !set) || (!flangeDiameter && set) || (set && flangeDiameter && !App::compareStr(flangeDiameter->useStringFormat(), set->useStringFormat())))
         save = true;
    if(flangeDiameter != nullptr)
        delete flangeDiameter;
    flangeDiameter = set;
}

void ProjectInfo::setChokePoint(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(chokePoint);
    if((chokePoint && !set) || (!chokePoint && set) || (set && chokePoint && !App::compareStr(chokePoint->useStringFormat(), set->useStringFormat())))
         save = true;
    if(chokePoint != nullptr)
        delete chokePoint;
    chokePoint = set;
}

void ProjectInfo::setFlowFactor(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(flowFactor);
    if((flowFactor && !set) || (!flowFactor && set) || (set && flowFactor && !App::compareStr(flowFactor->useStringFormat(), set->useStringFormat())))
         save = true;
    if(flowFactor != nullptr)
        delete flowFactor;
    flowFactor = set;
}

void ProjectInfo::setExpanstionNumber(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(expanstionNumber);
    if((expanstionNumber && !set) || (!expanstionNumber && set) || (set && expanstionNumber && !App::compareStr(expanstionNumber->useStringFormat(), set->useStringFormat())))
         save = true;
    if(expanstionNumber != nullptr)
        delete expanstionNumber;
    expanstionNumber = set;
}

void ProjectInfo::setReynoldsNumber(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(reynoldsNumber);
    if((reynoldsNumber && !set) || (!reynoldsNumber && set) || (set && reynoldsNumber && !App::compareStr(reynoldsNumber->useStringFormat(), set->useStringFormat())))
         save = true;
    if(reynoldsNumber != nullptr)
        delete reynoldsNumber;
    reynoldsNumber = set;
}

void ProjectInfo::setPressureLoss(FormatableData<ProjectMenagerParam> *set){
    SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(pressureLoss);
    if((pressureLoss && !set) || (!pressureLoss && set) || (set && pressureLoss && !App::compareStr(pressureLoss->useStringFormat(), set->useStringFormat())))
         save = true;
    if(pressureLoss != nullptr)
        delete pressureLoss;
    pressureLoss = set;
}

void ProjectInfo::createNewProject(){
  QString tempStr = QString();
  projectName = new QString(parent->getMainWin()->getInnerWindowCP()->getProjectNameTextEdit()->getDataUTextEdit()->text());
  checkProjectPath(parent->getMainWin()->getInnerWindowCP()->getProjectPathEditDataLabel()->text());
  path = new QString(parent->getMainWin()->getInnerWindowCP()->getProjectPathEditDataLabel()->text());
  created = QDateTime::currentDateTime().toString("dd.MM.yyyy H:mm");
  modified = created;
  defaultProj = parent->getMainWin()->getInnerWindowCP()->getDefaultProjectCB()->isChecked();
  setProjectNumb(1);
  tempStr.append(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getCompanyTextEdit()->getDataUTextEdit()->text());
  if(!tempStr.isEmpty()){
    companyName = new QString(tempStr);
    tempStr.clear();
    }
  tempStr.append(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getObjectTextEdit()->getDataUTextEdit()->text());
  if(!tempStr.isEmpty()){
      objectName = new QString(tempStr);
      tempStr.clear();
    }
  tempStr.append(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getFlangeTextEdit()->getDataUTextEdit()->text());
  if(!tempStr.isEmpty()){
    flangeName = new QString(tempStr);
    tempStr.clear();
  }
  parent->getConfiguration()->addNewProject();
  ProjectMenager::writeProject(this);
}

void ProjectInfo::checkProjectPath(QString path){
    uint len = path.length();
    uint i = 0;
    while(len - i - 1 != 0 && path.at(len - i - 1) != '\\')
        i++;
    path.chop(i + 1);
    len = path.length();
    QString defaultPath = parent->getConfiguration()->getDefaultPath();
    if((uint)defaultPath.length() == len){
           for(i = 0; i < len; i++)
                if(defaultPath.at(i) == path.at(i))
                    break;
            if(i == len)
                defaultProj = true;
            else
                defaultProj = false;
    }else{
        defaultProj = false;
    }
}

void ProjectInfo::readForEditData(){
    parent->getMainWin()->getInnerWindowCP()->setProjectCreatedDateLabel(created);
    parent->getMainWin()->getInnerWindowCP()->setProjectModifiedDateLabel(modified);
    if(!(*parent->getMainWin()->getInnerWindowCP()->getEditableData())){
    parent->getMainWin()->getInnerWindowCP()->setProjectNameEditDataLabel(*projectName);
    parent->getMainWin()->getInnerWindowCP()->setProjectPathEditDataLabel(*path);
    parent->getMainWin()->getInnerWindowCP()->getDefaultProjectLabel()->setText((defaultProj)? QString("Projekt domyślny:\tTak") : QString("Projekt domyślny:\tNie"));
    if(objectName != nullptr)
        parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setObjectEditDataLabel(*objectName);
    else
        parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setObjectEditDataLabel(NO_DATA_INFORMATION);
    if(flangeName != nullptr)
        parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setFlangeEditDataLabel(*flangeName);
    else
        parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setFlangeEditDataLabel(NO_DATA_INFORMATION);
    if(companyName != nullptr)
        parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setCompanyEditDataLabel(*companyName);
    else
        parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setCompanyEditDataLabel(NO_DATA_INFORMATION);
    }else{
        parent->getMainWin()->getInnerWindowCP()->setProjectNameTextEdit(*projectName);
        parent->getMainWin()->getInnerWindowCP()->setProjectPathEditDataLabel(*path);
        parent->getMainWin()->getInnerWindowCP()->getDefaultProjectLabel()->setText((defaultProj)? QString("Projekt domyślny:\tTak") : QString("Projekt domyślny:\tNie"));
        parent->getMainWin()->getInnerWindowCP()->setDefaulProjectCB(defaultProj);
        if(objectName !=  nullptr)
            parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setObjectTextEdit(*objectName);
        else
            parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setObjectTextEdit(QString());
        if(flangeName != nullptr)
            parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setFlangeTextEdit(*flangeName);
        else
            parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setFlangeTextEdit(QString());
        if(companyName != nullptr)
            parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setCompanyTextEdit(*companyName);
        else
            parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->setCompanyTextEdit(QString());

    }
}

void ProjectInfo::rewriteConfigData(){
    if(*parent->getMainWin()->getInnerWindowCP()->getEditableData()){
        setProjectName(parent->getMainWin()->getInnerWindowCP()->getProjectNameTextEdit()->getDataUTextEdit()->text());
        setPath(new QString(parent->getMainWin()->getInnerWindowCP()->getProjectPathEditDataLabel()->text()));
        defaultProj = parent->getMainWin()->getInnerWindowCP()->getDefaultProjectCB()->isChecked();
        setModified(QDateTime::currentDateTime().toString("dd.MM.yyyy H:mm"));
        if(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getCompanyTextEdit()->getDataUTextEdit()->text().isEmpty())
            setCompanyName(nullptr);
        else
            setCompanyName(new QString(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getCompanyTextEdit()->getDataUTextEdit()->text()));
        if(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getObjectTextEdit()->getDataUTextEdit()->text().isEmpty())
            setObjectName(nullptr);
        else
            setObjectName(new QString(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getObjectTextEdit()->getDataUTextEdit()->text()));
        if(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getFlangeTextEdit()->getDataUTextEdit()->text().isEmpty())
            setFlangeName(nullptr);
        else
            setFlangeName(new QString(parent->getMainWin()->getInnerWindowCP()->getOptionalProjectDataPanel()->getFlangeTextEdit()->getDataUTextEdit()->text()));
    }
}

void ProjectInfo::checkFormatableDataValidity(){
    if(pressureTapDiameter && !pressureTapDiameter->getData()){
       delete pressureTapDiameter;
        pressureTapDiameter = nullptr;
    }
    if(maxDiffPressure && !maxDiffPressure->getData()){
        delete maxDiffPressure;
        maxDiffPressure = nullptr;
    }
    if(flangeLinearExpanstionFactor && !flangeLinearExpanstionFactor->getData()){
        delete flangeLinearExpanstionFactor;
        flangeLinearExpanstionFactor = nullptr;
    }
    if(upstreamDeviceDistance && !upstreamDeviceDistance->getData()){
        delete upstreamDeviceDistance;
        upstreamDeviceDistance = nullptr;
    }
    if(downstreamDistance && !downstreamDistance->getData()){
        delete downstreamDistance;
        downstreamDistance = nullptr;
    }
    if(pipeDiameter && !pipeDiameter->getData()){
        delete pipeDiameter;
        pipeDiameter = nullptr;
    }
    if(maxUpstreamPressure && !maxUpstreamPressure->getData()){
        delete maxUpstreamPressure;
        maxUpstreamPressure = nullptr;
    }
    if(pipeLinearExpanstionFactor && !pipeLinearExpanstionFactor->getData()){
        delete pipeLinearExpanstionFactor;
        pipeLinearExpanstionFactor = nullptr;
    }
    if(maxFlowValue && !maxFlowValue->getData()){
        delete maxFlowValue;
        maxFlowValue = nullptr;
    }
    if(flow && !flow->getData()){
        delete flow;
        flow = nullptr;
    }
    if(temp && !temp->getData()){
        delete temp;
        temp = nullptr;
    }
    if(fluidDensity && !fluidDensity->getData()){
        delete fluidDensity;
        fluidDensity = nullptr;
    }
    if(fluidDensityFromFile && !fluidDensityFromFile->getData()){
        delete fluidDensityFromFile;
        fluidDensityFromFile = nullptr;
    }
    if(fluidDynamicViscosity && !fluidDynamicViscosity->getData()){
        delete fluidDynamicViscosity;
        fluidDynamicViscosity = nullptr;
    }
    if(fluidDynamicViscosityFromFile && !fluidDynamicViscosityFromFile->getData()){
        delete fluidDynamicViscosityFromFile;
        fluidDynamicViscosityFromFile = nullptr;
    }
    if(fluidHeatCapacityRatio && !fluidHeatCapacityRatio->getData()){
        delete fluidHeatCapacityRatio;
        fluidHeatCapacityRatio = nullptr;
    }
    if(compresibilityFactor && !compresibilityFactor->getData()){
        delete compresibilityFactor;
        compresibilityFactor = nullptr;
    }
    if(flangeDiameter && !flangeDiameter->getData()){
        delete flangeDiameter;
        flangeDiameter = nullptr;
    }
    if(chokePoint && !chokePoint->getData()){
        delete chokePoint;
        chokePoint = nullptr;
    }
    if(flowFactor && !flowFactor->getData()){
        delete flowFactor;
        flowFactor = nullptr;
    }
    if(expanstionNumber && !expanstionNumber->getData()){
        delete expanstionNumber;
        expanstionNumber = nullptr;
    }
    if(reynoldsNumber && !reynoldsNumber->getData()){
        delete reynoldsNumber;
        reynoldsNumber = nullptr;
    }
    if(pressureLoss && !pressureLoss->getData()){
        delete pressureLoss;
        pressureLoss = nullptr;
    }
    ProjectMenager::writeProject(this);
}
