#include"projectlistelementop.hpp"
#include<QPaintEvent>
#include"windowmacros.hpp"
#include<QLabel>
#include"projectlist.hpp"
#include"innerwindowcp.hpp"
#include"appwindow.hpp"
#include"appclass.hpp"
#include"projectinfo.hpp"
#include"tabbar.hpp"
#include"config.hpp"

ProjectListElementOP::ProjectListElementOP(){

}

ProjectListElementOP::ProjectListElementOP(ProjectList *setParent, uint numberFunc){
  number = new uint;
  *number = numberFunc;
  parent = setParent;
  init();
  setWindow();
  createWidgets();
  createLayout();
  connectAll();
}

ProjectListElementOP::~ProjectListElementOP(){
  disconnectAll();
  deleteWidgets();
  delete number;
  number = nullptr;
}

void ProjectListElementOP::init(){
  // Brak
}

void ProjectListElementOP::setWindow(){
  this->setBaseSize(PROJECT_LIST_ELEMENT_OP_WIDTH, PROJECT_LIST_ELEMENT_OP_HEIGHT);

}

void ProjectListElementOP::createWidgets(){
  numberLabel = new QLabel(this);
  numberLabel->show();
  upInfoLabel = new QLabel(this);
  upInfoLabel->show();
  removeButton = new QLabel(this);
  removeButton->show();
  downInfoLabel = new QLabel(this);
  downInfoLabel->show();
}

void ProjectListElementOP::createLayout(){
  numberLabel->setGeometry(PROJECT_LIST_ELEMENT_OP_HORIZONTAL_OFFSET, PROJECT_LIST_ELEMENT_OP_VERTICAL_OFFSET, PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT);
  numberLabel->setText(PROJECT_LIST_ELEMENT_OP_NUMBER_ICON(*number));
  upInfoLabel->setGeometry(2 * PROJECT_LIST_ELEMENT_OP_HORIZONTAL_OFFSET + PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENT_OP_VERTICAL_OFFSET, PROJECT_LIST_ELEMENT_OP_UP_INFO_WIDTH, PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT);
  removeButton->setGeometry(3 * PROJECT_LIST_ELEMENT_OP_HORIZONTAL_OFFSET + PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT + PROJECT_LIST_ELEMENT_OP_UP_INFO_WIDTH, PROJECT_LIST_ELEMENT_OP_VERTICAL_OFFSET, PROJECT_LIST_ELEMENT_OP_INFO_REMOVE_BUTTONS_WIDTH, PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT);
  removeButton->setPixmap(QPixmap(CLOSE_BUTTON_ICON));
  downInfoLabel->setGeometry(2 * PROJECT_LIST_ELEMENT_OP_HORIZONTAL_OFFSET + PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT, 2 * PROJECT_LIST_ELEMENT_OP_VERTICAL_OFFSET + PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENT_OP_DOWN_INFO_WIDTH, PROJECT_LIST_ELEMENT_OP_ELEMENTS_HEIGHT);
 }

void ProjectListElementOP::deleteWidgets(){
  delete numberLabel;
  numberLabel = nullptr;
  delete upInfoLabel;
  upInfoLabel = nullptr;
  delete removeButton;
  removeButton = nullptr;
  delete downInfoLabel;
  downInfoLabel = nullptr;
}

void ProjectListElementOP::connectAll(){

}

void ProjectListElementOP::disconnectAll(){

}

void ProjectListElementOP::removeButtonReleased(){
    uint *projects = nullptr;
    projects = new uint;
    *projects = *number;
      ushort *recents = nullptr;
      ushort numberOfRecents = 0;
      uint *positionToDelete = nullptr;
      if(parent->getParent()->getParent()->getApplication()->getProjectInfo() != nullptr)
      for(uint j = 0; parent->getParent()->getParent()->getApplication()->getProjectInfo() != nullptr && j < (*parent->getParent()->getParent()->getApplication()->getProjectInfo())->getPosition(); j++){
          if(*projects < *(*(parent->getParent()->getParent()->getApplication()->getProjectInfo() + j))->getProjectNumb())
              (*(parent->getParent()->getParent()->getApplication()->getProjectInfo() + j))->setProjectNumb(*(*(parent->getParent()->getParent()->getApplication()->getProjectInfo() + j))->getProjectNumb() - 1);
          else
              if(*projects  == *(*(parent->getParent()->getParent()->getApplication()->getProjectInfo() + j))->getProjectNumb())
              {
              positionToDelete = new uint;
              *positionToDelete = (*(parent->getParent()->getParent()->getApplication()->getProjectInfo() + j))->getPosition();
              }
      }
      if(positionToDelete != nullptr){
          parent->getParent()->getParent()->getApplication()->removeProjectInfo(*positionToDelete);
          delete positionToDelete;
      }
      parent->getParent()->getParent()->getApplication()->getMainWin()->getTabBar()->reload();
      parent->getParent()->getParent()->getApplication()->getConfiguration()->removeProjects(projects, 1, &recents, &numberOfRecents);
      parent->getParent()->getParent()->getApplication()->getConfiguration()->rewriteRecents(0, recents, numberOfRecents, true, false);
      delete []recents;
      recents = nullptr;
      delete projects;
      projects = nullptr;
      parent->getParent()->setInnerChange(true);
      parent->getParent()->reload();
}

void ProjectListElementOP::start(uint setNumb){
  number = new uint;
  *number = setNumb;
  init();
  setWindow();
  createWidgets();
  createLayout();
  connectAll();
}

void ProjectListElementOP::setNumber(uint setNumber){
  if(number == nullptr)
    {
      number = new uint;
      *number = setNumber;
    }else
      *number = setNumber;
}

QLabel* ProjectListElementOP::getNumberLabel(){
  return numberLabel;
}

QLabel* ProjectListElementOP::getUpInfoLabel(){
  return upInfoLabel;
}

QLabel* ProjectListElementOP::getDownInfoLabel(){
  return downInfoLabel;
}

QLabel* ProjectListElementOP::getRemoveButton(){
    return removeButton;
}

void ProjectListElementOP::setParent(ProjectList *set){
    parent = set;
}

