#include"materialschangeswidgetarray.hpp"
#include<QKeyEvent>
#include"materialschangeswidgetarraycatchevents.hpp"
#include"windowmacros.hpp"
#include"materialschangeswidgetdataholder.hpp"
#include"materialschangeswidget.hpp"
#include"mylist.hpp"
#include"formatabledatafield.hpp"
#include"mainmacros.hpp"
#include"formatabledata.hpp"

MaterialsChangesWidgetArray::MaterialsChangesWidgetArray(MaterialsChangesWidget<SubstancesParamsFileParam> *setParent){
    parent = setParent;
    QFrame::setParent(parent);
    init();
    setWindow();
    createWidgets();
    createLayout();
    connectAll();
}

MaterialsChangesWidgetArray::~MaterialsChangesWidgetArray(){
    deleteWidgets();
    delete catchEvents;
    catchEvents = nullptr;
}

void MaterialsChangesWidgetArray::init(){
    catchEvents = new MaterialsChangesWidgetArrayCatchEvents(this);
    row = 0;
    numberOfWrongData = 0;
    currentIndex = 0;
}

void MaterialsChangesWidgetArray::setWindow(){
    resize(width(), (row <= 8)? MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT : row * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
}

void MaterialsChangesWidgetArray::createWidgets(){
    // Brak
}

void MaterialsChangesWidgetArray::createLayout(){
    // Brak
}

void MaterialsChangesWidgetArray::deleteWidgets(){
    for(uint i = 0; i < row; i++){
        delete (*(removeButton + i));
        (*(removeButton + i)) = nullptr;
        delete (*(temperatureTextEdit + i));
        (*(temperatureTextEdit + i)) = nullptr;
        delete (*(dataTextEdit + i));
        (*(dataTextEdit + i)) = nullptr;
    }
    delete []removeButton;
    removeButton = nullptr;
    delete []temperatureTextEdit;
    temperatureTextEdit = nullptr;
    delete []dataTextEdit;
    dataTextEdit = nullptr;
}

void MaterialsChangesWidgetArray::connectAll(){
    // Brak
}

FormatableDataField<SubstancesParamsFileParam>** MaterialsChangesWidgetArray::getTemperatureTextEdit(){
    return temperatureTextEdit;
}

FormatableDataField<SubstancesParamsFileParam>** MaterialsChangesWidgetArray::getDataTextEdit(){
    return dataTextEdit;
}

QLabel** MaterialsChangesWidgetArray::getRemoveButton(){
    return removeButton;
}

void MaterialsChangesWidgetArray::createElements(MaterialsChangesWidgetDataHolder *dataHolder){
    for(uint i = 0; i < row; i++){
        delete *(removeButton + i);
        (*(removeButton + i)) = nullptr;
        delete (*(temperatureTextEdit + i));
        (*(temperatureTextEdit + i)) = nullptr;
        delete (*(dataTextEdit + i));
        (*(dataTextEdit + i)) = nullptr;
        }
    if(removeButton != nullptr)
    delete []removeButton;
    removeButton = nullptr;
    if(temperatureTextEdit != nullptr)
    delete []temperatureTextEdit;
    temperatureTextEdit = nullptr;
    if(dataTextEdit != nullptr)
    delete []dataTextEdit;
    dataTextEdit = nullptr;
    if(dataHolder->getDependency() == 2)
        row = *(dataHolder->getNumbOfElem() + *parent->getPressureList()->getCurrentIndex());
    else
        row = *(dataHolder->getNumbOfElem());
    if(row != 0){
        removeButton = new QLabel*[row];
        temperatureTextEdit = new FormatableDataField<SubstancesParamsFileParam>*[row];
        dataTextEdit = new FormatableDataField<SubstancesParamsFileParam>*[row];
        for(uint i = 0; i < row; i++){
            (*(removeButton + i)) = new QLabel(this);
            (*(removeButton + i))->setGeometry(width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * i, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            (*(removeButton + i))->setPixmap(QPixmap(REMOVE_BUTTON_ICON));
            (*(removeButton + i))->installEventFilter(catchEvents);
            (*(removeButton + i))->show();
            (*(temperatureTextEdit + i)) = new FormatableDataField<SubstancesParamsFileParam>(true, ((dataHolder->getDependency() == 1)? **(dataHolder->getDataD() + i) : **(*(dataHolder->getData() + *parent->getPressureList()->getCurrentIndex()) + i)), false, this, QString(), true);
            if((*(temperatureTextEdit + i))->getDataUTextEdit()->text().isEmpty())
                (*(temperatureTextEdit + i))->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
            (*(temperatureTextEdit + i))->setGeometry(0, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * i, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            (*(temperatureTextEdit + i))->getDataUTextEdit()->setGeometry(0, 0, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            (*(temperatureTextEdit + i))->getDataUTextEdit()->installEventFilter(catchEvents);
            (*(temperatureTextEdit + i))->show();
            (*(dataTextEdit + i)) = new FormatableDataField<SubstancesParamsFileParam>(true, (dataHolder->getDependency() == 1)? *(*(dataHolder->getDataD() + i) + 1) : *(*(*(dataHolder->getData() + *parent->getPressureList()->getCurrentIndex()) + i) + 1), false, this, QString(), true);
            if((*(dataTextEdit + i))->getDataUTextEdit()->text().isEmpty())
                (*(dataTextEdit + i))->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
            (*(dataTextEdit + i))->setGeometry((width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * i, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            (*(dataTextEdit + i))->getDataUTextEdit()->setGeometry(0, 0, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            (*(dataTextEdit + i))->getDataUTextEdit()->installEventFilter(catchEvents);
            (*(dataTextEdit + i))->show();
        }
    }
    if(parent->getPressureList())
        currentIndex = *parent->getPressureList()->getCurrentIndex();
    numberOfWrongData = 0;
    resize(width(), (row <= 8)? MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT : row * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
}

void MaterialsChangesWidgetArray::addElement(){
    {
        QLabel** temp = new QLabel*[row + 1];
        for(uint i = 0; i < row; i++)
           (*(temp + i)) = (*(removeButton + i));
        (*(temp + row)) = new QLabel(this);
        (*(temp + row))->setGeometry(width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * row, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        (*(temp + row))->setPixmap(QPixmap(REMOVE_BUTTON_ICON));
        (*(temp + row))->installEventFilter(catchEvents);
        (*(temp + row))->show();
        if(removeButton != nullptr)
            delete []removeButton;
        removeButton = temp;
    }
    FormatableDataField<SubstancesParamsFileParam>** temp = new FormatableDataField<SubstancesParamsFileParam>*[row + 1];
    for(uint i = 0; i < row; i++)
        (*(temp + i)) = (*(temperatureTextEdit + i));
    (*(temp + row)) = new FormatableDataField<SubstancesParamsFileParam>(true, nullptr, false, this, QString(), true);
    (*(temp + row))->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
    (*(temp + row))->setGeometry(0, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * row, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    (*(temp + row))->getDataUTextEdit()->setGeometry(0, 0, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    (*(temp + row))->getDataUTextEdit()->installEventFilter(catchEvents);
    (*(temp + row))->show();
    if(temperatureTextEdit)
        delete []temperatureTextEdit;
    temperatureTextEdit = temp;
    temp = new FormatableDataField<SubstancesParamsFileParam>*[row + 1];
    for(uint i = 0; i < row; i++)
        (*(temp + i)) = (*(dataTextEdit + i));
    (*(temp + row)) = new FormatableDataField<SubstancesParamsFileParam>(true, nullptr, false, this, QString(), true);
    (*(temp + row))->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
    (*(temp + row))->setGeometry((width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * row, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    (*(temp + row))->getDataUTextEdit()->setGeometry(0, 0, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    (*(temp + row))->getDataUTextEdit()->installEventFilter(catchEvents);
    (*(temp + row))->show();
    if(dataTextEdit)
        delete []dataTextEdit;
    dataTextEdit = temp;
    row++;
    numberOfWrongData += 2;
    if(parent->getPressureList())
        currentIndex = *parent->getPressureList()->getCurrentIndex();
    resize(width(), (row <= 8)? MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT : row * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
}

void MaterialsChangesWidgetArray::removeElement(uint index){
   if(row > 1){
        {
            QLabel** temp = new QLabel*[row - 1];
            for(uint i = 0; i < row; i++)
                if(index > i)
                    (*(temp + i)) = (*(removeButton + i));
                else
                {
                    if(index != i)
                    {
                        (*(temp + i - 1)) = (*(removeButton + i));
                        (*(temp + i - 1))->setGeometry(width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * (i - 1), MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                    }
                    else
                    {
                        if(!FormatableData<SubstancesParamsFileParam>::checkStringFormat((*(temperatureTextEdit + i))->getDataUTextEdit()->text(), true))
                            numberOfWrongData--;
                        if(!FormatableData<SubstancesParamsFileParam>::checkStringFormat((*(dataTextEdit + i))->getDataUTextEdit()->text(), false))
                            numberOfWrongData--;
                        delete (*(removeButton + i));
                        (*(removeButton + i)) = nullptr;
                    }
                }
            delete []removeButton;
            removeButton = temp;
        }
        FormatableDataField<SubstancesParamsFileParam>** temp = new FormatableDataField<SubstancesParamsFileParam>*[row - 1];
        for(uint i = 0; i < row; i++)
            if(index > i)
                (*(temp + i)) = (*(temperatureTextEdit + i));
            else
            {
                if(index != i){
                    (*(temp + i - 1)) = (*(temperatureTextEdit + i));
                    (*(temp + i - 1))->setGeometry(0, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * (i - 1), (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                }else{
                    delete (*(temperatureTextEdit + i));
                    (*(temperatureTextEdit + i)) = nullptr;
                }
            }
        delete []temperatureTextEdit;
        temperatureTextEdit = temp;
        temp = new FormatableDataField<SubstancesParamsFileParam>*[row - 1];
        for(uint i = 0; i < row; i++)
            if(index > i)
                (*(temp + i)) = (*(dataTextEdit + i));
            else
            {
                if(index != i){
                    (*(temp + i - 1)) = (*(dataTextEdit + i));
                    (*(temp + i - 1))->setGeometry((width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * (i - 1), (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
                }else{
                    delete (*(dataTextEdit + i));
                    (*(dataTextEdit + i)) = nullptr;
                }
            }
        delete []dataTextEdit;
        dataTextEdit = temp;
        row--;
   }else{
       if(FormatableData<SubstancesParamsFileParam>::checkStringFormat((*temperatureTextEdit)->getDataUTextEdit()->text(), true))
           numberOfWrongData--;
       if(FormatableData<SubstancesParamsFileParam>::checkStringFormat((*dataTextEdit)->getDataUTextEdit()->text(), false))
           numberOfWrongData--;
       delete (*removeButton);
       (*removeButton) = nullptr;
       delete (*temperatureTextEdit);
       (*temperatureTextEdit) = nullptr;
       delete (*dataTextEdit);
       (*dataTextEdit) = nullptr;
       delete []removeButton;
       removeButton = nullptr;
       delete []temperatureTextEdit;
       temperatureTextEdit = nullptr;
       delete []dataTextEdit;
       dataTextEdit = nullptr;
       row = 0;
   }
   resize(width(), (row <= 8)? MATERIALS_CHANGES_WIDGET_ARRAY_HEIGHT : row * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
}

MaterialsChangesWidget<SubstancesParamsFileParam>* MaterialsChangesWidgetArray::getParent(){
    return parent;
}

uint MaterialsChangesWidgetArray::getRow(){
    return row;
}

uint MaterialsChangesWidgetArray::getNumbOfWrongData(){
    return numberOfWrongData;
}

bool MaterialsChangesWidgetArray::lineEditKeyPressed(QKeyEvent* ev, uint index, bool temperatureObj){
    uint i = 0;
    FormatableDataField<SubstancesParamsFileParam>* obj = ((temperatureObj)? (*(temperatureTextEdit + index)) : (*(dataTextEdit + index)));
    uint len = obj->getDataUTextEdit()->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
    // Bezpośrenie połaczenie z KEY_PERIOD
    case Qt::Key_Period:
    {
        QLineEdit *te = obj->getDataUTextEdit();
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
        obj->getDataUTextEdit()->del();
        break;
    case Qt::Key_Backspace:
        obj->getDataUTextEdit()->backspace();
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
        if(temperatureObj){
            int selStart = obj->getDataUTextEdit()->selectionStart();
            if(selStart != -1){
                if(selStart == 0){
                    int i = 0;
                    for(i = obj->getDataUTextEdit()->selectionEnd(); i < len; i++){
                        if(obj->getDataUTextEdit()->text().at(i) == 'e'){
                            obj->getDataUTextEdit()->insert("-");
                            break;
                        }else
                            if(obj->getDataUTextEdit()->text().at(i) == '-'){
                                obj->getDataUTextEdit()->insert("");
                                break;
                            }
                    }
                    if(i == len)
                        obj->getDataUTextEdit()->insert("-");
                }else{
                    if(obj->getDataUTextEdit()->text().at(selStart - 1) == 'e')
                        obj->getDataUTextEdit()->insert("-");
                }
            }else{
                int curPos = obj->getDataUTextEdit()->cursorPosition();
                if(curPos == 0){
                    if(curPos == len || obj->getDataUTextEdit()->text().at(curPos) != '-')
                        obj->getDataUTextEdit()->insert("-");
                }else{
                    if( obj->getDataUTextEdit()->text().at(curPos - 1) == 'e' && (curPos == len || obj->getDataUTextEdit()->text().at(curPos) != '-'))
                        obj->getDataUTextEdit()->insert("-");
                }
            }
        }
    }
        break;
    case Qt::Key_E:
        for(uint i = 0; i < len; i++)
            if(obj->getDataUTextEdit()->text().at(i) == 'e')
                return true;
        obj->getDataUTextEdit()->insert(ev->text().toLower());
       break;
       default:
        if(ev->key() >= 0x30 && ev->key() <= 0x39)
            obj->getDataUTextEdit()->insert(ev->text());
        else
            return true;
    }
    if(FormatableData<SubstancesParamsFileParam>::checkStringFormat(obj->getDataUTextEdit()->text(), (temperatureObj)? true : false) && ((temperatureObj)? !isDuplicated(index) : true)){
        if(!obj->isFormatValid())
            numberOfWrongData--;
        obj->setFormatValidity(true);
        obj->getDataUTextEdit()->setStyleSheet("");
    }else{
        if(obj->isFormatValid())
            numberOfWrongData++;
        obj->setFormatValidity(false);
        obj->getDataUTextEdit()->setStyleSheet("QLineEdit{border: 1px solid red;}");
    }
    return true;
}

bool MaterialsChangesWidgetArray::checkLineEdits(){
    return (numberOfWrongData == 0)? true : false;
}

uint MaterialsChangesWidgetArray::getCurrentIndex(){
    return currentIndex;
}

void MaterialsChangesWidgetArray::setCurrentIndex(uint set){
    currentIndex = set;
}

void MaterialsChangesWidgetArray::updateGeometry(){
    for(uint i = 0; i < row; i++){
        (*(removeButton + i))->setGeometry(width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * i, MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        (*(temperatureTextEdit + i))->setGeometry(0, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * i, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        (*(dataTextEdit + i))->setGeometry((width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT * i, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        (*(temperatureTextEdit + i))->getDataUTextEdit()->setGeometry(0, 0, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        (*(dataTextEdit + i))->getDataUTextEdit()->setGeometry(0, 0, (width() - MATERIALS_CHANGES_WIDGET_CLOSE_BUTTON_WIDTH) * 0.5, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    }
}

bool MaterialsChangesWidgetArray::isDuplicated(uint index){
    auto value = (*(temperatureTextEdit + index))->getDataUTextEdit()->text().toDouble();
    for(uint i = 0; i < row; i++){
        if(index != i && (*(temperatureTextEdit + i))->getDataUTextEdit()->text().toDouble() == value)
            return true;
    }
    return false;
}

int MaterialsChangesWidgetArray::checkTemperatureExist(double temp){
    for(int i = 0; i < row; i++)
        if((*(temperatureTextEdit + i))->getDataUTextEdit()->text().toDouble() == temp)
            return i;
    return -1;
}
