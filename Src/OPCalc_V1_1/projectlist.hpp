#ifndef PROJECTLIST_HPP
#define PROJECTLIST_HPP
#include<QPainter>
#include<QFrame>
enum ListType{
    LIST_OPEN_PROJECT,
    LIST_DELETE_PROJECT
};
class InnerWindowCP;
class ProjectListElementOP;
class ProjectListElementDP;
class ProjectListCatchEvents;
class QScrollArea;

class ProjectList : public QFrame{
    Q_OBJECT
public:
    // Konstruktor
    ProjectList(ListType, InnerWindowCP*);

    // Destruktor
    ~ProjectList();

private:
    // Parent
    InnerWindowCP *parent = nullptr;

    // Typ listy
    ListType *listType = nullptr;

    // Ilość elementów listy
    uint *numbOfElements = nullptr;

    // Numer wybrangeo projektu
    uint *selectedProjectNumb = nullptr;

    // Elementy okna
    ProjectListElementOP **projectListElementOP = nullptr;
    ProjectListElementDP **projectListElementDP = nullptr;

    //Catch Events
    ProjectListCatchEvents *catchEvents = nullptr;

    // Funkcje sterujące
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();
    void reload();

public:
    // Funkcje get
    ListType* getListType();
    InnerWindowCP* getParent();
    uint* getNumbOfElements();
    QScrollArea* getScrollArea();
    ProjectListElementOP** getProjectListElementOP();
    ProjectListElementDP** getProjectListElementDP();
    uint* getSelectedProjectNumb();

    // Funkcje set
    void setSelectedProjectNumb(uint*);
};

#endif // PROJECTLIST_HPP
