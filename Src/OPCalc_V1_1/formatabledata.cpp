#include"formatabledata.hpp"
#include"pipematerialsfileparams.hpp"
#include"flangematerialsfileparams.hpp"
#include"substancesparamsfileparamenum.hpp"
#include<QDebug>
#include<math.h>

template <typename ParamName>
FormatableData<ParamName>::FormatableData(double data, ParamName param){
    this->data = new double;
    *(this->data) = data;
    this->param = param;
}

template <typename ParamName>
FormatableData<ParamName>::FormatableData(QString data, ParamName param, bool isFormat){
    this->param = param;
    if(isFormat)    this->format = new QString(data);
    else{
        this->data = new double;
        *(this->data) = data.toDouble();
        format = FormatableData::getFormatFromString(data);
    }
}

template <typename ParamName>
FormatableData<ParamName>::FormatableData(const FormatableData<ParamName>& copy){
    data = new double(*copy.data);
    param = copy.param;
    if(copy.format)
        format = new QString(*copy.format);
    else
        setDefaultFormat();
}

template <typename ParamName>
FormatableData<ParamName>::~FormatableData(){
    delete data;
    delete format;
}
template <typename ParamName>
double* FormatableData<ParamName>::getData(){
    return data;
}

template <typename ParamName>
QString* FormatableData<ParamName>::getFormat(){
    return format;
}

template <typename ParamName>
void FormatableData<ParamName>::setParam(ParamName set){
    param = set;
}

template <typename ParamName>
void FormatableData<ParamName>::setData(QString set){
    if(data)
        delete data;
    data = new double;
    *data = set.toDouble();
    if(format)
        delete format;
    format = FormatableData::getFormatFromString(set);
}

template <typename ParamName>
void FormatableData<ParamName>::setData(double *set){
    if(data)
        delete data;
    data = set;
}

template <typename ParamName>
void FormatableData<ParamName>::setFormat(QString *set){
    if(format)
        delete format;
    format = set;
}

template <typename ParamName>
QString* FormatableData<ParamName>::getFormatFromString(QString str){
    uint len = str.length();
    ushort c;
    int ePos = -1;
    bool end = false;
    bool periodFound = false;
    char format = 'f';
    uint digitsCounterAfterPeriod = 0;
    for(uint i  = 0; i < len && !end; i++){
        c = str.at(i).toLatin1();
        switch(c){
        case 'e':
            format = 'e';
            ePos = i;
            break;
        case '.':
            periodFound = true;
            break;
        case ' ':
            end = true;
            break;
        default:
            if(periodFound && ePos == -1)
                digitsCounterAfterPeriod++;
            break;
        }
    }
    return new QString(((ePos != -1)? QString("e") + str.right(len - ePos - 1) + "." + QString::number(digitsCounterAfterPeriod) : QString(c) + QString::number(digitsCounterAfterPeriod)));
}

template <typename ParamName>
bool FormatableData<ParamName>::checkStringFormat(QString str, bool negative){
    uint len = str.length();
    ushort c;
    bool eFound = false;
    for(uint i  = 0; i < len; i++){
        c = str.at(i).toLatin1();
        switch(c){
        case '-':
            if((i != 0 && str.at(i - 1) != 'e') || i + 1 == len || str.at(i+1) > 57 || str.at(i+1) < 48 || (!negative && !eFound))
                return false;
            break;
        case 'e':
            if(i == 0 || i + 1 == len || str.at(i-1) > 57 || str.at(i-1) < 48 || eFound )
                return false;
            eFound = true;
            break;
        case '.':
            if(i == 0 || i + 1 == len || str.at(i-1) > 57 || str.at(i-1) < 48 || str.at(i+1) > 57 || str.at(i+1) < 48 || eFound)
                return false;
            break;
        case ' ':
            return true;
        default:
            break;
        }
    }

    if(len != 0 && (negative)? true : str.toDouble() > 0)
        return true;
    else
        return false;
}

template <typename ParamName>
bool FormatableData<ParamName>::setDefaultFormat(){
    if(!format)
        switch(param){
        case NONE_PARAM:
            format = new QString(DEF_FORM_NONE);
            break;
        case PROJECT_MENAGER_PRESSURE_TAP_DIAMETER:
            format = new QString(DEF_FORM_PRESSURE_TAP_DIAMETER);
            break;
        case PROJECT_MENAGER_MAX_DIFF_PRESSURE:
            format = new QString(DEF_FORM_MAX_DIFF_PRESSURE);
            break;
        case PROJECT_MENAGER_FLANGE_DIAMETER:
            format = new QString(DEF_FORM_FLANGE_DIAMETER);
            break;
        case PROJECT_MENAGER_PIPE_DIAMETER:
            format = new QString(DEF_FORM_PIPE_DIAMETER);
            break;
        case PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE:
            format = new QString(DEF_FORM_MAX_UPSTREAM_PRESSURE);
            break;
        case PROJECT_MENAGER_MAX_FLOW_VALUE:
            format = new QString(DEF_FORM_MAX_FLOW_VALUE);
            break;
        case PROJECT_MENAGER_FLOW:
            format = new QString(DEF_FORM_FLOW);
            break;
        case  PROJECT_MENAGER_TEMP:
            format = new QString(DEF_FORM_TEMP);
            break;
        case PROJECT_MENAGER_WORK_DENSITY:
            format = new QString(DEF_FORM_FLUID_DENSITY);
            break;
        case PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY:
            format = new QString(DEF_FORM_DYNAMIC_VISCOSITY);
            break;
        case PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO:
            format = new QString(DEF_FORM_HEAT_CAPACITY_RATIO);
            break;
        case PROJECT_MENAGER_COMPRESIBILITY_FACTOR:
            format = new QString(DEF_FORM_COMPRESIBILITY_FACTOR);
            break;
        case PROJECT_MENAGER_DENSITY_FROM_FILE:
            format = new QString(DEF_FORM_FLUID_DENSITY_FROM_FILE);
            break;
        case PROJECT_MENAGER_VISCO_FROM_FILE:
            format = new QString(DEF_FORM_DYNAMIC_VISCOSITY_FROM_FILE);
            break;
        case PROJECT_MENAGER_CHOKE_POINT:
            format = new QString(DEF_FORM_CHOKE_POINT);
            break;
        case PROJECT_MENAGER_FLOW_FACTOR:
            format = new QString(DEF_FORM_FLOW_FACTOR);
            break;
        case PROJECT_MENAGER_EXPANSTION_NUMBER:
            format = new QString(DEF_FORM_EXPANSTION_NUMBER);
            break;
        case PROJECT_MENAGER_REYNOLDS_NUMBER:
            format = new QString(DEF_FORM_REYNOLDS_NUMBER);
            break;
        case PROJECT_MENAGER_PRESSURE_LOSS:
            format = new QString(DEF_FORM_PRESSURE_LOSE);
            break;
        case PROJECT_MENAGER_PIPE_LINEAR_EXPANSTION:
            format = new QString(DEF_FORM_PIPE_LINEAR_EXPANSTION);
            break;
        case PROJECT_MENAGER_FLANGE_LINEAR_EXPANSTION:
            format = new QString(DEF_FORM_FLANGE_LINEAR_EXPANSTION);
            break;
        case PROJECT_MENAGER_UPSTREAM_DEVICE_DISTANCE:
            format = new QString(DEF_FORM_UPSTREAM_DEVICE_DISTANCE);
            break;
        case PROJECT_MENAGER_DOWNSTREAM_DISTANCE:
            format = new QString(DEF_FORM_DOWNSTREAM_DISTANCE);
            break;
        default:
            format = new QString(DEF_FORM_NONE);
            break;
        }
    return true;
}

template <typename ParamName>
void FormatableData<ParamName>::incrementPrecision(){
    int dotPos = -1;
    int len = format->length();
    for(int i = 1; i < len; i++)
        if(format->at(i) == '.'){
            dotPos = i;
            break;
        }
    QString* newFormat = nullptr;
    if(dotPos == -1)
        newFormat = new QString(format->at(0) + QString::number(format->right(format->length()-1).toUShort() + 1));
        else
        newFormat = new QString(format->left(dotPos + 1) + QString::number(format->right(len - dotPos - 1).toUShort() + 1));
    delete format;
    format = newFormat;
}

template <typename ParamName>
void FormatableData<ParamName>::decrementPrecision(){
    int dotPos = -1;
    int len = format->length();
    for(int i = 1; i < len; i++)
        if(format->at(i) == '.'){
            dotPos = i;
            break;
        }
    QString* newFormat = nullptr;
    if(dotPos == -1)
        newFormat = new QString(format->at(0) + QString::number(format->right(format->length()-1).toUShort() - 1));
        else
        newFormat = new QString(format->left(dotPos + 1) + QString::number(format->right(len - dotPos - 1).toUShort() - 1));
    delete format;
    format = newFormat;
}

template <typename ParamName>
void FormatableData<ParamName>::changeFormatType(char c){
    int dotPos = -1;
    int len = format->length();
    for(int i = 1; i < len; i++)
        if(format->at(i) == '.'){
            dotPos = i;
            break;
        }
    QString* newFormat = nullptr;
    if(dotPos == -1)
        newFormat = new QString(c + format->right(len - 1).remove('+'));
    else
        newFormat = new QString(c + format->right(len - dotPos - 1));
    delete format;
    format = newFormat;
}

template <typename ParamName>
QString FormatableData<ParamName>::useStringFormat(){
    bool eFound = (format->at(0) == 'e')? true : false;
    int dotPos = -1;
    ushort c;
    uint len = format->length();
    for(uint i = 1; i < len; i++)
        if(format->at(i) == '.')
            dotPos = i;
    if(!eFound)
        return QString::number(*data, 'f', format->right(len - 1).remove('+').toInt());
    else{
        if(dotPos == -1)
            return QString::number(*data, 'e', format->right(len - 1).remove('+').toInt()).remove('+');
        else
            return QString::number(*data/pow(10, format->mid(1, dotPos - 1).toDouble()), 'f', format->right(len - dotPos - 1).toInt())+ format->left(dotPos);
    }
}

template <typename ParamName>
void FormatableData<ParamName>::cutOfZeros(){
    QString right = QString();
    QString str = useStringFormat();
    int len = str.length();
    int i = len - 1;
    if(format->at(0) == 'e'){
        while(i > -1 && str.at(i--) != 'e');
        right = str.right(len - i + 1);
    }
    while(i > -1 && str.at(i--) == '0');
    int numbOfCharToChop = len - i - 2;
    if(str.at(i + 1) != '.'){
        while(i > -1 && str.at(i--) != '.');
        if(i != -1)
        {
            str.chop(numbOfCharToChop);
            str.append(right);
        }
    }else{
        str.chop(numbOfCharToChop + 1);
    }
    setFormat(FormatableData::getFormatFromString(str));
}

template <typename ParamName>
void FormatableData<ParamName>:: changePrecision(ushort precision){
    int dotPos = -1;
    int len = format->length();
    for(int i = 1; i < len; i++)
        if(format->at(i) == '.'){
            dotPos = i;
            break;
        }
    QString* newFormat = nullptr;
    if(dotPos == -1)
        newFormat = new QString(format->at(0) + QString::number(precision));
    else
        newFormat = new QString(format->left(dotPos) + "." + QString::number(precision));
    delete format;
    format = newFormat;
}

template class FormatableData<ProjectMenagerParam>;
template class FormatableData<PipeMaterialsFileParams>;
template class FormatableData<FlangeMaterialsFileParams>;
template class FormatableData<SubstancesParamsFileParam>;
