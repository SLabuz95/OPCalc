#ifndef MATERIALSCHANGESWIDGETARRAY_HPP
#define MATERIALSCHANGESWIDGETARRAY_HPP
#include<QFrame>
#include"substancesparamsfileparamenum.hpp"
class MaterialsChangesWidgetDataHolder;
template <typename ParamName>class MaterialsChangesWidget;
template <typename ParamName> class FormatableDataField;
class QLabel;
class QLineEdit;
class MaterialsChangesWidgetArrayCatchEvents;
class MaterialsChangesWidgetArray : public QFrame{
public:
    // Konstruktor
    MaterialsChangesWidgetArray(MaterialsChangesWidget<SubstancesParamsFileParam>*);

    // Destruktor
    ~MaterialsChangesWidgetArray();
private:
    // Parent
    MaterialsChangesWidget<SubstancesParamsFileParam> *parent = nullptr;

    // EventFilter
    MaterialsChangesWidgetArrayCatchEvents *catchEvents = nullptr;

    // Zmienna informująca o tym czy wszystkie komorki sa zapisane poprawnie
    uint numberOfWrongData = 0;

    // Aktualny index ciśnienia
    uint currentIndex = 0;

    // Elementy
    QLabel **removeButton = nullptr;
    FormatableDataField<SubstancesParamsFileParam> **temperatureTextEdit = nullptr;
    FormatableDataField<SubstancesParamsFileParam> **dataTextEdit = nullptr;
    uint row = 0;

    // Funkcje sterujące
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();

public:
    FormatableDataField<SubstancesParamsFileParam>** getTemperatureTextEdit();
    FormatableDataField<SubstancesParamsFileParam>** getDataTextEdit();
    QLabel** getRemoveButton();
    MaterialsChangesWidget<SubstancesParamsFileParam>* getParent();
    uint getRow();
    uint getNumbOfWrongData();
    uint getCurrentIndex();

    void createElements(MaterialsChangesWidgetDataHolder*);
    void addElement();
    void removeElement(uint);
    bool lineEditKeyPressed(QKeyEvent*, uint, bool);
    bool checkLineEdits();
    void setCurrentIndex(uint);
    void updateGeometry();
    bool isDuplicated(uint index);
    int checkTemperatureExist(double);
};

#endif // MATERIALSCHANGESWIDGETARRAY_HPP
