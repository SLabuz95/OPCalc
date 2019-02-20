#ifndef INNERWINDOWCEP_HPP
#define INNERWINDOWCEP_HPP
#include"innerwinstatuscepenum.hpp"
#include<QFrame>
#include"projectmenagerparamenum.hpp"
#include"materialschangeswidgetstatsenum.hpp"
#include"materialschangeswidgettypeenum.hpp"
class QTimer;
class QLabel;
class QComboBox;
template <typename ParamName> class FormatableDataField;
class QCheckBox;
class QLineEdit;
class QKeyEvent;
class AppWindow;
class ProjectInfo;
class InnerWindowCEPCatchEvents;
class ModifyButtonPopUp;
template <typename ParamName> class MaterialsChangesWidget;
class ResultsGraphicPage;
class ResultsBasicDataPage;
class ResultsAdditionDataPage;
class ResultsRaportPage;
class ResultsTabBar;
class InnerWindowCEP : public QFrame{
    Q_OBJECT

public:
    // Konstruktor
    InnerWindowCEP(AppWindow*, ProjectInfo*);

    // Destruktor
    ~InnerWindowCEP();

private:
    // Parent
    AppWindow *parent = nullptr;

    // Wskaźnik do przechowywania adresu MaterialsChangesWidget, który jest szablonem (Wymagane do przechwycenia niszczenia obiektu)
    void *materialsChangesWidgetPtr = nullptr;

    // Zmienna przechowująca stan okna
    InnerWinStatusCEP *innerWinStatusCEP = nullptr;
    bool innerWinStatusCEPChanged = false;

    // Zmienna sterująca zmianami stanów
    QTimer *reloadTimer = nullptr;

    // Zmienna przechowująca informację o otwartym projekcie
    ProjectInfo* projectInfo = nullptr;

    // Event Filter
    InnerWindowCEPCatchEvents *innerWindowCEPCatchEvents = nullptr;

    // Flaga/ obiekt do usunięcia
    bool destroyed = false;

    // Pozycja własnych ustawien na liscie
    uint customDataPos = 0;

    // Elementy okna
    ModifyButtonPopUp *modifyButtonPopUp = nullptr;
    
    // - Stany:
    // - FLANGE:
    QLabel *measurementTypeLabel = nullptr;
    QLabel *measurementDeviceMaterialLabel = nullptr;
    QLabel *pressureTapDiameterLabel = nullptr;
    QLabel *maxDiffPressureLabel = nullptr;
    QComboBox *measurementDeviceMaterialList = nullptr;
    QComboBox *measurementTypeList = nullptr;
    FormatableDataField<ProjectMenagerParam> *pressureTapDiameterFDF = nullptr;
    FormatableDataField<ProjectMenagerParam> *maxDiffPressureFDF = nullptr;
    QLabel *pressureTapDiameterUnitLabel = nullptr;
    QComboBox *maxDiffPressureUnitList = nullptr;
    QLabel *measurementDeviceMaterialModifyButton = nullptr;

    // - PIPE:
    QLabel *pipeMaterialLabel = nullptr;
    QLabel *upstreamDeviceLabel = nullptr;
    QLabel *pipeDiameterLabel = nullptr;
    QLabel *maxUpstreamPressureLabel = nullptr;
    QComboBox *pipeMaterialList = nullptr;
    QComboBox *upstreamDeviceList = nullptr;
    FormatableDataField<ProjectMenagerParam> *pipeDiameterFDF = nullptr;
    FormatableDataField<ProjectMenagerParam> *maxUpstreamPressureFDF = nullptr;
    QLabel *pipeDiameterUnitLabel = nullptr;
    QComboBox *maxUpstreamPressureUnitList = nullptr;
    QLabel *pipeMaterialModifyButton = nullptr;

    // - FLOW:
    QLabel *fluidTypeLabel = nullptr;
    QLabel *maxFlowValueLabel = nullptr;
    QLabel *flowLabel = nullptr;
    QLabel *tempLabel = nullptr;
    QLabel *doubleSideFlowLabel = nullptr;
    QComboBox *fluidTypeList = nullptr;
    FormatableDataField<ProjectMenagerParam> *maxFlowValueFDF = nullptr;
    FormatableDataField<ProjectMenagerParam> *flowFDF = nullptr;
    FormatableDataField<ProjectMenagerParam> *tempFDF = nullptr;
    QCheckBox *doubleSideFlowCheckBox = nullptr;
    QComboBox *maxFlowValueUnitList = nullptr;
    QComboBox *tempUnitList = nullptr;
    QLabel *fluidTypeModifyButton = nullptr;
    
    // - FLUID:
    QLabel *substanceNameLabel = nullptr;
    QLabel *substanceNameDataLabel = nullptr;
    FormatableDataField<ProjectMenagerParam> *substanceNameTextEdit = nullptr;
    QLabel *substanceTypeLabel = nullptr;
    QLabel *substanceTypeDataLabel = nullptr;
    QCheckBox *substanceTypeFluidCheckBox = nullptr;
    QCheckBox *substanceTypeGaseCheckBox = nullptr;
    QLabel *workDensityLabel = nullptr;
    FormatableDataField<ProjectMenagerParam> *workDensityFDF = nullptr;
    QLabel *workDensityUnitlabel = nullptr;
    QLabel *dynamicViscosityLabel = nullptr;
    FormatableDataField<ProjectMenagerParam> *dynamicViscosityFDF = nullptr;
    QLabel *dynamicViscosityUnitLabel = nullptr;
    QLabel *heatCapacityRatioLabel = nullptr;
    FormatableDataField<ProjectMenagerParam> *heatCapacityRatioFDF = nullptr;
    QLabel *heatCapacityRatioUnitLabel = nullptr;
    QLabel *compresibilityFactorLabel = nullptr;
    FormatableDataField<ProjectMenagerParam> *compresibilityFactorFDF = nullptr;
    QLabel *substanceInfo = nullptr;

    // - RESULT:
    ResultsTabBar *resultsTabBar = nullptr;
    // -- GRAPHIC
    ResultsGraphicPage *resultsGraphicPage = nullptr;
    // -- BASIC
    ResultsBasicDataPage *resultsBasicDataPage = nullptr;
    // -- ADDITION
    ResultsAdditionDataPage *resultsAdditionDataPage = nullptr;
    // -- RAPORT
    ResultsRaportPage *resultsRaportPage = nullptr;
    
    // Funkcje sterujące
    void init();
    void setWindow();
    void initAndStartTimer();
    bool createWidgets();
    void createWidgetsForFlange();
    void createWidgetsForPipe();
    void createWidgetsForFlow();
    void createWidgetsForFluid();
    void createWidgetsForResult(bool);
    void createLayout();
    void createLayoutForFlange();
    void createLayoutForPipe();
    void createLayoutForFlow();
    void createLayoutForFluid();
    void createLayoutForResult(bool);
    void deleteWidgets();
    void deleteWidgetsForFlange();
    void deleteWidgetsForPipe();
    void deleteWidgetsForFlow();
    void deleteWidgetsForFluid();
    void deleteWidgetsForResult(bool);
    void connectAll();
    void disconnectAll();

public slots:
    void reloadTimerTask();
public:
    void reload();
    // FLANGE
    bool pressureTapDiameterFDFCheck(QKeyEvent*);
    void measurementDeviceMaterialModifyButtonReleased();
    // PIPE
    void pipeMaterialModifyButtonReleased();
    // FLOW
    bool temperatureFDFCheck(QKeyEvent*);
    bool FDFCheck(QKeyEvent*, FormatableDataField<ProjectMenagerParam>*, ProjectMenagerParam);
    void fluidTypeModifyButtonReleased();
    // FLUID
    bool substanceNameTextEditCheck(QKeyEvent*);
    void substanceTypeGaseCheckBoxReleased();
    void substanceTypeFluidCheckBoxReleased();
    bool workDensityFDFCheck(QKeyEvent*);
    void reactOnWorkDensityFDFBackspaceDeleteAction(bool);
    bool dynamicViscosityFDFCheck(QKeyEvent*);
    void reactOnDynamicViscosityFDFBackspaceDeleteAction(bool);
    bool heatCapacityRatioFDFCheck(QKeyEvent*);
    bool compresibilityFactorFDFCheck(QKeyEvent*);
    void createMaterialChangesWidget(MaterialsChangesWidgetStats ,MaterialsChangesWidgetType);
    // RESULT
    // Inner
    void reloadList();
    void reloadFromResultsTabBar();
    void saveFromFlangeData();
    void saveFromPipeData();
    void saveFromFlowData();
    void saveFromFluidData();

public:
    InnerWinStatusCEP* getInnerWinStatusCEP();
    ProjectInfo* getProjectInfo();
    void setInnerWinStatusCEP(InnerWinStatusCEP);
    void setDestroyed(bool);
    bool getDestroyed();
    FormatableDataField<ProjectMenagerParam>* getPressuteTapDiameterFDF();
    FormatableDataField<ProjectMenagerParam>* getMaxDiffPressureFDF();
    QLabel* getMeasurementDeviceMaterialModifyButton();
    QComboBox* getMeasurementDeviceMaterialList();
    QComboBox* getMeasurementTypeList();
    FormatableDataField<ProjectMenagerParam>* getPipeDiameterFDF();
    QComboBox* getNumbOfPressureTapsList();
    FormatableDataField<ProjectMenagerParam>* getMaxUpstreamPressureFDF();
    QComboBox* getMaxUpstreamPressureUnitList();
    QLabel* getPipeMaterialModifyButton();
    QComboBox* getPipeMaterialList();
    QComboBox* getUpstreamDeviceList();
    FormatableDataField<ProjectMenagerParam>* getMaxFlowValueFDF();
    FormatableDataField<ProjectMenagerParam>* getFlowFDF();
    FormatableDataField<ProjectMenagerParam>* getTempFDF();
    QComboBox* getMaxFlowValueUnitList();
    QComboBox* getFlowUnitList();
    QComboBox* getTempUnitList();
    QLabel* getFluidTypeModifyButton();
    QComboBox* getFluidTypeList();
    QLabel* getSubstanceNameDataLabel();
    FormatableDataField<ProjectMenagerParam>* getSubstanceNameTextEdit();
    QLabel* getSubstanceTypeDataLabel();
    QCheckBox* getSubstanceTypeFluidCheckBox();
    QCheckBox* getSubstanceTypeGaseCheckBox();
    FormatableDataField<ProjectMenagerParam>* getWorkDensityFDF();
    FormatableDataField<ProjectMenagerParam>* getDynamicViscosityFDF();
    FormatableDataField<ProjectMenagerParam>* getHeatCapacityRatioFDF();
    FormatableDataField<ProjectMenagerParam>* getCompresibilityFactorFDF();
    InnerWindowCEPCatchEvents* getInnerWindowCEPCatchEvents();
    ModifyButtonPopUp* getModifyButtonPopUp();
    void setModifyButtonPopUpNullPtr();
    AppWindow* getParent();
    ResultsTabBar* getResultsTabBar();
    ResultsGraphicPage* getResultsGraphicPage();
    ResultsBasicDataPage* getResultsBasicDataPage();
    ResultsAdditionDataPage* getResultsAdditionDataPage();
    ResultsRaportPage* getResultsRaportPage();
    uint getCustomDataPos();
    void* getMaterialsChangesWidgetPtr();

};

#endif // INNERWINDOWCEP_HPP
