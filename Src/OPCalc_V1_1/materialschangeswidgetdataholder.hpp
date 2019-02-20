#ifndef MATERIALSCHANGESWIDGETDATAHOLDER_HPP
#define MATERIALSCHANGESWIDGETDATAHOLDER_HPP
#include<QObject>
#include"substancesparamsfileparamenum.hpp"

template <typename ParamName> class FormatableData;
class QString;
class QFile;
class MaterialsChangesWidgetArray;
template <typename ParamName> class FormatableDataField;
class MaterialsChangesWidgetDataHolder{
  public:
    // Konstruktor
    MaterialsChangesWidgetDataHolder(int, ushort, ushort);

    // Destruktor
    ~MaterialsChangesWidgetDataHolder();

private:
    ushort pressureUnit = 0;
    int k = 1;
    ushort dependency = 1;
    uint numbOfPressures = 0;
    FormatableData <SubstancesParamsFileParam> **pressure = nullptr;
    uint *numbOfElem = nullptr;
    FormatableData<SubstancesParamsFileParam> ***dataD = nullptr;
    FormatableData<SubstancesParamsFileParam> ****data = nullptr;
    uint selectedIndex = 0;

    static SubstancesParamsFileParam checkFileParam(QString*);
    QString readValue(QFile*, char*);

public:
    void readDataFromArray(MaterialsChangesWidgetArray*);
    uint getNumbOfPressures();
    FormatableData<SubstancesParamsFileParam>** getPressure();
    uint* getNumbOfElem();
    FormatableData<SubstancesParamsFileParam>**** getData();
    FormatableData<SubstancesParamsFileParam>*** getDataD();
    int getK();
    ushort getDependency();
    uint getSelectedIndex();
    ushort getPressureUnit();
    void setSelectedIndex(uint);
    bool readFromFile(QFile*);
    bool writeToFile(QFile*, bool, QString*);
    void addPressure(uint, QString);
    void removePressure(uint);
    uint findPressureToAdd(double);
    uint changePressure(QString, uint);
    int check();
    void sort();
    void setK(int);
    void setDependency(ushort);
    void setPressureUnit(ushort);
    double getCorrectionFactorValue();
    double getReverseCorrectionFactorValue();
    void refreshPressureArray(FormatableDataField<SubstancesParamsFileParam>**);
    };

#endif // MATERIALSCHANGESWIDGETDATAHOLDER_HPP
