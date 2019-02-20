#ifndef OPTIONALPANEL_HPP
#define OPTIONALPANEL_HPP
#include<QFrame>
#include"projectmenagerparamenum.hpp"
class QLabel;
class QLineEdit;
class InnerWindowCP;
template <typename ParamName>
class FormatableDataField;
class OptionalProjectDataPanel : public QFrame{
    Q_OBJECT
public:
    // Konstruktor
    OptionalProjectDataPanel(InnerWindowCP*);

    // Destruktor
    ~OptionalProjectDataPanel();

private:
    //Parent
    InnerWindowCP *parent = nullptr;

    // Elementy okna
    QLabel *infoLabel = nullptr;
    QLabel *companyLabel = nullptr;
    QLabel *objectLabel = nullptr;
    QLabel *flangeLabel = nullptr;
    FormatableDataField<ProjectMenagerParam> *companyTextEdit = nullptr;
    FormatableDataField<ProjectMenagerParam> *objectTextEdit = nullptr;
    FormatableDataField<ProjectMenagerParam> *flangeTextEdit = nullptr;

    // A w przypadku edycji danych istniejącego projektu oraz bez włączonego trybu edycji
    QLabel *companyEditDataLabel = nullptr;
    QLabel *objectEditDataLabel = nullptr;
    QLabel *flangeEditDataLabel = nullptr;

    // Funkcje sterujące
    void init();
    void setWindow();
    void createWidgets();
    void createLayout();
    void deleteWidgets();
    void connectAll();
    void disconnectAll();

public:
    // Funkcje kontrolujące poprawoność wprowadzanych danych
    bool checkCompanyTextEdit(QKeyEvent*);
    bool checkObjectTextEdit(QKeyEvent*);
    bool checkFlangeTextEdit(QKeyEvent*);

    // Funkcje get
    QLabel* getCompanyLabel();
    QLabel* getObjectLabel();
    QLabel* getFlangeLabel();
    FormatableDataField<ProjectMenagerParam>* getCompanyTextEdit();
    FormatableDataField<ProjectMenagerParam>* getObjectTextEdit();
    FormatableDataField<ProjectMenagerParam>* getFlangeTextEdit();

    // Funkcje set
    void setCompanyEditDataLabel(QString);
    void setFlangeEditDataLabel(QString);
    void setObjectEditDataLabel(QString);
    void setCompanyTextEdit(QString);
    void setObjectTextEdit(QString);
    void setFlangeTextEdit(QString);
};


#endif // OPTIONALPANEL_HPP
