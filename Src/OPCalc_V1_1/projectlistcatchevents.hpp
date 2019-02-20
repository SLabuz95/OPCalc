#ifndef PROJECTLISTCATCHEVENTS_HPP
#define PROJECTLISTCATCHEVENTS_HPP
#include<QObject>

class ProjectList;
class ProjectListCatchEvents : public QObject{
public:
    // Konstruktor
    ProjectListCatchEvents(ProjectList*);
private:
    // Parent
    ProjectList *parent = nullptr;
protected:
    // Reimplementacja EventFilter
    bool eventFilter(QObject*, QEvent*);
};

#endif // PROJECTLISTCATCHEVENTS_HPP
