#ifndef PROJECTLISTELEMENTOP_HPP
#define PROJECTLISTELEMENTOP_HPP
#include<QFrame>
class QLabel;
class ProjectList;
class ProjectListElementOP : public QFrame{
    Q_OBJECT
public:
    // Konstruktor
    ProjectListElementOP();
    ProjectListElementOP(ProjectList*, uint);

    // Destruktor
    ~ProjectListElementOP();
private:
    // Liczba porządkowa
    uint *number = nullptr;

    // Parent
    ProjectList *parent = nullptr;

    // Elementy okna
    QLabel *numberLabel = nullptr;
    QLabel *upInfoLabel = nullptr;
    QLabel *removeButton = nullptr;
    QLabel *downInfoLabel = nullptr;

    // Funkcje sterujące
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();

public:
    void removeButtonReleased();
    void setNumber(uint);
    void start(uint);
    QLabel* getNumberLabel();
    QLabel* getUpInfoLabel();
    QLabel* getDownInfoLabel();
    QLabel* getRemoveButton();
    void setParent(ProjectList*);

};


#endif // PROJECTLISTELEMENTOP_HPP
