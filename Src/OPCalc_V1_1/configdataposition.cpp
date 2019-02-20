#include"configdataposition.hpp"
#include"config.hpp"
#include<QLabel>

ConfigDataPosition::ConfigDataPosition(Config *setParent){
    parent = setParent;
    packNamePosition = new unsigned long long[3];
}

ConfigDataPosition::~ConfigDataPosition(){
    delete []packNamePosition;
    packNamePosition = nullptr;
    delete []projectsPosition;
    projectsPosition = nullptr;
    delete []recentProjectsPosition;
    recentProjectsPosition = nullptr;
}

void ConfigDataPosition::createProjectsPosition(uint size){
    if(projectsPosition == nullptr && size != 0)
        projectsPosition = new unsigned long long[size];
    for(uint i = 0; i < size; i++)
      *(projectsPosition + i) = 0;
}

void ConfigDataPosition::createRecentProjectsPosition(ushort size){
    if(recentProjectsPosition == nullptr && size != 0)
        recentProjectsPosition = new unsigned long long[size];
        for(uint i = 0; i < size; i++)
            *(recentProjectsPosition + i) = 0;

}

void ConfigDataPosition::reallocProjectsPosition(unsigned long long *projectPosToAdd, uint oldNumbOfProjectsPos){
  uint numbOfProjectsPos = parent->getNumbOfProjects() * 0.05 + 1;
  if(projectPosToAdd != nullptr)
    {
     unsigned long long *temp = new unsigned long long[numbOfProjectsPos];
     uint i;
     for(i = 0; i < oldNumbOfProjectsPos; i++)
       *(temp + i) = *(projectsPosition + i);
     for( ; i < numbOfProjectsPos ; i++)
       *(temp + i) = *(projectPosToAdd + i - oldNumbOfProjectsPos);
     delete []projectsPosition;
     delete []projectPosToAdd;
     projectsPosition = temp;
    }else{
    uint i;
    for(i = numbOfProjectsPos - 1; i != 0 ; i--)
        if(*(projectsPosition + i) != 0)
            break;
        if(i != parent->getNumbOfProjects() - 1){
            unsigned long long *temp = new unsigned long long[i + 1];
            for(uint j = 0; j < (i + 1); j++)
                *(temp + j) = *(projectsPosition + j);
            delete []projectsPosition;
            projectsPosition = temp;
            parent->setNumbOfProjects(i + 1);
        }
    }
}

void ConfigDataPosition::reallocRecentProjectsPosition(unsigned long long *recentsPosToAdd, uint oldNumbOfRecentsPos){
  uint numbOfRecentsPos = parent->getNumbOfRecentProjects();
  if(recentsPosToAdd != nullptr)
    {
     unsigned long long *temp = new unsigned long long[numbOfRecentsPos];
     uint i;
     for(i = 0; i < oldNumbOfRecentsPos; i++)
       *(temp + i) = *(recentProjectsPosition + i);
     for( ; i < numbOfRecentsPos ; i++)
       *(temp + i) = *(recentsPosToAdd + i - oldNumbOfRecentsPos);
     delete []recentProjectsPosition;
     delete []recentsPosToAdd;
     recentProjectsPosition = temp;
    }else{
  ushort i;
    for(i = parent->getNumbOfRecentProjects() - 1; i != 0; i--)
        if(recentProjectsPosition + i != nullptr)
            break;
    if(i != parent->getNumbOfRecentProjects() - 1){
        unsigned long long *temp = new unsigned long long[i + 1];
        for(ushort j = 0; j < (i + 1); j++)
            *(temp + j) = *(recentProjectsPosition + j);
        delete []recentProjectsPosition;
        recentProjectsPosition = temp;
        parent->setNumbOfRecentProjects(i + 1);
    }
    }
}

void ConfigDataPosition::addNewProjectPos(unsigned long long newPos){
  if((parent->getNumbOfProjects() + 1)% 20 == 0 || (parent->getNumbOfProjects() + 1) == 1){
    if(projectsPosition != nullptr){
      uint numbOfProjectsPos = (parent->getNumbOfProjects() + 1) * 0.05 + 1;
  unsigned long long *temp = new unsigned long long[numbOfProjectsPos];
  *temp = newPos;
  for(uint i = 1; i < numbOfProjectsPos ; i++)
    *(temp + i) = *(projectsPosition + i - 1);
  delete []projectsPosition;
  projectsPosition = temp;
  }else{
      projectsPosition = new unsigned long long[1];
      *projectsPosition = newPos;
  }
  }
}

void ConfigDataPosition::addNewRecentPos(unsigned long long newPos){
  if(parent->getNumbOfRecentProjects() != 20){
      if(recentProjectsPosition != nullptr){
    ushort numbOfRecentsPos = parent->getNumbOfRecentProjects() + 1;
    unsigned long long *temp = new unsigned long long[numbOfRecentsPos];
    *temp = newPos;
    for(uint i = 1; i < numbOfRecentsPos; i++)
      *(temp + i) = *(recentProjectsPosition + i - 1);
    delete []recentProjectsPosition;
    recentProjectsPosition = temp;
  }else{
      recentProjectsPosition = new unsigned long long[1];
      *recentProjectsPosition = newPos;
  }
}
}

void ConfigDataPosition::removeProjectPos(uint numbOfRemProj){
    if(parent->getNumbOfProjects() - numbOfRemProj == 0){
        delete []projectsPosition;
        projectsPosition = nullptr;
    }else{
    uint numbOfProjectsPos = (parent->getNumbOfProjects() - numbOfRemProj) * 0.05 + 1;
    if(numbOfProjectsPos != (parent->getNumbOfProjects() * 0.05 + 1)){
    unsigned long long *temp = new unsigned long long[numbOfProjectsPos];
  for(uint i = 0; i < numbOfProjectsPos ; i++)
    *(temp + i) = 0;
  delete []projectsPosition;
  projectsPosition = temp;
    }
 }
}

void ConfigDataPosition::removeRecentPos(ushort numbOfRemRec){
  if(parent->getNumbOfRecentProjects() == numbOfRemRec)
    {
      delete []recentProjectsPosition;
      recentProjectsPosition = nullptr;
  }else{
  ushort numbOfRecentsPos = parent->getNumbOfRecentProjects() - numbOfRemRec;
  if(numbOfRecentsPos != 0){
      unsigned long long *temp = new unsigned long long[numbOfRecentsPos];
        for(uint i = 0; i < numbOfRecentsPos; i++)
            *(temp + i) = 0;
  delete []recentProjectsPosition;
  recentProjectsPosition = temp;
  }else{
      delete []recentProjectsPosition;
      recentProjectsPosition = nullptr;
  }
  }
}

void ConfigDataPosition::removeForRecentList(ushort removeNumb){
    removeNumb--;
    if(parent->getNumbOfRecentProjects() == 1)
      {
        delete []recentProjectsPosition;
        recentProjectsPosition = nullptr;
    }else{
    ushort numbOfRecentsPos = parent->getNumbOfRecentProjects() - 1;
    unsigned long long *temp = new unsigned long long[numbOfRecentsPos];
       for(uint i = 0; i < parent->getNumbOfRecentProjects(); i++)
       {
           if(i > removeNumb)
               *(temp + i - 1) = *(recentProjectsPosition + i);
           else
               if(i != removeNumb)
               *(temp + i) = *(recentProjectsPosition + i);
       }
    delete []recentProjectsPosition;
    recentProjectsPosition = temp;
    }
}

unsigned long long* ConfigDataPosition::getPackNamePosition(){
    return packNamePosition;
}

unsigned long long* ConfigDataPosition::getProjectsPosition(){
    return projectsPosition;
}

unsigned long long* ConfigDataPosition::getRecentProjectsPosition(){
    return recentProjectsPosition;
}

Config* ConfigDataPosition::getParent(){
    return parent;
}

void ConfigDataPosition::setRecentProjectsPosition(unsigned long long *set){
    delete []recentProjectsPosition;
    recentProjectsPosition = set;
}

void ConfigDataPosition::setProjectsPosition(unsigned long long *set){
    delete []projectsPosition;
    projectsPosition = set;
}
void ConfigDataPosition::setPackNamePositionAt(unsigned long long set, uint position){
    *(packNamePosition + position) = set;
}

void ConfigDataPosition::setProjectsPositionAt(unsigned long long set, uint position){
    *(projectsPosition + position) = set;
}

void ConfigDataPosition::setRecentProjectsPositionAt(unsigned long long set, uint position){
    *(recentProjectsPosition + position) = set;
}

void ConfigDataPosition::setParent(Config *set){
    parent = set;
}
