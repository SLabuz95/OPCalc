#include"projectlistcatchevents.hpp"
#include<QEvent>
#include<QScrollBar>
#include<QPalette>
#include"projectlist.hpp"
#include"windowmacros.hpp"
#include"projectlistelementop.hpp"
#include"innerwindowcp.hpp"
#include"projectinfopanelcp.hpp"
#include"projectlistelementdp.hpp"
#include<QScrollArea>
#include"appwindow.hpp"

ProjectListCatchEvents::ProjectListCatchEvents(ProjectList *setParent){
    parent = setParent;
}

bool ProjectListCatchEvents::eventFilter(QObject *obj, QEvent *ev){
    switch((uint) ev->type()){
    case (uint) QEvent::MouseButtonRelease:
    {
        if(*(parent->getListType()) == LIST_OPEN_PROJECT){
            if(*(parent->getNumbOfElements()) > 5){
               uint numb = parent->getParent()->getScrollArea()->verticalScrollBar()->value() / (PROJECT_LIST_VERTICAL_OFFSET + PROJECT_LIST_ELEMENT_OP_HEIGHT);
               if(*(parent->getNumbOfElements()) - 5 > numb)
               {
                   for(uint i = numb ; i < numb + 6 ; i++){
                       if(obj == *(parent->getProjectListElementOP()) + i){
                           parent->getParent()->getProjectInfoPanelOP()->generateInformationAboutProject(i + 1);
                           parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                           return true;
                       }
                       if(obj == ((*parent->getProjectListElementOP() + i))->getDownInfoLabel()){
                           parent->getParent()->getProjectInfoPanelOP()->generateInformationAboutProject(i + 1);
                           parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                           return true;
                       }
                       if(obj == (*parent->getProjectListElementOP() + i)->getRemoveButton()){
                           (*parent->getProjectListElementOP() + i)->removeButtonReleased();
                           return true;
                       }
                   }
               }else{
                   for(uint i = numb ; i <= *(parent->getNumbOfElements()) ; i++){
                       if(obj == *(parent->getProjectListElementOP()) + i){
                           parent->getParent()->getProjectInfoPanelOP()->generateInformationAboutProject(i + 1);
                           parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                           return true;
                       }
                       if(obj == ((*parent->getProjectListElementOP() + i))->getDownInfoLabel()){
                           parent->getParent()->getProjectInfoPanelOP()->generateInformationAboutProject(i + 1);
                           parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                           return true;
                       }
                       if(obj == (*parent->getProjectListElementOP() + i)->getRemoveButton()){
                           (*parent->getProjectListElementOP() + i)->removeButtonReleased();
                           return true;
                       }
               }
               }
            }else{
                for(uint i = 0 ; i <= *(parent->getNumbOfElements()) ; i++){
                    if(obj == *(parent->getProjectListElementOP()) + i){
                        parent->getParent()->getProjectInfoPanelOP()->generateInformationAboutProject(i + 1);
                        parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                        return true;
                    }
                    if(obj == ((*parent->getProjectListElementOP() + i))->getDownInfoLabel()){
                        parent->getParent()->getProjectInfoPanelOP()->generateInformationAboutProject(i + 1);
                        parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                        return true;
                    }
                    if(obj == (*parent->getProjectListElementOP() + i)->getRemoveButton()){
                        (*parent->getProjectListElementOP() + i)->removeButtonReleased();
                        return true;
                    }
            }
            }
        }else{
            if(*(parent->getNumbOfElements()) > 5){
               uint numb = parent->getParent()->getScrollArea()->verticalScrollBar()->value() / (PROJECT_LIST_VERTICAL_OFFSET + PROJECT_LIST_ELEMENT_DP_HEIGHT);
               if(*(parent->getNumbOfElements()) - 5 > numb)
               {
                   for(uint i = numb ; i < numb + 6 ; i++){
                       if(obj == *(parent->getProjectListElementDP()) + i){
                           parent->getParent()->getProjectInfoPanelDP()->generateInformationAboutProject(i + 1);
                           parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                           return true;
                       }
                       if(obj == ((*parent->getProjectListElementDP() + i))->getDownInfoLabel()){
                           parent->getParent()->getProjectInfoPanelDP()->generateInformationAboutProject(i + 1);
                           parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                           return true;
                       }
                       if(obj == (*parent->getProjectListElementDP() + i)->getRemoveButton()){
                           (*parent->getProjectListElementDP() + i)->removeButtonReleased();
                           return true;
                       }
                   }
               }else{
                   for(uint i = numb ; i <= *(parent->getNumbOfElements()) ; i++){
                       if(obj == *(parent->getProjectListElementDP()) + i){
                           parent->getParent()->getProjectInfoPanelDP()->generateInformationAboutProject(i + 1);
                           parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                           return true;
                       }
                       if(obj == ((*parent->getProjectListElementDP() + i))->getDownInfoLabel()){
                           parent->getParent()->getProjectInfoPanelDP()->generateInformationAboutProject(i + 1);
                           parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                           return true;
                       }
                       if(obj == (*parent->getProjectListElementDP() + i)->getRemoveButton()){
                           (*parent->getProjectListElementDP() + i)->removeButtonReleased();
                           return true;
                       }
               }
               }
            }else{
                for(uint i = 0 ; i <= *(parent->getNumbOfElements()) ; i++){
                    if(obj == *(parent->getProjectListElementDP()) + i){
                        parent->getParent()->getProjectInfoPanelDP()->generateInformationAboutProject(i + 1);
                        parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                        return true;
                    }
                    if(obj == ((*parent->getProjectListElementDP() + i))->getDownInfoLabel()){
                        parent->getParent()->getProjectInfoPanelDP()->generateInformationAboutProject(i + 1);
                        parent->setSelectedProjectNumb([](uint set)->uint*{uint* ret = new uint; *ret = set; return ret;}(i + 1));
                        return true;
                    }
                    if(obj == (*parent->getProjectListElementDP() + i)->getRemoveButton()){
                        (*parent->getProjectListElementDP() + i)->removeButtonReleased();
                        return true;
                    }
            }
            }
        }
    }
        break;
    case QEvent::Enter:
        if(*(parent->getListType()) == LIST_OPEN_PROJECT){
            if(*(parent->getNumbOfElements()) > 5){
               uint numb = parent->getParent()->getScrollArea()->verticalScrollBar()->value() / (PROJECT_LIST_VERTICAL_OFFSET + PROJECT_LIST_ELEMENT_OP_HEIGHT);
               if(*(parent->getNumbOfElements()) - 5 > numb)
               {
                   for(uint i = numb ; i < numb + 6 ; i++){
                       if(obj == (*parent->getProjectListElementOP() + i)->getRemoveButton()){
                           parent->getParent()->getParent()->getStatusBar()->showMessage(QString("Usuń projekt"));
                           return true;
                       }
                       if(obj == ((*parent->getProjectListElementOP() + i))->getDownInfoLabel()){
                           parent->getParent()->getParent()->getStatusBar()->showMessage((*parent->getProjectListElementOP() + i)->getDownInfoLabel()->text());
                           return true;
                       }
                   }
               }else{
                   for(uint i = numb ; i <= *(parent->getNumbOfElements()) ; i++){
                       if(obj == (*parent->getProjectListElementOP() + i)->getRemoveButton()){
                           parent->getParent()->getParent()->getStatusBar()->showMessage(QString("Usuń projekt"));
                           return true;
                       }
                       if(obj == ((*parent->getProjectListElementOP() + i))->getDownInfoLabel()){
                           parent->getParent()->getParent()->getStatusBar()->showMessage((*parent->getProjectListElementOP() + i)->getDownInfoLabel()->text());
                           return true;
                       }
               }
               }
            }else{
                for(uint i = 0 ; i <= *(parent->getNumbOfElements()) ; i++){
                    if(obj == (*parent->getProjectListElementOP() + i)->getRemoveButton()){
                        parent->getParent()->getParent()->getStatusBar()->showMessage(QString("Usuń projekt"));
                        return true;
                    }
                    if(obj == ((*parent->getProjectListElementOP() + i))->getDownInfoLabel()){
                        parent->getParent()->getParent()->getStatusBar()->showMessage((*parent->getProjectListElementOP() + i)->getDownInfoLabel()->text());
                        return true;
                    }
            }
            }
        }else{
            if(*(parent->getNumbOfElements()) > 5){
               uint numb = parent->getParent()->getScrollArea()->verticalScrollBar()->value() / (PROJECT_LIST_VERTICAL_OFFSET + PROJECT_LIST_ELEMENT_DP_HEIGHT);
               if(*(parent->getNumbOfElements()) - 5 > numb)
               {
                   for(uint i = numb ; i < numb + 6 ; i++){
                       if(obj == (*parent->getProjectListElementDP() + i)->getRemoveButton()){
                           parent->getParent()->getParent()->getStatusBar()->showMessage(QString("Usuń projekt"));
                           return true;
                       }
                       if(obj == ((*parent->getProjectListElementDP() + i))->getDownInfoLabel()){
                           parent->getParent()->getParent()->getStatusBar()->showMessage((*parent->getProjectListElementDP() + i)->getDownInfoLabel()->text());
                           return true;
                       }
                   }
               }else{
                   for(uint i = numb ; i <= *(parent->getNumbOfElements()) ; i++){
                       if(obj == (*parent->getProjectListElementDP() + i)->getRemoveButton()){
                           parent->getParent()->getParent()->getStatusBar()->showMessage(QString("Usuń projekt"));
                           return true;
                       }
                       if(obj == ((*parent->getProjectListElementDP() + i))->getDownInfoLabel()){
                           parent->getParent()->getParent()->getStatusBar()->showMessage((*parent->getProjectListElementDP() + i)->getDownInfoLabel()->text());
                           return true;
                       }
               }
               }
            }else{
                for(uint i = 0 ; i <= *(parent->getNumbOfElements()) ; i++){
                    if(obj == (*parent->getProjectListElementDP() + i)->getRemoveButton()){
                        parent->getParent()->getParent()->getStatusBar()->showMessage(QString("Usuń projekt"));
                        return true;
                    }
                    if(obj == ((*parent->getProjectListElementDP() + i))->getDownInfoLabel()){
                        parent->getParent()->getParent()->getStatusBar()->showMessage((*parent->getProjectListElementDP() + i)->getDownInfoLabel()->text());
                        return true;
                    }
            }
         }
        }
        break;
    case QEvent::Leave:
              parent->getParent()->getParent()->getStatusBar()->showMessage(QString("Gotowe"));
              return true;
    default:
        break;
    }
    return QObject::eventFilter(obj, ev);
}



