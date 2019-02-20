#include"projectinfopanelcepcatchevents.hpp"
#include"projectinfopanelcep.hpp"
#include"appwindow.hpp"
#include<QEvent>

ProjectInfoPanelCEPCatchEvents::ProjectInfoPanelCEPCatchEvents(ProjectInfoPanelCEP *setParent){
    parent = setParent;
}

ProjectInfoPanelCEPCatchEvents::~ProjectInfoPanelCEPCatchEvents(){

}

bool ProjectInfoPanelCEPCatchEvents::eventFilter(QObject *obj, QEvent *ev){
    switch(ev->type()){
    case QEvent::MouseButtonRelease:
        if(obj == parent->getInfoButton()){
            parent->infoButtonReleased();
            return true;
        }
        break;
    case QEvent::Enter:
        if(obj == parent->getInfoButton()){
            parent->getParent()->getStatusBar()->showMessage("Otwórz panel informacji o projekcie");
            return true;
        }
        break;
    case QEvent::Leave:
        parent->getParent()->getStatusBar()->showMessage("Gotowy");
        return true;
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, ev);
}

