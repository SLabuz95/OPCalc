#include"innerwindowcpcatchevents.hpp"
#include<QEvent>
#include<QKeyEvent>
#include"innerwindowcp.hpp"
#include"appwindow.hpp"
#include"titlewidget.hpp"
#include"optionalprojectdatapanel.hpp"
#include<QCheckBox>
#include<QLineEdit>
#include"appclass.hpp"
#include"config.hpp"
#include"formatabledatafield.hpp"
#include"promptpanel.hpp"
#include"mydialog.hpp"


InnerWindowCPCatchEvents::InnerWindowCPCatchEvents(InnerWindowCP *setParent){
    parent = setParent;
}

InnerWindowCPCatchEvents::~InnerWindowCPCatchEvents(){
    parent = nullptr;
}

bool InnerWindowCPCatchEvents::eventFilter(QObject *obj, QEvent *ev){
    switch(*parent->getInnerWinStatCP()){
    case (uint)CHOOSE_PROJECT_INNER:
        switch(ev->type()){
        case (uint)QEvent::MouseButtonRelease:
            if(parent->getNewProjectButtonCP() == obj){
                parent->newProjectButtonCPReleased();
                return true;
            }
            if(parent->getOpenProjectButtonCP() == obj){
                parent->openProjectButtonCPReleased();
                return true;
            }
            if(parent->getDeleteProjectButtonCP() == obj){
                parent->deleteProjectButtonCPReleased();
                return true;
            }
            break;
        case QEvent::Enter:
            if(obj == parent->getNewProjectButtonCP()){
                parent->getParent()->getStatusBar()->showMessage(QString("Utwórz nowy projekt"));
                parent->getNewProjectButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
                return true;
            }
            if(obj == parent->getOpenProjectButtonCP()){
                parent->getParent()->getStatusBar()->showMessage(QString("Otwórz projekt"));
                parent->getOpenProjectButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
                return true;
            }
            if(obj == parent->getDeleteProjectButtonCP()){
                parent->getParent()->getStatusBar()->showMessage(QString("Usuń projekt"));
                parent->getDeleteProjectButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
                return true;
            }
            break;
        case QEvent::Leave:
            if(obj == parent->getNewProjectButtonCP()){
                parent->getNewProjectButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            if(obj == parent->getOpenProjectButtonCP()){
                parent->getOpenProjectButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            if(obj == parent->getDeleteProjectButtonCP()){
                parent->getDeleteProjectButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            parent->getParent()->getStatusBar()->showMessage(QString("Gotowy"));
            return true;
         default:
            return QObject::eventFilter(obj, ev);
        }
        break;
    case (uint)NEW_PROJECT:
    switch(ev->type()){
    case QEvent::MouseButtonRelease:
        if(obj == parent->getProjectPathEditDataLabel() && static_cast<QMouseEvent*>(ev)->button() == Qt::RightButton){
            parent->getParent()->getPromptPanel()->addPrompt(PROMPT_TYPE_LONG_STRINGS, parent->getProjectPathEditDataLabel()->text());
            return true;
        }
        if(obj == parent->getResetToDefaultPathButton())
        {
            parent->resetToDefaultPathButtonReleased();
            return true;
        }
        if(obj == parent->getFileDialogButton() && !parent->getDefaultProjectCB()->isChecked()){
            parent->fileDialogButtonReleased();
            return true;
        }
        if(obj == parent->getDefaultProjectHelpButton()){
            parent->defaultProjectHelpButtonReleased();
            return true;
        }
        if(obj == parent->getCreateProjectButtonNP()){
            parent->createProjectButtonNPReleased();
            return true;
        }
        if(obj == parent->getCancelButtonNP()){
            parent->cancelButtonNPReleased();
            return true;
        }
        if(obj == parent->getEditButton()){
            parent->editButtonReleased();
            return true;
        }
        if(obj == parent->getFinalizeButton()){
            parent->finalizeButtonReleased();
            return true;
        }
        if(obj == parent->getDefaultProjectCB() && !parent->getDefaultProjectCB()->isChecked()){
            parent->resetToDefaultPathButtonReleased();
            MyDialog dlg(QUESTION_DIALOG, QString("Uwaga!"), QString("Przy zmianie ścieżki programu i zatwierdzeniu zmian należy zamknąć i uruchomić aplikację."), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.exec();
            break;
        }
        if(parent->getParent()->getTitleWidget() &&obj == parent->getParent()->getTitleWidget()->getCloseButtonCP())
        {
            parent->getParent()->getTitleWidget()->closeButtonCPRealesed();
            return true;
        }
        break;
    case QEvent::KeyPress:
    {
        QKeyEvent *evK = static_cast<QKeyEvent*>(ev);
        if(evK->matches(QKeySequence::Copy) || evK->matches(QKeySequence::Cut) || evK->matches(QKeySequence::Paste))
             return true;
        if(*parent->getEditProjectData() == false || *parent->getEditableData() == true){
            if(parent->getProjectNameTextEdit()->getDataUTextEdit() == obj){
                if(parent->checkProjectNameTextEdit(static_cast<QKeyEvent*>(ev))){
                    parent->fillProjectPath();
                    return true;
                }else{
                    return QObject::eventFilter(obj, ev);
                }
             }
             if(parent->getOptionalProjectDataPanel()->getCompanyTextEdit() == obj){
                 if(parent->getOptionalProjectDataPanel()->checkCompanyTextEdit(static_cast<QKeyEvent*>(ev))){
                         return true;
                     }
            }
            if(parent->getOptionalProjectDataPanel()->getFlangeTextEdit() == obj){
                if(parent->getOptionalProjectDataPanel()->checkFlangeTextEdit(static_cast<QKeyEvent*>(ev))){
                        return true;
                    }
            }
            if(parent->getOptionalProjectDataPanel()->getObjectTextEdit() == obj){
                if(parent->getOptionalProjectDataPanel()->checkObjectTextEdit(static_cast<QKeyEvent*>(ev))){
                        return true;
                    }
                 }
        }
        break;
    }
    case QEvent::Enter:
        if(obj == parent->getCreateProjectButtonNP()){
            parent->getParent()->getStatusBar()->showMessage(QString("Utwórz nowy projekt"));
            parent->getCreateProjectButtonNP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
        if(obj == parent->getCancelButtonNP()){
            parent->getParent()->getStatusBar()->showMessage(QString("Powrót do strony startowej"));
            parent->getCancelButtonNP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
        if(obj == parent->getEditButton()){
            if(*parent->getEditableData())
                parent->getParent()->getStatusBar()->showMessage(QString("Zapisz i zakończ edycję"));
            else
                parent->getParent()->getStatusBar()->showMessage(QString("Edytuj dane projektu"));
            parent->getEditButton()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
        if(obj == parent->getFinalizeButton()){
            if(*parent->getEditableData())
                parent->getParent()->getStatusBar()->showMessage(QString("Nie zapisuj zmian i powróć do obliczania parametrów"));
            else
                parent->getParent()->getStatusBar()->showMessage(QString("Powróć do obliczania parametrów"));
            parent->getFinalizeButton()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
        if(obj == parent->getFileDialogButton()){
            parent->getParent()->getStatusBar()->showMessage(QString("Wybierz ścieżkę zapisu projektu"));
            return true;
        }
        if(obj == parent->getProjectPathEditDataLabel()){
            parent->getParent()->getStatusBar()->showMessage(parent->getProjectPathEditDataLabel()->text());
            return true;
        }
        if(obj == parent->getResetToDefaultPathButton()){
            parent->getParent()->getStatusBar()->showMessage(QString("Wybierz ścieżkę domyślną zapisu projektów.\t") + parent->getParent()->getApplication()->getConfiguration()->getDefaultPath());
            return true;
        }
        if(parent->getParent()->getTitleWidget() &&obj == parent->getParent()->getTitleWidget()->getCloseButtonCP()){
            parent->getParent()->getStatusBar()->showMessage("Zamknij panel");
            parent->getParent()->getTitleWidget()->getCloseButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
            break;
    case QEvent::Leave:
        if(obj == parent->getCreateProjectButtonNP()){
            parent->getCreateProjectButtonNP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        }
        if(obj == parent->getCancelButtonNP()){
            parent->getCancelButtonNP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        }
        if(obj == parent->getEditButton()){
            parent->getEditButton()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        }
        if(obj == parent->getFinalizeButton()){
            parent->getFinalizeButton()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        }
        if(parent->getParent()->getTitleWidget() &&obj == parent->getParent()->getTitleWidget()->getCloseButtonCP()){
            parent->getParent()->getTitleWidget()->getCloseButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
        }
        parent->getParent()->getStatusBar()->showMessage("Gotowy");
        break;
    default:
        return QObject::eventFilter(obj, ev);
    }
    break;
    case (uint)OPEN_PROJECT:
        switch((uint)ev->type()){
      case QEvent::MouseButtonRelease:
        if(obj == parent->getOpenProjectButtonOP())
        {
            parent->openProjectButtonOPReleased();
            return true;
        }
        if(obj == parent->getOtherButtonOP()){
            parent->otherProjectButtonOPReleased();
            return true;
        }
        if(obj == parent->getCancelButtonOP()){
            parent->cancelButtonOPReleased();
            return true;
        }
        if(parent->getParent()->getTitleWidget() &&obj == parent->getParent()->getTitleWidget()->getCloseButtonCP())
        {
            parent->getParent()->getTitleWidget()->closeButtonCPRealesed();
            return true;
        }
         break;
        case QEvent::Enter:
        if(obj == parent->getOpenProjectButtonOP()){
            parent->getParent()->getStatusBar()->showMessage(QString("Otwórz wybrany projekt"));
            parent->getOpenProjectButtonOP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
        if(obj == parent->getOtherButtonOP()){
            parent->getParent()->getStatusBar()->showMessage(QString("Otwórz inny projekt"));
            parent->getOtherButtonOP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
        if(obj == parent->getCancelButtonOP()){
            parent->getParent()->getStatusBar()->showMessage(QString("Powrót do strony startowej"));
            parent->getCancelButtonOP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
        if(parent->getParent()->getTitleWidget() &&obj == parent->getParent()->getTitleWidget()->getCloseButtonCP()){
            parent->getParent()->getStatusBar()->showMessage("Zamknij panel");
            parent->getParent()->getTitleWidget()->getCloseButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
            return true;
        }
            break;
        case QEvent::Leave:
            if(obj == parent->getOpenProjectButtonOP()){
                parent->getOpenProjectButtonOP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            if(obj == parent->getOtherButtonOP()){
                parent->getOtherButtonOP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            if(obj == parent->getCancelButtonOP()){
                parent->getCancelButtonOP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            if(parent->getParent()->getTitleWidget() && obj == parent->getParent()->getTitleWidget()->getCloseButtonCP()){
                parent->getParent()->getTitleWidget()->getCloseButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            parent->getParent()->getStatusBar()->showMessage("Gotowy");
            return true;
         default:
            return QObject::eventFilter(obj, ev);
        }
        break;
    case (uint)DELETE_PROJECT:
        switch(ev->type()){
        case QEvent::MouseButtonRelease:
            if(obj == parent->getDeleteProjectButtonDP()){
                parent->deleteProjectButtonDPReleased();
                return true;
            }
            if(obj == parent->getCancelButtonDP()){
                parent->cancelButtonDPReleased();
                return true;
            }
            if(obj == parent->getParent()->getTitleWidget()->getCloseButtonCP())
            {
                parent->getParent()->getTitleWidget()->closeButtonCPRealesed();
                return true;
            }
            break;
        case QEvent::Enter:
            if(obj == parent->getDeleteProjectButtonDP()){
                parent->getParent()->getStatusBar()->showMessage(QString("Usuń zaznaczone projekty"));
                parent->getDeleteProjectButtonDP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
                return true;
            }
            if(obj == parent->getCancelButtonDP()){
                parent->getParent()->getStatusBar()->showMessage(QString("Powrót do strony startowej"));
                parent->getCancelButtonDP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
                return true;
            }
            if(parent->getParent()->getTitleWidget() && obj == parent->getParent()->getTitleWidget()->getCloseButtonCP()){
                parent->getParent()->getStatusBar()->showMessage("Zamknij panel");
                parent->getParent()->getTitleWidget()->getCloseButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(240,240,240,0.6);");
                return true;
            }
            break;
        case QEvent::Leave:
            if(obj == parent->getDeleteProjectButtonDP()){
                parent->getDeleteProjectButtonDP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            if(obj == parent->getCancelButtonDP()){
                parent->getCancelButtonDP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            if(parent->getParent()->getTitleWidget() && obj == parent->getParent()->getTitleWidget()->getCloseButtonCP()){
                parent->getParent()->getTitleWidget()->getCloseButtonCP()->setStyleSheet("font-family: \"Georgia\"; font-weight: 400;border-radius: 4px; border-color: rgba(190,190,190,0.6); border-width: 1px; background-color: rgba(190,190,190,0.6);");
            }
            parent->getParent()->getStatusBar()->showMessage("Gotowy");
            return true;
        default:
            return QObject::eventFilter(obj, ev);
        }
        break;
    }
    return QObject::eventFilter(obj, ev);
}
