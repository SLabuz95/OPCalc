#ifndef PROJECTINFO_HPP
#define PROJECTINFO_HPP

#define SET_TYPE_FORMATABLE_DATA_MOVE_FORMAT(fD) if(fD && fD->getFormat() && set && !set->getFormat()) set->setFormat(new QString(*fD->getFormat()));
#include"formatabledata.hpp"
#include<QObject>
#include"innerwinstatuscepenum.hpp"
#include"projectmenagerparamenum.hpp"
class App;
class InnerWindowCEP;
template <typename ParamName> class FormatableData;

class ProjectInfo : public QObject{
public:
    // Konstruktor
    ProjectInfo();
    ProjectInfo(App*, uint);

    // Destruktor
    ~ProjectInfo();

private:
     // Parent
     App *parent = nullptr;

     // Zmienna pamiętająca stan WindowCEP
     InnerWinStatusCEP *innerWinStatusCEP = nullptr;

    // Numer porządkowy otwartego projektu
    uint position;    
    bool save = false;

    // Parametry
    QString created = QString();
    QString modified = QString();
    QString *path = nullptr;
    uint *projectNumb = nullptr;
    bool defaultProj = false;
    QString *projectName = nullptr;
    QString *companyName = nullptr;
    QString *objectName = nullptr;
    QString *flangeName = nullptr;
    bool results = false;


    // [Flange] Project Pack Name
    uint *measurementType = nullptr;
    QString flangeMaterialName;
    uint *measurementDevice = nullptr;
    FormatableData<ProjectMenagerParam> *pressureTapDiameter = nullptr;
    FormatableData<ProjectMenagerParam> *maxDiffPressure =  nullptr;
    ushort *maxDiffPressureUnits = nullptr;
    FormatableData<ProjectMenagerParam> *flangeLinearExpanstionFactor = nullptr;

    // [Pipe] Project Pack Name
    uint *pipeMaterial = nullptr;
    uint *upstreamDevice = nullptr;
    QString pipeMaterialName;
    FormatableData<ProjectMenagerParam> *upstreamDeviceDistance = nullptr;
    FormatableData<ProjectMenagerParam> *downstreamDistance = nullptr;
    FormatableData<ProjectMenagerParam> *pipeDiameter = nullptr;
    FormatableData<ProjectMenagerParam> *maxUpstreamPressure = nullptr;
    ushort *maxUpstreamPressureUnits = nullptr;
    FormatableData<ProjectMenagerParam> *pipeLinearExpanstionFactor = nullptr;

    // [Flow] Project Pack Name
    uint *fluidType = nullptr;
    FormatableData<ProjectMenagerParam> *maxFlowValue = nullptr;
    FormatableData<ProjectMenagerParam> *flow = nullptr;
    ushort *flowUnits = nullptr;
    FormatableData<ProjectMenagerParam> *temp = nullptr;
    ushort *tempUnits = nullptr;
    bool *doubleSideFlow = nullptr;

    // [Fluid] Project Pack Name
    QString *fluidName = nullptr;
    QString *substanceStat = nullptr;
    FormatableData<ProjectMenagerParam> *fluidDensity = nullptr;
    FormatableData<ProjectMenagerParam> *fluidDensityFromFile = nullptr;
    bool *fluidDensityCustom = nullptr;
    FormatableData<ProjectMenagerParam> *fluidDynamicViscosity = nullptr;
    FormatableData<ProjectMenagerParam> *fluidDynamicViscosityFromFile = nullptr;
    bool *fluidDynamicViscosityCustom = nullptr;
    FormatableData<ProjectMenagerParam> *fluidHeatCapacityRatio = nullptr;
    QString *substanceDensityPath = nullptr;
    QString *substanceViscoPath = nullptr;
    FormatableData<ProjectMenagerParam> *compresibilityFactor = nullptr;
    QString *otherSubstanceName = nullptr;
    QString *otherSubstanceStat = nullptr;

    // [Result] Project Pack Name
    FormatableData<ProjectMenagerParam> *flangeDiameter = nullptr;
    FormatableData<ProjectMenagerParam> *chokePoint = nullptr;
    FormatableData<ProjectMenagerParam> *flowFactor = nullptr;
    FormatableData<ProjectMenagerParam> *expanstionNumber = nullptr;
    FormatableData<ProjectMenagerParam> *reynoldsNumber = nullptr;
    FormatableData<ProjectMenagerParam> *pressureLoss = nullptr;

public:
    // Inicjowanie dla wiekszej liczby

    // Funkcje get
    App* getParent();
    uint getPosition();
    InnerWinStatusCEP* getInnerWinStatusCEP();
    bool getSave();
    void setSave(bool);

    // General
    QString getCreated();
    QString getModified();
    QString* getPath();
    uint* getProjectNumb();
    bool isDefaultProj();
    QString* getProjectName();
    QString* getCompanyName();
    QString* getObjectName();
    QString* getFlangeName();
    bool isResults();

    // Flange
    ushort* getFlangeType();
    QString getFlangeMaterialName();
    uint* getMeasurementType();
    uint* getMeasurementDevice();
    FormatableData<ProjectMenagerParam>* getPressureTapDiameter();
    FormatableData<ProjectMenagerParam>* getMaxDiffPressure();
    ushort* getMaxDiffPressureUnits();
    FormatableData<ProjectMenagerParam>* getFlangeLinearExpanstionFactor();

    // Pipe
    uint* getPipeMaterial();
    QString getPipeMaterialName();
    uint* getUpstreamDevice();
    FormatableData<ProjectMenagerParam>* getUpstreamDeviceDistance();
    FormatableData<ProjectMenagerParam>* getDownstreamDistance();
    FormatableData<ProjectMenagerParam>* getPipeDiameter();
    FormatableData<ProjectMenagerParam>* getMaxUpstreamPressure();
    ushort* getMaxUpstreamPressureUnits();
    FormatableData<ProjectMenagerParam>* getPipeLinearExpanstionFactor();

    // Flow
    uint* getFluidType();
    FormatableData<ProjectMenagerParam>* getMaxFlowValue();
    FormatableData<ProjectMenagerParam>* getFlow();
    ushort* getFlowUnits();
    FormatableData<ProjectMenagerParam>* getTemp();
    ushort* getTempUnits();
    bool* getDoubleSideFlow();

    // Fluid
    QString* getFluidName();
    QString* getSubstanceStat();
    FormatableData<ProjectMenagerParam>* getFluidDensity();
    bool* getFluidDensityCustom();
    bool* getFluidDynamicViscosityCustom();
    FormatableData<ProjectMenagerParam>* getFluidDynamicViscosity();
    FormatableData<ProjectMenagerParam>* getFluidHeatCapacityRatio();
    QString* getSubstanceDensityPath();
    QString* getSubstanceViscoPath();
    FormatableData<ProjectMenagerParam>* getFluidDynamicViscosityFromFile();
    FormatableData<ProjectMenagerParam>* getFluidDensityFromFile();
    FormatableData<ProjectMenagerParam>* getCompresibilityFactor();
    QString* getOtherSubstanceName();
    QString* getOtherSubstanceStat();

    // Results
    FormatableData<ProjectMenagerParam>* getFlangeDiameter();
    FormatableData<ProjectMenagerParam>* getChokePoint();
    FormatableData<ProjectMenagerParam>* getFlowFactor();
    FormatableData<ProjectMenagerParam>* getExpanstionNumber();
    FormatableData<ProjectMenagerParam>* getReynoldsNumber();
    FormatableData<ProjectMenagerParam>* getPressureLoss();

    // Funkcje set
    void setParent(App*);
    void setPosition(uint);
    void setInnerWinStatusCEP(InnerWinStatusCEP*);
    void removeInnerWinStatusCEP();

    // General
    void setCreated(QString);
    void setModified(QString);
    void setPath(QString*);
    void setProjectNumb(uint);
    void setDefaultProj(bool);
    void setProjectName(QString);
    void setCompanyName(QString*);
    void setObjectName(QString*);
    void setFlangeName(QString*);
    void setResults(bool);

    // Flange
    void setFlangeType(ushort*);
    void setFlangeMaterialName(QString);
    void setMeasurementType(uint*);
    void setMeasurementDevice(uint*);
    void setPressureTapDiameter(FormatableData<ProjectMenagerParam>*);
    void setMaxDiffPressure(FormatableData<ProjectMenagerParam>*);
    void setMaxDiffPressureUnits(ushort*);
    void setFlangeLinearExpanstionFactor(FormatableData<ProjectMenagerParam>*);

    // Pipe
    void setPipeMaterial(uint*);
    void setPipeMaterialName(QString);
    void setUpstreamDevice(uint*);
    void setUpstreamDeviceDistance(FormatableData<ProjectMenagerParam>*);
    void setDownstreamDistance(FormatableData<ProjectMenagerParam>*);
    void setPipeDiameter(FormatableData<ProjectMenagerParam>*);
    void setMaxUpstreamPressure(FormatableData<ProjectMenagerParam>*);
    void setMaxUpstreamPressureUnits(ushort*);
    void setPipeLinearExpanstionFactor(FormatableData<ProjectMenagerParam>*);

    // Flow
    void setFluidType(uint*);
    void setMaxFlowValue(FormatableData<ProjectMenagerParam>*);
    void setFlow(FormatableData<ProjectMenagerParam>*);
    void setFlowUnits(ushort*);
    void setTemp(FormatableData<ProjectMenagerParam>*);
    void setTempUnits(ushort*);
    void setDoubleSideFlow(bool*);

    // Fluid
    void setFluidName(QString*);
    void setSubstanceStat(QString*);
    void setFluidDensity(FormatableData<ProjectMenagerParam>*);
    void setFluidDynamicViscosity(FormatableData<ProjectMenagerParam>*);
    void setFluidHeatCapacityRatio(FormatableData<ProjectMenagerParam>*);
    void setSubstanceDensityPath(QString*);
    void setSubstanceViscoPath(QString*);
    void setFluidDensityCustom(bool*);
    void setFluidDynamicViscosityCustom(bool*);
    void setFluidDensityFromFile(FormatableData<ProjectMenagerParam>*);
    void setFluidDynamicViscosityFromFile(FormatableData<ProjectMenagerParam>*);
    void setCompresibilityFactor(FormatableData<ProjectMenagerParam>*);
    void setOtherSubstanceName(QString*);
    void setOtherSubstanceStat(QString*);

    // Results
    void setFlangeDiameter(FormatableData<ProjectMenagerParam>*);
    void setChokePoint(FormatableData<ProjectMenagerParam>*);
    void setFlowFactor(FormatableData<ProjectMenagerParam>*);
    void setExpanstionNumber(FormatableData<ProjectMenagerParam>*);
    void setReynoldsNumber(FormatableData<ProjectMenagerParam>*);
    void setPressureLoss(FormatableData<ProjectMenagerParam>*);

    // Tworzenie projektu
    void createNewProject();

    // Sprawdzenie wprowadzonej ścieżki w celu określenia czy projekt jest utworzony w ścieżce domyślnej
    void checkProjectPath(QString);

    // Odczyt dla zmiany danych
    void readForEditData();

    // Zmiana danych konfiguracyjnych projektu
    void rewriteConfigData();

    // Sprawdzanie, czy istnieją FormatableData bez danych
    void checkFormatableDataValidity();
};

#endif // PROJECTINFO_HPP
