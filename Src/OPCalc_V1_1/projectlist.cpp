#include"projectlist.hpp"
#include"projectlistcatchevents.hpp"
#include"windowmacros.hpp"
#include"innerwindowcp.hpp"
#include"projectlistelementop.hpp"
#include"appwindow.hpp"
#include"appclass.hpp"
#include"config.hpp"
#include"projectlistelementdp.hpp"


ProjectList::ProjectList(ListType type, InnerWindowCP *setParent){
    parent = setParent;
    listType = new ListType;
    *listType = type;
    selectedProjectNumb = nullptr;
    init();
    setWindow();
    createWidgets();
    createLayout();
    connectAll();
}

ProjectList::~ProjectList(){
  disconnectAll();
  deleteWidgets();
  delete selectedProjectNumb;
  selectedProjectNumb = nullptr;
  delete catchEvents;
  catchEvents = nullptr;
  delete numbOfElements;
  numbOfElements = nullptr;
  delete listType;
  listType = nullptr;
}

void ProjectList::init(){
    catchEvents = new ProjectListCatchEvents(this);
    numbOfElements = new uint;
    *numbOfElements = (parent->getParent()->getApplication()->getConfiguration()->getNumbOfProjects());

}

void ProjectList::setWindow(){
setMinimumSize(PROJECT_LIST_WIDTH, PROJECT_LIST_HEIGHT(*numbOfElements));
}

void ProjectList::createWidgets(){
  switch((uint) *listType){
    case (uint)LIST_OPEN_PROJECT:
        projectListElementOP = new ProjectListElementOP*;
        *projectListElementOP = new ProjectListElementOP[*numbOfElements];
        for(uint i = 0; i < *numbOfElements ; i++){
            (*projectListElementOP + i)->start(i + 1);
            (*projectListElementOP + i)->setParent(this);
            static_cast<QFrame*>(*projectListElementOP + i)->setParent(this);
            (*projectListElementOP + i)->installEventFilter(catchEvents);
            (*projectListElementOP + i)->getRemoveButton()->installEventFilter(catchEvents);
            (*projectListElementOP + i)->getDownInfoLabel()->installEventFilter(catchEvents);
            (*projectListElementOP + i)->setStyleSheet("ProjectListElementOP{background-color: rgba(255,255, 255, 0.6); border: 2px ridge rgba(240, 240, 240, 1); border-radius: 6px;}");
            (*projectListElementOP + i)->show();
          }
        break;
    case (uint)LIST_DELETE_PROJECT:
        projectListElementDP = new ProjectListElementDP*;
        *projectListElementDP = new ProjectListElementDP[*numbOfElements];
        for(uint i = 0; i < *numbOfElements ; i++){
            (*projectListElementDP + i)->start(i + 1);
            (*projectListElementDP + i)->setParent(this);
            static_cast<QFrame*>(*projectListElementDP + i)->setParent(this);
            (*projectListElementDP + i)->installEventFilter(catchEvents);
            (*projectListElementDP + i)->setStyleSheet("ProjectListElementDP{background-color: rgba(255,255, 255, 0.6); border: 2px ridge rgba(240, 240, 240, 1); border-radius: 6px;}");
            (*projectListElementDP + i)->getRemoveButton()->installEventFilter(catchEvents);
            (*projectListElementDP + i)->getDownInfoLabel()->installEventFilter(catchEvents);
            (*projectListElementDP + i)->show();
          }
        break;
    }
}

void ProjectList::createLayout(){
   if(projectListElementOP != nullptr)
    for(uint i = 0; i < *numbOfElements; i++){
        (*projectListElementOP + i)->setGeometry(0, (i+1)*PROJECT_LIST_VERTICAL_OFFSET + i * PROJECT_LIST_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENTS_WIDTH, PROJECT_LIST_ELEMENTS_HEIGHT);
}
    if(projectListElementDP != nullptr)
        for(uint i = 0; i < *numbOfElements; i++){
         (*projectListElementDP + i)->setGeometry(0, (i+1)*PROJECT_LIST_VERTICAL_OFFSET + i * PROJECT_LIST_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENTS_WIDTH, PROJECT_LIST_ELEMENTS_HEIGHT);
        }

}

void ProjectList::deleteWidgets(){
   switch((uint)*listType){
    case (uint)LIST_OPEN_PROJECT:
        delete [](*projectListElementOP);
        *projectListElementOP = nullptr;
        delete projectListElementOP;
        projectListElementOP = nullptr;
        break;
    case (uint)LIST_DELETE_PROJECT:
        delete [](*projectListElementDP);
        *projectListElementDP = nullptr;
        delete projectListElementDP;
        projectListElementDP = nullptr;
        break;
    }
}

void ProjectList::connectAll(){
    // Brak
}

void ProjectList::disconnectAll(){
    // Bral
}

void ProjectList::reload(){
    disconnectAll();
    deleteWidgets();
    createWidgets();
    createLayout();
    connectAll();

}

ListType* ProjectList::getListType(){
    return listType;
}

InnerWindowCP* ProjectList::getParent(){
    return parent;
}

uint* ProjectList::getNumbOfElements(){
    return numbOfElements;
}

ProjectListElementOP** ProjectList::getProjectListElementOP(){
    return projectListElementOP;
}

ProjectListElementDP** ProjectList::getProjectListElementDP(){
    return projectListElementDP;
}

uint* ProjectList::getSelectedProjectNumb(){
    return selectedProjectNumb;
}

void ProjectList::setSelectedProjectNumb(uint* set){
    if(selectedProjectNumb != nullptr)
        delete selectedProjectNumb;
    selectedProjectNumb = set;
}
