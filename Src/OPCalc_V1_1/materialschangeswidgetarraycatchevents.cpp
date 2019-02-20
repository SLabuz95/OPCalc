#include"materialschangeswidgetarraycatchevents.hpp"
#include<QEvent>
#include<QKeyEvent>
#include<QScrollBar>
#include"materialschangeswidgetarray.hpp"
#include"windowmacros.hpp"
#include"materialschangeswidget.hpp"
#include"formatabledatafield.hpp"
#include<QScrollArea>
#include<QLabel>
#include<QLineEdit>

MaterialsChangesWidgetArrayCatchEvents::MaterialsChangesWidgetArrayCatchEvents(MaterialsChangesWidgetArray *setParent){
    parent = setParent;
}

MaterialsChangesWidgetArrayCatchEvents::~MaterialsChangesWidgetArrayCatchEvents(){
    // Brak
}

bool MaterialsChangesWidgetArrayCatchEvents::eventFilter(QObject *obj, QEvent *ev){
    uint numbOfElem = parent->getParent()->getScrollArea()->verticalScrollBar()->value() / MATERIALS_CHANGES_WIDGET_BUTTONS_LABEL_HEIGHT;
    uint row = parent->getRow();
    switch(ev->type()){
    case QEvent::MouseButtonRelease:
        for( ; numbOfElem < row ; numbOfElem++)
            if(*(parent->getRemoveButton() + numbOfElem) == obj){
                parent->getParent()->setIndexToDelete([](uint numb)->uint*{uint *ret = new uint; *ret = numb; return ret;}(numbOfElem));
                return true;
            }
        break;
    case QEvent::KeyPress:
        for( ; numbOfElem < row ; numbOfElem++){
            if((*(parent->getTemperatureTextEdit() + numbOfElem))->getDataUTextEdit() == obj){
                if(parent->lineEditKeyPressed(static_cast<QKeyEvent*>(ev), numbOfElem, true))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
            if((*(parent->getDataTextEdit() + numbOfElem))->getDataUTextEdit() == obj){
                if(parent->lineEditKeyPressed(static_cast<QKeyEvent*>(ev), numbOfElem, false))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
        }
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, ev);
}
