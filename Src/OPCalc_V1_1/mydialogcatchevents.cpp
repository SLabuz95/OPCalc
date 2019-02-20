#include"mydialogcatchevents.hpp"
#include<QEvent>
#include"mydialog.hpp"
#include<QKeyEvent>

MyDialogCatchEvents::MyDialogCatchEvents(MyDialog *setParent){
    parent = setParent;
}

MyDialogCatchEvents::~MyDialogCatchEvents(){
    // Brak
}

bool MyDialogCatchEvents::eventFilter(QObject *obj, QEvent *ev){
    switch(ev->type()){
    case QEvent::KeyPress:
       break;
    case QEvent::MouseButtonRelease:
        switch(parent->getType()){
        case DEFAULT_PATH_DIALOG:
            if(parent->getAcceptButton() == obj){
                parent->acceptButtonReleased();
                return true;
            }
            if(parent->getCancelButton() == obj){
                parent->cancelButtonReleased();
                return true;
            }
            if(parent->getFileDialogButton() == obj){
                parent->fileDialogButtonReleased();
                return true;
            }
            if(parent->getResetToDefaultPathButton() == obj){
                parent->resetToDefaultPathButtonReleased();
                return true;
            }
            break;
        case QUESTION_DIALOG:
            if(parent->getYesNoQuestion()){
                if(parent->getYesButton() == obj){
                    parent->yesButtonReleased();
                    return true;
                }
                if(parent->getNoButton() == obj){
                    parent->noButtonReleased();
                    return true;
                }
             }else{
                if(parent->getFirstOptionLabel() == obj){
                    parent->firstOptionLabelReleased();
                    return true;
                }
                if(parent->getSecondOptionLabel() == obj){
                    parent->secondOptionLabelReleased();
                    return true;
                }
                if(parent->getThirdOptionLabel() == obj){
                    parent->thirdOptionLabelReleased();
                    return true;
                }
            }
            break;
        case SUBSTANCE_FILE_ERRORS_LIST:
            if(parent->getFirstOptionLabel() == obj){
                parent->deleteLater();
                return true;
            }
            break;
        default:
            if(parent->getAcceptButton() == obj){
                parent->acceptButtonReleased();
                return true;
            }
            if(parent->getCancelButton() == obj){
                parent->cancelButtonReleased();
                return true;
            }
            break;
        }
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, ev);
}
