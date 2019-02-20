#ifndef MYLIST_HPP
#define MYLIST_HPP
#include<QFrame>
#include<QLabel>
#include<QLineEdit>
#include"substancesparamsfileparamenum.hpp"
template <typename ParamName> class FormatableDataField;
template <typename ParamName> class MaterialsChangesWidget;
class MaterialsChangesWidgetDataHolder;
class MyListCatchEvents;
class MyList : public QFrame{
public:
    // Konstruktor
    MyList(MaterialsChangesWidget<SubstancesParamsFileParam>* setParent);

    // Destruktor
    ~MyList();
    private:
    // Parent
    MaterialsChangesWidget<SubstancesParamsFileParam>* parent = nullptr;

    // CatchEvent
    MyListCatchEvents* catchEvent = nullptr;

    // Flaga edycji danych
    bool enteringData = false;
    bool addingData = false;
    bool addingDataForExistingArray = false;

    // Ostatnia kolejność elementów listy dla trybu edycji danych
    FormatableDataField<SubstancesParamsFileParam>** lastDataElement = nullptr;

    // Zmienna ilości elementów
    uint numbOfElem = 0;

    // Indeks wybranego elementu
    uint* currentIndex = nullptr;
    uint* enteringDataIndex = nullptr;

    // Elementy
    FormatableDataField<SubstancesParamsFileParam>** dataElement = nullptr;
    QLabel** removeButton = nullptr;

    // Funkcje streujące
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();

public:
    // Funkcje get
    MaterialsChangesWidget<SubstancesParamsFileParam>* getParent();
    bool getEnteringData();
    uint getNumbOfElem();
    uint* getCurrentIndex();
    FormatableDataField<SubstancesParamsFileParam>** getDataElement();
    QLabel** getRemoveButton();
    uint* getEnteringDataIndex();
    bool getAddingData();
    bool getAddingDataForExistingArray();
    void setAddingDataForExistingArray(bool);

    // Funkcje
    void createElements(MaterialsChangesWidgetDataHolder*);
    void addingElement();
    void addElement();
    void removeElement(uint);
    bool lineEditKeyPressed(QKeyEvent*, QLineEdit*);
    void dataElementDoubleClicked(uint);
    bool dataElementClicked(uint);
    void highlight(uint);
    void lowlight(uint);
    void setCurrentIndex(uint*);
    void editingDataConfirmed();
    void editingDataCanceled();
    void addingElementConfirmed();
    void addingElementCanceled();
    int isPressureExist(double);
    bool isPressureExist(double, uint);
    void rewriteList(double);
    void moveElement(int, int);
    void editingAllMode();
    void repositionElements();
    bool checkList();
    };

#endif // MYLIST_HPP
