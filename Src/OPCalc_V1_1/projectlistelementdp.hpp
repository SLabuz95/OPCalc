#ifndef PROJECTLISTELEMENTDP_HPP
#define PROJECTLISTELEMENTDP_HPP

#include<QFrame>
class QLabel;
class QCheckBox;
class ProjectList;
class ProjectListElementDP : public QFrame{
  Q_OBJECT

public:
  // Konstruktor
    ProjectListElementDP();
    ProjectListElementDP(ProjectList*, uint);
  // Destruktor
    ~ProjectListElementDP();
private:
  // Parent
  ProjectList* parent = nullptr;

  // Liczba porządkowa
  uint *number = nullptr;

  // Elementy okna
  QLabel *numberLabel = nullptr;
  QLabel *upInfoLabel = nullptr;
  QLabel *removeButton = nullptr;
  QCheckBox *selectBox = nullptr;
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
  QCheckBox* getSelectBox();
  void setParent(ProjectList*);

};

#endif // PROJECTLISTELEMENTDP_HPP
