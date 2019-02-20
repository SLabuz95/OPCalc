#include"innerwindowcep.hpp"
#include"appwindow.hpp"
#include"appclass.hpp"
#include"projectinfo.hpp"
#include"innerwindowcepcatchevents.hpp"
#include"materialsinfofiles.hpp"
#include<QFile>
#include"config.hpp"
#include<QComboBox>
#include"formatabledatafield.hpp"
#include<QCheckBox>
#include<QLineEdit>
#include"resultstabbar.hpp"
#include"resultsgraphicpage.hpp"
#include"resultsbasicdatapage.hpp"
#include"resultsadditiondatapage.hpp"
#include"resultsraportpage.hpp"
#include<QAbstractItemView>
#include"formatabledata.hpp"
#include"panelcep.hpp"
#include"panelcepelement.hpp"
#include<QKeyEvent>
#include"modifybuttonpopup.hpp"
#include"materialschangeswidget.cpp"
#include"promptpanel.hpp"


InnerWindowCEP::InnerWindowCEP(AppWindow *setParent, ProjectInfo* setProjectInfo){
    parent = setParent;
    projectInfo = setProjectInfo;
    parent->getApplication()->setProjectToShow(nullptr);
    init();
    if(projectInfo->getInnerWinStatusCEP() != nullptr)
        *innerWinStatusCEP = *projectInfo->getInnerWinStatusCEP();
    setWindow();
    createWidgets();
    createLayout();
    connectAll();
    initAndStartTimer();
 }

InnerWindowCEP::~InnerWindowCEP(){
    WINDOW_STATUS_SAVE(innerWinStatusCEP)
    disconnectAll();
    deleteWidgets();
    delete innerWinStatusCEP;
    innerWinStatusCEP = nullptr;
    delete reloadTimer;
    reloadTimer = nullptr;
    delete innerWindowCEPCatchEvents;
    innerWindowCEPCatchEvents = nullptr;
 }

void InnerWindowCEP::init(){
 innerWindowCEPCatchEvents = new InnerWindowCEPCatchEvents(this);
 innerWinStatusCEP =  new InnerWinStatusCEP[2];
 *innerWinStatusCEP = FLANGE;
 *(innerWinStatusCEP + 1) = FLANGE;
 customDataPos = 6;
 int numbOfMaterials = MaterialsInfoFiles::getNumbOfMaterials(new QFile(parent->getApplication()->getConfiguration()->getDefaultPath() + APP_NAME + "\\" + SUBSTANCES_FILE), *parent->getApplication()->getConfiguration());
 if(numbOfMaterials != -1 && numbOfMaterials != -2)
    customDataPos += numbOfMaterials;
}

void InnerWindowCEP::setWindow(){
    resize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
    setStyleSheet("QLabel{font-family: \"Arial\"; font-weight: normal; font-size: 12px;}");
    setContextMenuPolicy(Qt::NoContextMenu);
}

void InnerWindowCEP::initAndStartTimer(){
    reloadTimer = new QTimer();
    reloadTimer->setInterval(1);
    connect(reloadTimer, SIGNAL(timeout()), this, SLOT(reloadTimerTask()));
    reloadTimer->start();
}

bool InnerWindowCEP::createWidgets(){
    switch((uint) *innerWinStatusCEP){
    case (uint) FLANGE:
        createWidgetsForFlange();
        break;
    case (uint) PIPE:
        createWidgetsForPipe();
        break;
    case (uint)FLOW:
        createWidgetsForFlow();
        break;
    case (uint)FLUID:
        if(!projectInfo->getTemp()){
            MyDialog dlg(QUESTION_DIALOG, QString("Brak temperatury płynu"), QString("Podaj temperaturę płynu."), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }
        if(!projectInfo->getMaxUpstreamPressure()){
            MyDialog dlg(QUESTION_DIALOG, QString("Brak ciśnienia dopływowego"), QString("Podaj ciśnienie strony dopływowej."), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }
        createWidgetsForFluid();
        break;
    case (uint)RESULT:
        createWidgetsForResult(true);
        break;
    }
    return true;
}

void InnerWindowCEP::createWidgetsForFlange(){
    measurementTypeLabel = new QLabel(this);
    measurementTypeLabel->show();
    measurementDeviceMaterialLabel = new QLabel(this);
    measurementDeviceMaterialLabel->show();
    pressureTapDiameterLabel = new QLabel(this);
    pressureTapDiameterLabel->show();
    maxDiffPressureLabel = new QLabel(this);
    maxDiffPressureLabel->show();
    measurementDeviceMaterialList = new QComboBox(this);
    measurementTypeList = new QComboBox(this);
    pressureTapDiameterFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getPressureTapDiameter(),true, this, QString(), false);
    pressureTapDiameterFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    pressureTapDiameterFDF->show();
    pressureTapDiameterUnitLabel = new QLabel(this);
    pressureTapDiameterUnitLabel->show();
    maxDiffPressureFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getMaxDiffPressure(), true, this, QString(), false);
    if(maxDiffPressureFDF->getDataUTextEdit()->text().isEmpty())
        maxDiffPressureFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
    maxDiffPressureFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    maxDiffPressureFDF->show();
    maxDiffPressureUnitList = new QComboBox(this);
    maxDiffPressureUnitList->show();
    measurementDeviceMaterialModifyButton = new QLabel(this);
    measurementDeviceMaterialModifyButton->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
    measurementDeviceMaterialModifyButton->show();
}

void InnerWindowCEP::createWidgetsForPipe(){
    pipeMaterialLabel = new QLabel(this);
    pipeMaterialLabel->show();
    upstreamDeviceLabel = new QLabel(this);
    upstreamDeviceLabel->show();
    pipeDiameterLabel = new QLabel(this);
    pipeDiameterLabel->show();
    maxUpstreamPressureLabel = new QLabel(this);
    maxUpstreamPressureLabel->show();
    pipeMaterialList = new QComboBox(this);
    pipeMaterialList->show();
    upstreamDeviceList = new QComboBox(this);
    upstreamDeviceList->show();
    pipeDiameterFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getPipeDiameter(),true, this, QString(), false);
    if(pipeDiameterFDF->getDataUTextEdit()->text().isEmpty())
        pipeDiameterFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
    pipeDiameterFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    pipeDiameterFDF->show();
    maxUpstreamPressureFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getMaxUpstreamPressure(), true, this, QString(), false);
    if(maxUpstreamPressureFDF->getDataUTextEdit()->text().isEmpty())
        maxUpstreamPressureFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
    maxUpstreamPressureFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    maxUpstreamPressureFDF->show();
    pipeDiameterUnitLabel = new QLabel(this);
    pipeDiameterUnitLabel->show();
    maxUpstreamPressureUnitList = new QComboBox(this);
    maxUpstreamPressureUnitList->show();
    pipeMaterialModifyButton = new QLabel(this);
    pipeMaterialModifyButton->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
    pipeMaterialModifyButton->show();
    }

void InnerWindowCEP::createWidgetsForFlow(){
    fluidTypeLabel = new QLabel(this);
    fluidTypeLabel->show();
    maxFlowValueLabel = new QLabel(this);
    maxFlowValueLabel->show();
    flowLabel = new QLabel(this);
    flowLabel->show();
    tempLabel = new QLabel(this);
    tempLabel->show();
    doubleSideFlowLabel = new QLabel(this);
    doubleSideFlowLabel->show();
    fluidTypeList = new QComboBox(this);
    fluidTypeList->show();
    maxFlowValueFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getMaxFlowValue(), true, this, QString(), false);
    if(maxFlowValueFDF->getDataUTextEdit()->text().isEmpty())
        maxFlowValueFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
    maxFlowValueFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    maxFlowValueFDF->show();
    flowFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getFlow(), true, this, QString(), false);
    flowFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    flowFDF->show();
    tempFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getTemp(), true, this, QString(), false);
    if(tempFDF->getDataUTextEdit()->text().isEmpty())
        tempFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
    tempFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    tempFDF->show();
    doubleSideFlowCheckBox = new QCheckBox(this);
    doubleSideFlowCheckBox->show();
    maxFlowValueUnitList = new QComboBox(this);
    maxFlowValueUnitList->show();
    tempUnitList = new QComboBox(this);
    tempUnitList->show();
    fluidTypeModifyButton = new QLabel(this);
    fluidTypeModifyButton->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
    fluidTypeModifyButton->show();
}

void InnerWindowCEP::createWidgetsForFluid(){
    substanceNameLabel = new QLabel(this);
    substanceNameLabel->show();
    substanceTypeLabel = new QLabel(this);
    substanceTypeLabel->show();
    workDensityLabel = new QLabel(this);
    workDensityLabel->show();
    workDensityUnitlabel = new QLabel(this);
    workDensityUnitlabel->show();
    dynamicViscosityLabel = new QLabel(this);
    dynamicViscosityLabel->show();
    dynamicViscosityUnitLabel = new QLabel(this);
    dynamicViscosityUnitLabel->show();
    if(*projectInfo->getFluidType() != customDataPos){
        substanceNameDataLabel = new QLabel(this);
        substanceNameDataLabel->show();
        substanceTypeDataLabel = new QLabel(this);
        substanceTypeDataLabel->show();
     }else{
        substanceNameTextEdit = new FormatableDataField<ProjectMenagerParam>(true, nullptr, true, this, QString(), false);
        substanceNameTextEdit->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        substanceNameTextEdit->setContextMenuPolicy(Qt::NoContextMenu);
        substanceNameTextEdit->show();
        substanceTypeFluidCheckBox = new QCheckBox("Ciecz", this);
        substanceTypeFluidCheckBox->show();
        substanceTypeGaseCheckBox = new QCheckBox("Gaz", this);
        substanceTypeGaseCheckBox->show();
        }
    if(*projectInfo->getFluidType() != customDataPos){
        MaterialsInfoFiles::readSubstanceForFluidStat(this);
        if(!*projectInfo->getFluidDensityCustom())
            workDensityFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getFluidDensityFromFile(), true, this, QString(), false);
        else
            workDensityFDF = new FormatableDataField<ProjectMenagerParam>(true ,projectInfo->getFluidDensity(), true,this, QString(), false);
        if(workDensityFDF->getDataUTextEdit()->text().isEmpty())
            workDensityFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
        workDensityFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        if(!*projectInfo->getFluidDynamicViscosityCustom())
            dynamicViscosityFDF = new FormatableDataField<ProjectMenagerParam>(true,projectInfo->getFluidDynamicViscosityFromFile(), true,this, QString(), false);
        else
            dynamicViscosityFDF = new FormatableDataField<ProjectMenagerParam>(true,projectInfo->getFluidDynamicViscosity(), true,this, QString(), false);
        if(dynamicViscosityFDF->getDataUTextEdit()->text().isEmpty())
            dynamicViscosityFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
        dynamicViscosityFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    }else{
        workDensityFDF = new FormatableDataField<ProjectMenagerParam>(true ,projectInfo->getFluidDensity(), true,this, QString(), false);
        dynamicViscosityFDF = new FormatableDataField<ProjectMenagerParam>(true,projectInfo->getFluidDynamicViscosity(), true,this, QString(), false);
        workDensityFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        dynamicViscosityFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        if(workDensityFDF->getDataUTextEdit()->text().isEmpty())
            workDensityFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
        if(dynamicViscosityFDF->getDataUTextEdit()->text().isEmpty())
            dynamicViscosityFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");

    }
    workDensityFDF->show();
    dynamicViscosityFDF->show();
    substanceInfo = new QLabel(this);
    substanceInfo->show();
}

void InnerWindowCEP::createWidgetsForResult(bool reload){
    if(reload){
        resultsTabBar = new ResultsTabBar(this);
        static_cast<QWidget*>(resultsTabBar)->setParent(this);
        resultsTabBar->show();
    }
    switch(*resultsTabBar->getActualTab()){
    case RESULTS_GRAPHIC_TAB:
        resultsGraphicPage = new ResultsGraphicPage(this);
        static_cast<QWidget*>(resultsGraphicPage)->setParent(this);
        resultsGraphicPage->show();
        break;
    case RESULTS_BASIC_TAB:
        resultsBasicDataPage = new ResultsBasicDataPage(this);
        static_cast<QWidget*>(resultsBasicDataPage)->setParent(this);
        resultsBasicDataPage->show();
        break;
    case RESULTS_ADDITIONAL_TAB:
        resultsAdditionDataPage = new ResultsAdditionDataPage(this);
        static_cast<QWidget*>(resultsAdditionDataPage)->setParent(this);
        resultsAdditionDataPage->show();
        break;
    case RESULTS_RAPORT_TAB:
        resultsRaportPage = new ResultsRaportPage(this);
        static_cast<QWidget*>(resultsRaportPage)->setParent(this);
        resultsRaportPage->show();
        break;
    }
}

void InnerWindowCEP::createLayout(){
    switch((uint) *innerWinStatusCEP){
    case (uint)FLANGE:
        createLayoutForFlange();
        break;
    case (uint)PIPE:
        createLayoutForPipe();
        break;
    case (uint)FLOW:
        createLayoutForFlow();
        break;
    case (uint)FLUID:
        createLayoutForFluid();
        break;
    case (uint)RESULT:
        createLayoutForResult(false);
        break;
    }
}

void InnerWindowCEP::createLayoutForFlange(){
    measurementTypeLabel->setText(FLANGE_PANEL_MEASUREMENT_TYPE_TEXT);
    measurementTypeLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, ENTER_PARAMS_VERTICAL_OFFSET, FLANGE_PANEL_LABEL_WIDTH, FLANGE_PANEL_LABEL_HEIGHT);
    measurementDeviceMaterialLabel->setText(FLANGE_PANEL_MEASUREMENT_DEVICE_MATERIAL_TEXT);
    measurementDeviceMaterialLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLANGE_PANEL_LABEL_HEIGHT, FLANGE_PANEL_LABEL_WIDTH, FLANGE_PANEL_LABEL_HEIGHT);
    pressureTapDiameterLabel->setText(FLANGE_PANEL_FLANGE_TAP_DIAMETER_TEXT);
    pressureTapDiameterLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * FLANGE_PANEL_LABEL_HEIGHT, FLANGE_PANEL_LABEL_WIDTH, FLANGE_PANEL_LABEL_HEIGHT);
    maxDiffPressureLabel->setText(FLANGE_PANEL_MAX_DIFF_PRESSURE_TEXT);
    maxDiffPressureLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLANGE_PANEL_LABEL_HEIGHT, FLANGE_PANEL_LABEL_WIDTH, FLANGE_PANEL_LABEL_HEIGHT);
    measurementDeviceMaterialList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLANGE_PANEL_LABEL_WIDTH, 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLANGE_PANEL_LABEL_HEIGHT, FLANGE_PANEL_TEXTEDIT_LABEL_WIDTH + 130, FLANGE_PANEL_LABEL_HEIGHT);
    measurementDeviceMaterialList->setVisible(true);
    measurementDeviceMaterialList->addItem("Stal kwasoodporna 1.4301");
    measurementDeviceMaterialList->addItem("Stal węglowa");
    measurementDeviceMaterialList->addItem("Stal chromowa");
    measurementDeviceMaterialList->addItem("Stop Monela");
    measurementDeviceMaterialList->addItem("Brąz");
    measurementDeviceMaterialList->addItem("Aluminium");
    // Dodawanie materiałów kryz
    if(!MaterialsInfoFiles::readFlangeMaterials(this)){
        for(uint i = measurementDeviceMaterialList->count() - 1; i > 5; i--)
            measurementDeviceMaterialList->removeItem(i);
    }
    //
    if(projectInfo->getMeasurementDevice() != nullptr){
        if((int)*projectInfo->getMeasurementDevice() > measurementDeviceMaterialList->count() - 1){
            MyDialog dlg(QUESTION_DIALOG, QString("Uwaga!"), QString("Rodzaj materiału zwężki został zmieniony."), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.exec();
        }
        measurementDeviceMaterialList->setCurrentIndex((int)*projectInfo->getMeasurementDevice());
    }
    measurementTypeList->addItem("Pomiar przytarczowy");
    measurementTypeList->addItem("Pomiar kołnierzowy");
    measurementTypeList->addItem("Pomiar D i D/2");

    measurementTypeList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLANGE_PANEL_LABEL_WIDTH, ENTER_PARAMS_VERTICAL_OFFSET, FLANGE_PANEL_TEXTEDIT_LABEL_WIDTH + 130, FLANGE_PANEL_LABEL_HEIGHT);
    measurementTypeList->setVisible(true);
    if(projectInfo->getMeasurementType() != nullptr)
        measurementTypeList->setCurrentIndex((int)*projectInfo->getMeasurementType());
    pressureTapDiameterFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLANGE_PANEL_LABEL_WIDTH, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * FLANGE_PANEL_LABEL_HEIGHT, FLANGE_PANEL_FDF_WIDTH, FLANGE_PANEL_LABEL_HEIGHT);
    pressureTapDiameterFDF->getDataUTextEdit()->setGeometry(0,0,FLANGE_PANEL_TEXTEDIT_LABEL_WIDTH, FLANGE_PANEL_LABEL_HEIGHT);
    pressureTapDiameterFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid blue;}");
    maxDiffPressureFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLANGE_PANEL_LABEL_WIDTH, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLANGE_PANEL_LABEL_HEIGHT, FLANGE_PANEL_FDF_WIDTH, FLANGE_PANEL_LABEL_HEIGHT);
    maxDiffPressureFDF->getDataUTextEdit()->setGeometry(0,0,FLANGE_PANEL_TEXTEDIT_LABEL_WIDTH, FLANGE_PANEL_LABEL_HEIGHT);
    maxDiffPressureUnitList->addItem("kPa");
    maxDiffPressureUnitList->addItem(QString("mm H") + QChar(0x2082) + "O");
    if(projectInfo->getMaxDiffPressureUnits())
        maxDiffPressureUnitList->setCurrentIndex(*projectInfo->getMaxDiffPressureUnits());
    maxDiffPressureUnitList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLANGE_PANEL_LABEL_WIDTH + FLANGE_PANEL_FDF_WIDTH + 5, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLANGE_PANEL_LABEL_HEIGHT, 80, FLANGE_PANEL_LABEL_HEIGHT);
    pressureTapDiameterUnitLabel->setText("mm");
    pressureTapDiameterUnitLabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLANGE_PANEL_LABEL_WIDTH + FLANGE_PANEL_FDF_WIDTH + 5, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * FLANGE_PANEL_LABEL_HEIGHT, 80, FLANGE_PANEL_LABEL_HEIGHT);
    measurementDeviceMaterialModifyButton->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLANGE_PANEL_LABEL_WIDTH + FLANGE_PANEL_TEXTEDIT_LABEL_WIDTH + 135 , 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLANGE_PANEL_LABEL_HEIGHT, ENTER_PARAMS_PANEL_BUTTONS_WIDTH, ENTER_PARAMS_PANEL_BUTTONS_HEIGHT);
    measurementDeviceMaterialModifyButton->setScaledContents(true);
    measurementDeviceMaterialModifyButton->setAlignment(Qt::AlignCenter);
    measurementDeviceMaterialModifyButton->setPixmap(QPixmap(DOWN_EXTEND_ARROW_ICON));
}

void InnerWindowCEP::createLayoutForPipe(){
    pipeMaterialLabel->setText(PIPE_PANEL_MATERIAL_TEXT);
    pipeMaterialLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, ENTER_PARAMS_VERTICAL_OFFSET, PIPE_PANEL_LABEL_TEXT_WIDTH, PIPE_PANEL_LABEL_HEIGHT);
    upstreamDeviceLabel->setText(PIPE_PANEL_UPSTREAM_DEVICE_TEXT);
    upstreamDeviceLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 2 * ENTER_PARAMS_VERTICAL_OFFSET + PIPE_PANEL_LABEL_HEIGHT, PIPE_PANEL_LABEL_TEXT_WIDTH, PIPE_PANEL_LABEL_HEIGHT);
    pipeDiameterLabel->setText(PIPE_PANEL_PIPE_DIAMETER_TEXT);
    pipeDiameterLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * PIPE_PANEL_LABEL_HEIGHT, PIPE_PANEL_LABEL_TEXT_WIDTH, PIPE_PANEL_LABEL_HEIGHT);
    maxUpstreamPressureLabel->setText(PIPE_PANEL_MAX_UPSTREAM_PRESSURE_TEXT);
    maxUpstreamPressureLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * PIPE_PANEL_LABEL_HEIGHT, PIPE_PANEL_LABEL_TEXT_WIDTH, PIPE_PANEL_LABEL_HEIGHT);
    pipeMaterialList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + PIPE_PANEL_LABEL_TEXT_WIDTH, ENTER_PARAMS_VERTICAL_OFFSET, PIPE_PANEL_TEXTEDIT_WIDTH + 130, PIPE_PANEL_LABEL_HEIGHT);
    // Dodawanie materiałów rurociągow
    pipeMaterialList->addItem("Stal kwasoodporna 1.4301");
    pipeMaterialList->addItem("Stal węglowa");
    pipeMaterialList->addItem("Stal chromowa");
    pipeMaterialList->addItem("Stop Monela");
    pipeMaterialList->addItem("Brąz");
    pipeMaterialList->addItem("Aluminium");
    if(!MaterialsInfoFiles::readPipeMaterials(this)){
        for(uint i = pipeMaterialList->count() - 1; i > 5; i--)
            pipeMaterialList->removeItem(i);
    }
    //
    if(projectInfo->getPipeMaterial() != nullptr){
        if((int)*projectInfo->getPipeMaterial() > pipeMaterialList->count() - 1){
            MyDialog dlg(QUESTION_DIALOG, QString("Uwaga!"), QString("Rodzaj materiału rurociągu został zmieniony."), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.exec();
        }
        pipeMaterialList->setCurrentIndex((int)*projectInfo->getPipeMaterial());
    }
    upstreamDeviceList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + PIPE_PANEL_LABEL_TEXT_WIDTH, 2 * ENTER_PARAMS_VERTICAL_OFFSET + PIPE_PANEL_LABEL_HEIGHT, PIPE_PANEL_TEXTEDIT_WIDTH + 130, PIPE_PANEL_LABEL_HEIGHT);
    upstreamDeviceList->addItem(QString("Pojedyncze kolano 90") + QChar(0x00b0) + " lub dwa kolana 90" + QChar(0x00b0) + " w odstępie >30D");
    upstreamDeviceList->addItem(QString("Dwa kolana 90") + QChar(0x00b0) + " w tej samej płaszczyźnie w odstępie od 30D do 10D");
    upstreamDeviceList->addItem(QString("Dwa kolana 90") + QChar(0x00b0) + " w tej samej płaszczyźnie w odstępie <10D");
    upstreamDeviceList->addItem(QString("Dwa kolana 90") + QChar(0x00b0) +" w róźnej płaszczyźnie w odstępie >=5D");
    upstreamDeviceList->addItem(tr("Dwa kolana w róźnej płaszczyźnie w odstępie <5D"));
    upstreamDeviceList->addItem(QString("Trójnik 90") + QChar(0x00b0) + " lub kolano segmentowe 90" + QChar(0x00b0));
    upstreamDeviceList->addItem(QString("Kolano 45") + QChar(0x00b0) + " lub dwa kolana 45" + QChar(0x00b0) + " w odstępie >2D");
    upstreamDeviceList->addItem(tr("Konfuzor 2D do D na długości 1.5D do 3D"));
    upstreamDeviceList->addItem(tr("Dyfuzor 0.5D do D na długości D do 2D"));
    upstreamDeviceList->addItem(tr("Pełnoprzelotowy zawór kulowy lub zasuwa całkowicie otwarte"));
    upstreamDeviceList->addItem(tr("Skokowe zwężenie symetryczne"));
    upstreamDeviceList->addItem(tr("Tuleja lub gniazdo termometru o średnicy <=0.03D"));
    upstreamDeviceList->view()->setMinimumHeight(157);
    if(projectInfo->getUpstreamDevice())
        upstreamDeviceList->setCurrentIndex((int)*projectInfo->getUpstreamDevice());
    pipeDiameterFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + PIPE_PANEL_LABEL_TEXT_WIDTH, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * PIPE_PANEL_LABEL_HEIGHT, PIPE_PANEL_FDF_WIDTH, PIPE_PANEL_LABEL_HEIGHT);
    pipeDiameterFDF->getDataUTextEdit()->setGeometry(0,0,PIPE_PANEL_TEXTEDIT_WIDTH, PIPE_PANEL_LABEL_HEIGHT);
    maxUpstreamPressureFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + PIPE_PANEL_LABEL_TEXT_WIDTH, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * PIPE_PANEL_LABEL_HEIGHT, PIPE_PANEL_FDF_WIDTH, PIPE_PANEL_LABEL_HEIGHT);
    maxUpstreamPressureFDF->getDataUTextEdit()->setGeometry(0,0,PIPE_PANEL_TEXTEDIT_WIDTH, PIPE_PANEL_LABEL_HEIGHT);
    pipeDiameterUnitLabel->setText("mm");
    pipeDiameterUnitLabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + PIPE_PANEL_LABEL_TEXT_WIDTH + PIPE_PANEL_FDF_WIDTH + 5, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * PIPE_PANEL_LABEL_HEIGHT, 80, PIPE_PANEL_LABEL_HEIGHT);
    maxUpstreamPressureUnitList->addItem("MPa");
    maxUpstreamPressureUnitList->addItem("kPa");
    maxUpstreamPressureUnitList->addItem("Bar");
    if(projectInfo->getMaxUpstreamPressureUnits())
        maxUpstreamPressureUnitList->setCurrentIndex((int)*projectInfo->getMaxUpstreamPressureUnits());
    maxUpstreamPressureUnitList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + PIPE_PANEL_LABEL_TEXT_WIDTH + PIPE_PANEL_FDF_WIDTH + 5, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * PIPE_PANEL_LABEL_HEIGHT, 80, PIPE_PANEL_LABEL_HEIGHT);
    pipeMaterialModifyButton->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + PIPE_PANEL_LABEL_TEXT_WIDTH + PIPE_PANEL_TEXTEDIT_WIDTH + 135,ENTER_PARAMS_VERTICAL_OFFSET, ENTER_PARAMS_PANEL_BUTTONS_WIDTH, ENTER_PARAMS_PANEL_BUTTONS_HEIGHT);
    pipeMaterialModifyButton->setScaledContents(true);
    pipeMaterialModifyButton->setAlignment(Qt::AlignCenter);
    pipeMaterialModifyButton->setPixmap(QPixmap(DOWN_EXTEND_ARROW_ICON));
}

void InnerWindowCEP::createLayoutForFlow(){
    fluidTypeLabel->setText(FLOW_PANEL_FLUID_TYPE_TEXT);
    fluidTypeLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, ENTER_PARAMS_VERTICAL_OFFSET, FLOW_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    maxFlowValueLabel->setText(FLOW_PANEL_MAX_FLOW_VALUE_TEXT);
    maxFlowValueLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    flowLabel->setText(FLOW_PANEL_FLOW_TEXT);
    flowLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    tempLabel->setText(FLOW_PANEL_TEMP_TEXT);
    tempLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    doubleSideFlowLabel->setText(FLOW_PANEL_DOUBLE_SIDE_FLOW_TEXT);
    doubleSideFlowLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 5 * ENTER_PARAMS_VERTICAL_OFFSET + 4 * FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    fluidTypeList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLOW_PANEL_LABEL_WIDTH, ENTER_PARAMS_VERTICAL_OFFSET, FLOW_PANEL_TEXTEDIT_WIDTH + 130, FLOW_PANEL_LABEL_HEIGHT);
    // Dodawanie substancji
    fluidTypeList->addItem("Woda");
    fluidTypeList->addItem("Para wodna");
    fluidTypeList->addItem("Powietrze");
    fluidTypeList->addItem("Dwutlenek węgla");
    fluidTypeList->addItem("Wodór");
    fluidTypeList->addItem("Azot");
    if(!MaterialsInfoFiles::readSubstances(this)){
        for(uint i = fluidTypeList->count() - 1; i > 5; i--)
            fluidTypeList->removeItem(i);
    }
    fluidTypeList->addItem(tr("Inna substancja"));
    //
    customDataPos = (uint)fluidTypeList->count() - 1;
    if(projectInfo->getFluidType()){
        if((int)*projectInfo->getFluidType() > fluidTypeList->count() - 1){
            MyDialog dlg(QUESTION_DIALOG, QString("Uwaga!"), QString("Rodzaj substancji został zmieniony."), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.exec();
        }
        fluidTypeList->setCurrentIndex((int)*projectInfo->getFluidType());
    }
    maxFlowValueFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLOW_PANEL_LABEL_WIDTH, 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_FDF_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    maxFlowValueFDF->getDataUTextEdit()->setGeometry(0,0,FLOW_PANEL_TEXTEDIT_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    flowFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLOW_PANEL_LABEL_WIDTH, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_FDF_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    flowFDF->getDataUTextEdit()->setGeometry(0,0,FLOW_PANEL_TEXTEDIT_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    flowFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid blue;}");
    tempFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLOW_PANEL_LABEL_WIDTH, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_FDF_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    tempFDF->getDataUTextEdit()->setGeometry(0,0,FLOW_PANEL_TEXTEDIT_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
    doubleSideFlowCheckBox->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLOW_PANEL_LABEL_WIDTH, 5 * ENTER_PARAMS_VERTICAL_OFFSET + 4 * FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_LABEL_HEIGHT, FLOW_PANEL_LABEL_HEIGHT);
    doubleSideFlowCheckBox->setChecked((projectInfo->getDoubleSideFlow())? *projectInfo->getDoubleSideFlow() : false);
    maxFlowValueUnitList->addItem(QString("m") + QChar(0x00b3) + "/h");
    maxFlowValueUnitList->addItem("kg/s");
    maxFlowValueUnitList->addItem("kg/h");
    if((uint)fluidTypeList->currentIndex() == customDataPos)
        maxFlowValueUnitList->addItem(QString("Nm") + QChar(0x00b3) + "/h");
    maxFlowValueUnitList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLOW_PANEL_LABEL_WIDTH + FLOW_PANEL_FDF_WIDTH + 5, 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLOW_PANEL_LABEL_HEIGHT, 80, FLOW_PANEL_LABEL_HEIGHT);
    if(projectInfo->getFlowUnits())
        maxFlowValueUnitList->setCurrentIndex((int)*projectInfo->getFlowUnits());
    tempUnitList->addItem(QString(QChar(176)) + "C");
    tempUnitList->addItem(QString(QChar(176)) + "K");
    tempUnitList->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLOW_PANEL_LABEL_WIDTH + FLOW_PANEL_FDF_WIDTH + 5, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLOW_PANEL_LABEL_HEIGHT, 80, FLOW_PANEL_LABEL_HEIGHT);
    if(projectInfo->getTempUnits())
        tempUnitList->setCurrentIndex((int)*projectInfo->getTempUnits());
    fluidTypeModifyButton->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLOW_PANEL_LABEL_WIDTH + FLOW_PANEL_TEXTEDIT_WIDTH + 135, ENTER_PARAMS_VERTICAL_OFFSET, ENTER_PARAMS_PANEL_BUTTONS_WIDTH, ENTER_PARAMS_PANEL_BUTTONS_HEIGHT);
    fluidTypeModifyButton->setAlignment(Qt::AlignCenter);
    fluidTypeModifyButton->setScaledContents(true);
    fluidTypeModifyButton->setPixmap(QPixmap(DOWN_EXTEND_ARROW_ICON));
}

void InnerWindowCEP::createLayoutForFluid(){
    substanceInfo->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 3 * ENTER_PARAMS_VERTICAL_OFFSET + 2 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_TEXTEDIT_WIDTH + ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
    substanceInfo->setText("Substancja o parametrach: " + QString::number(*projectInfo->getTemp()->getData(), 'f', 2) + [](ProjectInfo* projectInfo)->QString{switch(*projectInfo->getTempUnits()){case 0: return QString(QChar(176)) + "C"; case 1: return QString(QChar(176)) + "K";} return QString(NO_DATA_INFORMATION);}(projectInfo) + QString(" , ") + QString::number(*projectInfo->getMaxUpstreamPressure()->getData()) + QString(" ") + [](ProjectInfo* projectInfo)->QString{switch(*projectInfo->getMaxUpstreamPressureUnits()){case 0: return QString("MPa"); case 1: return QString("kPa"); case 2: return QString("bar"); case 3: return QString("atm");}return QString(NO_DATA_INFORMATION);}(projectInfo));
    substanceInfo->setStyleSheet("QLabel{font-family: \"Georgia\"; font-size: 10;}");
    substanceInfo->show();
    if(*projectInfo->getFluidType() != customDataPos){
        if(App::compareStr(substanceTypeDataLabel->text(), "Gaz")){
            heatCapacityRatioLabel = new QLabel(this);
            heatCapacityRatioLabel->setText(FLUID_PANEL_HEAT_CAPACITY_RATIO_LABEL_TEXT);
            heatCapacityRatioLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLOW_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
            heatCapacityRatioLabel->show();
            heatCapacityRatioFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getFluidHeatCapacityRatio(), true, this, QString(), false);
            heatCapacityRatioFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
            heatCapacityRatioFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
            heatCapacityRatioFDF->setGeometry(2* ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLOW_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
            heatCapacityRatioFDF->show();
            heatCapacityRatioUnitLabel = new QLabel(this);
            heatCapacityRatioUnitLabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH + FLUID_PANEL_FDF_WIDTH + 5, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLOW_PANEL_LABEL_HEIGHT, 80, FLOW_PANEL_LABEL_HEIGHT);
            heatCapacityRatioUnitLabel->setText(FLUID_PANEL_HEAT_CAPACITY_RATIO_UNIT_LABEL_TEXT);
            heatCapacityRatioUnitLabel->show();
        }
        substanceNameDataLabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, ENTER_PARAMS_VERTICAL_OFFSET, FLUID_PANEL_TEXTEDIT_WIDTH + 130, FLUID_PANEL_LABEL_HEIGHT);
        substanceTypeDataLabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        workDensityFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        workDensityFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        dynamicViscosityFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 5 * ENTER_PARAMS_VERTICAL_OFFSET + 4 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        dynamicViscosityFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        if(projectInfo->getFluidDensityCustom() && (*(projectInfo->getFluidDensityCustom()))){
            workDensityFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid orange;}");
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DENSITY, workDensityFDF);
            }
        if(projectInfo->getFluidDynamicViscosityCustom() && (*(projectInfo->getFluidDynamicViscosityCustom()))){
            dynamicViscosityFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid orange;}");
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, dynamicViscosityFDF);
           }
    }
    else
    {
        if(projectInfo->getOtherSubstanceName())
            substanceNameTextEdit->getDataUTextEdit()->setText(*projectInfo->getOtherSubstanceName());
        substanceNameTextEdit->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, ENTER_PARAMS_VERTICAL_OFFSET, FLUID_PANEL_TEXTEDIT_WIDTH + 130, FLUID_PANEL_LABEL_HEIGHT);
        substanceNameTextEdit->setStyleSheet("QLineEdit{border: 1px solid blue;}");
        substanceTypeFluidCheckBox->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_TEXTEDIT_WIDTH * 0.5, FLUID_PANEL_LABEL_HEIGHT);
        substanceTypeGaseCheckBox->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH + FLUID_PANEL_TEXTEDIT_WIDTH * 0.5, 2 * ENTER_PARAMS_VERTICAL_OFFSET + FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_TEXTEDIT_WIDTH * 0.5, FLUID_PANEL_LABEL_HEIGHT);
        if(projectInfo->getOtherSubstanceStat() && App::compareStr(*projectInfo->getOtherSubstanceStat(), "Gaz"))
        {
            substanceTypeGaseCheckBox->setChecked(true);
            substanceTypeFluidCheckBox->setChecked(false);
        }else{
            substanceTypeFluidCheckBox->setChecked(true);
            substanceTypeGaseCheckBox->setChecked(false);
        }
        dynamicViscosityFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 5 * ENTER_PARAMS_VERTICAL_OFFSET + 4 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        dynamicViscosityFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        workDensityFDF->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        workDensityFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        if(substanceTypeGaseCheckBox->isChecked()){
            heatCapacityRatioLabel = new QLabel(this);
            heatCapacityRatioLabel->setText(FLUID_PANEL_HEAT_CAPACITY_RATIO_LABEL_TEXT);
            heatCapacityRatioLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLOW_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
            heatCapacityRatioLabel->show();
            heatCapacityRatioFDF = new FormatableDataField<ProjectMenagerParam>(true, projectInfo->getFluidHeatCapacityRatio(), true, this, QString(), false);
            heatCapacityRatioFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
            heatCapacityRatioFDF->setGeometry(2* ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLOW_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
            heatCapacityRatioFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
            heatCapacityRatioFDF->show();
            heatCapacityRatioUnitLabel = new QLabel(this);
            heatCapacityRatioUnitLabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH + FLUID_PANEL_FDF_WIDTH + 5, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLOW_PANEL_LABEL_HEIGHT, 80, FLOW_PANEL_LABEL_HEIGHT);
            heatCapacityRatioUnitLabel->setText(FLUID_PANEL_HEAT_CAPACITY_RATIO_UNIT_LABEL_TEXT);
            heatCapacityRatioUnitLabel->show();
            compresibilityFactorLabel = new QLabel(this);
            compresibilityFactorLabel->setText(FLUID_PANEL_COMPRESIBILITY_FACTOR_TEXT);
            compresibilityFactorLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 7 * ENTER_PARAMS_VERTICAL_OFFSET + 6 * FLOW_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
            compresibilityFactorLabel->show();
            compresibilityFactorFDF = new FormatableDataField<ProjectMenagerParam>(true,projectInfo->getCompresibilityFactor(), true,this, QString(), false);
            compresibilityFactorFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
            compresibilityFactorFDF->setGeometry(2* ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 7 * ENTER_PARAMS_VERTICAL_OFFSET + 6 * FLOW_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
            compresibilityFactorFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
            compresibilityFactorFDF->show();
        }
    }
    substanceNameLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, ENTER_PARAMS_VERTICAL_OFFSET, FLUID_PANEL_LABEL_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
    substanceNameLabel->setText(FLUID_PANEL_SUBSTANCE_NAME_LABEL_TEXT);
    substanceTypeLabel->setText(FLUID_PANEL_SUBSTANCE_TYPE_LABEL_TEXT);
    substanceTypeLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET,2 * ENTER_PARAMS_VERTICAL_OFFSET + FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
    workDensityLabel->setText(FLUID_PANEL_WORK_DENSITY_LABEL_TEXT);
    workDensityLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
    workDensityUnitlabel->setText(FLUID_PANEL_WORK_DENSITY_UNIT_LABEL_TEXT);
    workDensityUnitlabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH + FLUID_PANEL_FDF_WIDTH + 5, 4 * ENTER_PARAMS_VERTICAL_OFFSET + 3 * FLUID_PANEL_LABEL_HEIGHT, 80, FLUID_PANEL_LABEL_HEIGHT);
    dynamicViscosityLabel->setText(FLUID_PANEL_DYNAMIC_VISCOSITY_LABEL_TEXT);
    dynamicViscosityLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 5 * ENTER_PARAMS_VERTICAL_OFFSET + 4 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
    dynamicViscosityUnitLabel->setText(FLUID_PANEL_DYNAMIC_VISCOSITY_UNIT_LABEL_TEXT);
    dynamicViscosityUnitLabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH + FLUID_PANEL_FDF_WIDTH + 5, 5 * ENTER_PARAMS_VERTICAL_OFFSET + 4 * FLUID_PANEL_LABEL_HEIGHT, 80, FLUID_PANEL_LABEL_HEIGHT);
}

void InnerWindowCEP::createLayoutForResult(bool reload){
    if(reload)
        resultsTabBar->setGeometry(0, 0, RESULT_PANEL_TAB_WIDTH, RESULT_PANEL_TAB_HEIGHT);
    switch(*resultsTabBar->getActualTab()){
    case RESULTS_GRAPHIC_TAB:
        resultsGraphicPage->setGeometry(0, RESULT_PANEL_TAB_HEIGHT, RESULT_PANEL_TAB_WIDTH, RESULT_PANEL_TAB_CONTENT_HEIGHT);
        break;
    case RESULTS_BASIC_TAB:
        resultsBasicDataPage->setGeometry(0, RESULT_PANEL_TAB_HEIGHT, RESULT_PANEL_TAB_WIDTH, RESULT_PANEL_TAB_CONTENT_HEIGHT);
        break;
    case RESULTS_ADDITIONAL_TAB:
        resultsAdditionDataPage->setGeometry(0, RESULT_PANEL_TAB_HEIGHT, RESULT_PANEL_TAB_WIDTH, RESULT_PANEL_TAB_CONTENT_HEIGHT);
        break;
    case RESULTS_RAPORT_TAB:
        resultsRaportPage->setGeometry(0, RESULT_PANEL_TAB_HEIGHT, RESULT_PANEL_TAB_WIDTH, RESULT_PANEL_TAB_CONTENT_HEIGHT);
        break;
    }
}

void InnerWindowCEP::deleteWidgets(){
    switch((uint) *(innerWinStatusCEP + 1)){
    case (uint)FLANGE:
        deleteWidgetsForFlange();
        break;
    case (uint)PIPE:
        deleteWidgetsForPipe();
        break;
    case (uint)FLOW:
        deleteWidgetsForFlow();
        break;
    case (uint)FLUID:
        deleteWidgetsForFluid();
        break;
    case (uint)RESULT:
        deleteWidgetsForResult(true);
        break;
    }
}

void InnerWindowCEP::deleteWidgetsForFlange(){
    delete measurementTypeLabel;
    measurementTypeLabel = nullptr;
    delete measurementDeviceMaterialLabel;
    measurementDeviceMaterialLabel = nullptr;
    delete pressureTapDiameterLabel;
    pressureTapDiameterLabel = nullptr;
    delete maxDiffPressureLabel;
    maxDiffPressureLabel = nullptr;
    delete measurementTypeList;
    measurementTypeList = nullptr;
    delete measurementDeviceMaterialList;
    measurementDeviceMaterialList = nullptr;
    delete pressureTapDiameterFDF;
    pressureTapDiameterFDF = nullptr;
    delete maxDiffPressureFDF;
    maxDiffPressureFDF = nullptr;
    delete pressureTapDiameterUnitLabel;
    pressureTapDiameterUnitLabel = nullptr;
    delete maxDiffPressureUnitList;
    maxDiffPressureUnitList = nullptr;
    delete measurementDeviceMaterialModifyButton;
    measurementDeviceMaterialModifyButton = nullptr;
 }

void InnerWindowCEP::deleteWidgetsForPipe(){
    delete pipeMaterialLabel;
    pipeMaterialLabel = nullptr;
    delete upstreamDeviceLabel;
    upstreamDeviceLabel = nullptr;
    delete pipeDiameterLabel;
    pipeDiameterLabel = nullptr;
    delete maxUpstreamPressureLabel;
    maxUpstreamPressureLabel = nullptr;
    delete pipeMaterialList;
    pipeMaterialList = nullptr;
    delete upstreamDeviceList;
    upstreamDeviceList = nullptr;
    delete pipeDiameterFDF;
    pipeDiameterFDF = nullptr;
    delete maxUpstreamPressureFDF;
    maxUpstreamPressureFDF = nullptr;
    delete pipeDiameterUnitLabel;
    pipeDiameterUnitLabel = nullptr;
    delete maxUpstreamPressureUnitList;
    maxUpstreamPressureUnitList = nullptr;
    delete pipeMaterialModifyButton;
    pipeMaterialModifyButton = nullptr;
   }

void InnerWindowCEP::deleteWidgetsForFlow(){
    delete fluidTypeLabel;
    fluidTypeLabel = nullptr;
    delete maxFlowValueLabel;
    maxFlowValueLabel = nullptr;
    delete flowLabel;
    flowLabel = nullptr;
    delete tempLabel;
    tempLabel = nullptr;
    delete doubleSideFlowLabel;
    doubleSideFlowLabel = nullptr;
    delete fluidTypeList;
    fluidTypeList = nullptr;
    delete maxFlowValueFDF;
    maxFlowValueFDF = nullptr;
    delete flowFDF;
    flowFDF = nullptr;
    delete tempFDF;
    tempFDF = nullptr;
    delete doubleSideFlowCheckBox;
    doubleSideFlowCheckBox = nullptr;
    delete maxFlowValueUnitList;
    maxFlowValueUnitList = nullptr;
    delete tempUnitList;
    tempUnitList = nullptr;
    delete fluidTypeModifyButton;
    fluidTypeModifyButton = nullptr;
}

void InnerWindowCEP::deleteWidgetsForFluid(){
    delete substanceNameDataLabel;
    substanceNameDataLabel = nullptr;
    delete substanceNameLabel;
    substanceNameLabel = nullptr;
    delete substanceNameTextEdit;
    substanceNameTextEdit = nullptr;
    delete substanceTypeDataLabel;
    substanceTypeDataLabel = nullptr;
    delete substanceTypeFluidCheckBox;
    substanceTypeFluidCheckBox = nullptr;
    delete substanceTypeGaseCheckBox;
    substanceTypeGaseCheckBox = nullptr;
    delete substanceTypeLabel;
    substanceTypeLabel = nullptr;
    delete workDensityLabel;
    workDensityLabel = nullptr;
    delete workDensityFDF;
    workDensityFDF = nullptr;
    delete workDensityUnitlabel;
    workDensityUnitlabel = nullptr;
    delete dynamicViscosityLabel;
    dynamicViscosityLabel = nullptr;
    delete dynamicViscosityFDF;
    dynamicViscosityFDF = nullptr;
    delete dynamicViscosityUnitLabel;
    dynamicViscosityUnitLabel = nullptr;
    delete heatCapacityRatioLabel;
    heatCapacityRatioLabel = nullptr;
    delete heatCapacityRatioFDF;
    heatCapacityRatioFDF = nullptr;
    delete heatCapacityRatioUnitLabel;
    heatCapacityRatioUnitLabel = nullptr;
    delete compresibilityFactorLabel;
    compresibilityFactorLabel = nullptr;
    delete compresibilityFactorFDF;
    compresibilityFactorFDF = nullptr;
    delete substanceInfo;
    substanceInfo = nullptr;
}

void InnerWindowCEP::deleteWidgetsForResult(bool reload){
    switch(((reload)? *resultsTabBar->getActualTab() : *(resultsTabBar->getActualTab() + 1))){
    case RESULTS_GRAPHIC_TAB:
        delete resultsGraphicPage;
        resultsGraphicPage = nullptr;
        break;
    case RESULTS_BASIC_TAB:
        delete resultsBasicDataPage;
        resultsBasicDataPage = nullptr;
        break;
    case RESULTS_ADDITIONAL_TAB:
        delete resultsAdditionDataPage;
        resultsAdditionDataPage = nullptr;
        break;
    case RESULTS_RAPORT_TAB:
        delete resultsRaportPage;
        resultsRaportPage = nullptr;
        break;
    }
    if(reload)
    {
        delete resultsTabBar;
        resultsTabBar = nullptr;
    }
}

void InnerWindowCEP::connectAll(){
    switch((uint) *innerWinStatusCEP){
    case (uint)FLANGE:
        measurementDeviceMaterialList->view()->installEventFilter(innerWindowCEPCatchEvents);
        pressureTapDiameterFDF->installEventFilter(innerWindowCEPCatchEvents);
        pressureTapDiameterFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        maxDiffPressureFDF->installEventFilter(innerWindowCEPCatchEvents);
        maxDiffPressureFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        measurementDeviceMaterialModifyButton->installEventFilter(innerWindowCEPCatchEvents);
        break;
    case (uint) PIPE:
        pipeDiameterFDF->installEventFilter(innerWindowCEPCatchEvents);
        pipeDiameterFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        maxUpstreamPressureFDF->installEventFilter(innerWindowCEPCatchEvents);
        maxUpstreamPressureFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        pipeMaterialModifyButton->installEventFilter(innerWindowCEPCatchEvents);
        break;
    case (uint) FLOW:
        maxFlowValueFDF->installEventFilter(innerWindowCEPCatchEvents);
        maxFlowValueFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        flowFDF->installEventFilter(innerWindowCEPCatchEvents);
        flowFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        tempFDF->installEventFilter(innerWindowCEPCatchEvents);
        tempFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        fluidTypeModifyButton->installEventFilter(innerWindowCEPCatchEvents);
        break;
    case (uint) FLUID:
        if(*projectInfo->getFluidType() != customDataPos){
            workDensityFDF->installEventFilter(innerWindowCEPCatchEvents);
            workDensityFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
            dynamicViscosityFDF->installEventFilter(innerWindowCEPCatchEvents);
            dynamicViscosityFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
            if(App::compareStr(substanceTypeDataLabel->text(), "Gaz")){
                heatCapacityRatioFDF->installEventFilter(innerWindowCEPCatchEvents);
                heatCapacityRatioFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
            }
        }else{
            substanceNameTextEdit->installEventFilter(innerWindowCEPCatchEvents);
            substanceTypeFluidCheckBox->installEventFilter(innerWindowCEPCatchEvents);
            substanceTypeGaseCheckBox->installEventFilter(innerWindowCEPCatchEvents);
            workDensityFDF->installEventFilter(innerWindowCEPCatchEvents);
            workDensityFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
            dynamicViscosityFDF->installEventFilter(innerWindowCEPCatchEvents);
            dynamicViscosityFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
            if(substanceTypeGaseCheckBox->isChecked()){
                heatCapacityRatioFDF->installEventFilter(innerWindowCEPCatchEvents);
                heatCapacityRatioFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
                compresibilityFactorFDF->installEventFilter(innerWindowCEPCatchEvents);
                compresibilityFactorFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
            }
        }
        break;
    case (uint) RESULT:
        break;
    }
}

void InnerWindowCEP::disconnectAll(){
    // Brak
}

void InnerWindowCEP::reload(){
    reloadTimer->stop();
    innerWinStatusCEPChanged = false;
    disconnectAll();
    deleteWidgets();
    if(!createWidgets())
    {
        InnerWinStatusCEP temp = *(innerWinStatusCEP + 1);
        WINDOW_STATUS_SAVE(innerWinStatusCEP);
        *innerWinStatusCEP = temp;
        innerWinStatusCEPChanged = true;
        for(uint i = 0; i < 6; i++){
                (*(parent->getPanelCEP()->getPanelCEPElement() + i))->setStyleSheet(QString("PanelCEPElement{border: 1px solid rgba(255, 255, 255, 0.4);") + ((parent->getInnerWinCEP() != nullptr && i == *parent->getInnerWinCEP()->getInnerWinStatusCEP())? QString("background-color: qlineargradient(x1:0 , y1: 0.5, x2: 1, y2: 0.5, stop: 0 rgba(200,200,200,0.77), stop: 0.93 rgba(247,247,247,0.8), stop: 0.99 rgba(247,247,247,0.8));") : QString("background-color: rgba(255, 255, 255, 0.4);")) + ((i == 0)? QString("border-top-right-radius: 20px 15px;") : QString("")) +  ((i == 5)? QString("border-bottom-right-radius: 20px 15px;") : QString("")) + "}");
        }
        reloadTimer->start();
        return;
    }
    createLayout();
    connectAll();
    parent->getPanelCEP()->reload();
    reloadTimer->start();
}

void InnerWindowCEP::reloadFromResultsTabBar(){
    deleteWidgetsForResult(false);
    createWidgetsForResult(false);
    createLayoutForResult(false);
}

bool InnerWindowCEP::pressureTapDiameterFDFCheck(QKeyEvent* ev){
    QLineEdit *te = pressureTapDiameterFDF->getDataUTextEdit();
    uint i = 0;
    uint len = te->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    // Połączenie bezpośrednie
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(te->text().at(i) == '.')
                return true;
            else{
                if(te->text().at(i) == 'e' && te->cursorPosition() >= i)
                    return true;
            }
        te->insert(".");
    }
        break;
    case Qt::Key_Delete:
        te->del();
        break;
    case Qt::Key_Backspace:
        te->backspace();
        break;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Minus:
    {
        int selStart = te->selectionStart();
        if(selStart > 0 ){
            if(te->text().at(selStart - 1) == 'e')
                te->insert("-");
        }else{
            int curPos = te->cursorPosition();
            if(selStart == -1 && curPos > 0 && te->text().at(curPos - 1) == 'e' && (curPos == len || te->text().at(curPos) != '-'))
                te->insert("-");
        }
    }
        break;
    case Qt::Key_E:
        for( ; i < len; i++)
            if(te->text().at(i) == 'e')
                return true;
        te->insert(ev->text().toLower());
        break;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39 )
            te->insert(ev->text());
        else
            return true;
    }
    if(FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), false)){
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(pressureTapDiameterFDF->getFormatableData()){
              pressureTapDiameterFDF->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_PRESSURE_TAP_DIAMETER, false);
                pressureTapDiameterFDF->setFormatableData(tempFD);
                delete tempFD;
       }
    }
    return true;
}

bool InnerWindowCEP::temperatureFDFCheck(QKeyEvent* ev){
    QLineEdit *te = tempFDF->getDataUTextEdit();
    uint i = 0;
    uint len = te->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    // Połączenie bezpośrednie
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(te->text().at(i) == '.')
                return true;
            else{
                if(te->text().at(i) == 'e' && te->cursorPosition() >= i)
                    return true;
            }
        te->insert(".");
    }
        break;
    case Qt::Key_Delete:
        te->del();
        break;
    case Qt::Key_Backspace:
        te->backspace();
        break;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Minus:
    {
        int selStart = te->selectionStart();
        if(selStart != -1){
            if(selStart == 0){
                int i = 0;
                for(i = te->selectionEnd(); i < len; i++){
                    if(te->text().at(i) == 'e'){
                        te->insert("-");
                        break;
                    }else
                        if(te->text().at(i) == '-'){
                            te->insert("");
                            break;
                        }
                }
                if(i == len)
                    te->insert("-");
            }else{
                if(te->text().at(selStart - 1) == 'e')
                    te->insert("-");
            }
        }else{
            int curPos = te->cursorPosition();
            if(curPos == 0){
                if(curPos == len || te->text().at(curPos) != '-')
                    te->insert("-");
            }else{
                if( te->text().at(curPos - 1) == 'e' && (curPos == len || te->text().at(curPos) != '-'))
                    te->insert("-");
            }
        }
    }
        break;
    case Qt::Key_E:
        for( ; i < len; i++)
            if(te->text().at(i) == 'e')
                return true;
        te->insert(ev->text().toLower());
        break;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39 )
            te->insert(ev->text());
        else
            return true;
    }
    if(!FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), true))
        te->setStyleSheet("QLineEdit{border: 1px solid red;}");
    else{
        te->setStyleSheet("");
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(tempFDF->getFormatableData()){
              tempFDF->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_TEMP, false);
                tempFDF->setFormatableData(tempFD);
                delete tempFD;
       }
    }
    return true;
}

bool InnerWindowCEP::FDFCheck(QKeyEvent* ev, FormatableDataField<ProjectMenagerParam> *fdf, ProjectMenagerParam pmp){
    QLineEdit *te = fdf->getDataUTextEdit();
    uint i = 0;
    uint len = te->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    // Połączenie bezpośrednie
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(te->text().at(i) == '.')
                return true;
            else{
                if(te->text().at(i) == 'e' && te->cursorPosition() >= i)
                    return true;
            }
        te->insert(".");
    }
        break;
    case Qt::Key_Delete:
        te->del();
        break;
    case Qt::Key_Backspace:
        te->backspace();
        break;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Minus:
    {
        int selStart = te->selectionStart();
        if(selStart > 0 ){
            if(te->text().at(selStart - 1) == 'e')
                te->insert("-");
        }else{
            int curPos = te->cursorPosition();
            if(selStart == -1 && curPos > 0 && te->text().at(curPos - 1) == 'e' && (curPos == len || te->text().at(curPos) != '-'))
                te->insert("-");
        }
    }
        break;
    case Qt::Key_E:
        for( ; i < len; i++)
            if(te->text().at(i) == 'e')
                return true;
        te->insert(ev->text().toLower());
        break;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39 )
            te->insert(ev->text());
        else
            return true;
    }
    if(!FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), false)){
        if(!(pmp == PROJECT_MENAGER_FLOW || pmp ==PROJECT_MENAGER_PRESSURE_TAP_DIAMETER))
            te->setStyleSheet("QLineEdit{border: 1px solid red;}");
    }
    else{
        if(!(pmp == PROJECT_MENAGER_FLOW || pmp ==PROJECT_MENAGER_PRESSURE_TAP_DIAMETER))
            te->setStyleSheet("");
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(fdf->getFormatableData()){
              fdf->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), pmp, false);
                fdf->setFormatableData(tempFD);
                delete tempFD;
       }
    }
    return true;
}

void InnerWindowCEP::measurementDeviceMaterialModifyButtonReleased(){
    QCursor c(cursor());
    modifyButtonPopUp = new ModifyButtonPopUp(this, FLANGE_MATERIAL_TYPE);
    static_cast<QFrame*>(modifyButtonPopUp)->setParent(this);
    modifyButtonPopUp->show();
    modifyButtonPopUp->setGeometry(measurementDeviceMaterialModifyButton->x(), measurementDeviceMaterialModifyButton->y() + ENTER_PARAMS_PANEL_BUTTONS_HEIGHT, MODIFY_BUTTON_POPUP_WIDTH, MODIFY_BUTTON_POPUP_HEIGHT);
    c.setPos(mapToGlobal(QPoint(modifyButtonPopUp->x() + MODIFY_BUTTON_POPUP_BUTTON_WIDTH * 0.5, modifyButtonPopUp->y() + MODIFY_BUTTON_POPUP_BUTTON_HEIGHT * 0.5)));
    modifyButtonPopUp->installEventFilter(innerWindowCEPCatchEvents);
}

void InnerWindowCEP::pipeMaterialModifyButtonReleased(){
    QCursor c(cursor());
    modifyButtonPopUp = new ModifyButtonPopUp(this, PIPE_MATERIAL_TYPE);
    static_cast<QFrame*>(modifyButtonPopUp)->setParent(this);
    modifyButtonPopUp->show();
    modifyButtonPopUp->setGeometry(pipeMaterialModifyButton->x(), pipeMaterialModifyButton->y() + ENTER_PARAMS_PANEL_BUTTONS_HEIGHT, MODIFY_BUTTON_POPUP_WIDTH, MODIFY_BUTTON_POPUP_HEIGHT);
    c.setPos(mapToGlobal(QPoint(modifyButtonPopUp->x() + MODIFY_BUTTON_POPUP_BUTTON_WIDTH * 0.5, modifyButtonPopUp->y() + MODIFY_BUTTON_POPUP_BUTTON_HEIGHT * 0.5)));
    modifyButtonPopUp->installEventFilter(innerWindowCEPCatchEvents);
}

void InnerWindowCEP::fluidTypeModifyButtonReleased(){
    QCursor c(cursor());
    modifyButtonPopUp = new ModifyButtonPopUp(this, SUBSTANCE_TYPE);
    static_cast<QFrame*>(modifyButtonPopUp)->setParent(this);
    modifyButtonPopUp->show();
    modifyButtonPopUp->setGeometry(fluidTypeModifyButton->x(), fluidTypeModifyButton->y() + ENTER_PARAMS_PANEL_BUTTONS_HEIGHT, MODIFY_BUTTON_POPUP_WIDTH, MODIFY_BUTTON_POPUP_HEIGHT);
    c.setPos(mapToGlobal(QPoint(modifyButtonPopUp->x() + MODIFY_BUTTON_POPUP_BUTTON_WIDTH * 0.5, modifyButtonPopUp->y() + MODIFY_BUTTON_POPUP_BUTTON_HEIGHT * 0.5)));
    modifyButtonPopUp->installEventFilter(innerWindowCEPCatchEvents);
}

bool InnerWindowCEP::substanceNameTextEditCheck(QKeyEvent *ev){
    switch(ev->key()){
    case Qt::Key_Enter:
        return true;
    case Qt::Key_Tab:
        return true;
    case Qt::Key_Slash:
        return true;
    case Qt::Key_Backslash:
        return true;
    case Qt::Key_Return:
        return true;
    case Qt::Key_Escape:
        return true;
    case Qt::Key_Colon:
        return true;
    case Qt::Key_Asterisk:
        return true;
    case Qt::Key_Less:
        return true;
    case Qt::Key_Greater:
        return true;
    case Qt::Key_QuoteDbl:
        return true;
    case Qt::Key_Question:
        return true;
    case Qt::Key_Bar:
        return true;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Space:
        substanceNameTextEdit->getDataUTextEdit()->insert(" ");
        substanceNameTextEdit->multiSpaceKeyDetection();
        return true;
    case Qt::Key_Backspace:
    {
        substanceNameTextEdit->getDataUTextEdit()->backspace();
        substanceNameTextEdit->multiSpaceKeyDetection();
    }
        return true;
    case Qt::Key_Delete:
    {
        substanceNameTextEdit->getDataUTextEdit()->del();
        substanceNameTextEdit->multiSpaceKeyDetection();
    }
        return true;
    default:
    {
        if(ev->count()){
            substanceNameTextEdit->getDataUTextEdit()->insert(ev->text());
            substanceNameTextEdit->multiSpaceKeyDetection();
        }else
            return false;
    }
        return true;
    }
return false;
}

void InnerWindowCEP::substanceTypeFluidCheckBoxReleased(){
    if(!substanceTypeFluidCheckBox->isChecked()){
        substanceTypeGaseCheckBox->setChecked(false);
        substanceTypeFluidCheckBox->setChecked(true);
        delete heatCapacityRatioLabel;
        heatCapacityRatioLabel = nullptr;
        delete heatCapacityRatioUnitLabel;
        heatCapacityRatioUnitLabel = nullptr;
        delete heatCapacityRatioFDF;
        heatCapacityRatioFDF = nullptr;
        delete compresibilityFactorLabel;
        compresibilityFactorLabel = nullptr;
        delete compresibilityFactorFDF;
        compresibilityFactorFDF = nullptr;
    }
}

void InnerWindowCEP::substanceTypeGaseCheckBoxReleased(){
    if(!substanceTypeGaseCheckBox->isChecked()){
        substanceTypeFluidCheckBox->setChecked(false);
        substanceTypeGaseCheckBox->setChecked(true);
        heatCapacityRatioLabel = new QLabel(this);
        heatCapacityRatioLabel->show();
        heatCapacityRatioUnitLabel = new QLabel(this);
        heatCapacityRatioUnitLabel->show();
        heatCapacityRatioLabel->setText(FLUID_PANEL_HEAT_CAPACITY_RATIO_LABEL_TEXT);
        heatCapacityRatioLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        heatCapacityRatioUnitLabel->setGeometry(2 * ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH + FLUID_PANEL_FDF_WIDTH + 5, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        heatCapacityRatioUnitLabel->setText(FLUID_PANEL_HEAT_CAPACITY_RATIO_UNIT_LABEL_TEXT);
        heatCapacityRatioFDF = new FormatableDataField<ProjectMenagerParam>(true,projectInfo->getFluidHeatCapacityRatio(), true, this, QString(), false);
        if(heatCapacityRatioFDF->getDataUTextEdit()->text().isEmpty())
            heatCapacityRatioFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
        heatCapacityRatioFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        heatCapacityRatioFDF->setGeometry(2* ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 6 * ENTER_PARAMS_VERTICAL_OFFSET + 5 * FLUID_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        heatCapacityRatioFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        heatCapacityRatioFDF->show();
        heatCapacityRatioFDF->installEventFilter(innerWindowCEPCatchEvents);
        heatCapacityRatioFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        compresibilityFactorLabel = new QLabel(this);
        compresibilityFactorLabel->setText(FLUID_PANEL_COMPRESIBILITY_FACTOR_TEXT);
        compresibilityFactorLabel->setGeometry(ENTER_PARAMS_HORIZONTAL_OFFSET, 7 * ENTER_PARAMS_VERTICAL_OFFSET + 6 * FLOW_PANEL_LABEL_HEIGHT, FLUID_PANEL_LABEL_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
        compresibilityFactorLabel->show();
        compresibilityFactorFDF = new FormatableDataField<ProjectMenagerParam>(true,projectInfo->getCompresibilityFactor(), true, this, QString(), false);
        if(compresibilityFactorFDF->getDataUTextEdit()->text().isEmpty())
            compresibilityFactorFDF->getDataUTextEdit()->setStyleSheet("border: 1px solid red;");
        compresibilityFactorFDF->getDataUTextEdit()->setContextMenuPolicy(Qt::NoContextMenu);
        compresibilityFactorFDF->setGeometry(2* ENTER_PARAMS_HORIZONTAL_OFFSET + FLUID_PANEL_LABEL_WIDTH, 7 * ENTER_PARAMS_VERTICAL_OFFSET + 6 * FLOW_PANEL_LABEL_HEIGHT, FLUID_PANEL_FDF_WIDTH, FLOW_PANEL_LABEL_HEIGHT);
        compresibilityFactorFDF->getDataUTextEdit()->setGeometry(0,0,FLUID_PANEL_TEXTEDIT_WIDTH, FLUID_PANEL_LABEL_HEIGHT);
        compresibilityFactorFDF->installEventFilter(innerWindowCEPCatchEvents);
        compresibilityFactorFDF->getDataUTextEdit()->installEventFilter(innerWindowCEPCatchEvents);
        compresibilityFactorFDF->show();
    }
 }

bool InnerWindowCEP::workDensityFDFCheck(QKeyEvent *ev){
    QLineEdit *te = workDensityFDF->getDataUTextEdit();
    uint i = 0;
    uint len = te->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    // Połączenie bezpośrednie
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(te->text().at(i) == '.')
                return true;
            else{
                if(te->text().at(i) == 'e' && te->cursorPosition() >= i)
                    return true;
            }
        te->insert(".");
       }
        break;
    case Qt::Key_Delete:
    {
        reactOnWorkDensityFDFBackspaceDeleteAction(true);
    }
        return true;
    case Qt::Key_Backspace:
    {
       reactOnWorkDensityFDFBackspaceDeleteAction(false);
    }
        return true;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Return:
        return true;
    case Qt::Key_Escape:
        return true;
    case Qt::Key_Minus:
    {
        int selStart = te->selectionStart();
        if(selStart > 0 ){
            if(te->text().at(selStart - 1) == 'e')
                te->insert("-");
        }else{
            int curPos = te->cursorPosition();
            if(selStart == -1 && curPos > 0 && te->text().at(curPos - 1) == 'e' && (curPos == len || te->text().at(curPos) != '-'))
                te->insert("-");
        }
    }
        break;
    case Qt::Key_E:
        for( ; i < len; i++)
            if(te->text().at(i) == 'e')
                return true;
        te->insert(ev->text().toLower());
        break;
    default:
    {
        if(ev->key() >= 0x30 && ev->key() <= 0x39)
            te->insert(ev->text());
    }
   }
    if(!FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), false)){
        te->setStyleSheet("QLineEdit{border: 1px solid red;}");
        parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DENSITY, workDensityFDF);
    }else{
        if(*projectInfo->getFluidType() != customDataPos){
            if(!projectInfo->getFluidDensityFromFile() || !App::compareStr(te->text(), projectInfo->getFluidDensityFromFile()->useStringFormat()))
            {
                projectInfo->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
                te->setStyleSheet("QLineEdit{border: 1px solid orange;}");
                parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DENSITY, workDensityFDF);
            }else{
                projectInfo->setFluidDensityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(false));
                te->setStyleSheet("");
                 parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, workDensityFDF);
            }
        }else{
            te->setStyleSheet("");
        }
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(workDensityFDF->getFormatableData()){
              workDensityFDF->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_DENSITY, false);
                workDensityFDF->setFormatableData(tempFD);
                delete tempFD;
       }
    }
    return true;
}

void InnerWindowCEP::reactOnWorkDensityFDFBackspaceDeleteAction(bool deleteAction){
    QLineEdit *te = workDensityFDF->getDataUTextEdit();
    uint len = te->text().length();
    // PLACEHOLDER -- Jeśli textEdit jest pusty, wstawić wartość wprowadzoną przez użytkownika, jeśli została zapisana
    if(len == 0)
    {
        if(projectInfo->getFluidDensity())
        {
            te->setText(projectInfo->getFluidDensity()->useStringFormat());
            if(!projectInfo->getFluidDensityFromFile() || !App::compareStr(te->text(), projectInfo->getFluidDensityFromFile()->useStringFormat())){
                projectInfo->setFluidDensityCustom([]()->bool*{bool *ret = new bool; *ret = true; return ret;}());
                te->setStyleSheet("QLineEdit{border: 1px solid orange;}");
                parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DENSITY, workDensityFDF);
            }else{
                projectInfo->setFluidDensityCustom([]()->bool*{bool *ret = new bool; *ret = false; return ret;}());
                te->setStyleSheet("");
                 parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, workDensityFDF);
            }
            FormatableData<ProjectMenagerParam> *tempFD = nullptr;
              if(workDensityFDF->getFormatableData()){
                  workDensityFDF->getFormatableData()->setData(te->text());
              }else{
                    tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_DENSITY, false);
                    workDensityFDF->setFormatableData(tempFD);
                    delete tempFD;
           }
        }else{
            if(projectInfo->getFluidDensityFromFile()){
                te->setText(projectInfo->getFluidDensityFromFile()->useStringFormat());
                projectInfo->setFluidDensityCustom([]()->bool*{bool *ret = new bool; *ret = false; return ret;}());
                te->setStyleSheet("");
                 parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, workDensityFDF);
                FormatableData<ProjectMenagerParam> *tempFD = nullptr;
                  if(workDensityFDF->getFormatableData()){
                      workDensityFDF->getFormatableData()->setData(te->text());
                  }else{
                        tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_DENSITY, false);
                        workDensityFDF->setFormatableData(tempFD);
                        delete tempFD;
               }
            }
        }
        return;
    }
    // END PLACEHOLDER ----------------------------------------------------------------------------------------------
    if(deleteAction)
        te->del();
    else
        te->backspace();
    if(!FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), false)){
        te->setStyleSheet("QLineEdit{border: 1px solid red;}");
    parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DENSITY, workDensityFDF);
}
    else{
        if(*projectInfo->getFluidType() != customDataPos){
            if(!projectInfo->getFluidDensityFromFile() || !App::compareStr(te->text(), projectInfo->getFluidDensityFromFile()->useStringFormat())){
                projectInfo->setFluidDensityCustom([]()->bool*{bool *ret = new bool; *ret = true; return ret;}());
                te->setStyleSheet("QLineEdit{border: 1px solid orange;}");
                parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DENSITY, workDensityFDF);
            }else{
                projectInfo->setFluidDensityCustom([]()->bool*{bool *ret = new bool; *ret = false; return ret;}());
                te->setStyleSheet("");
                 parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, workDensityFDF);
            }
        }else{
            te->setStyleSheet("");
        }
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(workDensityFDF->getFormatableData()){
              workDensityFDF->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_DENSITY, false);
                workDensityFDF->setFormatableData(tempFD);
                delete tempFD;
       }
    }
}

bool InnerWindowCEP::dynamicViscosityFDFCheck(QKeyEvent *ev){
    QLineEdit *te = dynamicViscosityFDF->getDataUTextEdit();
    uint i = 0;
    uint len = te->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    // Połączenie bezpośrednie
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(te->text().at(i) == '.')
                return true;
            else{
                if(te->text().at(i) == 'e' && te->cursorPosition() >= i)
                    return true;
            }
        te->insert(".");
    }
        break;
    case Qt::Key_Delete:
        reactOnDynamicViscosityFDFBackspaceDeleteAction(true);
        return true;
    case Qt::Key_Backspace:
        reactOnDynamicViscosityFDFBackspaceDeleteAction(false);
        return true;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Return:
        return true;
    case Qt::Key_Escape:
        return true;
    case Qt::Key_Minus:
    {
        int selStart = te->selectionStart();
        if(selStart > 0 ){
            if(te->text().at(selStart - 1) == 'e')
                te->insert("-");
        }else{
            int curPos = te->cursorPosition();
            if(selStart == -1 && curPos > 0 && te->text().at(curPos - 1) == 'e' && (curPos == len || te->text().at(curPos) != '-'))
                te->insert("-");
        }
    }
        break;
    case Qt::Key_E:
        for( ; i < len; i++)
            if(te->text().at(i) == 'e')
                return true;
        te->insert(ev->text().toLower());
        break;
    default:
    {
        if(ev->key() >= 0x30 && ev->key() <= 0x39)
            te->insert(ev->text());
    }
  }
    if(!FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), false)){
        te->setStyleSheet("QLineEdit{border: 1px solid red;}");
        parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, dynamicViscosityFDF);
    }else{
        if(*projectInfo->getFluidType() != customDataPos){
            if(!projectInfo->getFluidDynamicViscosityFromFile() || !App::compareStr(te->text(), projectInfo->getFluidDynamicViscosityFromFile()->useStringFormat()))
            {
                projectInfo->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(true));
                te->setStyleSheet("QLineEdit{border: 1px solid orange;}");
                parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, dynamicViscosityFDF);
            }else{
                projectInfo->setFluidDynamicViscosityCustom([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(false));
                te->setStyleSheet("");
                 parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, dynamicViscosityFDF);
            }
        }else{
            te->setStyleSheet("");
        }
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(dynamicViscosityFDF->getFormatableData()){
              dynamicViscosityFDF->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, false);
                dynamicViscosityFDF->setFormatableData(tempFD);
                delete tempFD;
       }
    }
    return true;
}

void InnerWindowCEP::reactOnDynamicViscosityFDFBackspaceDeleteAction(bool deleteAction){
    QLineEdit *te = dynamicViscosityFDF->getDataUTextEdit();
    uint len = te->text().length();
    // PLACEHOLDER -- Jeśli textEdit jest pusty, wstawić wartość wprowadzoną przez użytkownika, jeśli została zapisana
    if(len == 0)
    {
        if(projectInfo->getFluidDynamicViscosity())
        {
            te->setText(projectInfo->getFluidDynamicViscosity()->useStringFormat());
            if(!projectInfo->getFluidDynamicViscosityFromFile() || !App::compareStr(te->text(), projectInfo->getFluidDynamicViscosityFromFile()->useStringFormat())){
                projectInfo->setFluidDynamicViscosityCustom([]()->bool*{bool *ret = new bool; *ret = true; return ret;}());
                te->setStyleSheet("QLineEdit{border: 1px solid orange;}");
                parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, dynamicViscosityFDF);
            }else{
                projectInfo->setFluidDynamicViscosityCustom([]()->bool*{bool *ret = new bool; *ret = false; return ret;}());
                te->setStyleSheet("");
                 parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, dynamicViscosityFDF);
            }
            FormatableData<ProjectMenagerParam> *tempFD = nullptr;
              if(dynamicViscosityFDF->getFormatableData()){
                  dynamicViscosityFDF->getFormatableData()->setData(te->text());
              }else{
                    tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, false);
                    dynamicViscosityFDF->setFormatableData(tempFD);
                    delete tempFD;
           }
        }else{
            if(projectInfo->getFluidDynamicViscosityFromFile()){
                te->setText(projectInfo->getFluidDynamicViscosityFromFile()->useStringFormat());
                projectInfo->setFluidDynamicViscosityCustom([]()->bool*{bool *ret = new bool; *ret = false; return ret;}());
                te->setStyleSheet("");
                 parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, dynamicViscosityFDF);
                FormatableData<ProjectMenagerParam> *tempFD = nullptr;
                  if(dynamicViscosityFDF->getFormatableData()){
                      dynamicViscosityFDF->getFormatableData()->setData(te->text());
                  }else{
                        tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, false);
                        dynamicViscosityFDF->setFormatableData(tempFD);
                        delete tempFD;
               }
            }
        }

        return;
    }
    // END PLACEHOLDER ----------------------------------------------------------------------------------------------
    if(deleteAction)
        te->del();
    else
        te->backspace();
    if(!FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), false)){
        te->setStyleSheet("QLineEdit{border: 1px solid red;}");
        parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, dynamicViscosityFDF);
    }
    else{
        if(*projectInfo->getFluidType() != customDataPos){
            if(!projectInfo->getFluidDynamicViscosityFromFile() || !App::compareStr(te->text(), projectInfo->getFluidDynamicViscosityFromFile()->useStringFormat())){
                projectInfo->setFluidDynamicViscosityCustom([]()->bool*{bool *ret = new bool; *ret = true; return ret;}());
                te->setStyleSheet("QLineEdit{border: 1px solid orange;}");
                parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, dynamicViscosityFDF);
            }else{
                projectInfo->setFluidDynamicViscosityCustom([]()->bool*{bool *ret = new bool; *ret = false; return ret;}());
                te->setStyleSheet("");
                 parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, dynamicViscosityFDF);
            }
        }else{
            te->setStyleSheet("");
             parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE, projectInfo, dynamicViscosityFDF);
        }
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(dynamicViscosityFDF->getFormatableData()){
              dynamicViscosityFDF->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, false);
                dynamicViscosityFDF->setFormatableData(tempFD);
                delete tempFD;
       }
    }
}

bool InnerWindowCEP::heatCapacityRatioFDFCheck(QKeyEvent *ev){
    QLineEdit *te = heatCapacityRatioFDF->getDataUTextEdit();
    uint i = 0;
    uint len = te->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    // Połączenie bezpośrednie
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(te->text().at(i) == '.')
                return true;
            else{
                if(te->text().at(i) == 'e' && te->cursorPosition() >= i)
                    return true;
            }
        te->insert(".");
    }
        break;
    case Qt::Key_Delete:
        if(len == 0){
            if(projectInfo->getFluidHeatCapacityRatio()){
                te->setText(projectInfo->getFluidHeatCapacityRatio()->useStringFormat());
                te->setStyleSheet("");
            }
        }else
            te->del();
        break;
    case Qt::Key_Backspace:
        if(len == 0){
            if(projectInfo->getFluidHeatCapacityRatio()){
                te->setText(projectInfo->getFluidHeatCapacityRatio()->useStringFormat());
                te->setStyleSheet("");
            }
        }
        else
            te->backspace();
        break;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Return:
        return true;
    case Qt::Key_Escape:
       return true;
    case Qt::Key_Minus:
    {
        int selStart = te->selectionStart();
        if(selStart > 0 ){
            if(te->text().at(selStart - 1) == 'e')
                te->insert("-");
        }else{
            int curPos = te->cursorPosition();
            if(selStart == -1 && curPos > 0 && te->text().at(curPos - 1) == 'e' && (curPos == len || te->text().at(curPos) != '-'))
                te->insert("-");
        }
    }
        break;
    case Qt::Key_E:
        for( ; i < len; i++)
            if(te->text().at(i) == 'e')
                return true;
        te->insert(ev->text().toLower());
        break;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39)
            te->insert(ev->text());
        else
            return true;

    }
    if(!FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), false))
        te->setStyleSheet("QLineEdit{border: 1px solid red;}");
    else{
        te->setStyleSheet("");
        te->setStyleSheet("");
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(heatCapacityRatioFDF->getFormatableData()){
              heatCapacityRatioFDF->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO, false);
                heatCapacityRatioFDF->setFormatableData(tempFD);
                delete tempFD;
       }
    }
    return true;
}

bool InnerWindowCEP::compresibilityFactorFDFCheck(QKeyEvent *ev){
    QLineEdit *te = compresibilityFactorFDF->getDataUTextEdit();
    uint i = 0;
    uint len = te->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    // Połaczenie bezpośrednie
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(te->text().at(i) == '.')
                return true;
            else{
                if(te->text().at(i) == 'e' && te->cursorPosition() >= i)
                    return true;
            }
        te->insert(".");
    }
        break;
    case Qt::Key_Delete:
        if(len == 0){
            if(projectInfo->getCompresibilityFactor()){
                te->setText(projectInfo->getCompresibilityFactor()->useStringFormat());
                te->setStyleSheet("");
            }
        }else
            te->del();
        break;
    case Qt::Key_Backspace:
        if(len == 0){
            if(projectInfo->getCompresibilityFactor()){
                te->setText(projectInfo->getCompresibilityFactor()->useStringFormat());
                te->setStyleSheet("");
            }
        }
        else
            te->backspace();
        break;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Return:
        return true;
    case Qt::Key_Escape:
       return true;
    case Qt::Key_Minus:
    {
        int selStart = te->selectionStart();
        if(selStart > 0 ){
            if(te->text().at(selStart - 1) == 'e')
                te->insert("-");
        }else{
            int curPos = te->cursorPosition();
            if(selStart == -1 && curPos > 0 && te->text().at(curPos - 1) == 'e' && (curPos == len || te->text().at(curPos) != '-'))
                te->insert("-");
        }
    }
        break;
    case Qt::Key_E:
        for( ; i < len; i++)
            if(te->text().at(i) == 'e')
                return true;
        te->insert(ev->text().toLower());
        break;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39)
            te->insert(ev->text());
        else
            return true;
    }
    if(!FormatableData<ProjectMenagerParam>::checkStringFormat(te->text(), false))
        te->setStyleSheet("QLineEdit{border: 1px solid red;}");
    else{
        te->setStyleSheet("");
        FormatableData<ProjectMenagerParam> *tempFD = nullptr;
          if(compresibilityFactorFDF->getFormatableData()){
              compresibilityFactorFDF->getFormatableData()->setData(te->text());
          }else{
                tempFD = new FormatableData<ProjectMenagerParam>(te->text(), PROJECT_MENAGER_COMPRESIBILITY_FACTOR, false);
                compresibilityFactorFDF->setFormatableData(tempFD);
                delete tempFD;
       }
    }
    return true;
}

InnerWinStatusCEP* InnerWindowCEP::getInnerWinStatusCEP(){
    return innerWinStatusCEP;
}

ProjectInfo* InnerWindowCEP::getProjectInfo(){
    return projectInfo;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getPressuteTapDiameterFDF(){
    return pressureTapDiameterFDF;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getMaxDiffPressureFDF(){
    return maxDiffPressureFDF;
}

QLabel* InnerWindowCEP::getMeasurementDeviceMaterialModifyButton(){
    return measurementDeviceMaterialModifyButton;
}

QComboBox* InnerWindowCEP::getMeasurementDeviceMaterialList(){
    return measurementDeviceMaterialList;
}

QComboBox* InnerWindowCEP::getMeasurementTypeList(){
    return measurementTypeList;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getPipeDiameterFDF(){
    return pipeDiameterFDF;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getMaxUpstreamPressureFDF(){
    return maxUpstreamPressureFDF;
}

QComboBox* InnerWindowCEP::getMaxUpstreamPressureUnitList(){
    return maxUpstreamPressureUnitList;
}

QLabel* InnerWindowCEP::getPipeMaterialModifyButton(){
    return pipeMaterialModifyButton;
}

QComboBox* InnerWindowCEP::getPipeMaterialList(){
    return pipeMaterialList;
}

QComboBox* InnerWindowCEP::getUpstreamDeviceList(){
    return upstreamDeviceList;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getMaxFlowValueFDF(){
    return maxFlowValueFDF;
}

QComboBox* InnerWindowCEP::getMaxFlowValueUnitList(){
    return maxFlowValueUnitList;
}

QComboBox* InnerWindowCEP::getTempUnitList(){
    return tempUnitList;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getFlowFDF(){
    return flowFDF;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getTempFDF(){
    return tempFDF;
}

QLabel* InnerWindowCEP::getSubstanceNameDataLabel(){
    return substanceNameDataLabel;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getSubstanceNameTextEdit(){
    return substanceNameTextEdit;
}

QLabel* InnerWindowCEP::getSubstanceTypeDataLabel(){
    return substanceTypeDataLabel;
}

QCheckBox* InnerWindowCEP::getSubstanceTypeFluidCheckBox(){
    return substanceTypeFluidCheckBox;
}

QCheckBox* InnerWindowCEP::getSubstanceTypeGaseCheckBox(){
    return substanceTypeGaseCheckBox;
}

uint InnerWindowCEP::getCustomDataPos(){
    return customDataPos;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getWorkDensityFDF(){
    return workDensityFDF;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getDynamicViscosityFDF(){
    return dynamicViscosityFDF;
}


FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getHeatCapacityRatioFDF(){
    return heatCapacityRatioFDF;
}

QLabel* InnerWindowCEP::getFluidTypeModifyButton(){
    return fluidTypeModifyButton;
}

AppWindow* InnerWindowCEP::getParent(){
    return parent;
}

ResultsTabBar* InnerWindowCEP::getResultsTabBar(){
    return resultsTabBar;
}

void InnerWindowCEP::setInnerWinStatusCEP(InnerWinStatusCEP winStatus){
    WINDOW_STATUS_SAVE(innerWinStatusCEP)
    *innerWinStatusCEP = winStatus;
    innerWinStatusCEPChanged = true;
}

void InnerWindowCEP::setModifyButtonPopUpNullPtr(){
    modifyButtonPopUp = nullptr;
}

InnerWindowCEPCatchEvents* InnerWindowCEP::getInnerWindowCEPCatchEvents(){
    return innerWindowCEPCatchEvents;
}

ResultsGraphicPage* InnerWindowCEP::getResultsGraphicPage(){
    return resultsGraphicPage;
}

ModifyButtonPopUp* InnerWindowCEP::getModifyButtonPopUp(){
    return modifyButtonPopUp;
}

ResultsBasicDataPage* InnerWindowCEP::getResultsBasicDataPage(){
    return resultsBasicDataPage;
}

ResultsAdditionDataPage* InnerWindowCEP::getResultsAdditionDataPage(){
    return resultsAdditionDataPage;
}

ResultsRaportPage* InnerWindowCEP::getResultsRaportPage(){
    return resultsRaportPage;
}

FormatableDataField<ProjectMenagerParam>* InnerWindowCEP::getCompresibilityFactorFDF(){
    return compresibilityFactorFDF;
}

void InnerWindowCEP::reloadTimerTask(){
  if(innerWinStatusCEPChanged)
      reload();
  if(*innerWinStatusCEP == FLOW && fluidTypeList!= nullptr && maxFlowValueUnitList != nullptr)
  {
      if((uint)fluidTypeList->currentIndex() == customDataPos && projectInfo->getOtherSubstanceStat() != nullptr && App::compareStr(*projectInfo->getOtherSubstanceStat(), "Gaz")){
          if(maxFlowValueUnitList->count() != 4){
              maxFlowValueUnitList->addItem("Nm^3/h");
          }
      }else{
          if(maxFlowValueUnitList->count() == 4){
              if(maxFlowValueUnitList->currentIndex() == 3)
                  maxFlowValueUnitList->setCurrentIndex(0);
              maxFlowValueUnitList->removeItem(3);
          }
      }
  }
}

void InnerWindowCEP::reloadList(){
    switch(*innerWinStatusCEP){
    case FLANGE:
        measurementDeviceMaterialList->clear();
        measurementDeviceMaterialList->addItem("Stal kwasoodporna 1.4301");
        measurementDeviceMaterialList->addItem("Stal węglowa");
        measurementDeviceMaterialList->addItem("Stal chromowa");
        measurementDeviceMaterialList->addItem("Stop Monela");
        measurementDeviceMaterialList->addItem("Brąz");
        measurementDeviceMaterialList->addItem("Aluminium");
        if(!MaterialsInfoFiles::readFlangeMaterials(this)){
            for(uint i = measurementDeviceMaterialList->count() - 1; i > 5; i--)
                measurementDeviceMaterialList->removeItem(i);
        }
        break;
    case PIPE:
        pipeMaterialList->clear();
        pipeMaterialList->addItem("Stal kwasoodporna 1.4301");
        pipeMaterialList->addItem("Stal węglowa");
        pipeMaterialList->addItem("Stal chromowa");
        pipeMaterialList->addItem("Stop Monela");
        pipeMaterialList->addItem("Brąz");
        pipeMaterialList->addItem("Aluminium");
        if(!MaterialsInfoFiles::readPipeMaterials(this)){
            for(uint i = pipeMaterialList->count() - 1; i > 5; i--)
                pipeMaterialList->removeItem(i);
        }
        break;
    case FLOW:
        fluidTypeList->clear();
        fluidTypeList->addItem("Woda");
        fluidTypeList->addItem("Para wodna");
        fluidTypeList->addItem("Powietrze");
        fluidTypeList->addItem("Dwutlenek węgla");
        fluidTypeList->addItem("Wodór");
        fluidTypeList->addItem("Azot");
        if(!MaterialsInfoFiles::readSubstances(this)){
            for(uint i = fluidTypeList->count() - 1; i > 5; i--)
                fluidTypeList->removeItem(i);
        }
        break;
    default:
        break;
    }
}

void InnerWindowCEP::saveFromFlangeData(){
    parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DATA_NOT_SAVED);
    projectInfo->setMeasurementDevice([](uint set)->uint*{uint *ret = new uint; *ret = set; return ret;}((uint)measurementDeviceMaterialList->currentIndex()));
    projectInfo->setMeasurementType([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}((uint)measurementTypeList->currentIndex()));
    projectInfo->setFlangeMaterialName(measurementDeviceMaterialList->currentText());
    if(FormatableData<ProjectMenagerParam>::checkStringFormat(pressureTapDiameterFDF->getDataUTextEdit()->text(), false))
        projectInfo->setPressureTapDiameter(new FormatableData<ProjectMenagerParam>(*pressureTapDiameterFDF->getFormatableData()));
    else{
        if(pressureTapDiameterFDF->getDataUTextEdit()->text().isEmpty())
            projectInfo->setPressureTapDiameter(nullptr);
        else
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_PRESSURE_TAP_DIAMETER, pressureTapDiameterFDF);
    }
    if(FormatableData<ProjectMenagerParam>::checkStringFormat(maxDiffPressureFDF->getDataUTextEdit()->text(), false))
        projectInfo->setMaxDiffPressure(new FormatableData<ProjectMenagerParam>(*maxDiffPressureFDF->getFormatableData()));
    else{
        if(maxDiffPressureFDF->getDataUTextEdit()->text().isEmpty())
            projectInfo->setMaxDiffPressure(nullptr);
        else
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_MAX_DIFF_PRESSURE, maxDiffPressureFDF);
    }
    projectInfo->setMaxDiffPressureUnits([](ushort set)->ushort*{ushort* ret = new ushort; *ret = set; return ret;}((uint)maxDiffPressureUnitList->currentIndex()));
}

void InnerWindowCEP::saveFromPipeData(){
    parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DATA_NOT_SAVED);
    projectInfo->setPipeMaterial([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}((uint)pipeMaterialList->currentIndex()));
    projectInfo->setPipeMaterialName(pipeMaterialList->currentText());
    projectInfo->setUpstreamDevice([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}((uint)upstreamDeviceList->currentIndex()));
    if(FormatableData<ProjectMenagerParam>::checkStringFormat(maxUpstreamPressureFDF->getDataUTextEdit()->text(),false))
       projectInfo->setMaxUpstreamPressure(new FormatableData<ProjectMenagerParam>(*maxUpstreamPressureFDF->getFormatableData()));
    else{
        if(maxUpstreamPressureFDF->getDataUTextEdit()->text().isEmpty())
            projectInfo->setMaxUpstreamPressure(nullptr);
        else
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE, maxUpstreamPressureFDF);
    }
    if(FormatableData<ProjectMenagerParam>::checkStringFormat(pipeDiameterFDF->getDataUTextEdit()->text(), false))
       projectInfo->setPipeDiameter(new FormatableData<ProjectMenagerParam>(*pipeDiameterFDF->getFormatableData()));
    else{
        if(pipeDiameterFDF->getDataUTextEdit()->text().isEmpty())
            projectInfo->setPipeDiameter(nullptr);
        else
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_PIPE_DIAMETER, pipeDiameterFDF);
    }
    projectInfo->setMaxUpstreamPressureUnits([](ushort set)->ushort*{ushort* ret = new ushort; *ret = set; return ret;}((ushort)maxUpstreamPressureUnitList->currentIndex()));
}

void InnerWindowCEP::saveFromFlowData(){
    parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DATA_NOT_SAVED);
    if(*projectInfo->getFluidType() != (uint)fluidTypeList->currentIndex() && (uint)fluidTypeList->currentIndex() == customDataPos ){
        projectInfo->setFluidName(nullptr);
        projectInfo->setFluidType(nullptr);
    }
    projectInfo->setFluidType([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}((uint)fluidTypeList->currentIndex()));
    customDataPos = (uint)fluidTypeList->count() - 1;
    if(FormatableData<ProjectMenagerParam>::checkStringFormat(maxFlowValueFDF->getDataUTextEdit()->text(), false))
        projectInfo->setMaxFlowValue(new FormatableData<ProjectMenagerParam>(*maxFlowValueFDF->getFormatableData()));
    else{
        if(maxFlowValueFDF->getDataUTextEdit()->text().isEmpty())
            projectInfo->setMaxFlowValue(nullptr);
        else
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_MAX_FLOW_VALUE, maxFlowValueFDF);
    }
    if(FormatableData<ProjectMenagerParam>::checkStringFormat(flowFDF->getDataUTextEdit()->text(), false))
        projectInfo->setFlow(new FormatableData<ProjectMenagerParam>(*flowFDF->getFormatableData()));
    else{
        if(flowFDF->getDataUTextEdit()->text().isEmpty())
            projectInfo->setFlow(nullptr);
        else
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_FLOW, flowFDF);
    }
    if(FormatableData<ProjectMenagerParam>::checkStringFormat(tempFDF->getDataUTextEdit()->text(), true))
        projectInfo->setTemp(new FormatableData<ProjectMenagerParam>(*tempFDF->getFormatableData()));
    else{
        if(tempFDF->getDataUTextEdit()->text().isEmpty())
            projectInfo->setTemp(nullptr);
        else
            parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_TEMP, tempFDF);
    }
    projectInfo->setFlowUnits([](ushort set)->ushort*{ushort* ret = new ushort; *ret = set; return ret;}((ushort)maxFlowValueUnitList->currentIndex()));
    projectInfo->setTempUnits([](ushort set)->ushort*{ushort* ret = new ushort; *ret = set; return ret;}((ushort)tempUnitList->currentIndex()));
    projectInfo->setDoubleSideFlow([](bool set)->bool*{bool *ret = new bool; *ret = set; return ret;}(doubleSideFlowCheckBox->isChecked()));
}

void InnerWindowCEP::saveFromFluidData(){
     parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DATA_NOT_SAVED);
     parent->getPromptPanel()->removePrompt(PROMPT_TYPE_DISPLAY_DATA_AVAILABLE);
    if(*projectInfo->getFluidType() != customDataPos){
        projectInfo->setFluidName(new QString(substanceNameDataLabel->text()));
        projectInfo->setSubstanceStat(new QString(substanceTypeDataLabel->text()));
        if(*projectInfo->getFluidDensityCustom()){
            if(FormatableData<ProjectMenagerParam>::checkStringFormat(workDensityFDF->getDataUTextEdit()->text(), false))
                projectInfo->setFluidDensity(new FormatableData<ProjectMenagerParam>(*workDensityFDF->getFormatableData()));
            else{
                if(workDensityFDF->getDataUTextEdit()->text().isEmpty()){
                    projectInfo->setFluidDensity(nullptr);
                    projectInfo->setFluidDensityCustom(new bool(false));
                }
                else{
                    if(!projectInfo->getFluidDensity())
                        projectInfo->setFluidDensityCustom(new bool(false));
                    parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_WORK_DENSITY, workDensityFDF);
                }
            }
        }
        if(*projectInfo->getFluidDynamicViscosityCustom()){
            if(FormatableData<ProjectMenagerParam>::checkStringFormat(dynamicViscosityFDF->getDataUTextEdit()->text(), false))
                projectInfo->setFluidDynamicViscosity(new FormatableData<ProjectMenagerParam>(*dynamicViscosityFDF->getFormatableData()));
            else{
                if(dynamicViscosityFDF->getDataUTextEdit()->text().isEmpty()){
                    projectInfo->setFluidDynamicViscosity(nullptr);
                    projectInfo->setFluidDynamicViscosityCustom(new bool(false));
                }else{
                    if(!projectInfo->getFluidDynamicViscosity())
                        projectInfo->setFluidDynamicViscosityCustom(new bool(false));
                    parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, dynamicViscosityFDF);
                }
            }
        }
        if(heatCapacityRatioFDF)
        {
            if(FormatableData<ProjectMenagerParam>::checkStringFormat(heatCapacityRatioFDF->getDataUTextEdit()->text(), false))
                projectInfo->setFluidHeatCapacityRatio(new FormatableData<ProjectMenagerParam>(*heatCapacityRatioFDF->getFormatableData()));
            else{
                if(heatCapacityRatioFDF->getDataUTextEdit()->text().isEmpty())
                    projectInfo->setFluidHeatCapacityRatio(nullptr);
                else
                    parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO, heatCapacityRatioFDF);
            }
        }
    }else{
        projectInfo->setOtherSubstanceName(new QString(substanceNameTextEdit->getDataUTextEdit()->text()));
        projectInfo->setOtherSubstanceStat(new QString((substanceTypeFluidCheckBox->isChecked())? "Ciecz" : "Gaz"));
        if(FormatableData<ProjectMenagerParam>::checkStringFormat(workDensityFDF->getDataUTextEdit()->text(), false))
            projectInfo->setFluidDensity(new FormatableData<ProjectMenagerParam>(*workDensityFDF->getFormatableData()));
        else{
            if(workDensityFDF->getDataUTextEdit()->text().isEmpty())
                projectInfo->setFluidDensity(nullptr);
            else
                parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_WORK_DENSITY, workDensityFDF);
        }
        if(FormatableData<ProjectMenagerParam>::checkStringFormat(dynamicViscosityFDF->getDataUTextEdit()->text(), false))
            projectInfo->setFluidDynamicViscosity(new FormatableData<ProjectMenagerParam>(*dynamicViscosityFDF->getFormatableData()));
        else{
            if(dynamicViscosityFDF->getDataUTextEdit()->text().isEmpty())
                projectInfo->setFluidDynamicViscosity(nullptr);
            else
                parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY, dynamicViscosityFDF);
        }
        if(heatCapacityRatioFDF){
            if(FormatableData<ProjectMenagerParam>::checkStringFormat(heatCapacityRatioFDF->getDataUTextEdit()->text(), false))
                projectInfo->setFluidHeatCapacityRatio(new FormatableData<ProjectMenagerParam>(*heatCapacityRatioFDF->getFormatableData()));
            else{
                if(heatCapacityRatioFDF->getDataUTextEdit()->text().isEmpty())
                    projectInfo->setFluidHeatCapacityRatio(nullptr);
                else
                    parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO, heatCapacityRatioFDF);
            }
        }
        if(compresibilityFactorFDF){
            if(FormatableData<ProjectMenagerParam>::checkStringFormat(compresibilityFactorFDF->getDataUTextEdit()->text(), false))
                projectInfo->setCompresibilityFactor(new FormatableData<ProjectMenagerParam>(*compresibilityFactorFDF->getFormatableData()));
            else{
                if(compresibilityFactorFDF->getDataUTextEdit()->text().isEmpty())
                    projectInfo->setCompresibilityFactor(nullptr);
                else
                    parent->getPromptPanel()->addPrompt(PROMPT_TYPE_DATA_NOT_SAVED, projectInfo, PROJECT_MENAGER_COMPRESIBILITY_FACTOR, compresibilityFactorFDF);
            }
        }
    }
}

void InnerWindowCEP::createMaterialChangesWidget(MaterialsChangesWidgetStats stat, MaterialsChangesWidgetType type){
    switch(type){
    case FLANGE_MATERIAL_TYPE:
    {
        MaterialsChangesWidget<FlangeMaterialsFileParams> *mcw = new MaterialsChangesWidget<FlangeMaterialsFileParams>(stat, type, parent->getApplication()->getConfiguration());
        materialsChangesWidgetPtr = (void*)(mcw);
        mcw->installEventFilter(innerWindowCEPCatchEvents);
        mcw->setParent(this, Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
        mcw->setWindowModality(Qt::WindowModal);
        mcw->show();
    }
        break;
    case PIPE_MATERIAL_TYPE:
    {
        MaterialsChangesWidget<PipeMaterialsFileParams> *mcw = new MaterialsChangesWidget<PipeMaterialsFileParams>(stat, type, parent->getApplication()->getConfiguration());
        materialsChangesWidgetPtr = (void*)(mcw);
        mcw->installEventFilter(innerWindowCEPCatchEvents);
        mcw->setParent(this, Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
        mcw->setWindowModality(Qt::WindowModal);
        mcw->show();
    }
    break;
    case SUBSTANCE_TYPE:
    {
        MaterialsChangesWidget<SubstancesParamsFileParam> *mcw =  new MaterialsChangesWidget<SubstancesParamsFileParam>(stat, type, parent->getApplication()->getConfiguration());
        materialsChangesWidgetPtr = (void*)(mcw);
        mcw->installEventFilter(innerWindowCEPCatchEvents);
        mcw->setParent(this, Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
        mcw->setWindowModality(Qt::WindowModal);
        mcw->show();
    }
    break;
    }
}

void InnerWindowCEP::setDestroyed(bool set){
    destroyed = set;
}

bool InnerWindowCEP::getDestroyed(){
    return destroyed;
}

QComboBox* InnerWindowCEP::getFluidTypeList(){
    return fluidTypeList;
}

void* InnerWindowCEP::getMaterialsChangesWidgetPtr(){
    return materialsChangesWidgetPtr;
}
