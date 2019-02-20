#include"mylistcatchevents.hpp"
#include<QTimer>
#include<QEvent>
#include<QScrollBar>
#include"mylist.hpp"
#include"windowmacros.hpp"
#include"materialschangeswidget.hpp"
#include<QKeyEvent>
#include<QMouseEvent>
#include<QScrollArea>
#include"formatabledatafield.hpp"
#include"formatchangepopup.hpp"
#include"mydialog.hpp"
#include"materialschangeswidget.hpp"

MyListCatchEvents::MyListCatchEvents(MyList *setParent){
    parent = setParent;
    timer.setInterval(1000);
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, [this](){this->pressedSetFalse();});
}

MyListCatchEvents::~MyListCatchEvents(){
    // Brak
}

bool MyListCatchEvents::eventFilter(QObject *obj, QEvent *ev){
    switch(ev->type()){
    case QEvent::MouseButtonPress:
        if(!parent->getEnteringData()){
            uint numbOfElem = parent->getParent()->getPressureScrollArea()->verticalScrollBar()->value() / MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT;
            uint row = (numbOfElem + 11 < parent->getNumbOfElem())? numbOfElem + 11 : parent->getNumbOfElem();
            for( ; numbOfElem < row ; numbOfElem++)
                    if(*(parent->getDataElement() + numbOfElem) == obj){
                        if(!pressed){
                            pressed = true;
                            timerObj = obj;
                            timer.start();
                        }
                        return true;
                    }

        }
        break;
    case QEvent::MouseButtonRelease:
        if(!parent->getEnteringData()){
            if(pressed){
                pressed = false;
                timer.stop();
                uint numbOfElem = parent->getParent()->getPressureScrollArea()->verticalScrollBar()->value() / MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT;
                uint row = (numbOfElem + 11 < parent->getNumbOfElem())? numbOfElem + 11 : parent->getNumbOfElem();
                for( ; numbOfElem < row ; numbOfElem++)
                        if(*(parent->getDataElement() + numbOfElem) == obj){
                            parent->dataElementClicked(numbOfElem);
                            timerObj = nullptr;
                            return true;
                        }
            }else{
                uint numbOfElem = parent->getParent()->getPressureScrollArea()->verticalScrollBar()->value() / MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT;
                uint row = (numbOfElem + 11 < parent->getNumbOfElem())? numbOfElem + 11 : parent->getNumbOfElem();
                for( ; numbOfElem < row ; numbOfElem++)
                    if(*(parent->getRemoveButton() + numbOfElem) == obj){
                        parent->getParent()->setPressureIndexToDelete([](uint numb)->uint*{uint *ret = new uint; *ret = numb; return ret;}(numbOfElem));
                        return true;
                    }
              }
        }
        break;
    case QEvent::KeyPress:
        if(parent->getEnteringData()){
            uint numbOfElem = parent->getParent()->getPressureScrollArea()->verticalScrollBar()->value() / MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT;
            uint row = (numbOfElem + 11 < parent->getNumbOfElem())? numbOfElem + 11 : parent->getNumbOfElem();
            for( ; numbOfElem < row ; numbOfElem++)
            {
                if(*(parent->getDataElement() + numbOfElem) == obj){
                switch(static_cast<QKeyEvent*>(ev)->key()){
                case Qt::Key_Return:
                    if(!parent->getAddingData())
                        parent->editingDataConfirmed();
                    else
                        parent->addingElementConfirmed();
                    return true;
                case Qt::Key_Escape:
                    if(!parent->getAddingData())
                        parent->editingDataCanceled();
                    else
                        parent->addingElementCanceled();
                    return true;
                default:
                    if(parent->lineEditKeyPressed(static_cast<QKeyEvent*>(ev), static_cast<FormatableDataField<SubstancesParamsFileParam>*>(obj)->getDataUTextEdit()))
                        return true;
                    break;
                }
                numbOfElem = row;
              }
            }
    }
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, ev);
}

void MyListCatchEvents::pressedSetFalse(){
    if(pressed){
        pressed = false;
        timer.stop();
        uint numbOfElem = parent->getParent()->getPressureScrollArea()->verticalScrollBar()->value() / MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT;
        uint row = (numbOfElem + 11 < parent->getNumbOfElem())? numbOfElem + 11 : parent->getNumbOfElem();
        for( ; numbOfElem < row ; numbOfElem++)
            if(*(parent->getDataElement() + numbOfElem) == timerObj){
                parent->dataElementDoubleClicked(numbOfElem);
                timerObj = nullptr;
                return;
            }
    }
}


