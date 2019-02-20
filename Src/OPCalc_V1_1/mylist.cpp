#include"mylist.hpp"
#include"mylistcatchevents.hpp"
#include"materialschangeswidgetdataholder.hpp"
#include"windowmacros.hpp"
#include"materialschangeswidget.hpp"
#include"mainmacros.hpp"
#include"formatabledata.hpp"
#include"materialschangeswidgetarray.hpp"
#include<QEvent>
#include"dialogtypeenum.hpp"
#include"mydialog.hpp"
#include"config.hpp"
#include<QKeyEvent>
#include"appclass.hpp"
#include"appwindow.hpp"
#include"formatabledatafield.hpp"
#include<QComboBox>


MyList::MyList(MaterialsChangesWidget<SubstancesParamsFileParam> *setParent){
    parent = setParent;
    init();
    setWindow();
    createWidgets();
    createLayout();
    connectAll();
}

MyList::~MyList(){
    delete catchEvent;
    deleteWidgets();
}

void MyList::init(){
    catchEvent =  new MyListCatchEvents(this);
}

void MyList::setWindow(){
    resize(MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, (numbOfElem <= 8)? (8 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT) : (numbOfElem * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT));
}

void MyList::createWidgets(){
    // Brak
}

void MyList::createLayout(){
    // Brak
}

void MyList::deleteWidgets(){
    if(numbOfElem != 0){
        for(uint i = 0; i < numbOfElem; i++){
            delete (*(dataElement + i));
            delete (*(removeButton + i));
            }
        delete []dataElement;
        delete []removeButton;
        delete []lastDataElement;
        lastDataElement = nullptr;
        delete currentIndex;
        }
}

void MyList::connectAll(){
    // Brak
}

void MyList::disconnectAll(){
    // Brak
}

MaterialsChangesWidget<SubstancesParamsFileParam>* MyList::getParent(){
    return parent;
}

bool MyList::getEnteringData(){
    return enteringData;
}

uint MyList::getNumbOfElem(){
    return numbOfElem;
}

uint* MyList::getCurrentIndex(){
    return currentIndex;
}

FormatableDataField<SubstancesParamsFileParam>** MyList::getDataElement(){
    return dataElement;
}

QLabel** MyList::getRemoveButton(){
    return removeButton;
}

void MyList::createElements(MaterialsChangesWidgetDataHolder *dataHolder){
    if(dataElement != nullptr){
        for(uint i = 0; i < numbOfElem; i++){
            delete (*(dataElement + i));
            (*(dataElement + i)) = nullptr;
            delete (*(removeButton + i));
            (*(removeButton + i)) =  nullptr;
        }
        delete []dataElement;
        dataElement = nullptr;
        delete []removeButton;
        removeButton = nullptr;
    }
     numbOfElem = dataHolder->getNumbOfPressures();
        if(numbOfElem != 0){
            dataElement =  new FormatableDataField<SubstancesParamsFileParam>*[numbOfElem];
            removeButton = new QLabel*[numbOfElem];
            for(uint i = 0; i < numbOfElem; i++){
                (*(dataElement + i)) = new FormatableDataField<SubstancesParamsFileParam>(false, (*(dataHolder->getPressure() + i)), false, this , QString(), true);
                (*(dataElement + i))->setGeometry(3, 25 * i, width() - 3 - 25, 25);
                (*(dataElement + i))->getDataULabel()->setGeometry(0, 0, (*(dataElement + i))->width(), 25);
                (*(dataElement + i))->installEventFilter(catchEvent);
                (*(dataElement + i))->show();
                (*(removeButton + i)) =  new QLabel(this);
                (*(removeButton + i))->setPixmap(QPixmap(REMOVE_BUTTON_ICON));
                (*(removeButton + i))->setGeometry(width() - 25, 25 * i, 25, 25);
                (*(removeButton + i))->installEventFilter(catchEvent);
                (*(removeButton + i))->show();
            }
            if(currentIndex)
            {
                if(parent->getDensityTabActive())
                    *currentIndex = parent->getDensityDataHolder()->getSelectedIndex();
                else
                    *currentIndex = parent->getDynamicViscosityDataHolder()->getSelectedIndex();
                highlight(*currentIndex);
            }
         }
    resize(MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, (numbOfElem <= 8)? (8 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT) : (numbOfElem * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT));
}

void MyList::addElement(){
    {
        FormatableDataField<SubstancesParamsFileParam>** temp = new FormatableDataField<SubstancesParamsFileParam>*[numbOfElem + 1];
        FormatableDataField<SubstancesParamsFileParam>* newElement = new FormatableDataField<SubstancesParamsFileParam>(true, nullptr, false ,this, QString(), true);
        for(uint i = 0; i < numbOfElem; i++)
            (*(temp + i)) = (*(dataElement + i));
        newElement->setGeometry(3, 25 * numbOfElem, width() - 3, 25);
        newElement->getDataUTextEdit()->setGeometry(0 , 0, newElement->width(), 25);
        newElement->installEventFilter(catchEvent);
        newElement->show();
        (*(temp + numbOfElem)) = newElement;
        delete []dataElement;
        dataElement = temp;
    }
        QLabel **temp = new QLabel*[numbOfElem + 1];
        for(uint i = 0; i < numbOfElem; i++)
            (*(temp + i)) = (*(removeButton + i));
        (*(temp + numbOfElem )) = new QLabel(this);
        (*(temp + numbOfElem ))->setPixmap(QPixmap(REMOVE_BUTTON_ICON));
        (*(temp + numbOfElem ))->setGeometry(width() - 25, 25 * (numbOfElem), 25, 25);
        (*(temp + numbOfElem ))->installEventFilter(catchEvent);
        delete []removeButton;
        removeButton = temp;
        numbOfElem++;
        resize(MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, (numbOfElem <= 8)? (8 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT) : (numbOfElem * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT));
}

void MyList::removeElement(uint index){
    if(numbOfElem > 1)
    {
        {
            FormatableDataField<SubstancesParamsFileParam>** temp = new FormatableDataField<SubstancesParamsFileParam>*[numbOfElem - 1];
            for(uint i = 0; i < numbOfElem; i++)
                if(i < index){
                    (*(temp + i)) = *(dataElement + i);
                }else{
                    if(i != index){
                        (*(temp + i - 1)) = (*(dataElement + i));
                        if((*(temp + i - 1))->isEditable()){
                            (*(temp + i - 1))->setGeometry(3, 25 * (i - 1), width() - 3 , 25);
                            (*(temp + i - 1))->getDataUTextEdit()->setGeometry(0, 0, (*(temp + i - 1))->width(), 25);
                        }
                        else{
                            (*(temp + i - 1))->setGeometry(3, 25 * (i - 1), width() - 3 - 25 , 25);
                            (*(temp + i - 1))->getDataULabel()->setGeometry(0, 0, (*(temp + i - 1))->width(), 25);
                        }

                    }
                    else{
                        delete (*(dataElement + i));
                        (*(dataElement + i)) = nullptr;
                    }
                }
            delete []dataElement;
            dataElement = temp;
        }
        QLabel **temp = new QLabel*[numbOfElem - 1];
        for(uint i = 0; i < numbOfElem; i++)
            if(i < index){
                (*(temp + i)) = *(removeButton + i);
            }else{
                if(i != index){
                    (*(temp + i - 1)) = (*(removeButton + i));
                    (*(temp + i - 1))->setGeometry(width() - 25, 25 * (i - 1), 25, 25);
                }
                else
                {
                    delete (*(removeButton + i));
                    (*(removeButton + i)) = nullptr;
                }
            }
        delete []removeButton;
        removeButton = temp;
        numbOfElem--;

    }else{
        delete (*dataElement);
        delete []dataElement;
        dataElement = nullptr;
        delete (*removeButton);
        delete []removeButton;
        removeButton = nullptr;
        numbOfElem--;
    }
    if(currentIndex){
        if(*currentIndex == index){
            delete currentIndex;
            currentIndex = nullptr;
        }else{
            if(index < *currentIndex){
                (*currentIndex)--;
            }
        }
    }
    resize(MATERIALS_CHANGES_PRESSURE_LIST_WIDTH, (numbOfElem <= 8)? (8 * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT) : (numbOfElem * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT));
}

bool MyList::lineEditKeyPressed(QKeyEvent *ev, QLineEdit* te){
    uint len = te->text().length();
    switch(ev->key()){
    case Qt::Key_Comma:
        //Bezpośrednie połaczenie z KEY_PERIOD
    case Qt::Key_Period:
        {
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
            if(selStart == -1 && te->cursorPosition() > 0 && te->text().at(te->cursorPosition() - 1) == 'e')
                te->insert("-");
        }
    }
        break;
    case Qt::Key_E:
        for(uint i = 0; i < len; i++)
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
    if(FormatableData<SubstancesParamsFileParam>::checkStringFormat(te->text(), false))
        te->setStyleSheet("QLineEdit{border: 1px solid black;}");
    else
        te->setStyleSheet("QLineEdit{border: 1px solid red;}");
    return true;
}

void MyList::highlight(uint index){
    if(!(*(dataElement + index))->isEditable())
        (*(dataElement + index))->getDataULabel()->setStyleSheet("background-color: rgba(4,4,4,100)");
    (*(removeButton + index))->setStyleSheet("background-color: rgba(4,4,4,100)");

}

void MyList::lowlight(uint index){
    if(!(*(dataElement + index))->isEditable())
        (*(dataElement + index))->getDataULabel()->setStyleSheet("");
    (*(removeButton + index))->setStyleSheet("");

}

void MyList::dataElementDoubleClicked(uint index){
    enteringData = true;
    parent->getPressureUnitList()->setEnabled(false);
    enteringDataIndex = new uint;
    *enteringDataIndex = index;
    if(currentIndex && *currentIndex == index)
        lowlight(index);
    (*(dataElement + index))->switchDisplayForm(false);
    (*(dataElement + index))->setGeometry(3, index * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT, width() - 3, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    (*(dataElement + index))->getDataUTextEdit()->setGeometry(0, 0, (*(dataElement + index))->width(), MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
    (*(removeButton + index))->hide();
    parent->createEditDesitionsButtons();
}

bool MyList::dataElementClicked(uint index){
    if(parent->getDataArray())
    {
        if(!parent->getDataArray()->checkLineEdits()){
          parent->moveToArrayError();
          return false;
          }
        if(parent->getDensityTabActive())
            parent->getDensityDataHolder()->readDataFromArray(parent->getDataArray());
        else
            parent->getDynamicViscosityDataHolder()->readDataFromArray(parent->getDataArray());
    }else{
        parent->createArray();
    }
    parent->getDataArray()->setCurrentIndex(index);
    if(currentIndex){
        lowlight(*currentIndex);
        *currentIndex = index;
    }else{
        currentIndex = new uint;
        *currentIndex = index;
    }
    highlight(index);
    if(parent->getDensityTabActive()){
        parent->getDataArray()->createElements(parent->getDensityDataHolder());
        parent->getDensityDataHolder()->setSelectedIndex(index);
    }
    else{
        parent->getDataArray()->createElements(parent->getDynamicViscosityDataHolder());
        parent->getDynamicViscosityDataHolder()->setSelectedIndex(index);
    }
    return true;
}

void MyList::editingDataConfirmed(){
    if(enteringDataIndex){
        if(FormatableData<SubstancesParamsFileParam>::checkStringFormat((*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text(), false) && isPressureExist((*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text().toDouble(), *enteringDataIndex)){
            int newIndex;
            if(currentIndex)
                lowlight(*currentIndex);
            if(parent->getDensityTabActive()){
                newIndex = (int)parent->getDensityDataHolder()->changePressure((*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text(), *enteringDataIndex);
                if(currentIndex)
                    *currentIndex = parent->getDensityDataHolder()->getSelectedIndex();
            }else{
                newIndex = (int)parent->getDynamicViscosityDataHolder()->changePressure((*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text(), *enteringDataIndex);
                if(currentIndex)
                *currentIndex = parent->getDynamicViscosityDataHolder()->getSelectedIndex();
            }
            if(parent->getDataArray())
                parent->getDataArray()->setCurrentIndex(*currentIndex);
            (*(dataElement + *enteringDataIndex))->switchDisplayForm(true);
            (*(dataElement + *enteringDataIndex))->setGeometry(3, *enteringDataIndex * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT, width() -  3 - 25, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            (*(dataElement + *enteringDataIndex))->setGeometry(0, 0, (*(dataElement + *enteringDataIndex))->width(), MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
            (*(removeButton + *enteringDataIndex))->show();
            moveElement((int)(*enteringDataIndex), newIndex);
            if(currentIndex)
                highlight(*currentIndex);
            delete enteringDataIndex;
            enteringDataIndex = nullptr;
            parent->deleteEditDesitionsButtons();
            enteringData = false;
        }
    }else{
        if(!lastDataElement){
            lastDataElement = new FormatableDataField<SubstancesParamsFileParam>*[numbOfElem];
            for(uint i = 0 ; i < numbOfElem; i++)
                (*(lastDataElement + i)) = (*(dataElement + i));
        }
        App::quickSort(dataElement, (int)numbOfElem);
        uint newCurrentIndex = numbOfElem;
        if(currentIndex){
            FormatableDataField<SubstancesParamsFileParam> *currentSelectedElement = (*(lastDataElement + *currentIndex));
            uint i = 0;
            while(currentSelectedElement != (*(dataElement + i)) && i < numbOfElem){
                i++;
            }
            if(i < numbOfElem) // Raczej na 100% niemożliwe spełnienie warunku
                newCurrentIndex = i;
        }
        repositionElements();
        for(uint i = 0; i < numbOfElem - 1; i++){
            if((*(dataElement + i))->getDataUTextEdit()->text().toDouble() == (*(dataElement + i + 1))->getDataUTextEdit()->text().toDouble()){
                (*(dataElement + i))->getDataUTextEdit()->setStyleSheet(QString("QLineEdit{}"));
                (*(dataElement + i + 1))->getDataUTextEdit()->setStyleSheet(QString("QLineEdit{}"));
                parent->moveToEditingError(i);
                return;
            }
        }
        for(uint i =  0; i <  numbOfElem; i++){
            (*(dataElement + i))->switchDisplayForm(true);
            (*(dataElement + i))->setGeometry(3, i * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT, width() - 3 - 25, 25);
            (*(dataElement + i))->getDataULabel()->setGeometry(0, 0, (*(dataElement + i))->width(), 25);
            (*(removeButton + i))->show();
        }
        if(newCurrentIndex < numbOfElem){
            *currentIndex = newCurrentIndex;
            parent->getDataArray()->setCurrentIndex(*currentIndex);
            highlight(*currentIndex);
        }
        if(parent->getDensityTabActive()){
            parent->getDensityDataHolder()->refreshPressureArray(dataElement);
            if(currentIndex)
                parent->getDensityDataHolder()->setSelectedIndex(*currentIndex);
        }
        else{
            parent->getDynamicViscosityDataHolder()->refreshPressureArray(dataElement);
            if(currentIndex)
                parent->getDynamicViscosityDataHolder()->setSelectedIndex(*currentIndex);
        }
        delete []lastDataElement;
        lastDataElement = nullptr;
        parent->deleteEditDesitionsButtons();
        parent->getPressureUnitList()->setEnabled(true);
        enteringData = false;
    }
}

void MyList::editingDataCanceled(){
    if(enteringDataIndex){
        (*(dataElement + *enteringDataIndex))->switchDisplayForm(true);
        (*(dataElement + *enteringDataIndex))->setGeometry(3, *enteringDataIndex * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT, width() - 3, 25);
        (*(dataElement + *enteringDataIndex))->getDataULabel()->setGeometry(0,0, (*(dataElement + *enteringDataIndex))->width() - 25, 25);
        (*(removeButton + *enteringDataIndex))->show();
        delete enteringDataIndex;
        enteringDataIndex = nullptr;
    }else{
        delete []dataElement;
        dataElement = lastDataElement;
        lastDataElement = nullptr;
        for(uint i =  0; i <  numbOfElem; i++){
            (*(dataElement + i))->switchDisplayForm(true);
            (*(dataElement + i))->setGeometry(3, i * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT, width() - 3, 25);
            (*(dataElement + i))->getDataULabel()->setGeometry(0, 0, (*(dataElement + i))->width() - 25, 25);
            (*(removeButton + i))->show();
        }
    }
    enteringData = false;
    if(currentIndex)
        highlight(*currentIndex);
    parent->getPressureUnitList()->setEnabled(true);
    parent->deleteEditDesitionsButtons();
}

void MyList::addingElementConfirmed(){
    if(parent->getDataArray() && !parent->getDataArray()->checkLineEdits()){
      parent->moveToArrayError();
      return;
      }
    if(FormatableData<SubstancesParamsFileParam>::checkStringFormat((*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text(), false) && isPressureExist((*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text().toDouble(), *enteringDataIndex)){
        if(currentIndex)
            lowlight(*currentIndex);
        uint index = (parent->getDensityTabActive())? parent->getDensityDataHolder()->findPressureToAdd((*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text().toDouble()) : parent->getDynamicViscosityDataHolder()->findPressureToAdd((*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text().toDouble());
        if(parent->getDensityTabActive()){
            parent->getDensityDataHolder()->addPressure(index ,(*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text());
            (*(dataElement + numbOfElem - 1))->setFormatableData(*(parent->getDensityDataHolder()->getPressure() + index));
        }
        else{
            parent->getDynamicViscosityDataHolder()->addPressure(index ,(*(dataElement + *enteringDataIndex))->getDataUTextEdit()->text());
            (*(dataElement + numbOfElem - 1))->setFormatableData(*(parent->getDynamicViscosityDataHolder()->getPressure() + index));
        }
        (*(dataElement + numbOfElem - 1))->switchDisplayForm(true);
        (*(removeButton + numbOfElem - 1))->show();
        moveElement((int)numbOfElem - 1, (int)index);
        if(parent->getDataArray())
        {
            if(parent->getDensityTabActive())
                parent->getDensityDataHolder()->readDataFromArray(parent->getDataArray());
            else
                parent->getDynamicViscosityDataHolder()->readDataFromArray(parent->getDataArray());
        }
        else{
            parent->createArray();
        }
        if(!currentIndex)
            currentIndex = new uint;
        *currentIndex = index;
        highlight(index);
        if(parent->getDensityTabActive()){
            parent->getDensityDataHolder()->setSelectedIndex(*currentIndex);
            parent->getDataArray()->createElements(parent->getDensityDataHolder());
        }
        else{
            parent->getDynamicViscosityDataHolder()->setSelectedIndex(*currentIndex);
            parent->getDataArray()->createElements(parent->getDynamicViscosityDataHolder());
        }

        delete enteringDataIndex;
        enteringDataIndex = nullptr;
        addingData = false;
        enteringData = false;
        addingDataForExistingArray = false;
        parent->getPressureUnitList()->setEnabled(true);
        parent->deleteEditDesitionsButtons();
    }

}

void MyList::addingElementCanceled(){
    removeElement(numbOfElem - 1);
    delete enteringDataIndex;
    enteringDataIndex = nullptr;
    addingData = false;
    enteringData = false;
    parent->deleteEditDesitionsButtons();
    parent->getPressureUnitList()->setEnabled(true);
    parent->canceledAddingElement();    
}

void MyList::setCurrentIndex(uint *set){
    if(currentIndex != nullptr)
        delete currentIndex;
    currentIndex = set;
}

int MyList::isPressureExist(double value){
    for(int i = 0; i < numbOfElem; i++){
        if(*((*(dataElement + i))->getFormatableData()->getData()) == value)
           return i;
    }
    return -1;
}

bool MyList::isPressureExist(double value, uint exceptIndex){
    for(uint i = 0; i < numbOfElem; i++){
        if(exceptIndex != i)
            if(*((*(dataElement + i))->getFormatableData()->getData()) == value)
                return false;
    }
    return true;
}

void MyList::addingElement(){
    enteringData = true;
    addingData = true;
    enteringDataIndex = new uint;
    *enteringDataIndex = numbOfElem;
    addElement();
    parent->getPressureUnitList()->setEnabled(false);
    parent->createEditDesitionsButtons();
    parent->moveToAddingElement();
}

bool MyList::getAddingData(){
    return addingData;
}

bool MyList::getAddingDataForExistingArray(){
    return addingDataForExistingArray;
}

void MyList::setAddingDataForExistingArray(bool set){
    addingDataForExistingArray = set;
}

void MyList::rewriteList(double correctionFactor){
    FormatableData<SubstancesParamsFileParam> *temp = nullptr;
    for(uint i = 0; i < numbOfElem; i++){
        temp = (*(dataElement + i))->getFormatableData();
        temp->setData(new double(*temp->getData() * correctionFactor));
        if(temp->getFormat()->right((*(dataElement + i))->getFormatableData()->getFormat()->length() - 1).toUShort() < 2)
            temp->changePrecision(2);
        temp->cutOfZeros();
        (*(dataElement + i))->getDataULabel()->setText(temp->useStringFormat());
    }
}

void MyList::moveElement(int index, int newPos){
    if(index > -1 && newPos > -1 && index != newPos && index < numbOfElem && newPos < numbOfElem){
        FormatableDataField<SubstancesParamsFileParam> *element = (*(dataElement + index));
        QLabel* removeButtonEle = (*(removeButton + index));
        FormatableDataField<SubstancesParamsFileParam> **actualElement = nullptr;
        int i = 0;
        short dir = 0;
        if(index > newPos){
            i = index - 1;
            dir = -1;
        }else{
            i = index + 1;
            dir = 1;
        }
        actualElement = dataElement + i;
        for( ; i != newPos; i += dir, actualElement += dir){
            (*(actualElement - dir)) = (*actualElement);
            (*(removeButton + i - dir)) = (*(removeButton + i));
        }
        (*(actualElement - dir)) = (*actualElement);
        (*(removeButton + i - dir)) = (*(removeButton + i));
        (*actualElement) = element;
        (*(removeButton + i)) = removeButtonEle;
    }
    repositionElements();
}

void MyList::editingAllMode(){
    enteringData = true;
    parent->getPressureUnitList()->setEnabled(false);
    if(currentIndex)
        lowlight(*currentIndex);
    for(uint i = 0; i < numbOfElem; i++){
        (*(dataElement + i))->switchDisplayForm(false);
        (*(dataElement + i))->setGeometry(3, i * MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT, width() - 3, MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT);
        (*(dataElement + i))->getDataUTextEdit()->setGeometry(0, 0, (*(dataElement + i))->width(), 25);
        (*(removeButton + i))->hide();
    }
}

void MyList::repositionElements(){
    for(uint i = 0; i < numbOfElem; i++){
        if((*(dataElement + i))->isEditable()){
            (*(dataElement + i))->setGeometry(3, 25 * i, width() - 3, 25);
            (*(dataElement + i))->getDataUTextEdit()->setGeometry(0, 0, (*(dataElement + i))->width() , 25);
            (*(removeButton + i))->setGeometry(width() - 25, 25 * i, 25, 25);
        }else{
            (*(dataElement + i))->setGeometry(3, 25 * i, width() - 3 - 25, 25);
            (*(dataElement + i))->getDataULabel()->setGeometry(0, 0, (*(dataElement + i))->width() , 25);
            (*(removeButton + i))->setGeometry(width() - 25, 25 * i, 25, 25);
        }
    }
}

