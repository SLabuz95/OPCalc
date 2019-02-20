#include"panelcepcatchevents.hpp"
#include"panelcepelementflangetype.hpp"
#include"panelcep.hpp"
#include"appwindow.hpp"
#include"innerwindowcep.hpp"
#include"appclass.hpp"
#include<QEvent>


PanelCEPCatchEvents::PanelCEPCatchEvents(PanelCEP *setParent){
parent = setParent;
}

bool PanelCEPCatchEvents::eventFilter(QObject *obj, QEvent *ev){
    switch((uint)ev->type()){
    {
    case (uint) QEvent::MouseButtonRelease:
            parent->saveInnerWinCEPData();
            if(static_cast<PanelCEPElement*>(obj)->getNumberOfPanel() != *parent->getParent()->getInnerWinCEP()->getInnerWinStatusCEP())
                switch(static_cast<PanelCEPElement*>(obj)->getNumberOfPanel()){
                case (uint)CHOOSE_PROJECT_CEP:
                    (parent)->getParent()->setWinStat(CHOOSE_PROJECT);
                    return true;
                case (uint)FLANGE:
                    (parent)->getParent()->getInnerWinCEP()->setInnerWinStatusCEP(FLANGE);
                    break;
                case (uint)PIPE:
                    (parent)->getParent()->getInnerWinCEP()->setInnerWinStatusCEP(PIPE);
                    break;
                case (uint)FLOW:
                    (parent)->getParent()->getInnerWinCEP()->setInnerWinStatusCEP(FLOW);
                    break;
                case (uint)FLUID:
                    (parent)->getParent()->getInnerWinCEP()->setInnerWinStatusCEP(FLUID);
                    break;
                case (uint)RESULT:
                    if(App::isReadyToCalculate(parent->getParent()->getInnerWinCEP()->getProjectInfo(), parent->getParent()->getInnerWinCEP()->getCustomDataPos()))
                    {
                        if(App::calculate(parent->getParent()->getInnerWinCEP()->getProjectInfo(), parent->getParent()->getInnerWinCEP()->getCustomDataPos()))
                            parent->getParent()->getInnerWinCEP()->setInnerWinStatusCEP(RESULT);
                    }
                    break;
                }
            for(uint i = 0; i < 6; i++){
                    (*(parent->getPanelCEPElement() + i))->setStyleSheet(QString("PanelCEPElement{border: 1px solid rgba(255, 255, 255, 0.4);") + ((parent->getParent()->getInnerWinCEP() != nullptr && i == *parent->getParent()->getInnerWinCEP()->getInnerWinStatusCEP())? QString("background-color: qlineargradient(x1:0 , y1: 0.5, x2: 1, y2: 0.5, stop: 0 rgba(200,200,200,0.77), stop: 0.93 rgba(247,247,247,0.8), stop: 0.99 rgba(247,247,247,0.8));") : QString("background-color: rgba(255, 255, 255, 0.4);")) + ((i == 0)? QString("border-top-right-radius: 20px 15px;") : QString("")) +  ((i == 5)? QString("border-bottom-right-radius: 20px 15px;") : QString("")) + "}");
            }
            return true;
    }
    case QEvent::Enter:
        switch(static_cast<PanelCEPElement*>(obj)->getNumberOfPanel()){
        case 0:
            parent->getParent()->getStatusBar()->showMessage("Powrót do strony startowej");
            return true;
            break;
        case 1:
            parent->getParent()->getStatusBar()->showMessage("Przejdź do zakładki \"Kryza\"");
            return true;
            break;
        case 2:
            parent->getParent()->getStatusBar()->showMessage("Przejdź do zakładki \"Rurociąg\"");
            return true;
            break;
        case 3:
            parent->getParent()->getStatusBar()->showMessage("Przejdź do zakładki \"Przepływ\"");
            return true;
            break;
        case 4:
            parent->getParent()->getStatusBar()->showMessage("Przejdź do zakładki \"Substancja\"");
            return true;
            break;
        case 5:
            parent->getParent()->getStatusBar()->showMessage("Przejdź do zakładki \"Wyniki\"");
            return true;
        default:
            break;
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
