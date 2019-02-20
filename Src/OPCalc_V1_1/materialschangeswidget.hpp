#ifndef MATERIALSCHANGESWIDGET_HPP
#define MATERIALSCHANGESWIDGET_HPP
#include<QFrame>
#include"materialschangeswidgetstatsenum.hpp"
#include"materialschangeswidgettypeenum.hpp"
#include"flangematerialsfileparams.hpp"
#include"pipematerialsfileparams.hpp"
#include"substancesparamsfileparamenum.hpp"
#include<QTimer>

class QLabel;
class QLineEdit;
class QCheckBox;
class QButtonGroup;
class QRadioButton;
class Config;
class QListWidget;
class QScrollArea;
class QComboBox;
class MaterialsChangesWidgetArray;
template <typename ParamName> class MaterialsChangesWidgetCatchEvents;
class MaterialsChangesWidgetDataHolder;
class MyDialog;
class MyList;
template <typename ParamName> class FormatableDataField;
template <typename ParamName>
class MaterialsChangesWidget : public QFrame{

public:
    // Konstruktor
    MaterialsChangesWidget(MaterialsChangesWidgetStats, MaterialsChangesWidgetType, Config*);

    // Destruktor
    ~MaterialsChangesWidget();
 private:
    // Zmienna stanów
    MaterialsChangesWidgetStats *materialsChangesWidgetStats = nullptr;

    // Config Adres
    Config* config = nullptr;

    // Zmienna kolejnego stanu
    MaterialsChangesWidgetStats *nextStat = nullptr;
    MaterialsChangesWidgetType type;

    // Dialog o parametry
    MyDialog *dialog = nullptr;

    // Zmienna przchowująca o położeniu nowego indeksu ciśnienia
    uint newPressureIndex = 0;

    // Zmienna informująca o próbie zmiany stanu
    bool changeStat = false;

    // Zmienna przechowująca index usuwanego elementu tabeli
    uint *indexToDelete = nullptr;
    uint *pressureIndexToDelete = nullptr;

    // Timer
    QTimer *reloadTimer = nullptr;

    // Wybrany materiał
    QString selectedFromList = QString();

    // Zmienna EventFilter
    MaterialsChangesWidgetCatchEvents<ParamName> *materialsChangesWidgetCatchEvents = nullptr;

    // Zmienna określająca wybraną zakładke
    bool densityTabActive = false;

    // Zmienna przechowująca ścieżkę do pliku z substancją
    QString densityPath = QString();
    QString viscoPath = QString();

    // Elementy dla poszczególnych stanów:
    // - NIEZALEŹNE
    QLabel *titleLabel = nullptr;
    QLabel *closeButton = nullptr;
    QLabel *okButton = nullptr;
    QLabel *cancelButton = nullptr;
    // - CHOOSING
    QListWidget *list = nullptr;
    // - ADD
    QLabel *nameLabel = nullptr;
    FormatableDataField<ParamName> *nameTextEdit = nullptr;
    // Brak BackButton
    // Zależnie od typu:
    // -  FLANGE_MATERIAL_TYPE
    QLabel *linearExpanstionLabel = nullptr;
    FormatableDataField<ParamName> *linearExpanstionFDF = nullptr;
    QLabel *linearExpanstionUnitLabel = nullptr;
    QLabel *fiducialTemperatureLabel = nullptr;
    FormatableDataField<ParamName> *fiducialTemperatureFDF = nullptr;
    QLabel *fiducialTemperatureUnitLabel = nullptr;
    // - PIPE_MATERIAL_TYPE
    // - To co w FLANGE_MATERIAL_TYPE
    // - SUBSTANCE_TYPE
    QLabel *typeLabel = nullptr;
    QCheckBox *fluidTypeCheckBox = nullptr;
    QCheckBox *gaseTypeCheckBox = nullptr;
    QComboBox *pressureUnitList = nullptr;
    QLabel *densityTabLabel = nullptr;
    QLabel *temperatureArrayLabel = nullptr;
    QLabel *pressureListInfoLabel = nullptr;
    MyList *pressureList = nullptr;
    QLabel *pressureListExpandButton = nullptr;
    QLabel *pressureListAddButton = nullptr;
    QLabel *pressureListEditAllButton = nullptr;
    QLabel *pressureListEditConfirmedButton = nullptr;
    QLabel *pressureListEditCancelledButton = nullptr;
    QLabel *dataArrayLabel = nullptr;
    QLabel *dynamicViscosityTabLabel = nullptr;
    QLabel *addArrayRow = nullptr;
    QScrollArea *scrollArea = nullptr;
    QScrollArea *pressureScrollArea = nullptr;
    QLabel *noPressureInfo = nullptr;
    MaterialsChangesWidgetDataHolder *densityDataHolder = nullptr;
    MaterialsChangesWidgetDataHolder *dynamicViscosityDataHolder = nullptr;
    MaterialsChangesWidgetArray *dataArray = nullptr;
    QLabel *kLabel = nullptr;
    QLineEdit *kTextEdit = nullptr;
    QLabel *dependencyLabel = nullptr;
    QButtonGroup *dependencyButtonGroup = nullptr;
    QRadioButton *tempDependencyButton = nullptr;
    QRadioButton *tempPresDependencyButton = nullptr;
    // REMOVE
    // EDIT
    QLabel* backButton = nullptr;
    // Tak jak add tylko ze zmianą danych i ma backButton

    MyDialog *errorDlg = nullptr;
    MyDialog *errorDenDlg = nullptr;
    MyDialog *errorViscoDlg = nullptr;

    // Flaga wystąpienia pierwszego odczytu
    bool firstRead = false;
    // Flaga rozwiniętej listy dla gęstości
    bool pressureListExpandedDensity = false;
    // Flaga rozwiniętej listy dla lepkości
    bool pressureListExpandedVisco = false;

    // Funkcje sterujące
    void initAndStartTimers();
    void init();
    void setWindow();
    void changeWindowStyle();
    void createWidgets();
    void createWidgetsForChoosing();
    void createWidgetsForAdd();
    void createWidgetsForEdit();
    void createLayout();
    void createLayoutForAdd();
    void createLayoutForEdit();
    void deleteWidgets();
    void deleteWidgetsForAdd();
    void deleteWidgetsForEdit();
    void connectAll();
    void disconnectAll();
    void reload();
    void readMaterials();
    void createPressureListObj(MaterialsChangesWidget<FlangeMaterialsFileParams>*);
    void createPressureListObj(MaterialsChangesWidget<PipeMaterialsFileParams>*);
    void createPressureListObj(MaterialsChangesWidget<SubstancesParamsFileParam>*);
    void createArrayObj(MaterialsChangesWidget<FlangeMaterialsFileParams>*);
    void createArrayObj(MaterialsChangesWidget<PipeMaterialsFileParams>*);
    void createArrayObj(MaterialsChangesWidget<SubstancesParamsFileParam>*);

public:
    // Funkcje get
    QLabel* getCloseButton();
    QLabel* getOkButton();
    QLabel* getCancelButton();
    QLabel* getBackButton();
    QListWidget* getList();
    FormatableDataField<ParamName>* getNameTextEdit();
    QTimer* getTimer();
    FormatableDataField<ParamName>* getLinearExpanstionFDF();
    FormatableDataField<ParamName>* getFiducialTemperatureFDF();
    MaterialsChangesWidgetStats* getMaterialsChangesWidgetStats();
    MaterialsChangesWidgetStats* getNextStat();
    QString getSelectedFromList();
    MaterialsChangesWidgetType getType();
    QCheckBox* getFluidTypeCheckBox();
    QCheckBox* getGaseTypeCheckBox();
    QLabel* getDensityTabLabel();
    QLabel* getDynamicViscosityTabLabel();
    QLabel* getPressureListEditAllButton();
    MyList* getPressureList();
    QLabel* getPressureListAddButton();
    QLabel* getPressureListEditConfirmedButton();
    QLabel* getPressureListEditCancelledButton();
    QLabel* getAddArrayRow();
    QComboBox* getPressureUnitList();
    QRadioButton* getTempDependencyButton();
    QRadioButton* getTempPresDependencyButton();
    QLineEdit* getKTextEdit();
    QScrollArea* getScrollArea();
    QScrollArea* getPressureScrollArea();
    MaterialsChangesWidgetArray* getDataArray();
    QLabel* getPressureListExpandButton();
    bool isPressureListExpanded();
    bool getFirstRead();
    bool getDensityTabActive();
    MaterialsChangesWidgetDataHolder* getDensityDataHolder();
    MaterialsChangesWidgetDataHolder* getDynamicViscosityDataHolder();
    void setDensityPath(QString);
    void setViscoPath(QString);
    void setIndexToDelete(uint*);
    void setNewPressureIndex(uint);
    void setPressureIndexToDelete(uint*);
    Config* getConfig();
    MyDialog* getErrorDlg();
    MyDialog* getErrorDenDlg();
    MyDialog* getErrorViscoDlg();
    void setErrorDlg(MyDialog*);
    void setErrorDenDlg(MyDialog*);
    void setErrorViscoDlg(MyDialog*);

public:
    void reloadTimerTask();

    // Funkcje realizacji zdarzeń
    void closeButtonReleased();
    void okButtonReleased();
    void cancelButtonReleased();
    void backButtonReleased();
    void confirmEditButtonReleased();
    void cancelEditButtonReleased();
    bool nameTextEditKeyPressed(QKeyEvent*);
    bool linearExpanstionFDFKeyPressed(QKeyEvent*);
    bool fiducialTemperatureFDFKeyPressed(QKeyEvent*);
    void fluidCheckBoxReleased();
    void gaseCheckBoxReleased();
    void addArrayRowReleased();
    bool tempDependencyButtonReleased();
    bool tempPresDependencyButtonReleased();
    bool kTextEditKeyPressed(QKeyEvent*);
    bool densityTabLabelReleased();
    bool dynamicViscosityTabLabelReleased();
    void pressureListAddButtonReleased();
    void pressureListExpandButtonReleased();
    void pressureListEditAllButtonReleased();
    void pressureListEditConfirmedButtonReleased();
    void pressureListEditCancelledButtonReleased();
    void pressureListEditConfirmedButtonHighlight();
    void pressureListEditConfirmedButtonLowlight();
    void pressureListEditCancelledButtonHighlight();
    void pressureListEditCancelledButtonLowlight();
    void createArray();
    void canceledAddingElement();
    void createEditDesitionsButtons();
    void deleteEditDesitionsButtons();

    // Funkcja wyłuskania nazwy z listy
    static QString getNameFromListElement(QString);

    void installEventFilter(QObject* set){QFrame::installEventFilter(set);}
    void setParent(QWidget* set, Qt::WindowFlags f){QFrame::setParent(set,f);}
    void moveToArrayError();
    void moveToArrayError(int);
    void moveToAddingElement();
    void moveToAddingArrayElement();
    void moveToEditingError(uint);
    void pressureUnitListLeft();
    void moveToError(bool densityTab, double pressure, double temp);
    void moveToError(bool densityTab, double temp);

};

#endif // MATERIALSCHANGESWIDGET_HPP
