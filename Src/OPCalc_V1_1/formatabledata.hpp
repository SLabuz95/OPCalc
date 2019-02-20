#ifndef FORMATABLEDATA_HPP
#define FORMATABLEDATA_HPP
#include<QString>
#include"projectmenagerparamenum.hpp"
#include"mainmacros.hpp"
template <typename ParamName> class FormatableData{
public:
    FormatableData(QString, ParamName, bool);
    FormatableData(double, ParamName);
    FormatableData(const FormatableData<ParamName>&);
    ~FormatableData();

private:
    ParamName param;
    double *data = nullptr;
    QString *format = nullptr;

public:
    // Ustawianie formatu napisów
    QString useStringFormat();
    double* getData();
    QString* getFormat();
    void setParam(ParamName);
    void setData(QString);
    void setData(double*);
    void setFormat(QString*);
    static QString* getFormatFromString(QString);
    static bool checkStringFormat(QString, bool);
    // Ustawianie formatu domyślnego jeśli nie istnieje format(Zawsze zwraca TRUE)
    bool setDefaultFormat();
    void incrementPrecision();
    void decrementPrecision();
    void changeFormatType(char);
    void cutOfZeros();
    void changePrecision(ushort);
};

#endif // FORMATABLEDATA_HPP
