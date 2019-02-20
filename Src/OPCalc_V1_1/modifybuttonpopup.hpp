#ifndef MODIFYBUTTONPOPUP_HPP
#define MODIFYBUTTONPOPUP_HPP
#include<QFrame>

#include"materialschangeswidgetstatsenum.hpp"
#include"materialschangeswidgettypeenum.hpp"
#include"materialschangeswidget.hpp"
class QLabel;
class InnerWindowCEP;
class ModifyButtonPopUp : public QFrame{
public:
    // Konstruktor
    ModifyButtonPopUp(InnerWindowCEP*, MaterialsChangesWidgetType);

    // Destruktor
    ~ModifyButtonPopUp();

    // Parent
    InnerWindowCEP *parent = nullptr;

    // Type
    MaterialsChangesWidgetType type;

    // Elementy okna
    QLabel *addButton = nullptr;
    QLabel *removeButton = nullptr;
    QLabel *editButton = nullptr;

private:
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();

public:
    QLabel* getAddButton();
    QLabel* getRemoveButton();
    QLabel* getEditButton();

    // Funkcje realizujące zdarzenia
    void addButtonReleased();
    void removeButtonReleased();
    void editButtonReleased();
};
#endif // MODIFYBUTTONPOPUP_HPP
