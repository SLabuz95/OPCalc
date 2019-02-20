#ifndef PROJECTINFOPANELCEPCATCHEVENTS_HPP
#define PROJECTINFOPANELCEPCATCHEVENTS_HPP
#include<QObject>
class ProjectInfoPanelCEP;
class ProjectInfoPanelCEPCatchEvents : public QObject{
public:
    // Konstruktor
    ProjectInfoPanelCEPCatchEvents(ProjectInfoPanelCEP*);

    // Destruktor
    ~ProjectInfoPanelCEPCatchEvents();
private:
    // Parent
    ProjectInfoPanelCEP *parent = nullptr;

public:
     bool eventFilter(QObject*, QEvent*);
};


#endif // PROJECTINFOPANELCEPCATCHEVENTS_HPP
