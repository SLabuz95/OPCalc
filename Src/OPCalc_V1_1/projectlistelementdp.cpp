#include"projectlistelementdp.hpp"
#include"windowmacros.hpp"
#include<QLabel>
#include<QCheckBox>
#include"projectlist.hpp"
#include"innerwindowcp.hpp"
#include"appwindow.hpp"
#include"appclass.hpp"
#include"projectinfo.hpp"
#include"config.hpp"
#include"tabbar.hpp"

ProjectListElementDP::ProjectListElementDP(){

}

ProjectListElementDP::ProjectListElementDP(ProjectList *setParent, uint numberFunc){
  number = new uint;
  *number = numberFunc;
  parent = setParent;
  init();
  setWindow();
  createWidgets();
  createLayout();
  connectAll();
}

void ProjectListElementDP::init(){
  // Brak
}

void ProjectListElementDP::setWindow(){
  this->setBaseSize(PROJECT_LIST_ELEMENT_OP_WIDTH, PROJECT_LIST_ELEMENT_OP_HEIGHT);

}

void ProjectListElementDP::createWidgets(){
    numberLabel = new QLabel(this);
    numberLabel->show();
    upInfoLabel = new QLabel(this);
    upInfoLabel->show();
    removeButton = new QLabel(this);
    removeButton->show();
    selectBox = new QCheckBox(this);
    selectBox->show();
    downInfoLabel = new QLabel(this);
    downInfoLabel->show();
}

void ProjectListElementDP::createLayout(){
    //numberLabel->setIcon(PROJECT_LIST_ELEMENT_DP_NUMBER_ICON);
    numberLabel->setText(PROJECT_LIST_ELEMENT_DP_NUMBER_ICON(*number));
    numberLabel->setGeometry(PROJECT_LIST_ELEMENT_DP_HORIZONTAL_OFFSET, PROJECT_LIST_ELEMENT_DP_VERTICAL_OFFSET, PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT);
    upInfoLabel->setGeometry(2 * PROJECT_LIST_ELEMENT_DP_HORIZONTAL_OFFSET + PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENT_DP_VERTICAL_OFFSET, PROJECT_LIST_ELEMENT_DP_UP_INFO_WIDTH, PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT);
    removeButton->setPixmap(QPixmap(CLOSE_BUTTON_ICON));
    removeButton->setGeometry(3 * PROJECT_LIST_ELEMENT_DP_HORIZONTAL_OFFSET + PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT + PROJECT_LIST_ELEMENT_DP_UP_INFO_WIDTH, PROJECT_LIST_ELEMENT_DP_VERTICAL_OFFSET, PROJECT_LIST_ELEMENT_DP_INFO_REMOVE_BUTTONS_WIDTH, PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT);
    selectBox->setChecked(false);
    selectBox->setGeometry(PROJECT_LIST_ELEMENT_DP_HORIZONTAL_OFFSET, 2 * PROJECT_LIST_ELEMENT_DP_VERTICAL_OFFSET + PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT);
    downInfoLabel->setGeometry(2 * PROJECT_LIST_ELEMENT_DP_HORIZONTAL_OFFSET + PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT, 2 * PROJECT_LIST_ELEMENT_DP_VERTICAL_OFFSET + PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT, PROJECT_LIST_ELEMENT_DP_DOWN_INFO_WIDTH, PROJECT_LIST_ELEMENT_DP_ELEMENTS_HEIGHT);

}

void ProjectListElementDP::deleteWidgets(){
    delete numberLabel;
    numberLabel = nullptr;
    delete upInfoLabel;
    upInfoLabel = nullptr;
    delete removeButton;
    removeButton = nullptr;
    delete selectBox;
    selectBox = nullptr;
    delete downInfoLabel;
    downInfoLabel = nullptr;
}

void ProjectListElementDP::connectAll(){

}

void ProjectListElementDP::disconnectAll(){

}

void ProjectListElementDP::removeButtonReleased(){
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

void ProjectListElementDP::start(uint setNumb){
  number = new uint;
  *number = setNumb;
  init();
  setWindow();
  createWidgets();
  createLayout();
  connectAll();
}

void ProjectListElementDP::setNumber(uint setNumber){
  if(number == nullptr)
    {
      number = new uint;
      *number = setNumber;
    }else
      *number = setNumber;
}

ProjectListElementDP::~ProjectListElementDP(){
    disconnectAll();
   deleteWidgets();
}

QLabel* ProjectListElementDP::getNumberLabel(){
  return numberLabel;
}

QLabel* ProjectListElementDP::getUpInfoLabel(){
  return upInfoLabel;
}

QLabel* ProjectListElementDP::getDownInfoLabel(){
  return downInfoLabel;
}

QLabel* ProjectListElementDP::getRemoveButton(){
    return removeButton;
}

QCheckBox* ProjectListElementDP::getSelectBox(){
    return selectBox;
}

void ProjectListElementDP::setParent(ProjectList *set){
    parent = set;
}

