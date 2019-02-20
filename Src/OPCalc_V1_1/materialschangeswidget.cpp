#include"materialschangeswidget.hpp"
#include"mydialog.hpp"
#include<QKeyEvent>
#include"materialschangeswidgetdataholder.hpp"
#include<QTimer>
#include"windowmacros.hpp"
#include<QListWidget>
#include<QCheckBox>
#include<QRadioButton>
#include<QButtonGroup>
#include"materialschangeswidgetarray.hpp"
#include"mylist.hpp"
#include"mainmacros.hpp"
#include<QScrollArea>
#include"materialsinfofiles.hpp"
#include"config.hpp"
#include"appclass.hpp"
#include"appwindow.hpp"
#include"materialschangeswidgetcatchevents.hpp"
#include"formatabledatafield.hpp"
#include"formatabledata.hpp"
#include<QScrollBar>
#include<QComboBox>
#include"substancefileerrorslist.hpp"

template <typename ParamName>
MaterialsChangesWidget<ParamName>::MaterialsChangesWidget(MaterialsChangesWidgetStats stat, MaterialsChangesWidgetType setType, Config* setCnf){
    materialsChangesWidgetStats = new MaterialsChangesWidgetStats[2];
    config = setCnf;
    MaterialsInfoFiles::rewriteSubstancesPaths(config, false);
    *materialsChangesWidgetStats = stat;
    *(materialsChangesWidgetStats + 1) = stat;
    type = setType;
    materialsChangesWidgetCatchEvents = new MaterialsChangesWidgetCatchEvents<ParamName>(this);
    if(type == SUBSTANCE_TYPE){
        densityDataHolder = new MaterialsChangesWidgetDataHolder(0, 2, 0);
        dynamicViscosityDataHolder = new MaterialsChangesWidgetDataHolder(0, 2, 0);
    }
    init();
    setWindow();
    createWidgets();
    createLayout();
    connectAll();
    initAndStartTimers();
 }

template <typename ParamName>
 MaterialsChangesWidget<ParamName>::~MaterialsChangesWidget(){
    disconnectAll();
    deleteWidgets();
    deleteEditDesitionsButtons();
    delete errorDlg;
    errorDlg = nullptr;
    delete errorDenDlg;
    errorDenDlg = nullptr;
    delete errorViscoDlg;
    errorViscoDlg = nullptr;
    delete dialog;
    dialog = nullptr;
    reloadTimer->stop();
    delete reloadTimer;
    reloadTimer = nullptr;
    delete []materialsChangesWidgetStats;
    materialsChangesWidgetStats = nullptr;
    delete nextStat;
    nextStat = nullptr;
}

 template <typename ParamName>
void MaterialsChangesWidget<ParamName>::initAndStartTimers(){
    reloadTimer = new QTimer(nullptr);
    reloadTimer->installEventFilter(materialsChangesWidgetCatchEvents);
    reloadTimer->setInterval(10);
    reloadTimer->start();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::init(){
    if(nextStat == nullptr)
    {
        switch(*materialsChangesWidgetStats){
        case REMOVE:
            *materialsChangesWidgetStats = CHOOSING;
            *(materialsChangesWidgetStats + 1) = CHOOSING;
            nextStat = new MaterialsChangesWidgetStats;
            *nextStat = REMOVE;
            break;
        case EDIT:
            *materialsChangesWidgetStats = CHOOSING;
            *(materialsChangesWidgetStats + 1) = CHOOSING;
            nextStat = new MaterialsChangesWidgetStats;
            *nextStat = EDIT;
            break;
        default:
            break;
        }
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setWindow(){
    if(type == SUBSTANCE_TYPE && *materialsChangesWidgetStats != CHOOSING)
        resize(MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH,MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_HEIGHT);
    else
        resize(MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT);
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::changeWindowStyle(){
    if(type == SUBSTANCE_TYPE && *materialsChangesWidgetStats != CHOOSING)
        resize(MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH, MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_HEIGHT);
    else
        resize(MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT);
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createWidgets(){
    if(titleLabel == nullptr){
        titleLabel = new QLabel(this);
        titleLabel->show();
    }
    if(closeButton == nullptr){
        closeButton = new QLabel(this);
        closeButton->show();
    }
    if(okButton == nullptr){
        okButton = new QLabel(this);
        okButton->show();
    }
    if(cancelButton == nullptr){
        cancelButton = new QLabel(this);
        cancelButton->show();
    }
    switch(*materialsChangesWidgetStats){
    case CHOOSING:
        createWidgetsForChoosing();
        break;
    case ADD:
        createWidgetsForAdd();
        break;
    case EDIT:
        createWidgetsForEdit();
        break;
    default:
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createWidgetsForChoosing(){
    list = new QListWidget(this);
    list->show();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createWidgetsForAdd(){
    nameLabel = new QLabel(this);
    nameLabel->show();
    nameTextEdit = new FormatableDataField<ParamName>(true, nullptr, false, this, QString(), false);
    nameTextEdit->show();
    switch(type){
    case FLANGE_MATERIAL_TYPE:
        linearExpanstionLabel = new QLabel(this);
        linearExpanstionLabel->show();
        linearExpanstionFDF = new FormatableDataField<ParamName>(true, nullptr, false, this, QString(), false);
        linearExpanstionFDF->show();
        linearExpanstionUnitLabel = new QLabel(this);
        linearExpanstionUnitLabel->show();
        fiducialTemperatureLabel = new QLabel(this);
        fiducialTemperatureLabel->show();
        fiducialTemperatureFDF = new FormatableDataField<ParamName>(true, nullptr, false, this, QString(), false);
        fiducialTemperatureFDF->show();
        fiducialTemperatureUnitLabel = new QLabel(this);
        fiducialTemperatureUnitLabel->show();
        break;
    case PIPE_MATERIAL_TYPE:
        linearExpanstionLabel = new QLabel(this);
        linearExpanstionLabel->show();
        linearExpanstionFDF = new FormatableDataField<ParamName>(true, nullptr,false,  this, QString(), false);
        linearExpanstionFDF->show();
        linearExpanstionUnitLabel = new QLabel(this);
        linearExpanstionUnitLabel->show();
        fiducialTemperatureLabel = new QLabel(this);
        fiducialTemperatureLabel->show();
        fiducialTemperatureFDF = new FormatableDataField<ParamName>(true, nullptr, false, this, QString(), false);
        fiducialTemperatureFDF->show();
        fiducialTemperatureUnitLabel = new QLabel(this);
        fiducialTemperatureUnitLabel->show();
        break;
    case SUBSTANCE_TYPE:
        densityTabActive = true;
        typeLabel = new QLabel(this);
        typeLabel->show();
        fluidTypeCheckBox = new QCheckBox(this);
        fluidTypeCheckBox->show();
        fluidTypeCheckBox->setChecked(true);
        gaseTypeCheckBox = new QCheckBox(this);
        gaseTypeCheckBox->show();
        densityTabLabel = new QLabel(this);
        densityTabLabel->show();
        dynamicViscosityTabLabel = new QLabel(this);
        dynamicViscosityTabLabel->show();
        pressureListInfoLabel = new QLabel(this);
        pressureListExpandButton = new QLabel(this);
        pressureListExpandButton->show();        
        createPressureListObj(this);
        pressureUnitList = new QComboBox(this);
        pressureUnitList->addItem("at");
        pressureUnitList->addItem("atm");
        pressureUnitList->addItem("bar");
        pressureUnitList->addItem("psi");
        pressureUnitList->addItem("kPa");
        pressureUnitList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureUnitList->view()->installEventFilter(materialsChangesWidgetCatchEvents);
        static_cast<QWidget*>(pressureList)->setParent(this);
        pressureScrollArea = new QScrollArea(this);
        pressureListAddButton = new QLabel(this);
        pressureListEditAllButton = new QLabel(this);
        noPressureInfo = new QLabel(QString("Wybierz ciśnienie z listy"), this);
        noPressureInfo->setAlignment(Qt::AlignCenter);
        noPressureInfo->setStyleSheet("family-name: \"Comic Sans MS\";");
        noPressureInfo->show();
        kLabel = new QLabel(this);
        kLabel->show();
        kTextEdit = new QLineEdit(this);
        kTextEdit->show();
        dependencyLabel = new QLabel(this);
        dependencyLabel->show();
        tempDependencyButton = new QRadioButton(this);
        tempDependencyButton->show();
        tempPresDependencyButton = new QRadioButton(this);
        tempPresDependencyButton->show();
        dependencyButtonGroup = new QButtonGroup(static_cast<QFrame*>(this));
        dependencyButtonGroup->addButton(tempDependencyButton);
        dependencyButtonGroup->addButton(tempPresDependencyButton);
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createWidgetsForEdit(){
    createWidgetsForAdd();
    backButton = new QLabel(this);
    backButton->show();
    if(type == SUBSTANCE_TYPE){
        temperatureArrayLabel = new QLabel(this);
        temperatureArrayLabel->show();
        dataArrayLabel = new QLabel(this);
        dataArrayLabel->show();
        createArrayObj(this);
        static_cast<QWidget*>(dataArray)->setParent(this);
        dataArray->show();
        addArrayRow = new QLabel(this);
        addArrayRow->show();
        scrollArea = new QScrollArea(this);
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createLayout(){
    titleLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET *2, MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LIST_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    titleLabel->setStyleSheet("font-family: \"Comic Sans MS\"; font-weight: normal; font-size: 14px;");
    if(type == SUBSTANCE_TYPE && *materialsChangesWidgetStats != CHOOSING)
        closeButton->setGeometry(MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - (MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5 - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    else
        closeButton->setGeometry(MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH - (MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5 - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    closeButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
    closeButton->setPixmap(QPixmap(CLOSE_BUTTON_ICON));
    if(type != SUBSTANCE_TYPE)
    {
        okButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + MATERIALS_CHANGES_LIST_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        okButton->setText("Zatwierdź");
        okButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        okButton->setAlignment(Qt::AlignCenter);
        cancelButton->setGeometry(MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH - MATERIALS_CHANGES_BIG_BUTTONS_WIDTH - MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET , MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + MATERIALS_CHANGES_LIST_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        cancelButton->setText("Anuluj");
        cancelButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        cancelButton->setAlignment(Qt::AlignCenter);
    }else{
        okButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET * 2, height() - MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET - MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        okButton->setText("Zatwierdź");
        okButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        okButton->setAlignment(Qt::AlignCenter);
        cancelButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET * 2 + MATERIALS_CHANGES_BIG_BUTTONS_WIDTH + MATERIALS_CHANGES_BIG_BUTTONS_OFFSET, height() - MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        cancelButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        cancelButton->setText("Anuluj");
        cancelButton->setAlignment(Qt::AlignCenter);
    }
    switch(*materialsChangesWidgetStats){
    case CHOOSING:
        okButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + MATERIALS_CHANGES_LIST_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        okButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        okButton->setText("Zatwierdź");
        okButton->setAlignment(Qt::AlignCenter);
        cancelButton->setGeometry(MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH - MATERIALS_CHANGES_BIG_BUTTONS_WIDTH - MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET , MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + MATERIALS_CHANGES_LIST_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        cancelButton->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        cancelButton->setText("Anuluj");
        cancelButton->setAlignment(Qt::AlignCenter);
        list->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_LIST_HEIGHT);
        if(!MaterialsInfoFiles::readMaterials(this)){
            list->clear();
        }
        break;
    case ADD:
        createLayoutForAdd();
        break;
    case EDIT:
        createLayoutForEdit();
        break;
    default:
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createLayoutForAdd(){
    titleLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    nameLabel->setText(MATERIALS_CHANGES_SUBSTANCE_NAME_LABEL_TEXT);
    nameTextEdit->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    nameTextEdit->getDataUTextEdit()->setGeometry(0,0, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    nameTextEdit->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
    switch(type){
    case FLANGE_MATERIAL_TYPE:
        titleLabel->setText("Dodawanie materiału kryzy");
        linearExpanstionLabel->setText(MATERIALS_CHANGES_LINEAR_EXPANSTION_LABEL_TEXT);
        linearExpanstionLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
        linearExpanstionFDF->getDataUTextEdit()->setGeometry(0, 0, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->installEventFilter(materialsChangesWidgetCatchEvents);
        linearExpanstionUnitLabel->setText(MATERIALS_CHANGES_LINEAR_EXPANSTION_UNIT_LABEL_TEXT);
        linearExpanstionUnitLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH + MATERIALS_CHANGES_UNIT_LABEL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_UNIT_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureLabel->setText(MATERIALS_CHANGES_FIDUCIAL_TEMPEARTURE_LABEL_TEXT);
        fiducialTemperatureLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
        fiducialTemperatureFDF->getDataUTextEdit()->setGeometry(0, 0, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->installEventFilter(materialsChangesWidgetCatchEvents);
        fiducialTemperatureUnitLabel->setText(MATERIALS_CHANGES_FIDUCIAL_TEMPEARTURE_UNIT_LABEL_TEXT);
        fiducialTemperatureUnitLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH + MATERIALS_CHANGES_UNIT_LABEL_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_UNIT_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        break;
    case PIPE_MATERIAL_TYPE:
        titleLabel->setText("Dodawanie materiału rurociągu");
        linearExpanstionLabel->setText(MATERIALS_CHANGES_LINEAR_EXPANSTION_LABEL_TEXT);
        linearExpanstionLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->getDataUTextEdit()->setGeometry(0,0, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
        linearExpanstionUnitLabel->setText(MATERIALS_CHANGES_LINEAR_EXPANSTION_UNIT_LABEL_TEXT);
        linearExpanstionUnitLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH + MATERIALS_CHANGES_UNIT_LABEL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_UNIT_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureLabel->setText(MATERIALS_CHANGES_FIDUCIAL_TEMPEARTURE_LABEL_TEXT);
        fiducialTemperatureLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->getDataUTextEdit()->setGeometry(0, 0, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
        fiducialTemperatureUnitLabel->setText(MATERIALS_CHANGES_FIDUCIAL_TEMPEARTURE_UNIT_LABEL_TEXT);
        fiducialTemperatureUnitLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH + MATERIALS_CHANGES_UNIT_LABEL_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_UNIT_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        break;
    case SUBSTANCE_TYPE:
        titleLabel->setText("Dodawanie substancji");
        typeLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        typeLabel->setText(MATERIALS_CHANGES_WIDGET_SUBSTANCE_TYPE_LABEL_TEXT);
        fluidTypeCheckBox->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET,2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fluidTypeCheckBox->setText("Ciecz");
        fluidTypeCheckBox->setChecked(true);
        gaseTypeCheckBox->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH * 0.5,2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        gaseTypeCheckBox->setText("Gaz");
        densityTabLabel->setGeometry(0, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        densityTabLabel->setText("Gęstość");
        densityTabLabel->setStyleSheet("font-family: \"Georgia\"; font-weight: 400; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        densityTabLabel->setAlignment(Qt::AlignCenter);
        dynamicViscosityTabLabel->setGeometry(MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH * 0.5, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        dynamicViscosityTabLabel->setText("Lepkość dynamiczna");
        dynamicViscosityTabLabel->setStyleSheet("font-family: \"Georgia\"; font-weight: 400; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        dynamicViscosityTabLabel->setAlignment(Qt::AlignCenter);
        kLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 6 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        kLabel->setText(MATERIALS_CHANGES_K_LABEL_TEXT);
        kTextEdit->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET,5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 6 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        dependencyLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 4 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        dependencyLabel->setText(MATERIALS_CHANGES_DEPENDENCY_LABEL_TEXT);
        tempDependencyButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET,4 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        tempDependencyButton->setText(MATERIALS_CHANGES_TEMP_DEP_BUTTON_TEXT);
        tempPresDependencyButton->setText(MATERIALS_CHANGES_TEMP_PRES_BUTTON_TEXT);
        tempPresDependencyButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH * 0.5,4 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        tempPresDependencyButton->setChecked(true);
        pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListInfoLabel->setText("Ciśnienie");
        pressureListInfoLabel->setAlignment(Qt::AlignCenter);
        pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
        pressureListExpandButton->setAlignment(Qt::AlignCenter);
        pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10 , 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
        pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListAddButton->setAlignment(Qt::AlignCenter);
        pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
        pressureListAddButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
        pressureListEditAllButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10 + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListEditAllButton->setAlignment(Qt::AlignCenter);
        pressureListEditAllButton->setPixmap(QPixmap(EDIT_ALL_BUTTON_ICON));
        pressureListEditAllButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
        pressureScrollArea->setStyleSheet("background-color: transparent");

        pressureScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px; } QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white; border: 1px solid black;}");
        pressureScrollArea->setWidget(pressureList);
        pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH,  MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT + 2);
        pressureScrollArea->setLayoutDirection(Qt::RightToLeft);
        pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        if(densityTabActive){
            if(pressureListExpandedDensity){
                pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                // NO_PRESSURE_SHOW
                noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                pressureListInfoLabel->show();
                pressureUnitList->show();
                pressureList->show();
                pressureListAddButton->show();
                pressureListEditAllButton->show();
                pressureScrollArea->show();
            }else{
                pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                // NO_PRESSURE_HIDE
                 noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                pressureListInfoLabel->hide();
                pressureUnitList->hide();
                pressureList->hide();
                pressureListAddButton->hide();
                pressureListEditAllButton->hide();
                pressureScrollArea->hide();
            }
        }else{
            if(pressureListExpandedVisco){
                pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                // NO_PRESSURE_SHOW
                noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH+ MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                pressureList->show();
                pressureListInfoLabel->show();
                pressureUnitList->show();
                pressureListAddButton->show();
                pressureListEditAllButton->show();
                pressureScrollArea->show();
            }else{
                pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                // NO_PRESSURE_HIDE
                 noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                pressureList->hide();
                pressureListInfoLabel->hide();
                pressureUnitList->hide();
                pressureListAddButton->hide();
                pressureListEditAllButton->hide();
                pressureScrollArea->hide();
            }
        }
        noPressureInfo->setAlignment(Qt::AlignCenter);
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createLayoutForEdit(){
    createLayoutForAdd();
    nameTextEdit->getDataUTextEdit()->setStyleSheet("");
    switch(type){
    case FLANGE_MATERIAL_TYPE:
        if(!MaterialsInfoFiles::readMaterialParams(this)){
            WINDOW_STATUS_SAVE(materialsChangesWidgetStats)
            *materialsChangesWidgetStats = CHOOSING;
            changeStat = true;
            break;
        }
        fiducialTemperatureFDF->getDataUTextEdit()->setStyleSheet("");
        linearExpanstionFDF->getDataUTextEdit()->setStyleSheet("");
        break;
    case PIPE_MATERIAL_TYPE:
        if(!MaterialsInfoFiles::readMaterialParams(this)){
            WINDOW_STATUS_SAVE(materialsChangesWidgetStats)
            *materialsChangesWidgetStats = CHOOSING;
            changeStat = true;
            break;
        }
        fiducialTemperatureFDF->getDataUTextEdit()->setStyleSheet("");
        linearExpanstionFDF->getDataUTextEdit()->setStyleSheet("");
        break;
    case SUBSTANCE_TYPE:
        if(!MaterialsInfoFiles::readMaterialParams(this)){
            WINDOW_STATUS_SAVE(materialsChangesWidgetStats)
            *materialsChangesWidgetStats = CHOOSING;
            changeStat = true;
            break;
        }
    backButton->setGeometry((MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_BACK_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_BACK_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    backButton->setPixmap(QPixmap(CLOSE_BUTTON_ICON));
    if(densityTabActive){
        kTextEdit->setText(QString::number(densityDataHolder->getK()));
        pressureUnitList->setCurrentIndex(densityDataHolder->getPressureUnit());
        if(densityDataHolder->getDependency() == 2){
            tempPresDependencyButton->setChecked(true);
            pressureList->setCurrentIndex(new uint(densityDataHolder->getSelectedIndex()));
            pressureList->createElements(densityDataHolder);
            if(densityDataHolder->getNumbOfPressures() != 0){
                delete noPressureInfo;
                noPressureInfo = nullptr;
                temperatureArrayLabel->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 2) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                addArrayRow->setGeometry(7 +MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                if(densityTabActive)
                    dataArrayLabel->setText("Gęstość [kg/m3]");
                else
                    dataArrayLabel->setText("Lep. dynamiczna [Pas]");
                dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white; border: 1px solid black;}");
                scrollArea->setWidget(dataArray);
                scrollArea->setLayoutDirection(Qt::RightToLeft);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
                dataArray->setCurrentIndex(densityDataHolder->getSelectedIndex());
                dataArray->createElements(densityDataHolder);
                dataArray->updateGeometry();
                dataArray->show();
                scrollArea->show();
            }
        }else{
            tempDependencyButton->setChecked(true);
            delete noPressureInfo;
            noPressureInfo = nullptr;
            delete pressureList;
            pressureList = nullptr;
            delete pressureListAddButton;
            pressureListAddButton = nullptr;
            delete pressureListEditAllButton;
            pressureListEditAllButton = nullptr;
            delete pressureListInfoLabel;
            pressureListInfoLabel = nullptr;
            delete pressureListExpandButton;
            pressureListExpandButton = nullptr;
            delete pressureScrollArea;
            pressureScrollArea = nullptr;
            temperatureArrayLabel->setGeometry(7 +MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
            addArrayRow->setGeometry( 7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET , 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
            temperatureArrayLabel->setText("Temperatura [C]");
            temperatureArrayLabel->setAlignment(Qt::AlignCenter);
            temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
            dataArrayLabel->setAlignment(Qt::AlignCenter);
            if(densityTabActive)
                dataArrayLabel->setText("Gęstość [kg/m3]");
            else
                dataArrayLabel->setText("Lep. dynamiczna [Pas]");
            dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
            scrollArea->setStyleSheet("background-color: transparent");
            scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white; border: 1px solid black;}");
            scrollArea->setWidget(dataArray);
            scrollArea->setLayoutDirection(Qt::RightToLeft);
            scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
            addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
            dataArray->createElements(densityDataHolder);
            dataArray->updateGeometry();
            dataArray->show();
            scrollArea->show();
            }
    }else{
        kTextEdit->setText(QString::number(dynamicViscosityDataHolder->getK()));
        pressureUnitList->setCurrentIndex(dynamicViscosityDataHolder->getPressureUnit());
        if(dynamicViscosityDataHolder->getDependency() == 2){
            pressureList->setCurrentIndex(new uint(dynamicViscosityDataHolder->getSelectedIndex()));
            pressureList->createElements(dynamicViscosityDataHolder);
            pressureList->show();
            if(dynamicViscosityDataHolder->getNumbOfPressures() != 0){
                delete noPressureInfo;
                noPressureInfo = nullptr;

                temperatureArrayLabel->setGeometry(7 +MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(7+ MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                addArrayRow->setGeometry( 7 +MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                if(densityTabActive)
                    dataArrayLabel->setText("Gęstość [kg/m3]");
                else
                    dataArrayLabel->setText("Lep. dynamiczna [Pas]");
                dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white; border: 1px solid black;}");
                scrollArea->setWidget(dataArray);
                scrollArea->setLayoutDirection(Qt::RightToLeft);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);

                dataArray->setCurrentIndex(dynamicViscosityDataHolder->getSelectedIndex());
                dataArray->createElements(dynamicViscosityDataHolder);
                dataArray->updateGeometry();
                scrollArea->show();

           }
        }else{
            tempDependencyButton->setChecked(true);
            delete noPressureInfo;
            noPressureInfo = nullptr;
            delete pressureList;
            pressureList = nullptr;
            delete pressureListAddButton;
            pressureListAddButton = nullptr;
            delete pressureListEditAllButton;
            pressureListEditAllButton = nullptr;
            delete pressureListInfoLabel;
            pressureListInfoLabel = nullptr;
            delete pressureListExpandButton;
            pressureListExpandButton = nullptr;
            delete pressureScrollArea;
            pressureScrollArea = nullptr;
            temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(7+ MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
            addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
            temperatureArrayLabel->setText("Temperatura [C]");
            temperatureArrayLabel->setAlignment(Qt::AlignCenter);
            temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
            dataArrayLabel->setAlignment(Qt::AlignCenter);
            if(densityTabActive)
                dataArrayLabel->setText("Gęstość [kg/m3]");
            else
                dataArrayLabel->setText("Lep. dynamiczna [Pas]");
            dataArrayLabel->setText("border-width: 2px; border-color: grey;border-style: solid;");
            scrollArea->setStyleSheet("background-color: transparent");
            scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
            scrollArea->setWidget(dataArray);
            scrollArea->setLayoutDirection(Qt::RightToLeft);
            scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
            addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
            dataArray->createElements(dynamicViscosityDataHolder);
            dataArray->show();
            dataArray->updateGeometry();
            scrollArea->show();
            }
    }
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::deleteWidgets(){
    switch(*(materialsChangesWidgetStats + 1)){
    case CHOOSING:
        delete list;
        list = nullptr;
        break;
    case ADD:
        deleteWidgetsForAdd();
        break;
    case EDIT:
        deleteWidgetsForEdit();
        break;
    default:
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::deleteWidgetsForAdd(){
    delete nameLabel;
    nameLabel = nullptr;
    delete nameTextEdit;
    nameTextEdit = nullptr;
    switch(type){
    case FLANGE_MATERIAL_TYPE:
        delete linearExpanstionLabel;
        linearExpanstionLabel = nullptr;
        delete linearExpanstionFDF;
        linearExpanstionFDF = nullptr;
        delete linearExpanstionUnitLabel;
        linearExpanstionUnitLabel = nullptr;
        delete fiducialTemperatureLabel;
        fiducialTemperatureLabel = nullptr;
        delete fiducialTemperatureFDF;
        fiducialTemperatureFDF = nullptr;
        delete fiducialTemperatureUnitLabel;
        fiducialTemperatureUnitLabel = nullptr;
        break;
    case PIPE_MATERIAL_TYPE:
        delete linearExpanstionLabel;
        linearExpanstionLabel = nullptr;
        delete linearExpanstionFDF;
        linearExpanstionFDF = nullptr;
        delete linearExpanstionUnitLabel;
        linearExpanstionUnitLabel = nullptr;
        delete fiducialTemperatureLabel;
        fiducialTemperatureLabel = nullptr;
        delete fiducialTemperatureFDF;
        fiducialTemperatureFDF = nullptr;
        delete fiducialTemperatureUnitLabel;
        fiducialTemperatureUnitLabel = nullptr;
        break;
    case SUBSTANCE_TYPE:
        delete typeLabel;
        typeLabel = nullptr;
        delete fluidTypeCheckBox;
        fluidTypeCheckBox = nullptr;
        delete gaseTypeCheckBox;
        gaseTypeCheckBox = nullptr;
        delete densityTabLabel;
        densityTabLabel = nullptr;
        delete dynamicViscosityTabLabel;
        dynamicViscosityTabLabel = nullptr;
        delete pressureListInfoLabel;
        pressureListInfoLabel = nullptr;
        delete pressureListExpandButton;
        pressureListExpandButton = nullptr;
        delete pressureList;
        pressureList = nullptr;
        delete pressureListAddButton;
        pressureListAddButton = nullptr;
        delete pressureListEditAllButton;
        pressureListEditAllButton = nullptr;
        delete pressureScrollArea;
        pressureScrollArea = nullptr;
        delete temperatureArrayLabel;
        temperatureArrayLabel = nullptr;
        delete dataArrayLabel;
        dataArrayLabel = nullptr;
        delete dataArray;
        dataArray = nullptr;
        delete addArrayRow;
        addArrayRow = nullptr;
        delete scrollArea;
        scrollArea = nullptr;
        delete noPressureInfo;
        noPressureInfo = nullptr;
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::deleteWidgetsForEdit(){
    deleteWidgetsForAdd();
    delete backButton;
    backButton = nullptr;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::connectAll(){
    closeButton->installEventFilter(materialsChangesWidgetCatchEvents);
    okButton->installEventFilter(materialsChangesWidgetCatchEvents);
    cancelButton->installEventFilter(materialsChangesWidgetCatchEvents);
    switch(*materialsChangesWidgetStats){
    case CHOOSING:
        break;
    case REMOVE:
        break;
    default:
       nameTextEdit->getDataUTextEdit()->installEventFilter(materialsChangesWidgetCatchEvents);
       switch(type){
       case FLANGE_MATERIAL_TYPE:
           linearExpanstionFDF->getDataUTextEdit()->installEventFilter(materialsChangesWidgetCatchEvents);
           fiducialTemperatureFDF->getDataUTextEdit()->installEventFilter(materialsChangesWidgetCatchEvents);
           break;
       case PIPE_MATERIAL_TYPE:
           linearExpanstionFDF->getDataUTextEdit()->installEventFilter(materialsChangesWidgetCatchEvents);
           fiducialTemperatureFDF->getDataUTextEdit()->installEventFilter(materialsChangesWidgetCatchEvents);
           break;
       case SUBSTANCE_TYPE:
           fluidTypeCheckBox->installEventFilter(materialsChangesWidgetCatchEvents);
           gaseTypeCheckBox->installEventFilter(materialsChangesWidgetCatchEvents);
           densityTabLabel->installEventFilter(materialsChangesWidgetCatchEvents);
           dynamicViscosityTabLabel->installEventFilter(materialsChangesWidgetCatchEvents);
           tempDependencyButton->installEventFilter(materialsChangesWidgetCatchEvents);
           tempPresDependencyButton->installEventFilter(materialsChangesWidgetCatchEvents);
           if(addArrayRow)
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
       if(tempPresDependencyButton->isChecked())
       {
           pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
           pressureList->installEventFilter(materialsChangesWidgetCatchEvents);
           pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
           pressureListEditAllButton->installEventFilter(materialsChangesWidgetCatchEvents);
       }
           break;
       }
       if(backButton != nullptr)
           backButton->installEventFilter(materialsChangesWidgetCatchEvents);
    }

}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::disconnectAll(){
    // Brak
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::reload(){
    reloadTimer->stop();
    changeStat = false;
    firstRead = false;
    deleteWidgets();
    deleteEditDesitionsButtons();
    init();
    changeWindowStyle();
    createWidgets();
    createLayout();
    connectAll();
    reloadTimer->start();
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getCloseButton(){
    return closeButton;
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getOkButton(){
    return okButton;
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getCancelButton(){
    return cancelButton;
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getBackButton(){
    return backButton;
}

template <typename ParamName>
QListWidget* MaterialsChangesWidget<ParamName>::getList(){
    return list;
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getPressureListEditAllButton(){
    return pressureListEditAllButton;
}

template <typename ParamName>
FormatableDataField<ParamName>* MaterialsChangesWidget<ParamName>::getLinearExpanstionFDF(){
    return linearExpanstionFDF;
}

template <typename ParamName>
FormatableDataField<ParamName>* MaterialsChangesWidget<ParamName>::getFiducialTemperatureFDF(){
    return fiducialTemperatureFDF;
}

template <typename ParamName>
FormatableDataField<ParamName>* MaterialsChangesWidget<ParamName>::getNameTextEdit(){
    return nameTextEdit;
}

template <typename ParamName>
QCheckBox* MaterialsChangesWidget<ParamName>::getFluidTypeCheckBox(){
    return fluidTypeCheckBox;
}

template <typename ParamName>
QCheckBox* MaterialsChangesWidget<ParamName>::getGaseTypeCheckBox(){
    return gaseTypeCheckBox;
}

template <typename ParamName>
MaterialsChangesWidgetStats* MaterialsChangesWidget<ParamName>::getMaterialsChangesWidgetStats(){
    return materialsChangesWidgetStats;
}

template <typename ParamName>
MaterialsChangesWidgetStats* MaterialsChangesWidget<ParamName>::getNextStat(){
    return nextStat;
}

template <typename ParamName>
QComboBox* MaterialsChangesWidget<ParamName>::getPressureUnitList(){
    return pressureUnitList;
}

template <typename ParamName>
QString MaterialsChangesWidget<ParamName>::getSelectedFromList(){
    return selectedFromList;
}

template <typename ParamName>
MaterialsChangesWidgetType MaterialsChangesWidget<ParamName>::getType(){
    return type;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::getFirstRead(){
    return firstRead;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::getDensityTabActive(){
    return densityTabActive;
}

template <typename ParamName>
MyDialog* MaterialsChangesWidget<ParamName>::getErrorDlg(){
    return errorDlg;
}

template <typename ParamName>
MyDialog* MaterialsChangesWidget<ParamName>::getErrorDenDlg(){
    return errorDenDlg;
}

template <typename ParamName>
MyDialog* MaterialsChangesWidget<ParamName>::getErrorViscoDlg(){
    return errorViscoDlg;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setErrorDlg(MyDialog* dlg){
    errorDlg = dlg;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setErrorDenDlg(MyDialog* dlg){
    errorDenDlg = dlg;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setErrorViscoDlg(MyDialog* dlg){
    errorViscoDlg = dlg;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setDensityPath(QString setPath){
    densityPath = setPath;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setViscoPath(QString setPath){
    viscoPath = setPath;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::reloadTimerTask(){
    reloadTimer->stop();
    if(changeStat)
        reload();
    if(indexToDelete){
        dataArray->removeElement(*indexToDelete);
        delete indexToDelete;
        indexToDelete = nullptr;
    }
    if(pressureIndexToDelete){
        pressureList->removeElement(*pressureIndexToDelete);
        if(densityTabActive)
            densityDataHolder->removePressure(*pressureIndexToDelete);
        else
            dynamicViscosityDataHolder->removePressure(*pressureIndexToDelete);
        if(dataArray){
            if(*pressureIndexToDelete == dataArray->getCurrentIndex())
            {
                delete dataArray;
                dataArray = nullptr;
                delete temperatureArrayLabel;
                temperatureArrayLabel = nullptr;
                delete dataArrayLabel;
                dataArrayLabel = nullptr;
                delete addArrayRow;
                addArrayRow = nullptr;
                delete scrollArea;
                scrollArea = nullptr;
                noPressureInfo = new QLabel(QString("Wybierz ciśnienie z listy"), this);
                noPressureInfo->show();
                noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH+ MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                noPressureInfo->setAlignment(Qt::AlignCenter);
                noPressureInfo->setStyleSheet("family-name: \"Comic Sans MS\";");
            }else{
                if(*pressureIndexToDelete < dataArray->getCurrentIndex()){
                    dataArray->setCurrentIndex(dataArray->getCurrentIndex() - 1);
                }
            }
        }
        delete pressureIndexToDelete;
        pressureIndexToDelete = nullptr;
    }
    reloadTimer->start();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::closeButtonReleased(){
    QFrame::deleteLater();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::okButtonReleased(){
    if(dataArray){
        if(!dataArray->checkLineEdits()){
            moveToArrayError();
          return;
          }
        if(densityTabActive)
            densityDataHolder->readDataFromArray(dataArray);
        else
            dynamicViscosityDataHolder->readDataFromArray(dataArray);
    }
    int checkAns = -1;
    switch(*materialsChangesWidgetStats){
    case CHOOSING:
        if(nextStat != nullptr){
            switch(*nextStat){
            case REMOVE:
                if(list->count() > 0){
                    if(type != SUBSTANCE_TYPE)
                        selectedFromList = list->currentItem()->text();
                    else
                        selectedFromList = getNameFromListElement(list->currentItem()->text());
                    MaterialsInfoFiles::removeMaterial(this);
                    deleteLater();
                }
                break;
            case EDIT:
                if(list->count() > 0)
                {
                    if(type != SUBSTANCE_TYPE)
                        selectedFromList = list->currentItem()->text();
                    else
                        selectedFromList = getNameFromListElement(list->currentItem()->text());
                    WINDOW_STATUS_SAVE(materialsChangesWidgetStats)
                    *materialsChangesWidgetStats = EDIT;
                    changeStat = true;
                }
                break;
            default:
                break;
            }
        }
        break;
    case ADD:{
        if(nameTextEdit->getDataUTextEdit()->text().isEmpty())
            return;
        checkAns = MaterialsInfoFiles::checkMaterialsNames(this);
        switch(checkAns){
        case 0:{
            dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Podana nazwa już istnieje."), false, QString("Ok"), QString(), QString(), this);
            dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
            dialog->setWindowModality(Qt::ApplicationModal);
            dialog->show();
            dialog->exec();
            delete dialog;
            dialog = nullptr;
            return;
        }
        case 1:
            break;
        case 2:
            break;
        case 3:
            return;
        }
        switch(type){
        case SUBSTANCE_TYPE:
            dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Wybierz co chcesz zapisać:"), false, QString("Gęstość"), QString("Lepkość"), QString("Wszystko"), this);
            dialog->setWindowModality(Qt::ApplicationModal);
            dialog->show();
            switch(dialog->exec()){
            case FIRST_OPTION_RESULT:
            {
                switch(int r = densityDataHolder->check())
                {
                case 1:
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Błądna liczba wartości ciśnienia"), QString("Dla zaleźności od ciśnienia i temperatury muszą być określone conajmniej dwie wartości ciśnienia."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                case 0:
                    break;
                default:
                    delete dialog;
                    if(!densityTabActive)
                        densityTabLabelReleased();
                    if(pressureList){
                            if(!pressureListExpandedDensity){
                                pressureListExpandButtonReleased();
                            }
                            pressureList->dataElementClicked(r - 2);
                        }
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Błędna liczba wartości temperatury"), QString("Dla każdej wartości ciśnienia muszą być określone conajmniej dwie wartości temperatur."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                    break;
                }
                densityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    densityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Fluid") + QString("Density") + RESOURCES_FILE_EXTENSTION), true, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Density") + RESOURCES_FILE_EXTENSTION));
                else
                    densityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Gase") + QString("Density") + RESOURCES_FILE_EXTENSTION), true, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Gase") + QString("Density") + RESOURCES_FILE_EXTENSTION));
                MyDialog dlg(QUESTION_DIALOG, QString(), QString("Czy program ma sprawdzić, czy wprowadzono poprawne dane?"),true, QString(), QString(), QString(), this);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                switch(dlg.exec()){
                case YES_RESULT:
                {
                    if(errorDlg){
                        delete errorDlg;
                        errorDlg = nullptr;
                    }
                    errorDlg = new MyDialog(SUBSTANCE_FILE_ERRORS_LIST, QString("Błędy pliku gęstości dla substancji ") + nameTextEdit->getDataUTextEdit()->text(), QString("Lista zawiera prawdopodobnie błędnie wpisane dane. Zamknięcie tego okna nie spowoduje zapisania zmienionych danych."), false, QString("Zamknij"), QString(), QString(), this);
                    errorDlg->getSubstanceFileErrorsList()->densityTab = true;
                    errorDlg->installEventFilter(materialsChangesWidgetCatchEvents);
                    if(errorDlg->checkFileSFEL(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + ((fluidTypeCheckBox->isChecked())? QString("Fluid") : QString("Gase")) + QString("Density") + RESOURCES_FILE_EXTENSTION)))
                        return;
                }
                    break;
                default:
                    break;
                }
            }
                break;
            case SECOND_OPTION_RESULT:
            {
                switch(int r = dynamicViscosityDataHolder->check())
                {
                case 1:
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Błądna liczba wartości ciśnienia"), QString("Dla zaleźności od ciśnienia i temperatury muszą być określone conajmniej dwie wartości ciśnienia."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                case 0:
                    break;
                default:
                    delete dialog;
                    if(densityTabActive)
                        dynamicViscosityTabLabelReleased();
                    if(pressureList){
                            if(!pressureListExpandedDensity){
                                pressureListExpandButtonReleased();
                            }
                            pressureList->dataElementClicked(r - 2);
                        }
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Błędna liczba wartości temperatury"), QString("Dla każdej wartości ciśnienia muszą być określone conajmniej dwie wartości temperatur."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                    break;
                }
                dynamicViscosityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    dynamicViscosityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION), false, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION));
                else
                    dynamicViscosityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Gase") + QString("Visco") + RESOURCES_FILE_EXTENSTION), false,new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Gase") + QString("Visco") + RESOURCES_FILE_EXTENSTION));
                MyDialog dlg(QUESTION_DIALOG, QString(), QString("Czy program ma sprawdzić, czy wprowadzono poprawne dane?"),true, QString(), QString(), QString(), this);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                switch(dlg.exec()){
                case YES_RESULT:
                {
                    if(errorDlg){
                        delete errorDlg;
                        errorDlg = nullptr;
                    }
                    errorDlg = new MyDialog(SUBSTANCE_FILE_ERRORS_LIST, QString("Błędy pliku lepkości dla substancji ") + nameTextEdit->getDataUTextEdit()->text(), QString("Lista zawiera prawdopodobnie błędnie wpisane dane. Zamknięcie tego okna nie spowoduje zapisania zmienionych danych."), false, QString("Zamknij"), QString(), QString(), this);
                    errorDlg->getSubstanceFileErrorsList()->densityTab = false;
                    errorDlg->installEventFilter(materialsChangesWidgetCatchEvents);
                    if(errorDlg->checkFileSFEL(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + ((fluidTypeCheckBox->isChecked())? QString("Fluid") : QString("Gase")) + QString("Visco") + RESOURCES_FILE_EXTENSTION)))
                        return;
                }
                    break;
                default:
                    break;
                }
            }
                break;
            case THIRD_OPTION_RESULT:
            {
                switch(int r = densityDataHolder->check())
                {
                case 1:
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Błądna liczba wartości ciśnienia"), QString("Dla zaleźności od ciśnienia i temperatury muszą być określone conajmniej dwie wartości ciśnienia."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                case 0:
                    break;
                default:
                    delete dialog;
                    if(!densityTabActive)
                        densityTabLabelReleased();
                    if(pressureList){
                       if(!pressureListExpandedDensity){
                            pressureListExpandButtonReleased();
                            }
                            pressureList->dataElementClicked(r - 2);
                        }
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Błędna liczba wartości temperatury"), QString("Dla każdej wartości ciśnienia muszą być określone conajmniej dwie wartości temperatur."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                    break;
                }
                switch(int r = dynamicViscosityDataHolder->check())
                {
                case 1:
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Błądna liczba wartości ciśnienia"), QString("Dla zaleźności od ciśnienia i temperatury muszą być określone conajmniej dwie wartości ciśnienia."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                case 0:
                    break;
                default:
                    delete dialog;
                    if(densityTabActive)
                        dynamicViscosityTabLabelReleased();
                    if(pressureList){
                            if(!pressureListExpandedDensity){
                                pressureListExpandButtonReleased();
                            }
                            pressureList->dataElementClicked(r - 2);
                    }
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Błędna liczba wartości temperatury"), QString("Dla każdej wartości ciśnienia muszą być określone conajmniej dwie wartości temperatur."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                    break;
                }
                densityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    densityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Fluid") + QString("Density") + RESOURCES_FILE_EXTENSTION), true, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Density") + RESOURCES_FILE_EXTENSTION));
                else
                    densityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Gase") + QString("Density") + RESOURCES_FILE_EXTENSTION), true,new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Gase") + QString("Density") + RESOURCES_FILE_EXTENSTION));

                dynamicViscosityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    dynamicViscosityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION), false,new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION));
                else
                    dynamicViscosityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Gase") + QString("Visco") + RESOURCES_FILE_EXTENSTION), false, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Gase") + QString("Visco") + RESOURCES_FILE_EXTENSTION));
                MyDialog dlg(QUESTION_DIALOG, QString(), QString("Czy program ma sprawdzić, czy wprowadzono poprawne dane?"),true, QString(), QString(), QString(), this);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                switch(dlg.exec()){
                case YES_RESULT:
                {
                    if(errorDenDlg){
                        delete errorDenDlg;
                        errorDenDlg = nullptr;
                    }
                    if(errorViscoDlg){
                        delete errorViscoDlg;
                        errorViscoDlg = nullptr;
                    }
                    errorDenDlg = new MyDialog(SUBSTANCE_FILE_ERRORS_LIST, QString("Błędy pliku gęstości dla substancji ") + nameTextEdit->getDataUTextEdit()->text(), QString("Lista zawiera prawdopodobnie błędnie wpisane dane. Zamknięcie tego okna nie spowoduje zapisania zmienionych danych."), false, QString("Zamknij"), QString(), QString(), this);
                    errorViscoDlg = new MyDialog(SUBSTANCE_FILE_ERRORS_LIST, QString("Błędy pliku lepkości dla substancji ") + nameTextEdit->getDataUTextEdit()->text(), QString("Lista zawiera prawdopodobnie błędnie wpisane dane. Zamknięcie tego okna nie spowoduje zapisania zmienionych danych."), false, QString("Zamknij"), QString(), QString(), this);
                    errorDenDlg->getSubstanceFileErrorsList()->densityTab = true;
                    errorViscoDlg->getSubstanceFileErrorsList()->densityTab = false;
                    errorDenDlg->installEventFilter(materialsChangesWidgetCatchEvents);
                    errorViscoDlg->installEventFilter(materialsChangesWidgetCatchEvents);
                    if(errorDenDlg->checkFileSFEL(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + ((fluidTypeCheckBox->isChecked())? QString("Fluid") : QString("Gase")) + QString("Density") + RESOURCES_FILE_EXTENSTION)) || errorViscoDlg->checkFileSFEL(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + ((fluidTypeCheckBox->isChecked())? QString("Fluid") : QString("Gase")) + QString("Visco") + RESOURCES_FILE_EXTENSTION)))
                        return;
                }
                    break;
                default:
                    break;
                }
            }
                break;
            default:
                break;
            }
            break;
        default:
            if(!FormatableData<ParamName>::checkStringFormat(linearExpanstionFDF->getDataUTextEdit()->text(), false))
                return;
            if(!FormatableData<ParamName>::checkStringFormat(fiducialTemperatureFDF->getDataUTextEdit()->text(), true))
                return;
            break;
        }
        MaterialsInfoFiles::addMaterial(this);
        deleteLater();
    }
        break;
        case EDIT:{
            if(nameTextEdit->getDataUTextEdit()->text().isEmpty())
                return;
            if(!App::compareStr(nameTextEdit->getDataUTextEdit()->text(), selectedFromList)){
            checkAns = MaterialsInfoFiles::checkMaterialsNames(this);
            switch(checkAns){
            case 0:{
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Podana nazwa już istnieje."), false, QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                dialog->exec();
                delete dialog;
                dialog = nullptr;
                return;
            }
            case 1:
                break;
            case 2:
                break;
            case 3:
                return;
            }
            }
            switch(type){
            case SUBSTANCE_TYPE:
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Wybierz co chcesz zapisać:"), false, QString("Gęstość"), QString("Lepkość"), QString("Wszystko"), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                switch(dialog->exec()){
                case FIRST_OPTION_RESULT:
                {
                    switch(int r = densityDataHolder->check())
                    {
                    case 1:
                        delete dialog;
                        dialog = new MyDialog(QUESTION_DIALOG, QString("Błądna liczba wartości ciśnienia"), QString("Dla zaleźności od ciśnienia i temperatury muszą być określone conajmniej dwie wartości ciśnienia."), false, QString("Ok"), QString(), QString(), this);
                        dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                        dialog->setWindowModality(Qt::ApplicationModal);
                        dialog->show();
                        dialog->exec();
                        delete dialog;
                        dialog = nullptr;
                        return;
                    case 0:
                        break;
                    default:
                        delete dialog;
                        if(!densityTabActive)
                            densityTabLabelReleased();
                        if(pressureList){
                                if(!pressureListExpandedDensity){
                                    pressureListExpandButtonReleased();
                                }
                                pressureList->dataElementClicked(r - 2);
                        }
                        dialog = new MyDialog(QUESTION_DIALOG, QString("Błędna liczba wartości temperatury"), QString("Dla każdej wartości ciśnienia muszą być określone conajmniej dwie wartości temperatur."), false, QString("Ok"), QString(), QString(), this);
                        dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                        dialog->setWindowModality(Qt::ApplicationModal);
                        dialog->show();
                        dialog->exec();
                        delete dialog;
                        dialog = nullptr;
                        return;
                        break;
                    }
                    densityDataHolder->sort();
                    if(fluidTypeCheckBox->isChecked())
                        densityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Fluid") + QString("Density") + RESOURCES_FILE_EXTENSTION), true, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Density") + RESOURCES_FILE_EXTENSTION));
                    else
                        densityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Gase") + QString("Density") + RESOURCES_FILE_EXTENSTION), true,new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Gase") + QString("Density") + RESOURCES_FILE_EXTENSTION));
                    MyDialog dlg(QUESTION_DIALOG, QString(), QString("Czy program ma sprawdzić, czy wprowadzono poprawne dane?"),true, QString(), QString(), QString(), this);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    switch(dlg.exec()){
                    case YES_RESULT:
                    {
                        if(errorDlg){
                            delete errorDlg;
                            errorDlg = nullptr;
                        }
                        errorDlg = new MyDialog(SUBSTANCE_FILE_ERRORS_LIST, QString("Błędy pliku gęstości dla substancji ") + nameTextEdit->getDataUTextEdit()->text(), QString("Lista zawiera prawdopodobnie błędnie wpisane dane. Zamknięcie tego okna nie spowoduje zapisania zmienionych danych."), false, QString("Zamknij"), QString(), QString(), this);
                        errorDlg->getSubstanceFileErrorsList()->densityTab = true;
                        errorDlg->installEventFilter(materialsChangesWidgetCatchEvents);
                        if(errorDlg->checkFileSFEL(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + ((fluidTypeCheckBox->isChecked())? QString("Fluid") : QString("Gase")) + QString("Density") + RESOURCES_FILE_EXTENSTION)))
                            return;
                    }
                        break;
                    default:
                        break;
                    }
                }
                    break;
                case SECOND_OPTION_RESULT:
                {
                    switch(int r = dynamicViscosityDataHolder->check())
                    {
                    case 1:
                        delete dialog;
                        dialog = new MyDialog(QUESTION_DIALOG, QString("Błądna liczba wartości ciśnienia"), QString("Dla zaleźności od ciśnienia i temperatury muszą być określone conajmniej dwie wartości ciśnienia."), false, QString("Ok"), QString(), QString(), this);
                        dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                        dialog->setWindowModality(Qt::ApplicationModal);
                        dialog->show();
                        dialog->exec();
                        delete dialog;
                        dialog = nullptr;
                        return;
                    case 0:
                        break;
                    default:
                        delete dialog;
                        if(densityTabActive)
                            dynamicViscosityTabLabelReleased();
                        if(pressureList){
                                if(!pressureListExpandedDensity){
                                    pressureListExpandButtonReleased();
                                }
                                pressureList->dataElementClicked(r - 2);
                        }
                        dialog = new MyDialog(QUESTION_DIALOG, QString("Błędna liczba wartości temperatury"), QString("Dla każdej wartości ciśnienia muszą być określone conajmniej dwie wartości temperatur."), false, QString("Ok"), QString(), QString(), this);
                        dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                        dialog->setWindowModality(Qt::ApplicationModal);
                        dialog->show();
                        dialog->exec();
                        delete dialog;
                        dialog = nullptr;
                        return;
                        break;
                    }
                    dynamicViscosityDataHolder->sort();
                    if(fluidTypeCheckBox->isChecked())
                        dynamicViscosityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION), false, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION));
                    else
                        dynamicViscosityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Gase") + QString("Visco") + RESOURCES_FILE_EXTENSTION), false, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION));
                    MyDialog dlg(QUESTION_DIALOG, QString(), QString("Czy program ma sprawdzić, czy wprowadzono poprawne dane?"),true, QString(), QString(), QString(), this);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    switch(dlg.exec()){
                    case YES_RESULT:
                    {
                        if(errorDlg){
                            delete errorDlg;
                            errorDlg = nullptr;
                        }
                        errorDlg = new MyDialog(SUBSTANCE_FILE_ERRORS_LIST, QString("Błędy pliku lepkości dla substancji ") + nameTextEdit->getDataUTextEdit()->text(), QString("Lista zawiera prawdopodobnie błędnie wpisane dane. Zamknięcie tego okna nie spowoduje zapisania zmienionych danych."), false, QString("Zamknij"), QString(), QString(), this);
                        errorDlg->getSubstanceFileErrorsList()->densityTab = false;
                        errorDlg->installEventFilter(materialsChangesWidgetCatchEvents);
                        if(errorDlg->checkFileSFEL(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + ((fluidTypeCheckBox->isChecked())? QString("Fluid") : QString("Gase")) + QString("Visco") + RESOURCES_FILE_EXTENSTION)))
                            return;
                    }
                        break;
                    default:
                        break;
                    }
                }
                    break;
                case THIRD_OPTION_RESULT:
                {
                    switch(int r = densityDataHolder->check())
                    {
                    case 1:
                        delete dialog;
                        dialog = new MyDialog(QUESTION_DIALOG, QString("Błądna liczba wartości ciśnienia"), QString("Dla zaleźności od ciśnienia i temperatury muszą być określone conajmniej dwie wartości ciśnienia."), false, QString("Ok"), QString(), QString(), this);
                        dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                        dialog->setWindowModality(Qt::ApplicationModal);
                        dialog->show();
                        dialog->exec();
                        delete dialog;
                        dialog = nullptr;
                        return;
                    case 0:
                        break;
                    default:
                        delete dialog;
                        if(!densityTabActive)
                            densityTabLabelReleased();
                        if(pressureList){
                                if(!pressureListExpandedDensity){
                                    pressureListExpandButtonReleased();
                                }
                                pressureList->dataElementClicked(r - 2);
                        }
                        dialog = new MyDialog(QUESTION_DIALOG, QString("Błędna liczba wartości temperatury"), QString("Dla każdej wartości ciśnienia muszą być określone conajmniej dwie wartości temperatur."), false, QString("Ok"), QString(), QString(), this);
                        dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                        dialog->setWindowModality(Qt::ApplicationModal);
                        dialog->show();
                        dialog->exec();
                        delete dialog;
                        dialog = nullptr;
                        return;
                        break;
                    }
                    switch(int r = dynamicViscosityDataHolder->check())
                    {
                    case 1:
                        delete dialog;
                        dialog = new MyDialog(QUESTION_DIALOG, QString("Błądna liczba wartości ciśnienia"), QString("Dla zaleźności od ciśnienia i temperatury muszą być określone conajmniej dwie wartości ciśnienia."), false, QString("Ok"), QString(), QString(), this);
                        dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                        dialog->setWindowModality(Qt::ApplicationModal);
                        dialog->show();
                        dialog->exec();
                        delete dialog;
                        dialog = nullptr;
                        return;
                    case 0:
                        break;
                    default:
                        delete dialog;
                        if(densityTabActive)
                            dynamicViscosityTabLabelReleased();
                        if(pressureList){
                                if(!pressureListExpandedDensity){
                                    pressureListExpandButtonReleased();
                                }
                                pressureList->dataElementClicked(r - 2);
                        }
                        dialog = new MyDialog(QUESTION_DIALOG, QString("Błędna liczba wartości temperatury"), QString("Dla każdej wartości ciśnienia muszą być określone conajmniej dwie wartości temperatur."), false, QString("Ok"), QString(), QString(), this);
                        dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                        dialog->setWindowModality(Qt::ApplicationModal);
                        dialog->show();
                        dialog->exec();
                        delete dialog;
                        dialog = nullptr;
                        return;
                        break;
                    }
                    densityDataHolder->sort();
                    if(fluidTypeCheckBox->isChecked())
                        densityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Fluid") + QString("Density") + RESOURCES_FILE_EXTENSTION), true, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Density") + RESOURCES_FILE_EXTENSTION));
                    else
                        densityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Gase") + QString("Density") + RESOURCES_FILE_EXTENSTION), true, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Gase") + QString("Density") + RESOURCES_FILE_EXTENSTION));

                    dynamicViscosityDataHolder->sort();
                    if(fluidTypeCheckBox->isChecked())
                        dynamicViscosityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION), false, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Fluid") + QString("Visco") + RESOURCES_FILE_EXTENSTION));
                    else
                        dynamicViscosityDataHolder->writeToFile(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + QString("Gase") + QString("Visco") + RESOURCES_FILE_EXTENSTION),false, new QString(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + selectedFromList + QString("Gase") + QString("Visco") + RESOURCES_FILE_EXTENSTION));
                    MyDialog dlg(QUESTION_DIALOG, QString(), QString("Czy program ma sprawdzić, czy wprowadzono poprawne dane?"),true, QString(), QString(), QString(), this);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    switch(dlg.exec()){
                    case YES_RESULT:
                    {
                        if(errorDenDlg){
                            delete errorDenDlg;
                            errorDenDlg = nullptr;
                        }
                        if(errorViscoDlg){
                            delete errorViscoDlg;
                            errorViscoDlg = nullptr;
                        }
                        errorDenDlg = new MyDialog(SUBSTANCE_FILE_ERRORS_LIST, QString("Błędy pliku gęstości dla substancji ") + nameTextEdit->getDataUTextEdit()->text(), QString("Lista zawiera prawdopodobnie błędnie wpisane dane. Zamknięcie tego okna nie spowoduje zapisania zmienionych danych."), false, QString("Zamknij"), QString(), QString(), this);
                        errorDenDlg->getSubstanceFileErrorsList()->densityTab = true;
                        errorDenDlg->installEventFilter(materialsChangesWidgetCatchEvents);
                        errorViscoDlg = new MyDialog(SUBSTANCE_FILE_ERRORS_LIST, QString("Błędy pliku lepkości dla substancji ") + nameTextEdit->getDataUTextEdit()->text(), QString("Lista zawiera prawdopodobnie błędnie wpisane dane. Zamknięcie tego okna nie spowoduje zapisania zmienionych danych."), false, QString("Zamknij"), QString(), QString(), this);
                        errorViscoDlg->getSubstanceFileErrorsList()->densityTab = false;
                        errorViscoDlg->installEventFilter(materialsChangesWidgetCatchEvents);
                        if(errorDenDlg->checkFileSFEL(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + ((fluidTypeCheckBox->isChecked())? QString("Fluid") : QString("Gase")) + QString("Density") + RESOURCES_FILE_EXTENSTION)) || errorViscoDlg->checkFileSFEL(new QFile(config->getDefaultPath() + QString(APP_NAME) + "\\" + QString("Fluids\\") + nameTextEdit->getDataUTextEdit()->text() + ((fluidTypeCheckBox->isChecked())? QString("Fluid") : QString("Gase")) + QString("Visco") + RESOURCES_FILE_EXTENSTION)))
                            return;
                    }
                        break;
                    default:
                        break;
                    }
                }
                    break;
                default:
                    break;
                }
                break;
            default:
                if(!FormatableData<ParamName>::checkStringFormat(linearExpanstionFDF->getDataUTextEdit()->text(), false))
                    return;
                if(!FormatableData<ParamName>::checkStringFormat(fiducialTemperatureFDF->getDataUTextEdit()->text(), true))
                    return;
                break;
            }
            MaterialsInfoFiles::editMaterial(this);
            WINDOW_STATUS_SAVE(materialsChangesWidgetStats)
            *materialsChangesWidgetStats = CHOOSING;
            changeStat = true;
        }
        break;
    default:
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::cancelButtonReleased(){
    deleteLater();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::backButtonReleased(){
    WINDOW_STATUS_SAVE(materialsChangesWidgetStats)
    *materialsChangesWidgetStats = CHOOSING;
    changeStat = true;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::nameTextEditKeyPressed(QKeyEvent* ev){
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
        nameTextEdit->getDataUTextEdit()->insert(" ");
        break;
    case Qt::Key_Backspace:
        nameTextEdit->getDataUTextEdit()->backspace();
        break;
    case Qt::Key_Delete:
        nameTextEdit->getDataUTextEdit()->del();
        break;
    default:
        if(ev->count())
            nameTextEdit->getDataUTextEdit()->insert(ev->text());
        else
            return false;
    }
    nameTextEdit->multiSpaceKeyDetection();
    if(nameTextEdit->getDataUTextEdit()->text().isEmpty())
        nameTextEdit->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
    else
        nameTextEdit->getDataUTextEdit()->setStyleSheet("");
    return true;
}

template <typename ParamName>
QString MaterialsChangesWidget<ParamName>::getNameFromListElement(QString listStr){
    uint i = listStr.length() - 1;
    while(listStr.at(i) != '(')
        i--;
    listStr.chop(listStr.length() - i + 1);
    return listStr;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::linearExpanstionFDFKeyPressed(QKeyEvent *ev){
    uint len = linearExpanstionFDF->getDataUTextEdit()->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
        // Bezpośrednie połaczenie z KEY_PERIOD
    case Qt::Key_Period:
        {
        QLineEdit *te = linearExpanstionFDF->getDataUTextEdit();
        uint i = 0;
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
        linearExpanstionFDF->getDataUTextEdit()->del();
        break;
    case Qt::Key_Backspace:
        linearExpanstionFDF->getDataUTextEdit()->backspace();
        break;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Minus:
    {
        QLineEdit *te = linearExpanstionFDF->getDataUTextEdit();
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
        for(uint i = 0; i < len; i++)
            if(linearExpanstionFDF->getDataUTextEdit()->text().at(i) == 'e')
                return true;
        linearExpanstionFDF->getDataUTextEdit()->insert(ev->text().toLower());
        break;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39)
            linearExpanstionFDF->getDataUTextEdit()->insert(ev->text());
        else
            return true;
    }
    if(FormatableData<ParamName>::checkStringFormat(linearExpanstionFDF->getDataUTextEdit()->text(), false))
        linearExpanstionFDF->getDataUTextEdit()->setStyleSheet("");
    else
        linearExpanstionFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
    return true;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::fiducialTemperatureFDFKeyPressed(QKeyEvent *ev){
    uint len = fiducialTemperatureFDF->getDataUTextEdit()->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
        // Bezpośrednie połaczenie z KEY_PERIOD
    case Qt::Key_Period:
        {
        QLineEdit* te = fiducialTemperatureFDF->getDataUTextEdit();
        uint i = 0;
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
        fiducialTemperatureFDF->getDataUTextEdit()->del();
        break;
    case Qt::Key_Backspace:
        fiducialTemperatureFDF->getDataUTextEdit()->backspace();
        break;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    case Qt::Key_Minus:
    {
        QLineEdit *te = fiducialTemperatureFDF->getDataUTextEdit();
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
        for(uint i = 0; i < len; i++)
            if(fiducialTemperatureFDF->getDataUTextEdit()->text().at(i) == 'e')
                return true;
        fiducialTemperatureFDF->getDataUTextEdit()->insert(ev->text().toLower());
        break;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39)
            fiducialTemperatureFDF->getDataUTextEdit()->insert(ev->text());
        else
            return true;
    }
    if(FormatableData<ParamName>::checkStringFormat(fiducialTemperatureFDF->getDataUTextEdit()->text(), true))
        fiducialTemperatureFDF->getDataUTextEdit()->setStyleSheet("");
    else
        fiducialTemperatureFDF->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
    return true;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::addArrayRowReleased(){
    dataArray->addElement();
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getDensityTabLabel(){
    return densityTabLabel;
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getDynamicViscosityTabLabel(){
    return dynamicViscosityTabLabel;
}

template <typename ParamName>
MyList* MaterialsChangesWidget<ParamName>::getPressureList(){
    return pressureList;
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getPressureListAddButton(){
    return pressureListAddButton;
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getPressureListEditConfirmedButton(){
    return pressureListEditConfirmedButton;
}

template<typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getPressureListEditCancelledButton(){
    return pressureListEditCancelledButton;
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getAddArrayRow(){
    return addArrayRow;
}

template <typename ParamName>
QRadioButton* MaterialsChangesWidget<ParamName>::getTempDependencyButton(){
    return tempDependencyButton;
}

template <typename ParamName>
QRadioButton* MaterialsChangesWidget<ParamName>::getTempPresDependencyButton(){
    return tempPresDependencyButton;
}

template <typename ParamName>
QLineEdit* MaterialsChangesWidget<ParamName>::getKTextEdit(){
    return kTextEdit;
}

template <typename ParamName>
QScrollArea* MaterialsChangesWidget<ParamName>::getScrollArea(){
    return scrollArea;
}

template <typename ParamName>
MaterialsChangesWidgetArray* MaterialsChangesWidget<ParamName>::getDataArray(){
    return dataArray;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setIndexToDelete(uint *set){
    if(indexToDelete != nullptr)
        delete indexToDelete;
    indexToDelete = set;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setNewPressureIndex(uint set){
    newPressureIndex = set;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::tempDependencyButtonReleased(){
    if(!tempDependencyButton->isChecked()){
        if(((densityTabActive)? (densityDataHolder->getNumbOfPressures() != 0) : (dynamicViscosityDataHolder->getNumbOfPressures() != 0))){
            MyDialog dlg(QUESTION_DIALOG, QString(), QString("Wszystkie wprowadzane dane zostaną usunięte, z wyjątkiem wybranej tablicy. Czy na pewno chcesz zmienić zaleźność?"), true, QString(), QString(), QString(), this);
            switch(dlg.exec()){
            case NO_RESULT:
                return false;
            default:
                break;
            }
        }
            delete pressureUnitList;
            pressureUnitList = nullptr;
            delete pressureList;
            pressureList = nullptr;
            delete pressureListAddButton;
            pressureListAddButton = nullptr;
            delete pressureListEditAllButton;
            pressureListEditAllButton = nullptr;
            delete pressureListInfoLabel;
            pressureListInfoLabel = nullptr;
            delete pressureListExpandButton;
            pressureListExpandButton = nullptr;
            delete pressureScrollArea;
            pressureScrollArea = nullptr;
            if(dataArray)
            {
                temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                if((densityTabActive)? pressureListExpandedDensity : pressureListExpandedVisco)
                    dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                addArrayRow->setGeometry(7+ MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                dataArray->updateGeometry();
            }else{
                delete noPressureInfo;
                noPressureInfo = nullptr;
                temperatureArrayLabel = new QLabel(this);
                temperatureArrayLabel->show();
                dataArrayLabel = new QLabel(this);
                dataArrayLabel->show();
                createArrayObj(this);
                dataArray->show();
                addArrayRow = new QLabel(this);
                addArrayRow->show();
                scrollArea = new QScrollArea(this);
                temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(7+ MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET-7) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                if(densityTabActive)
                    dataArrayLabel->setText("Gęstość [kg/m3]");
                else
                    dataArrayLabel->setText("Lep. dynamiczna [Pas]");
                dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                scrollArea->setWidget(dataArray);
                scrollArea->setLayoutDirection(Qt::RightToLeft);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
                dataArray->updateGeometry();

            }
            if(densityTabActive){
                ushort pressureUnit = densityDataHolder->getPressureUnit();
                delete densityDataHolder;
                densityDataHolder = new MaterialsChangesWidgetDataHolder((kTextEdit->text().isEmpty()? 0 : kTextEdit->text().toInt()), 1, pressureUnit);
                if(dataArray && dataArray->checkLineEdits())
                    densityDataHolder->readDataFromArray(dataArray);
            }else{
                ushort pressureUnit = dynamicViscosityDataHolder->getPressureUnit();
                delete dynamicViscosityDataHolder;
                dynamicViscosityDataHolder = new MaterialsChangesWidgetDataHolder((kTextEdit->text().isEmpty())? 0 : kTextEdit->text().toInt(), 1, pressureUnit);
                if(dataArray && dataArray->checkLineEdits())
                    dynamicViscosityDataHolder->readDataFromArray(dataArray);
            }
            tempDependencyButton->setChecked(true);
            if(errorDlg && errorDlg->getSubstanceFileErrorsList()->densityTab == densityTabActive)
                errorDlg->close();
            if(densityTabActive){
                if(errorDenDlg && errorDenDlg->getSubstanceFileErrorsList()->densityTab == densityTabActive)
                {
                    errorDenDlg->close();
                }
            }else{
                if(errorViscoDlg && errorViscoDlg->getSubstanceFileErrorsList()->densityTab == densityTabActive)
                   errorViscoDlg->close();
                }
            return true;
    }
   return false;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::tempPresDependencyButtonReleased(){
    if(!tempPresDependencyButton->isChecked()){
        createPressureListObj(this);
        pressureUnitList = new QComboBox(this);
        pressureUnitList->addItem("at");
        pressureUnitList->addItem("atm");
        pressureUnitList->addItem("bar");
        pressureUnitList->addItem("psi");
        pressureUnitList->addItem("kPa");
        pressureUnitList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureUnitList->view()->installEventFilter(materialsChangesWidgetCatchEvents);
        pressureUnitList->setCurrentIndex((densityTabActive)? densityDataHolder->getPressureUnit() : dynamicViscosityDataHolder->getPressureUnit());
        static_cast<QWidget*>(pressureList)->setParent(this);
        if(dataArray)
        {
            pressureList->setAddingDataForExistingArray(true);
            temperatureArrayLabel->setText("Temperatura [C]");
            temperatureArrayLabel->setAlignment(Qt::AlignCenter);
            temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
            dataArrayLabel->setAlignment(Qt::AlignCenter);
            if(densityTabActive)
                dataArrayLabel->setText("Gęstość [kg/m3]");
            else
                dataArrayLabel->setText("Lep. dynamiczna [Pas]");
            dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
            scrollArea->setStyleSheet("background-color: transparent");
            scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
            scrollArea->setWidget(dataArray);
            scrollArea->setLayoutDirection(Qt::RightToLeft);
            scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
            addArrayRow->setAlignment(Qt::AlignCenter);
        }else{
            noPressureInfo = new QLabel(QString("Wybierz ciśnienie z listy"), this);
            noPressureInfo->show();
            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET+ MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
            noPressureInfo->setAlignment(Qt::AlignCenter);
        }
        pressureListInfoLabel = new QLabel(this);
        pressureListExpandButton = new QLabel(this);
        pressureListExpandButton->show();
        pressureScrollArea = new QScrollArea(this);
        pressureListAddButton = new QLabel(this);
        pressureListEditAllButton =  new QLabel(this);
        pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListInfoLabel->setText("Ciśnienie");
        pressureListInfoLabel->setAlignment(Qt::AlignCenter);
        pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
        pressureListExpandButton->setAlignment(Qt::AlignCenter);
        pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
        pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
        pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListAddButton->setAlignment(Qt::AlignCenter);
        pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
        pressureListAddButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
        pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
        pressureListEditAllButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10 + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListEditAllButton->setAlignment(Qt::AlignCenter);
        pressureListEditAllButton->setPixmap(QPixmap(EDIT_ALL_BUTTON_ICON));
        pressureListEditAllButton->installEventFilter(materialsChangesWidgetCatchEvents);
        pressureListEditAllButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
        pressureScrollArea->setStyleSheet("background-color: transparent");

        pressureScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
        pressureScrollArea->setWidget(pressureList);
        pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT + 2);
        pressureScrollArea->setLayoutDirection(Qt::RightToLeft);
        pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        if(densityTabActive){
            if(pressureListExpandedDensity){
                pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                temperatureArrayLabel->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(7+ MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH+ MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH - 7, dataArray->height());
                dataArray->updateGeometry();
                addArrayRow->setGeometry( 7+ MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                scrollArea->show();
                pressureListInfoLabel->show();
                pressureUnitList->show();
                pressureList->show();
                pressureListAddButton->show();
                pressureListEditAllButton->show();
                pressureScrollArea->show();
            }else{
                pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                scrollArea->show();
                pressureListInfoLabel->hide();
                pressureUnitList->hide();
                pressureList->hide();
                pressureListAddButton->hide();
                pressureListEditAllButton->hide();
                pressureScrollArea->hide();
            }
            densityDataHolder->setDependency(2);
        }else{
            if(pressureListExpandedVisco){
                temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH - 7, dataArray->height());
                dataArray->updateGeometry();
                addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                scrollArea->show();
                pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                pressureListInfoLabel->show();
                pressureUnitList->show();
                pressureList->show();
                pressureListAddButton->show();
                pressureListEditAllButton->show();
                pressureScrollArea->show();
            }else{
                pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                scrollArea->show();
                pressureListInfoLabel->hide();
                pressureUnitList->hide();
                pressureList->hide();
                pressureListAddButton->hide();
                pressureListEditAllButton->hide();
                pressureScrollArea->hide();
            }
            dynamicViscosityDataHolder->setDependency(2);
        }
        tempPresDependencyButton->setChecked(true);
        pressureList->addingElement();
        if(errorDlg && errorDlg->getSubstanceFileErrorsList()->densityTab == densityTabActive)
            errorDlg->close();
        if(densityTabActive){
            if(errorDenDlg && errorDenDlg->getSubstanceFileErrorsList()->densityTab == densityTabActive)
            {
                errorDenDlg->close();
            }
        }else{
            if(errorViscoDlg && errorViscoDlg->getSubstanceFileErrorsList()->densityTab == densityTabActive)
               errorViscoDlg->close();
            }
     return true;
   }
    return false;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::fluidCheckBoxReleased(){
    if(!fluidTypeCheckBox->isChecked()){
        fluidTypeCheckBox->setChecked(true);
        gaseTypeCheckBox->setChecked(false);
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::gaseCheckBoxReleased(){
    if(!gaseTypeCheckBox->isChecked()){
        fluidTypeCheckBox->setChecked(false);
        gaseTypeCheckBox->setChecked(true);
    }
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::kTextEditKeyPressed(QKeyEvent* ev){
    switch(ev->key()){
    case Qt::Key_Delete:
        kTextEdit->del();
        if(kTextEdit->text().isEmpty())
            kTextEdit->setText("0");
        return true;
    case Qt::Key_Backspace:
        kTextEdit->backspace();
        if(kTextEdit->text().isEmpty())
            kTextEdit->setText("0");
        return true;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39)
           return false;
        else
            return true;
    }
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::densityTabLabelReleased(){
    if(!densityTabActive){
        if(dataArray)
        {
            if(!dataArray->checkLineEdits()){
                moveToArrayError();
                return false;
            }else{
                dynamicViscosityDataHolder->readDataFromArray(dataArray);
            }
        }
        densityTabActive = true;
        if(densityDataHolder->getDependency() == 2){
            tempPresDependencyButton->setChecked(true);
            if(densityDataHolder->getNumbOfPressures() == 0){
                if(temperatureArrayLabel != nullptr)
                    delete temperatureArrayLabel;
                temperatureArrayLabel = nullptr;
                if(dataArrayLabel != nullptr)
                    delete dataArrayLabel;
                dataArrayLabel = nullptr;
                if(dataArray != nullptr)
                    delete dataArray;
                dataArray = nullptr;
                if(addArrayRow != nullptr)
                    delete addArrayRow;
                addArrayRow = nullptr;
                if(scrollArea != nullptr)
                    delete scrollArea;
                scrollArea = nullptr;
                if(noPressureInfo == nullptr)
                {
                    noPressureInfo = new QLabel(QString("Wybierz ciśnienie z listy"), this);
                    noPressureInfo->setAlignment(Qt::AlignCenter);
                    noPressureInfo->show();
                }
                if(pressureList == nullptr){
                    pressureListInfoLabel = new QLabel(this);
                    pressureListExpandButton = new QLabel(this);
                    pressureListExpandButton->show();
                    createPressureListObj(this);
                    pressureUnitList = new QComboBox(this);
                    pressureUnitList->addItem("at");
                    pressureUnitList->addItem("atm");
                    pressureUnitList->addItem("bar");
                    pressureUnitList->addItem("psi");
                    pressureUnitList->addItem("kPa");
                    pressureUnitList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureUnitList->view()->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea = new QScrollArea(this);
                    pressureListAddButton = new QLabel(this);
                    pressureListEditAllButton =  new QLabel(this);
                }
                    pressureUnitList->setCurrentIndex(densityDataHolder->getPressureUnit());
                    pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListInfoLabel->setText("Ciśnienie");
                    pressureListInfoLabel->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
                    pressureListExpandButton->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListAddButton->setAlignment(Qt::AlignCenter);
                    pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
                    pressureListAddButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                    pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureListEditAllButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10 + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListEditAllButton->setAlignment(Qt::AlignCenter);
                    pressureListEditAllButton->setPixmap(QPixmap(EDIT_ALL_BUTTON_ICON));
                    pressureListEditAllButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureListEditAllButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                    pressureScrollArea->setStyleSheet("background-color: transparent");

                    pressureScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar::vertical{background:transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                    pressureScrollArea->setWidget(pressureList);
                    pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT + 2);
                    pressureScrollArea->setLayoutDirection(Qt::RightToLeft);
                    pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                    pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    if(pressureListExpandedDensity){
                            pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                            // NO_PRESSURE_SET_SHOW
                            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH+ MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->show();
                            pressureUnitList->show();
                            pressureList->show();
                            pressureListAddButton->show();
                            pressureListEditAllButton->show();
                            pressureScrollArea->show();
                        }else{
                            pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                            // NO_PRESSURE_SET_HIDE
                            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->hide();
                            pressureUnitList->hide();
                            pressureList->hide();
                            pressureListAddButton->hide();
                            pressureListEditAllButton->hide();
                            pressureScrollArea->hide();
                    }
                pressureList->createElements(densityDataHolder);
            }else{
                if(noPressureInfo != nullptr){
                    delete noPressureInfo;
                    noPressureInfo = nullptr;
                }
                if(dataArray == nullptr){
                    temperatureArrayLabel = new QLabel(this);
                    temperatureArrayLabel->show();
                    dataArrayLabel = new QLabel(this);
                    dataArrayLabel->show();
                    createArrayObj(this);
                    dataArray->show();
                    addArrayRow = new QLabel(this);
                    addArrayRow->show();
                    scrollArea = new QScrollArea(this);
                }
                if(pressureList == nullptr){
                    pressureListInfoLabel = new QLabel(this);
                    pressureListExpandButton = new QLabel(this);
                    pressureListExpandButton->show();
                    createPressureListObj(this);
                    pressureUnitList = new QComboBox(this);
                    pressureUnitList->addItem("at");
                    pressureUnitList->addItem("atm");
                    pressureUnitList->addItem("bar");
                    pressureUnitList->addItem("psi");
                    pressureUnitList->addItem("kPa");
                    pressureUnitList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureUnitList->view()->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea = new QScrollArea(this);
                    pressureListAddButton = new QLabel(this);
                    pressureListEditAllButton = new QLabel(this);
                }
                pressureUnitList->setCurrentIndex(densityDataHolder->getPressureUnit());
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                dataArrayLabel->setText("Gęstość [kg/m3]");
                dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar::vertical{background:transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                scrollArea->setWidget(dataArray);
                scrollArea->setLayoutDirection(Qt::RightToLeft);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->show();
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->setAlignment(Qt::AlignCenter);
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
                dataArray->setCurrentIndex(densityDataHolder->getSelectedIndex());
                pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                pressureListInfoLabel->setText("Ciśnienie");
                pressureListInfoLabel->setAlignment(Qt::AlignCenter);
                pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
                pressureListExpandButton->setAlignment(Qt::AlignCenter);
                pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
                pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                pressureListAddButton->setAlignment(Qt::AlignCenter);
                pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
                pressureListAddButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
                pressureListEditAllButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10 + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                pressureListEditAllButton->setAlignment(Qt::AlignCenter);
                pressureListEditAllButton->setPixmap(QPixmap(EDIT_ALL_BUTTON_ICON));
                pressureListEditAllButton->installEventFilter(materialsChangesWidgetCatchEvents);
                pressureListEditAllButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                pressureScrollArea->setStyleSheet("background-color: transparent");
                pressureScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                pressureScrollArea->setWidget(pressureList);
                pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT + 2);
                pressureScrollArea->setLayoutDirection(Qt::RightToLeft);
                pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                if(pressureListExpandedDensity){
                    temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH - 7, dataArray->height());
                    addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                    pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                    pressureListInfoLabel->show();
                    pressureUnitList->show();
                    pressureList->show();
                    pressureListAddButton->show();
                    pressureListEditAllButton->show();
                    pressureScrollArea->show();
                }else{
                            // ARRAY_SET_HIDE
                        temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                        dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                        dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                        addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                        scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                            pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                            pressureListInfoLabel->hide();
                            pressureUnitList->hide();
                            pressureList->hide();
                            pressureListAddButton->hide();
                            pressureListEditAllButton->hide();
                            pressureScrollArea->hide();
               }
                pressureList->createElements(densityDataHolder);
                dataArray->createElements(densityDataHolder);
            }
        }else{
            tempDependencyButton->setChecked(true);
            if(pressureList != nullptr){
                delete pressureListInfoLabel;
                pressureListInfoLabel = nullptr;
                delete pressureListExpandButton;
                pressureListExpandButton = nullptr;
                delete pressureList;
                pressureList = nullptr;
                delete pressureListAddButton;
                pressureListAddButton = nullptr;
                delete pressureListEditAllButton;
                pressureListEditAllButton = nullptr;
                delete pressureScrollArea;
                pressureScrollArea = nullptr;
           }
            if(noPressureInfo != nullptr){
                delete noPressureInfo;
                noPressureInfo = nullptr;
                temperatureArrayLabel = new QLabel(this);
                temperatureArrayLabel->show();
                dataArrayLabel = new QLabel(this);
                dataArrayLabel->show();
                createArrayObj(this);
                dataArray->show();
                addArrayRow = new QLabel(this);
                addArrayRow->show();
                scrollArea = new QScrollArea(this);
            }
                // ARRAY_SET_HIDE
                temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                dataArrayLabel->setText("Gęstość [kg/m3]");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                scrollArea->setWidget(dataArray);
                scrollArea->setLayoutDirection(Qt::RightToLeft);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->show();
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
            dataArray->setCurrentIndex(densityDataHolder->getSelectedIndex());
            dataArray->createElements(densityDataHolder);
        }
    }
    return true;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::dynamicViscosityTabLabelReleased(){
    if(densityTabActive){
        if(dataArray){
            if(dataArray->checkLineEdits())
                densityDataHolder->readDataFromArray(dataArray);
            else{
                moveToArrayError();
                return false;
            }
        }
        densityTabActive = false;
        if(dynamicViscosityDataHolder->getDependency() == 2){
            tempPresDependencyButton->setChecked(true);
            if(dynamicViscosityDataHolder->getNumbOfPressures() == 0){
                if(temperatureArrayLabel != nullptr)
                    delete temperatureArrayLabel;
                temperatureArrayLabel = nullptr;
                if(dataArrayLabel != nullptr)
                    delete dataArrayLabel;
                dataArrayLabel = nullptr;
                if(dataArray != nullptr)
                    delete dataArray;
                dataArray = nullptr;
                if(addArrayRow != nullptr)
                    delete addArrayRow;
                addArrayRow = nullptr;
                if(scrollArea != nullptr)
                    delete scrollArea;
                scrollArea = nullptr;
                if(noPressureInfo == nullptr)
                {
                    noPressureInfo = new QLabel(QString("Wybierz ciśnienie z listy"), this);
                    noPressureInfo->setAlignment(Qt::AlignCenter);
                    noPressureInfo->show();
                }
                if(pressureList == nullptr){
                    pressureListInfoLabel = new QLabel(this);
                    pressureListExpandButton = new QLabel(this);
                    pressureListExpandButton->show();
                    createPressureListObj(this);
                    pressureUnitList = new QComboBox(this);
                    pressureUnitList->addItem("at");
                    pressureUnitList->addItem("atm");
                    pressureUnitList->addItem("bar");
                    pressureUnitList->addItem("psi");
                    pressureUnitList->addItem("kPa");
                    pressureUnitList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureUnitList->view()->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea = new QScrollArea(this);
                    pressureListAddButton = new QLabel(this);
                    pressureListEditAllButton = new QLabel(this);
                }
                pressureUnitList->setCurrentIndex(dynamicViscosityDataHolder->getPressureUnit());
                    pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListInfoLabel->setText("Ciśnienie");
                    pressureListInfoLabel->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
                    pressureListExpandButton->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListAddButton->setAlignment(Qt::AlignCenter);
                    pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
                    pressureListAddButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                    pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureListEditAllButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10 + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListEditAllButton->setAlignment(Qt::AlignCenter);
                    pressureListEditAllButton->setPixmap(QPixmap(EDIT_ALL_BUTTON_ICON));
                    pressureListEditAllButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                    pressureListEditAllButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea->setStyleSheet("background-color: transparent");

                    pressureScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                    pressureScrollArea->setWidget(pressureList);
                    pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT + 2);
                    pressureScrollArea->setLayoutDirection(Qt::RightToLeft);
                    pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                    pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    if(pressureListExpandedVisco){
                            pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                            // NO_PRESSURE_SET_SHOW
                            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->show();
                            pressureUnitList->show();
                            pressureList->show();
                            pressureListAddButton->show();
                            pressureListEditAllButton->show();
                            pressureScrollArea->show();
                    }else{
                            pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                            // NO_PRESSURE_SET_HIDE
                            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->hide();
                            pressureUnitList->hide();
                            pressureList->hide();
                            pressureListAddButton->hide();
                            pressureListEditAllButton->hide();
                            pressureScrollArea->hide();

                }
                pressureList->createElements(dynamicViscosityDataHolder);
            }else{
                if(noPressureInfo != nullptr){
                    delete noPressureInfo;
                    noPressureInfo = nullptr;
                }
                if(pressureList == nullptr){
                    pressureListInfoLabel = new QLabel(this);
                    pressureListExpandButton = new QLabel(this);
                    pressureListExpandButton->show();
                    createPressureListObj(this);
                    pressureUnitList = new QComboBox(this);
                    pressureUnitList->addItem("at");
                    pressureUnitList->addItem("atm");
                    pressureUnitList->addItem("bar");
                    pressureUnitList->addItem("psi");
                    pressureUnitList->addItem("kPa");
                    pressureUnitList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureUnitList->view()->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea = new QScrollArea(this);
                    pressureListAddButton = new QLabel(this);
                    pressureListEditAllButton = new QLabel(this);
                }
                pressureUnitList->setCurrentIndex(dynamicViscosityDataHolder->getPressureUnit());
                    pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - MATERIALS_CHANGES_WIDGET_UNIT_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListInfoLabel->setText("Ciśnienie");
                    pressureListInfoLabel->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
                    pressureListExpandButton->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListAddButton->setAlignment(Qt::AlignCenter);
                    pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
                    pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureListAddButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                    pressureListEditAllButton->setAlignment(Qt::AlignCenter);
                    pressureListEditAllButton->setPixmap(QPixmap(EDIT_ALL_BUTTON_ICON));
                    pressureListEditAllButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                    pressureListEditAllButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea->setStyleSheet("background-color: transparent");

                    pressureScrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                    pressureScrollArea->setWidget(pressureList);
                    pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT + 2);
                    pressureScrollArea->setLayoutDirection(Qt::RightToLeft);
                    pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                    pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    if(dataArray == nullptr){
                        temperatureArrayLabel = new QLabel(this);
                        temperatureArrayLabel->show();
                        dataArrayLabel = new QLabel(this);
                        dataArrayLabel->show();
                        createArrayObj(this);
                        dataArray->show();
                        addArrayRow = new QLabel(this);
                        addArrayRow->show();
                        scrollArea = new QScrollArea(this);
                    }
                    temperatureArrayLabel->setText("Temperatura [C]");
                    temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                    temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                    dataArrayLabel->setAlignment(Qt::AlignCenter);
                    dataArrayLabel->setText("Lep. dynamiczna [Pas]");
                    dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                    scrollArea->setStyleSheet("background-color: transparent");
                    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                    scrollArea->setWidget(dataArray);
                    scrollArea->setLayoutDirection(Qt::RightToLeft);
                    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    scrollArea->show();
                    addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                    addArrayRow->setAlignment(Qt::AlignCenter);
                    addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
                    dataArray->setCurrentIndex(dynamicViscosityDataHolder->getSelectedIndex());
                    if(pressureListExpandedVisco){
                            pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                            temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH - 7, dataArray->height());
                            addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                            pressureListInfoLabel->show();
                            pressureUnitList->show();
                            pressureList->show();
                            pressureListAddButton->show();
                            pressureListEditAllButton->show();
                            pressureScrollArea->show();
                    }else{
                            pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                            // ARRAY_SET_HIDE
                            temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                            addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                            pressureListInfoLabel->hide();
                            pressureUnitList->hide();
                            pressureList->hide();
                            pressureListAddButton->hide();
                            pressureListEditAllButton->hide();
                            pressureScrollArea->hide();
                    }
                    pressureList->createElements(dynamicViscosityDataHolder);
                    dataArray->createElements(dynamicViscosityDataHolder);
           }
        }else{
            tempDependencyButton->setChecked(true);
            if(pressureList != nullptr){
                delete pressureListInfoLabel;
                pressureListInfoLabel = nullptr;
                delete pressureListExpandButton;
                pressureListExpandButton = nullptr;
                delete pressureList;
                pressureList = nullptr;
                delete pressureListAddButton;
                pressureListAddButton = nullptr;
                delete pressureListEditAllButton;
                pressureListEditAllButton = nullptr;
                delete pressureScrollArea;
                pressureScrollArea = nullptr;
           }
            if(noPressureInfo != nullptr){
                delete noPressureInfo;
                noPressureInfo = nullptr;
                temperatureArrayLabel = new QLabel(this);
                temperatureArrayLabel->show();
                dataArrayLabel = new QLabel(this);
                dataArrayLabel->show();
                createArrayObj(this);
                dataArray->show();
                addArrayRow = new QLabel(this);
                addArrayRow->show();
                scrollArea = new QScrollArea(this);
            }
                // ARRAY_SET_HIDE
                temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                dataArrayLabel->setText("Lep. dynamiczna [Pas]");
                dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
                scrollArea->setWidget(dataArray);
                scrollArea->setLayoutDirection(Qt::RightToLeft);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->show();
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->setAlignment(Qt::AlignCenter);
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
            dataArray->setCurrentIndex(dynamicViscosityDataHolder->getSelectedIndex());
            dataArray->createElements(dynamicViscosityDataHolder);

        }
    }
    return true;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListAddButtonReleased(){
    pressureList->addingElement();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListEditConfirmedButtonReleased(){
    if(!pressureList->getAddingData())
        pressureList->editingDataConfirmed();
    else
        pressureList->addingElementConfirmed();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListEditCancelledButtonReleased(){
    if(!pressureList->getAddingData())
        pressureList->editingDataCanceled();
    else
        pressureList->addingElementCanceled();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListEditConfirmedButtonHighlight(){
    pressureListEditConfirmedButton->setStyleSheet("background-color: rgba(230,230,230, 0.6);");
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListEditConfirmedButtonLowlight(){
    pressureListEditConfirmedButton->setStyleSheet("background-color: rgba(245,245,245, 0.6);");
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListEditCancelledButtonHighlight(){
    pressureListEditCancelledButton->setStyleSheet("background-color: rgba(230,230,230, 0.6);");
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListEditCancelledButtonLowlight(){
    pressureListEditCancelledButton->setStyleSheet("background-color: rgba(245,245,245, 0.6);");
}

template <typename ParamName>
MaterialsChangesWidgetDataHolder* MaterialsChangesWidget<ParamName>::getDensityDataHolder(){
    return densityDataHolder;
}

template <typename ParamName>
MaterialsChangesWidgetDataHolder* MaterialsChangesWidget<ParamName>::getDynamicViscosityDataHolder(){
    return dynamicViscosityDataHolder;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::setPressureIndexToDelete(uint *set){
    if(pressureIndexToDelete != nullptr)
        delete pressureIndexToDelete;
    pressureIndexToDelete = set;
}

template <typename ParamName>
QScrollArea* MaterialsChangesWidget<ParamName>::getPressureScrollArea(){
    return pressureScrollArea;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createArray(){
    if(noPressureInfo != nullptr){
        delete noPressureInfo;
        noPressureInfo = nullptr;
    }
    temperatureArrayLabel = new QLabel(this);
    temperatureArrayLabel->show();
    dataArrayLabel = new QLabel(this);
    dataArrayLabel->show();
    createArrayObj(this);
    addArrayRow = new QLabel(this);
    addArrayRow->show();
    scrollArea = new QScrollArea(this);
    if(densityTabActive){
        dataArrayLabel->setText("Gęstość [kg/m3]");
        if(pressureListExpandedDensity){
            temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH - 7, dataArray->height());
            addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
        }else{
            // ARRAY_SET_HIDE
            temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
            addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
        }
    }
    else{
        dataArrayLabel->setText("Lep. dynamiczna [Pas]");
        if(pressureListExpandedVisco){
            temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH - 7, dataArray->height());
            addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
        }else{
            // ARRAY_SET_HIDE
            temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
            addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
        }
    }
    temperatureArrayLabel->setText("Temperatura [C]");
    temperatureArrayLabel->setAlignment(Qt::AlignCenter);
    temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
    dataArrayLabel->setAlignment(Qt::AlignCenter);
    dataArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
    scrollArea->setStyleSheet("background-color: transparent");
    scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white;   border: 1px solid black;}");
    scrollArea->setWidget(dataArray);
    scrollArea->setLayoutDirection(Qt::RightToLeft);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->show();
    addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
    addArrayRow->setAlignment(Qt::AlignCenter);
    addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
}

template <typename ParamName>
Config* MaterialsChangesWidget<ParamName>::getConfig(){
    return config;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListExpandButtonReleased(){
    if(densityTabActive){
        if(pressureListExpandedDensity)
            pressureListExpandedDensity = false;
        else
            pressureListExpandedDensity = true;
        if(tempPresDependencyButton->isChecked()){
            if(pressureListExpandedDensity){
                if(dataArray){
                    temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH - 7, dataArray->height());
                    addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                    dataArray->updateGeometry();
                }else{
                    noPressureInfo->setGeometry(MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }
                    pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                    pressureListInfoLabel->show();
                    pressureUnitList->show();
                    pressureList->show();
                    pressureListAddButton->show();
                    pressureListEditAllButton->show();
                    pressureScrollArea->show();
            }else{
                if(dataArray){
                    temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET - 7), dataArray->height());
                    addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                    dataArray->updateGeometry();
                }else{
                    noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }
                    pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                    pressureListInfoLabel->hide();
                    pressureUnitList->hide();
                    pressureList->hide();
                    pressureListAddButton->hide();
                    pressureListEditAllButton->hide();
                    pressureScrollArea->hide();
            }
        }
    }else{
        if(pressureListExpandedVisco)
            pressureListExpandedVisco = false;
        else
            pressureListExpandedVisco = true;
        if(tempPresDependencyButton->isChecked()){
            if(pressureListExpandedVisco){
                if(dataArray){
                    temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH - 7, dataArray->height());
                    dataArray->updateGeometry();
                    addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                }else{
                    noPressureInfo->setGeometry(MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }
                    pressureListExpandButton->setPixmap(QPixmap("Icons\\rightExtendArrowIcon.png"));
                    pressureListInfoLabel->show();
                    pressureUnitList->show();
                    pressureList->show();
                    pressureListAddButton->show();
                    pressureListEditAllButton->show();
                    pressureScrollArea->show();
            }else{
                if(dataArray){
                    temperatureArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(7 + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 7), dataArray->height());
                    dataArray->updateGeometry();
                    addArrayRow->setGeometry(7+MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT + 2);
                }else{
                    noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }
                    pressureListExpandButton->setPixmap(QPixmap("Icons\\leftExtendArrowIcon.png"));
                    pressureListInfoLabel->hide();
                    pressureUnitList->hide();
                    pressureList->hide();
                    pressureListAddButton->hide();
                    pressureListEditAllButton->hide();
                    pressureScrollArea->hide();
            }
        }
    }
}

template <typename ParamName>
QLabel* MaterialsChangesWidget<ParamName>::getPressureListExpandButton(){
    return pressureListExpandButton;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::isPressureListExpanded(){
    return (densityTabActive)? pressureListExpandedDensity : pressureListExpandedVisco;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::canceledAddingElement(){
    if(pressureList->getAddingDataForExistingArray()){
        tempDependencyButtonReleased();
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createArrayObj(MaterialsChangesWidget<FlangeMaterialsFileParams> *mcw){

}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createArrayObj(MaterialsChangesWidget<PipeMaterialsFileParams> *mcw){

}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createArrayObj(MaterialsChangesWidget<SubstancesParamsFileParam> *mcw){
    dataArray = new MaterialsChangesWidgetArray(mcw);
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createPressureListObj(MaterialsChangesWidget<FlangeMaterialsFileParams> *mcw){

}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createPressureListObj(MaterialsChangesWidget<PipeMaterialsFileParams> *mcw){

}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createPressureListObj(MaterialsChangesWidget<SubstancesParamsFileParam> *mcw){
    pressureList = new MyList(mcw);
}

template <typename ParamName>
QTimer* MaterialsChangesWidget<ParamName>::getTimer(){
    return reloadTimer;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::moveToArrayError(){
    auto temperatureData = dataArray->getTemperatureTextEdit();
    auto data = dataArray->getDataTextEdit();
    auto numbOfRows = dataArray->getRow();
    for(uint i = 0; i < numbOfRows; i++){
        if(!(*(temperatureData + i))->isFormatValid() || !(*(data + i))->isFormatValid()){
            if(i > 1)
                scrollArea->verticalScrollBar()->setValue((int)(i - 2) * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            else
                scrollArea->verticalScrollBar()->setValue(0);
        }
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::moveToArrayError(int index){
    if(index > 0)
        scrollArea->verticalScrollBar()->setValue((index - 2) * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    else
        scrollArea->verticalScrollBar()->setValue(0);
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::moveToAddingElement(){
    pressureScrollArea->verticalScrollBar()->setValue(pressureScrollArea->maximumHeight());
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::moveToAddingArrayElement(){
    scrollArea->verticalScrollBar()->setValue(scrollArea->maximumHeight());
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureUnitListLeft(){
    if(pressureUnitList->currentIndex() != ((densityTabActive)? densityDataHolder->getPressureUnit() : dynamicViscosityDataHolder->getPressureUnit())){
        double correctionFactor = 1;
        if(densityTabActive){
            correctionFactor = densityDataHolder->getReverseCorrectionFactorValue();
            densityDataHolder->setPressureUnit((ushort)pressureUnitList->currentIndex());
            correctionFactor *= densityDataHolder->getCorrectionFactorValue();
        }
        else{
            correctionFactor = dynamicViscosityDataHolder->getReverseCorrectionFactorValue();
            dynamicViscosityDataHolder->setPressureUnit((ushort)pressureUnitList->currentIndex());
            correctionFactor *= dynamicViscosityDataHolder->getCorrectionFactorValue();
        }
        pressureList->rewriteList(correctionFactor);
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListEditAllButtonReleased(){
    pressureList->editingAllMode();
    createEditDesitionsButtons();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createEditDesitionsButtons(){
    pressureListEditConfirmedButton = new QLabel(QString(MATERIALS_CHANGES_WIDGET_EDIT_CONFIRMED_TEXT), this);
    pressureListEditConfirmedButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, height() - 2 *MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_EDIT_OPERATIONS_LABEL_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    pressureListEditConfirmedButton->setAlignment(Qt::AlignCenter);
    pressureListEditConfirmedButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
    pressureListEditConfirmedButton->installEventFilter(materialsChangesWidgetCatchEvents);
    pressureListEditConfirmedButton->show();
    pressureListEditCancelledButton = new QLabel(QString(MATERIALS_CHANGES_WIDGET_EDIT_CANCELLED_TEXT), this);
    pressureListEditCancelledButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_WIDGET_EDIT_OPERATIONS_LABEL_WIDTH * 0.5, height() - 2 *MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_EDIT_OPERATIONS_LABEL_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    pressureListEditCancelledButton->setAlignment(Qt::AlignCenter);
    pressureListEditCancelledButton->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
    pressureListEditCancelledButton->installEventFilter(materialsChangesWidgetCatchEvents);
    pressureListEditCancelledButton->show();
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::deleteEditDesitionsButtons(){
    delete pressureListEditConfirmedButton;
    pressureListEditConfirmedButton = nullptr;
    delete pressureListEditCancelledButton;
    pressureListEditCancelledButton = nullptr;
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::moveToEditingError(uint index){
    scrollArea->verticalScrollBar()->setValue((int)(index) * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::moveToError(bool densityTab, double pressure, double temp){
    if(pressureList){
        if(pressureList->getEnteringData()){
            MyDialog dlg(QUESTION_DIALOG, QString("Błąd"), QString("Zakończ modyfikację danych."), false, QString("Ok"), QString(), QString(), this);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return;
        }
    }
    if(densityTab){
        if(!densityTabActive)
            if(!dynamicViscosityTabLabelReleased())
                return;
    }else{
        if(densityTabActive)
            if(!densityTabLabelReleased())
                return;
    }
    if(pressureList){
      int index = pressureList->isPressureExist(pressure);
      if(index != -1){
            if(!pressureList->dataElementClicked(static_cast<uint>(index)))
                return;
            moveToEditingError(index);
            if(dataArray){
                int tempIndex = dataArray->checkTemperatureExist(temp);
                if(tempIndex != -1){
                    moveToArrayError(tempIndex);
                    (*(dataArray->getDataTextEdit() + tempIndex))->getDataUTextEdit()->setStyleSheet("border: 1px solid yellow;");
                }else{
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd"), QString("Podana temperatura nie istnieje."), false, QString("Ok"), QString(), QString(), this);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                }
            }else{
                MyDialog dlg(QUESTION_DIALOG, QString("Błąd"), QString("Podana temperatura nie istnieje."), false, QString("Ok"), QString(), QString(), this);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
            }
      }else{
          MyDialog dlg(QUESTION_DIALOG, QString("Błąd"), QString("Podane ciśnienie nie istnieje."), false, QString("Ok"), QString(), QString(), this);
          dlg.setWindowModality(Qt::ApplicationModal);
          dlg.show();
          dlg.exec();
      }
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString("Błąd"), QString("Podane ciśnienie nie istnieje."), false, QString("Ok"), QString(), QString(), this);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::moveToError(bool densityTab, double temp){
    if(pressureList && pressureList->getEnteringData()){
        MyDialog dlg(QUESTION_DIALOG, QString("Błąd"), QString("Zakończ modyfikację danych."), false, QString("Ok"), QString(), QString(), this);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return;
    }
        if(densityTab){
            if(!densityTabActive)
                if(!dynamicViscosityTabLabelReleased())
                    return;
        }else{
            if(densityTabActive)
                if(!densityTabLabelReleased())
                    return;
        }
        if(tempDependencyButton->isChecked()){
            if(dataArray){
                int tempIndex = dataArray->checkTemperatureExist(temp);
                if(tempIndex != -1){
                    moveToArrayError(tempIndex);
                    (*(dataArray->getDataTextEdit() + tempIndex))->getDataUTextEdit()->setStyleSheet("border: 1px solid yellow;");
                }else{
                    MyDialog dlg(QUESTION_DIALOG, QString("Błąd"), QString("Podana temperatura nie istnieje."), false, QString("Ok"), QString(), QString(), this);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                }
            }else{
                MyDialog dlg(QUESTION_DIALOG, QString("Błąd"), QString("Podana temperatura nie istnieje."), false, QString("Ok"), QString(), QString(), this);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
            }
        }
}

template class MaterialsChangesWidget<FlangeMaterialsFileParams>;
template class MaterialsChangesWidget<PipeMaterialsFileParams>;
template class MaterialsChangesWidget<SubstancesParamsFileParam>;
