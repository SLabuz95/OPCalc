#ifndef MATERIALSCHANGESWIDGET_IMPL_HPP
#define MATERIALSCHANGESWIDGET_IMPL_HPP
#include"materialschangeswidget.hpp"
#include"mydialog.hpp"
#include<QKeyEvent>
#include<QMessageBox>
template <typename ParamName> class MaterialsChangesWidget;
template <typename ParamName>
MaterialsChangesWidget<ParamName>::MaterialsChangesWidget(MaterialsChangesWidgetStats stat, MaterialsChangesWidgetType setType, Config* setCnf){
    materialsChangesWidgetStats = new MaterialsChangesWidgetStats[2];
    config = setCnf;
    *materialsChangesWidgetStats = stat;
    *(materialsChangesWidgetStats + 1) = stat;
    type = setType;
    materialsChangesWidgetCatchEvents = new MaterialsChangesWidgetCatchEvents<ParamName>(this);
    if(type == SUBSTANCE_TYPE){
        densityDataHolder = new MaterialsChangesWidgetDataHolder(0, 2);
        dynamicViscosityDataHolder = new MaterialsChangesWidgetDataHolder(0, 2);
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
    connect(reloadTimer, SIGNAL(timeout()), this, SLOT(reloadTimerTask()));
    reloadTimer->setInterval(1);
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
        setFixedSize(MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH,MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_HEIGHT);
    else
        setFixedSize(MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT);

}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::changeWindowStyle(){
    if(type == SUBSTANCE_TYPE && *materialsChangesWidgetStats != CHOOSING)
        setFixedSize(MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH, MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_HEIGHT);
    else
        setFixedSize(MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT);
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
    nameTextEdit = new QLineEdit(this);
    nameTextEdit->show();
    switch(type){
    case FLANGE_MATERIAL_TYPE:
        linearExpanstionLabel = new QLabel(this);
        linearExpanstionLabel->show();
        linearExpanstionFDF = new FormatableDataField<ParamName>(true, nullptr, this, QString());
        linearExpanstionFDF->show();
        linearExpanstionUnitLabel = new QLabel(this);
        linearExpanstionUnitLabel->show();
        fiducialTemperatureLabel = new QLabel(this);
        fiducialTemperatureLabel->show();
        fiducialTemperatureFDF = new FormatableDataField<ParamName>(true, nullptr, this, QString());
        fiducialTemperatureFDF->show();
        fiducialTemperatureUnitLabel = new QLabel(this);
        fiducialTemperatureUnitLabel->show();
        break;
    case PIPE_MATERIAL_TYPE:
        linearExpanstionLabel = new QLabel(this);
        linearExpanstionLabel->show();
        linearExpanstionFDF = new FormatableDataField<ParamName>(true, nullptr, this, QString());
        linearExpanstionFDF->show();
        linearExpanstionUnitLabel = new QLabel(this);
        linearExpanstionUnitLabel->show();
        fiducialTemperatureLabel = new QLabel(this);
        fiducialTemperatureLabel->show();
        fiducialTemperatureFDF = new FormatableDataField<ParamName>(true, nullptr, this, QString());
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
        pressureList = new MyList(this);
        static_cast<QWidget*>(pressureList)->setParent(this);
        pressureScrollArea = new QScrollArea(this);
        pressureListAddButton = new QLabel(this);
        noPressureInfo = new QLabel(QString("Wybierz ciśnienie z listy"), this);
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
        dependencyButtonGroup = new QButtonGroup(this);
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
        dataArray = new MaterialsChangesWidgetArray(this);
        static_cast<QWidget*>(dataArray)->setParent(this);
        dataArray->show();
        addArrayRow = new QLabel(this);
        addArrayRow->show();
        scrollArea = new QScrollArea(this);
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::createLayout(){
    titleLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LIST_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    if(type == SUBSTANCE_TYPE && *materialsChangesWidgetStats != CHOOSING)
        closeButton->setGeometry(MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - (MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5 - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    else
        closeButton->setGeometry(MATERIALS_CHANGES_WIDGET_DEFAULT_WIDTH - (MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5 - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    closeButton->setPixmap(QPixmap(CLOSE_BUTTON_ICON));
    if(type != SUBSTANCE_TYPE)
    {
        okButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + MATERIALS_CHANGES_LIST_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        okButton->setText("Zatwierdź");
        okButton->setAlignment(Qt::AlignCenter);
        cancelButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_BIG_BUTTONS_WIDTH + MATERIALS_CHANGES_BIG_BUTTONS_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + MATERIALS_CHANGES_LIST_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        cancelButton->setText("Anuluj");
        cancelButton->setAlignment(Qt::AlignCenter);
    }else{
        okButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 8 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 8 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + 5  + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        okButton->setText("Zatwierdź");
        okButton->setAlignment(Qt::AlignCenter);
        cancelButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_BIG_BUTTONS_WIDTH + MATERIALS_CHANGES_BIG_BUTTONS_OFFSET, 8 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 8 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + 5  + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        cancelButton->setText("Anuluj");
        cancelButton->setAlignment(Qt::AlignCenter);
    }
    switch(*materialsChangesWidgetStats){
    case CHOOSING:
        okButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + MATERIALS_CHANGES_LIST_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        okButton->setText("Zatwierdź");
        okButton->setAlignment(Qt::AlignCenter);
        cancelButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_BIG_BUTTONS_WIDTH + MATERIALS_CHANGES_BIG_BUTTONS_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + MATERIALS_CHANGES_LIST_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_BIG_BUTTONS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        cancelButton->setText("Anuluj");
        cancelButton->setAlignment(Qt::AlignCenter);
        list->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LIST_LABEL_WIDTH, MATERIALS_CHANGES_LIST_HEIGHT);
        switch(type){
        case FLANGE_MATERIAL_TYPE:
            titleLabel->setText("Wybierz materiał kryzy:");
            MaterialsInfoFiles::readFlangeMaterials(this);
            break;
        case PIPE_MATERIAL_TYPE:
            titleLabel->setText("Wybierz materiał rurociągu:");
            MaterialsInfoFiles::readPipeMaterials(this);
            break;
        case SUBSTANCE_TYPE:
            titleLabel->setText("Wybierz substancje:");
            MaterialsInfoFiles::readSubstances(this);
            break;
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
    nameLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    nameLabel->setText(MATERIALS_CHANGES_SUBSTANCE_NAME_LABEL_TEXT);
    nameTextEdit->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    nameTextEdit->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
    switch(type){
    case FLANGE_MATERIAL_TYPE:
        titleLabel->setText("Dodawanie materiału kryzy");
        linearExpanstionLabel->setText(MATERIALS_CHANGES_LINEAR_EXPANSTION_LABEL_TEXT);
        linearExpanstionLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        linearExpanstionUnitLabel->setText(MATERIALS_CHANGES_LINEAR_EXPANSTION_UNIT_LABEL_TEXT);
        linearExpanstionUnitLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH + MATERIALS_CHANGES_UNIT_LABEL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_UNIT_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureLabel->setText(MATERIALS_CHANGES_FIDUCIAL_TEMPEARTURE_LABEL_TEXT);
        fiducialTemperatureLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        fiducialTemperatureUnitLabel->setText(MATERIALS_CHANGES_FIDUCIAL_TEMPEARTURE_UNIT_LABEL_TEXT);
        fiducialTemperatureUnitLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH + MATERIALS_CHANGES_UNIT_LABEL_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_UNIT_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        break;
    case PIPE_MATERIAL_TYPE:
        titleLabel->setText("Dodawanie materiału rurociągu");
        linearExpanstionLabel->setText(MATERIALS_CHANGES_LINEAR_EXPANSTION_LABEL_TEXT);
        linearExpanstionLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        linearExpanstionFDF->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        linearExpanstionUnitLabel->setText(MATERIALS_CHANGES_LINEAR_EXPANSTION_UNIT_LABEL_TEXT);
        linearExpanstionUnitLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH + MATERIALS_CHANGES_UNIT_LABEL_OFFSET, 2 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 2 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_UNIT_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureLabel->setText(MATERIALS_CHANGES_FIDUCIAL_TEMPEARTURE_LABEL_TEXT);
        fiducialTemperatureLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        fiducialTemperatureFDF->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        fiducialTemperatureUnitLabel->setText(MATERIALS_CHANGES_FIDUCIAL_TEMPEARTURE_UNIT_LABEL_TEXT);
        fiducialTemperatureUnitLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_LEFT_ELEMENTS_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH + MATERIALS_CHANGES_UNIT_LABEL_OFFSET, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 3 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_UNIT_LABEL_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
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
        densityTabLabel->setAlignment(Qt::AlignCenter);
        dynamicViscosityTabLabel->setGeometry(MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH * 0.5, 3 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 4 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        dynamicViscosityTabLabel->setText("Lepkość dynamiczna");
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
        pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListInfoLabel->setText("Ciśnienie [at]");
        pressureListInfoLabel->setAlignment(Qt::AlignCenter);
        pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
        pressureListExpandButton->setAlignment(Qt::AlignCenter);
        pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
        pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListAddButton->setAlignment(Qt::AlignCenter);
        pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
        pressureScrollArea->setStyleSheet("background-color: transparent");
        pressureScrollArea->setWidget(pressureList);
        pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
        pressureScrollArea->setupViewport(pressureList);
        pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        if(densityTabActive){
            if(pressureListExpandedDensity){
                pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                // NO_PRESSURE_SHOW
                noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                pressureListInfoLabel->show();
                pressureList->show();
                pressureListAddButton->show();
                pressureScrollArea->show();
            }else{
                pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                // NO_PRESSURE_HIDE
                 noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                pressureListInfoLabel->hide();
                pressureList->hide();
                pressureListAddButton->hide();
                pressureScrollArea->hide();
            }
        }else{
            if(pressureListExpandedVisco){
                pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                // NO_PRESSURE_SHOW
                noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH+ MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                pressureList->show();
                pressureListInfoLabel->show();
                pressureListAddButton->show();
                pressureScrollArea->show();
            }else{
                pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                // NO_PRESSURE_HIDE
                 noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                pressureList->hide();
                pressureListInfoLabel->hide();
                pressureListAddButton->hide();
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
    switch(type){
    case FLANGE_MATERIAL_TYPE:
        titleLabel->setText("Edycja materiału kryzy");
        MaterialsInfoFiles::readFlangeMaterialParams(this);
        break;
    case PIPE_MATERIAL_TYPE:
        titleLabel->setText("Edycja materiału rurociągu");
        MaterialsInfoFiles::readPipeMaterialParams(this);
        break;
    case SUBSTANCE_TYPE:
        titleLabel->setText("Edycja substancji");
        MaterialsInfoFiles::readSubstanceParams(this);
        break;
    }
    backButton->setGeometry((MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_BACK_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_BACK_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    backButton->setPixmap(QPixmap("testowy.png"));
    if(densityTabActive){
        kTextEdit->setText(QString::number(densityDataHolder->getK()));
        if(densityDataHolder->getDependency() == 2){
            tempPresDependencyButton->setChecked(true);
            if(densityDataHolder->getNumbOfPressures() != 0){
                delete noPressureInfo;
                noPressureInfo = nullptr;
                pressureList->createElements(densityDataHolder);
                dataArray->createElements(densityDataHolder);
            }
        }else{
            tempDependencyButton->setChecked(true);
            delete noPressureInfo;
            noPressureInfo = nullptr;
            delete pressureList;
            pressureList = nullptr;
            delete pressureListAddButton;
            pressureListAddButton = nullptr;
            delete pressureListInfoLabel;
            pressureListInfoLabel = nullptr;
            delete pressureListExpandButton;
            pressureListExpandButton = nullptr;
            delete pressureScrollArea;
            pressureScrollArea = nullptr;
            dataArray->createElements(densityDataHolder);
            }
    }else{
        kTextEdit->setText(QString::number(dynamicViscosityDataHolder->getK()));
        if(dynamicViscosityDataHolder->getDependency() == 2){
            if(dynamicViscosityDataHolder->getNumbOfPressures() != 0){
                delete noPressureInfo;
                noPressureInfo = nullptr;
                dataArray->createElements(dynamicViscosityDataHolder);
                pressureList->createElements(dynamicViscosityDataHolder);
           }
        }else{
            tempDependencyButton->setChecked(true);
            delete noPressureInfo;
            noPressureInfo = nullptr;
            delete pressureList;
            pressureList = nullptr;
            delete pressureListAddButton;
            pressureListAddButton = nullptr;
            delete pressureListInfoLabel;
            pressureListInfoLabel = nullptr;
            delete pressureListExpandButton;
            pressureListExpandButton = nullptr;
            delete pressureScrollArea;
            pressureScrollArea = nullptr;
            dataArray->createElements(dynamicViscosityDataHolder);
            }
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
       nameTextEdit->installEventFilter(materialsChangesWidgetCatchEvents);
       switch(type){
       case FLANGE_MATERIAL_TYPE:
           linearExpanstionFDF->installEventFilter(materialsChangesWidgetCatchEvents);
           fiducialTemperatureFDF->installEventFilter(materialsChangesWidgetCatchEvents);
           break;
       case PIPE_MATERIAL_TYPE:
           linearExpanstionFDF->installEventFilter(materialsChangesWidgetCatchEvents);
           fiducialTemperatureFDF->installEventFilter(materialsChangesWidgetCatchEvents);
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
FormatableDataField<ParamName>* MaterialsChangesWidget<ParamName>::getLinearExpanstionFDF(){
    return linearExpanstionFDF;
}

template <typename ParamName>
FormatableDataField<ParamName>* MaterialsChangesWidget<ParamName>::getFiducialTemperatureFDF(){
    return fiducialTemperatureFDF;
}

template <typename ParamName>
QLineEdit* MaterialsChangesWidget<ParamName>::getNameTextEdit(){
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
        if(densityTabActive)
            densityDataHolder->readDataFromArray(dataArray);
        else
            dynamicViscosityDataHolder->readDataFromArray(dataArray);
    }
    switch(*materialsChangesWidgetStats){
    case CHOOSING:
        if(nextStat != nullptr){
            switch(*nextStat){
            case REMOVE:
                if(list->count() > 0){
                    selectedFromList = list->currentItem()->text();
                    switch(type){
                    case FLANGE_MATERIAL_TYPE:
                        MaterialsInfoFiles::removeFlangeMaterial(this);
                        break;
                    case PIPE_MATERIAL_TYPE:
                        MaterialsInfoFiles::removePipeMaterial(this);
                        break;
                    case SUBSTANCE_TYPE:
                        selectedFromList = getNameFromListElement(list->currentItem()->text());
                        MaterialsInfoFiles::removeSubstance(this);
                        break;
                    }
                QFrame::deleteLater();
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
    case ADD:
        if(nameTextEdit->text().isEmpty())
            return;
        switch(type){
        case SUBSTANCE_TYPE:
            if(!MaterialsInfoFiles::checkSubstanceNames(this)){
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Podana nazwa już istnieje."), false, QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                dialog->exec();
                delete dialog;
                dialog = nullptr;
                return;
            }
            break;
        case FLANGE_MATERIAL_TYPE:
            if(!MaterialsInfoFiles::checkFlangeMaterialsNames(this)){
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Podana nazwa już istnieje."), false, QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                dialog->exec();
                delete dialog;
                dialog = nullptr;
                return;
            }
            break;
        case PIPE_MATERIAL_TYPE:
            if(!MaterialsInfoFiles::checkPipeMaterialsNames(this)){
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Podana nazwa już istnieje."), false, QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                dialog->exec();
                delete dialog;
                dialog = nullptr;
                return;
            }
            break;
        }
        switch(type){
        case SUBSTANCE_TYPE:
            dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Wybierz co chcesz \nzapisać:"), false, QString("Gęstość"), QString("Lepkość"), QString("Wszystko"), this);
            dialog->setWindowModality(Qt::ApplicationModal);
            dialog->show();
            switch(dialog->exec()){
            case FIRST_OPTION_RESULT:
                if(!densityDataHolder->check())
                {
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Niepoprawnie wypełniona \n tabelka."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
                densityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    densityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Density.rec"), true);
               else
                    densityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Gase") + "Density.rec"), true);
                break;
            case SECOND_OPTION_RESULT:
                if(!dynamicViscosityDataHolder->check())
                {
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Niepoprawnie wypełniona \n tabelka."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
                dynamicViscosityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    dynamicViscosityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Visco.rec"), false);
                else
                    dynamicViscosityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Gase") + "Visco.rec"), false);
               break;
            case THIRD_OPTION_RESULT:
                if(!densityDataHolder->check())
                {
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Gęstość"), QString("Niepoprawnie wypełniona \n tabelka."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
                densityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    densityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Density.rec"), true);
                else
                    densityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Gase") + "Density.rec"), true);
                if(!densityDataHolder->check())
                {
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Lepkość"), QString("Niepoprawnie wypełniona \n tabelka."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
                dynamicViscosityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    dynamicViscosityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Visco.rec"), false);
                else
                    dynamicViscosityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Gase") + "Visco.rec"), false);

                break;
            }
            break;
        default:
            if(linearExpanstionFDF->text().isEmpty() && linearExpanstionFDF->text().at(0) == '.')
                return;
            if(fiducialTemperatureFDF->text().isEmpty() && fiducialTemperatureFDF->text().at(0) == '.')
                return;
            break;
        }
        switch(type){
        case FLANGE_MATERIAL_TYPE:
            MaterialsInfoFiles::addFlangeMaterial(this);
            break;
        case PIPE_MATERIAL_TYPE:
            MaterialsInfoFiles::addPipeMaterial(this);
            break;
        case SUBSTANCE_TYPE:
            MaterialsInfoFiles::addSubstance(this);
            break;
        }
        QFrame::deleteLater();
        break;
    case EDIT:
        if(nameTextEdit->text().isEmpty())
            return;
        if(!App::compareStr(nameTextEdit->text(), selectedFromList))
        switch(type){
        case SUBSTANCE_TYPE:
            if(!MaterialsInfoFiles::checkSubstanceNames(this)){
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Podana nazwa już istnieje."), false, QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                dialog->exec();
                delete dialog;
                dialog = nullptr;
                return;
            }
            break;
        case FLANGE_MATERIAL_TYPE:
            if(!MaterialsInfoFiles::checkFlangeMaterialsNames(this)){
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Podana nazwa już istnieje."), false, QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                dialog->exec();
                delete dialog;
                dialog = nullptr;
                return;
            }
            break;
        case PIPE_MATERIAL_TYPE:
            if(!MaterialsInfoFiles::checkPipeMaterialsNames(this)){
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Podana nazwa już istnieje."), false, QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                dialog->exec();
                delete dialog;
                dialog = nullptr;
                return;
            }
            break;
        }
        switch(type){
        case SUBSTANCE_TYPE:
            dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Wybierz co chcesz \nzapisać:"), false, QString("Gęstość"), QString("Lepkość"), QString("Wszystko"), this);
            dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
            dialog->setWindowModality(Qt::ApplicationModal);
            dialog->show();
            switch(dialog->exec()){
            case FIRST_OPTION_RESULT:
                if(!densityDataHolder->check())
                {
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Niepoprawnie wypełniona \n tablica."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
                densityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    densityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Density.rec"), true);
                else
                    densityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Gase") + "Density.tec"), true);
                break;
            case SECOND_OPTION_RESULT:
                if(!dynamicViscosityDataHolder->check())
                {
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Niepoprawnie wypełniona \n tabelka."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
                dynamicViscosityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                dynamicViscosityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Visco.rec"), false);
                else
                    dynamicViscosityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Gase") + "Visco.rec"), false);
                break;
            case THIRD_OPTION_RESULT:
                if(!densityDataHolder->check())
                {
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Gęstość"), QString("Niepoprawnie wypełniona \n tabelka."), false, QString("Ok"), QString(), QString(), this);
                    dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
                densityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    densityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Density.rec"), true);
                else
                    densityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Gase") + "Density.rec"), true);
                if(!densityDataHolder->check())
                {
                    delete dialog;
                    dialog = new MyDialog(QUESTION_DIALOG, QString("Lepkość"), QString("Niepoprawnie wypełniona \n tabelka."), false, QString("Ok"), QString(), QString(), this);
                    dialog->setWindowModality(Qt::ApplicationModal);
                    dialog->show();
                    dialog->exec();
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
                dynamicViscosityDataHolder->sort();
                if(fluidTypeCheckBox->isChecked())
                    dynamicViscosityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Visco.rec"), false);
                else
                    dynamicViscosityDataHolder->writeToFile(new QFile(RESOURCES_FOLDER_PATH + QString("Custom\\") + nameTextEdit->text() + QString("Fluid") + "Visco.rec"),false);
                break;
            }
            break;
        default:
            if(linearExpanstionFDF->text().isEmpty() && linearExpanstionFDF->text().at(0) == '.')
                return;
            if(fiducialTemperatureFDF->text().isEmpty() && fiducialTemperatureFDF->text().at(0) == '.')
                return;
            break;
        }
        switch(type){
        case FLANGE_MATERIAL_TYPE:
            MaterialsInfoFiles::editFlangeMaterial(this);
            break;
        case PIPE_MATERIAL_TYPE:
            MaterialsInfoFiles::editPipeMaterial(this);
            break;
        case SUBSTANCE_TYPE:
            MaterialsInfoFiles::editSubstance(this);
            break;
        }
        WINDOW_STATUS_SAVE(materialsChangesWidgetStats)
        *materialsChangesWidgetStats = CHOOSING;
        changeStat = true;
        break;
    default:
        break;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::cancelButtonReleased(){
    QFrame::deleteLater();
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
    case Qt::Key_Space:
        if(!nameTextEdit->text().isEmpty()){
        if(nameTextEdit->text().at(nameTextEdit->text().length() - 1).isSpace())
            return true;
        else{
            nameTextEdit->setStyleSheet("");
            return false;
        }
        }else{
            return true;
        }
        break;
    case Qt::Key_Backspace:
        if(nameTextEdit->text().length() == 1 && nameTextEdit->cursorPosition() == 1)
            nameTextEdit->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        return false;
        break;
    case Qt::Key_Delete:
        if(nameTextEdit->text().length() == 1 && nameTextEdit->cursorPosition() == 0)
            nameTextEdit->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        return false;
        break;
    default:
        nameTextEdit->setStyleSheet("");
        break;
    }
return false;
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
    uint i = 0;
    uint len = linearExpanstionFDF->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    {
        for( ; i < len; i++)
            if(linearExpanstionFDF->text().at(i) == '.')
                break;
        if(i == len)
            QCoreApplication::sendEvent(linearExpanstionFDF, new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier, "."));
        return true;
    }
        break;
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(linearExpanstionFDF->text().at(i) == '.')
                break;
        if(i == len)
            return false;
        else
            return true;
    }
        break;
    case Qt::Key_Delete:
        if(linearExpanstionFDF->text().length() == 1)
            linearExpanstionFDF->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        return false;
    case Qt::Key_Backspace:
        if(linearExpanstionFDF->text().length() == 1)
            linearExpanstionFDF->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        return false;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39){
            linearExpanstionFDF->setStyleSheet("");
            return false;
        }else
            return true;
    }
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::fiducialTemperatureFDFKeyPressed(QKeyEvent *ev){
    uint i = 0;
    uint len = fiducialTemperatureFDF->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    {
        for( ; i < len; i++)
            if(fiducialTemperatureFDF->text().at(i) == '.')
                break;
        if(i == len)
            QCoreApplication::sendEvent(fiducialTemperatureFDF, new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier, "."));
        return true;
    }
        break;
    case Qt::Key_Period:
    {
        for( ; i < len; i++)
            if(fiducialTemperatureFDF->text().at(i) == '.')
                break;
        if(i == len)
            return false;
        else
            return true;
    }
        break;
    case Qt::Key_Delete:
        if(fiducialTemperatureFDF->text().length() == 1)
            fiducialTemperatureFDF->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        return false;
    case Qt::Key_Backspace:
        if(fiducialTemperatureFDF->text().length() == 1)
            fiducialTemperatureFDF->setStyleSheet("QLineEdit{background-color: qradialgradient(cx:0.5, cy:0.5, radius: 1.2,fx:0.5, fy:0.5, stop:0 white, stop:1 rgb(255,0,0));}");
        return false;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39){
            fiducialTemperatureFDF->setStyleSheet("");
            return false;
        }else
            return true;
    }
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
            QMessageBox destroyDataQuestion;
            destroyDataQuestion.setText(QString("Wszystkie wprowadzane dane zostaną usunięte, z wyjątkiem wybranej tablicy."));
            destroyDataQuestion.setInformativeText(QString("Czy na pewno chcesz zmienić zaleźność?"));
            destroyDataQuestion.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            destroyDataQuestion.setDefaultButton(QMessageBox::No);
            switch(destroyDataQuestion.exec()){
            case QMessageBox::No:
                return false;
            default:
                break;
            }
        }
            delete pressureList;
            pressureList = nullptr;
            delete pressureListAddButton;
            pressureListAddButton = nullptr;
            delete pressureListInfoLabel;
            pressureListInfoLabel = nullptr;
            delete pressureListExpandButton;
            pressureListExpandButton = nullptr;
            delete pressureScrollArea;
            pressureScrollArea = nullptr;
            if(dataArray)
            {
                temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry( MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
                addArrayRow->setGeometry( MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
            }else{
                delete noPressureInfo;
                noPressureInfo = nullptr;
                temperatureArrayLabel = new QLabel(this);
                temperatureArrayLabel->show();
                dataArrayLabel = new QLabel(this);
                dataArrayLabel->show();
                dataArray = new MaterialsChangesWidgetArray(this);
                dataArray->show();
                addArrayRow = new QLabel(this);
                addArrayRow->show();
                scrollArea = new QScrollArea(this);
                temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry( MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
                addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                if(densityTabActive)
                    dataArrayLabel->setText("Gęstość [kg/m3]");
                else
                    dataArrayLabel->setText("Lep. dynamiczna [Pas]");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->setWidget(dataArray);
                scrollArea->setupViewport(dataArray);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->show();
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
            }
            if(densityTabActive){
                delete densityDataHolder;
                densityDataHolder = new MaterialsChangesWidgetDataHolder((kTextEdit->text().isEmpty()? 0 : kTextEdit->text().toInt()), 1);
                if(dataArray)
                    densityDataHolder->readDataFromArray(dataArray);
            }else{
                delete dynamicViscosityDataHolder;
                dynamicViscosityDataHolder = new MaterialsChangesWidgetDataHolder((kTextEdit->text().isEmpty())? 0 : kTextEdit->text().toInt(), 1);
                if(dataArray)
                    dynamicViscosityDataHolder->readDataFromArray(dataArray);
            }
            dataArray->updateGeometry();
            tempDependencyButton->setChecked(true);

        return true;
    }
   return false;
}

template <typename ParamName>
bool MaterialsChangesWidget<ParamName>::tempPresDependencyButtonReleased(){
    if(!tempPresDependencyButton->isChecked()){
        pressureList = new MyList(this);
        static_cast<QWidget*>(pressureList)->setParent(this);
        if(dataArray && dataArray->getRow())
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
            scrollArea->setStyleSheet("background-color: transparent");
            scrollArea->setWidget(dataArray);
            scrollArea->setupViewport(dataArray);
            scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
        pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListInfoLabel->setText("Ciśnienie [at]");
        pressureListInfoLabel->setAlignment(Qt::AlignCenter);
        pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
        pressureListExpandButton->setAlignment(Qt::AlignCenter);
        pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
        pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
        pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        pressureListAddButton->setAlignment(Qt::AlignCenter);
        pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
        pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
        pressureScrollArea->setStyleSheet("background-color: transparent");
        pressureScrollArea->setWidget(pressureList);
        pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
        pressureScrollArea->setupViewport(pressureList);
        pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        if(densityTabActive){
            if(pressureListExpandedDensity){
                pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, dataArray->height());
                dataArray->updateGeometry();
                addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                scrollArea->show();
                pressureListInfoLabel->show();
                pressureList->show();
                pressureListAddButton->show();
                pressureScrollArea->show();
            }else{
                pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                scrollArea->show();
                pressureListInfoLabel->hide();
                pressureList->hide();
                pressureListAddButton->hide();
                pressureScrollArea->hide();
            }
        }else{
            if(pressureListExpandedVisco){
                temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, dataArray->height());
                dataArray->updateGeometry();
                addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                scrollArea->show();

                pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                pressureListInfoLabel->show();
                pressureList->show();
                pressureListAddButton->show();
                pressureScrollArea->show();
            }else{
                pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                scrollArea->show();

                pressureListInfoLabel->hide();
                pressureList->hide();
                pressureListAddButton->hide();
                pressureScrollArea->hide();
            }
        }
        tempPresDependencyButton->setChecked(true);
        pressureList->addingElement();
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
        if(kTextEdit->text().length() == 1 && kTextEdit->cursorPosition() == 0)
            kTextEdit->setText(QString("0"));
        return true;
    case Qt::Key_Backspace:
        if(kTextEdit->text().length() == 1 && kTextEdit->cursorPosition() == 1)
            kTextEdit->setText(QString("0"));
        return false;
    case Qt::Key_Left:
        return false;
    case Qt::Key_Right:
        return false;
    default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39){
           return false;
        }
        else
            return true;
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::densityTabLabelReleased(){
    if(!densityTabActive){
        if(dataArray)
        {
            if(!dataArray->checkLineEdits()){
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Wszystkie komórki tablicy muszą byc wypłenione poprawnie."),false,QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                switch(dialog->exec()){
                case FIRST_OPTION_RESULT:
                    delete dialog;
                    dialog = nullptr;
                    return;
                default:
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
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
                    pressureList = new MyList(this);
                    pressureScrollArea = new QScrollArea(this);
                    pressureListAddButton = new QLabel(this);
                }
                    pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListInfoLabel->setText("Ciśnienie [at]");
                    pressureListInfoLabel->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
                    pressureListExpandButton->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListAddButton->setAlignment(Qt::AlignCenter);
                    pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
                    pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea->setStyleSheet("background-color: transparent");
                    pressureScrollArea->setWidget(pressureList);
                    pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureScrollArea->setupViewport(pressureList);
                    pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    if(pressureListExpandedDensity){
                            pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                            // NO_PRESSURE_SET_SHOW
                            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH+ MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->show();
                            pressureList->show();
                            pressureListAddButton->show();
                            pressureScrollArea->show();
                        }else{
                            pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                            // NO_PRESSURE_SET_HIDE
                            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->hide();
                            pressureList->hide();
                            pressureListAddButton->hide();
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
                    dataArray = new MaterialsChangesWidgetArray(this);
                    dataArray->show();
                    addArrayRow = new QLabel(this);
                    addArrayRow->show();
                    scrollArea = new QScrollArea(this);
                }
                if(pressureList == nullptr){
                    pressureListInfoLabel = new QLabel(this);
                    pressureListExpandButton = new QLabel(this);
                    pressureListExpandButton->show();
                    pressureList = new MyList(this);
                    pressureScrollArea = new QScrollArea(this);
                    pressureListAddButton = new QLabel(this);
                }
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                dataArrayLabel->setText("Gęstość [kg/m3]");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->setWidget(dataArray);
                scrollArea->setupViewport(dataArray);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->show();
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->setAlignment(Qt::AlignCenter);
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
                dataArray->setCurrentIndex(densityDataHolder->getSelectedIndex());
                pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                pressureListInfoLabel->setText("Ciśnienie [at]");
                pressureListInfoLabel->setAlignment(Qt::AlignCenter);
                pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
                pressureListExpandButton->setAlignment(Qt::AlignCenter);
                pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
                pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                pressureListAddButton->setAlignment(Qt::AlignCenter);
                pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
                pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
                pressureScrollArea->setStyleSheet("background-color: transparent");
                pressureScrollArea->setWidget(pressureList);
                pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                pressureScrollArea->setupViewport(pressureList);
                pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                if(pressureListExpandedDensity){
                    temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, dataArray->height());
                    addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                    pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                    pressureListInfoLabel->show();
                    pressureList->show();
                    pressureListAddButton->show();
                    pressureScrollArea->show();
                }else{
                            // ARRAY_SET_HIDE
                        temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                        dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                        dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
                        addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                        scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                            pressureListInfoLabel->hide();
                            pressureList->hide();
                            pressureListAddButton->hide();
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
                dataArray = new MaterialsChangesWidgetArray(this);
                dataArray->show();
                addArrayRow = new QLabel(this);
                addArrayRow->show();
                scrollArea = new QScrollArea(this);
            }
                // ARRAY_SET_HIDE
                temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
                addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                dataArrayLabel->setText("Gęstość [kg/m3]");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->setWidget(dataArray);
                scrollArea->setupViewport(dataArray);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->show();
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);

            dataArray->setCurrentIndex(densityDataHolder->getSelectedIndex());
            dataArray->createElements(densityDataHolder);
        }
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::dynamicViscosityTabLabelReleased(){
    if(densityTabActive){
        if(dataArray != nullptr){
            if(dataArray->checkLineEdits())
            densityDataHolder->readDataFromArray(dataArray);
            else{
                dialog = new MyDialog(QUESTION_DIALOG, QString(), QString("Wszystkie komórki tablicy muszą byc wypłenione poprawnie."),false,QString("Ok"), QString(), QString(), this);
                dialog->move(config->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dialog->width()) * 0.5, (APP_WINDOW_HEIGHT - dialog->height()) * 0.5)));
                dialog->setWindowModality(Qt::ApplicationModal);
                dialog->show();
                switch(dialog->exec()){
                case FIRST_OPTION_RESULT:
                    delete dialog;
                    dialog = nullptr;
                    return;
                default:
                    delete dialog;
                    dialog = nullptr;
                    return;
                }
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
                    pressureList = new MyList(this);
                    pressureScrollArea = new QScrollArea(this);
                    pressureListAddButton = new QLabel(this);
                }
                    pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListInfoLabel->setText("Ciśnienie [at]");
                    pressureListInfoLabel->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
                    pressureListExpandButton->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListAddButton->setAlignment(Qt::AlignCenter);
                    pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
                    pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea->setStyleSheet("background-color: transparent");
                    pressureScrollArea->setWidget(pressureList);
                    pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureScrollArea->setupViewport(pressureList);
                    pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    if(pressureListExpandedVisco){
                            pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                            // NO_PRESSURE_SET_SHOW
                            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->show();
                            pressureList->show();
                            pressureListAddButton->show();
                            pressureScrollArea->show();
                    }else{
                            pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                            // NO_PRESSURE_SET_HIDE
                            noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->hide();
                            pressureList->hide();
                            pressureListAddButton->hide();
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
                    pressureList = new MyList(this);
                    pressureScrollArea = new QScrollArea(this);
                    pressureListAddButton = new QLabel(this);
                }
                    pressureListInfoLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListInfoLabel->setText("Ciśnienie [at]");
                    pressureListInfoLabel->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->setGeometry(0, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_DEFAULT_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT - 5 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET);
                    pressureListExpandButton->setAlignment(Qt::AlignCenter);
                    pressureListExpandButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureList->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureListAddButton->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    pressureListAddButton->setAlignment(Qt::AlignCenter);
                    pressureListAddButton->setPixmap(QPixmap(ADD_BUTTON_ICON));
                    pressureListAddButton->installEventFilter(materialsChangesWidgetCatchEvents);
                    pressureScrollArea->setStyleSheet("background-color: transparent");
                    pressureScrollArea->setWidget(pressureList);
                    pressureScrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - 10, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, MATERIALS_CHANGES_WIDGET_PRESSURE_LIST_HEIGHT);
                    pressureScrollArea->setupViewport(pressureList);
                    pressureScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    pressureScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    if(dataArray == nullptr){
                        temperatureArrayLabel = new QLabel(this);
                        temperatureArrayLabel->show();
                        dataArrayLabel = new QLabel(this);
                        dataArrayLabel->show();
                        dataArray = new MaterialsChangesWidgetArray(this);
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
                    scrollArea->setStyleSheet("background-color: transparent");
                    scrollArea->setWidget(dataArray);
                    scrollArea->setupViewport(dataArray);
                    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    scrollArea->show();
                    addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                    addArrayRow->setAlignment(Qt::AlignCenter);
                    addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);
                    dataArray->setCurrentIndex(dynamicViscosityDataHolder->getSelectedIndex());
                    if(pressureListExpandedVisco){
                            pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                            temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, dataArray->height());
                            addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->show();
                            pressureList->show();
                            pressureListAddButton->show();
                            pressureScrollArea->show();
                    }else{
                            pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                            // ARRAY_SET_HIDE
                            temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
                            addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                            pressureListInfoLabel->hide();
                            pressureList->hide();
                            pressureListAddButton->hide();
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
                dataArray = new MaterialsChangesWidgetArray(this);
                dataArray->show();
                addArrayRow = new QLabel(this);
                addArrayRow->show();
                scrollArea = new QScrollArea(this);
            }
                // ARRAY_SET_HIDE
                temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
                addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                temperatureArrayLabel->setText("Temperatura [C]");
                temperatureArrayLabel->setAlignment(Qt::AlignCenter);
                temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
                dataArrayLabel->setAlignment(Qt::AlignCenter);
                dataArrayLabel->setText("Lep. dynamiczna [Pas]");
                scrollArea->setStyleSheet("background-color: transparent");
                scrollArea->setWidget(dataArray);
                scrollArea->setupViewport(dataArray);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->show();
                addArrayRow->setPixmap(QPixmap(ADD_BUTTON_ICON));
                addArrayRow->setAlignment(Qt::AlignCenter);
                addArrayRow->installEventFilter(materialsChangesWidgetCatchEvents);

            dataArray->setCurrentIndex(dynamicViscosityDataHolder->getSelectedIndex());
            dataArray->createElements(dynamicViscosityDataHolder);

        }
    }
}

template <typename ParamName>
void MaterialsChangesWidget<ParamName>::pressureListAddButtonReleased(){
    pressureList->addingElement();
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
    dataArray = new MaterialsChangesWidgetArray(this);
    addArrayRow = new QLabel(this);
    addArrayRow->show();
    scrollArea = new QScrollArea(this);
    if(densityTabActive){
        dataArrayLabel->setText("Gęstość [kg/m3]");
        if(pressureListExpandedDensity){
            temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, dataArray->height());
            addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
        }else{
            // ARRAY_SET_HIDE
            temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
            addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
        }
    }
    else{
        dataArrayLabel->setText("Lep. dynamiczna [Pas]");
        if(pressureListExpandedVisco){
            temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, dataArray->height());
            addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
        }else{
            // ARRAY_SET_HIDE
            temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
            addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
        }
    }
    temperatureArrayLabel->setText("Temperatura [C]");
    temperatureArrayLabel->setAlignment(Qt::AlignCenter);
    temperatureArrayLabel->setStyleSheet("border-width: 2px; border-color: grey;border-style: solid;");
    dataArrayLabel->setAlignment(Qt::AlignCenter);

    scrollArea->setStyleSheet("background-color: transparent");
    scrollArea->setWidget(dataArray);
    scrollArea->setupViewport(dataArray);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
                    temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, dataArray->height());
                    addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                    dataArray->updateGeometry();
                }else{
                    noPressureInfo->setGeometry(MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }
                    pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                    pressureListInfoLabel->show();
                    pressureList->show();
                    pressureListAddButton->show();
                    pressureScrollArea->show();
            }else{
                if(dataArray){
                    temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET), dataArray->height());
                    addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET - MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                    dataArray->updateGeometry();
                }else{
                    noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }
                    pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                    pressureListInfoLabel->hide();
                    pressureList->hide();
                    pressureListAddButton->hide();
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
                    temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_TEMP_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_DATA_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, dataArray->height());
                    dataArray->updateGeometry();
                    addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }else{
                    noPressureInfo->setGeometry(MATERIALS_CHANGES_PRESSURE_LIST_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , MATERIALS_CHANGES_WIDGET_ARRAY_WIDTH, MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }
                    pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_REV_ICON));
                    pressureListInfoLabel->show();
                    pressureList->show();
                    pressureListAddButton->show();
                    pressureScrollArea->show();
            }else{
                if(dataArray){
                    temperatureArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArrayLabel->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    dataArray->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), dataArray->height());
                    dataArray->updateGeometry();
                    addArrayRow->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET + MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    scrollArea->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET, 7 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET, (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }else{
                    noPressureInfo->setGeometry(MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH + MATERIALS_CHANGES_WIDGET_MIDDLE_OFFSET + MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, 6 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT + 7 * MATERIALS_CHANGES_WIDGET_VERTICAL_OFFSET , (MATERIALS_CHANGES_WIDGET_SUBSTANCE_NOT_CHOOSING_WIDTH - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH - 2 * MATERIALS_CHANGES_WIDGET_HORIZONTAL_OFFSET), MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT);
                }
                    pressureListExpandButton->setPixmap(QPixmap(PRESSURE_LIST_EXPAND_BUTTON_ICON));
                    pressureListInfoLabel->hide();
                    pressureList->hide();
                    pressureListAddButton->hide();
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
        pressureList->setAddingDataForExistingArray(false);
        tempDependencyButtonReleased();
    }
}
#endif // MATERIALSCHANGESWIDGET_IMPL_HPP
