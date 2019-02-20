#ifndef MYDIALOG_HPP
#define MYDIALOG_HPP

#define YES_NO_BUTTON_WIDTH (90)
#define BUTTONS_HEIGHT (25)
#define HORIZONTAL_OFFSET (10)
#define VERTICAL_BORDER_OFFSET (10)
#define VERTICAL_OFFSET (5)
#define MIN_REFERENCE_WIDTH (250)
#define MAX_TEST_RECT_HEIGHT (200)

#include<QDialog>
#include<QLabel>
#include<QLineEdit>
#include<QKeyEvent>
#include"dialogtypeenum.hpp"
#include"dialogresultenum.hpp"
#include"substancesparamsfileparamenum.hpp"

class SubstanceFileErrorsList;
class MyDialogCatchEvents;
class QScrollArea;
template <typename ParamName> class MaterialsChangesWidget;
class MyDialog : public QDialog{
    Q_OBJECT
public:
    // Konstruktor dla typu ADD_PRESSURE, EDIT_PRESSURE
    MyDialog(DialogType setType, QWidget* setParent);
    // Konstruktor dla typu QUESTION
    MyDialog(DialogType setType, QString title, QString content, bool yesNoQuestion,QString setFirstOption, QString setSecondOption, QString setThirdOption,QWidget* setParent);

    // Destruktor
    ~MyDialog();
private:
    // Catch Events
    MyDialogCatchEvents *catchEvent = nullptr;

    // Parent
    QWidget* parent = nullptr;

    // Type
    DialogType type;

    // Flaga dla QUESTION
    bool yesNoQuestion = false;

    // Elementy
    // Dla typu DEFAULT_PATH_DIALOG
    QLabel* infoLabel =  nullptr;
    QLabel *projectPathInfo = nullptr;
    QLabel *projectPathLabel = nullptr;
    QLabel *resetToDefaultPathButton = nullptr;
    QLabel *fileDialogButton = nullptr;
    QLabel* acceptButton = nullptr;
    QLabel* cancelButton = nullptr;

    // Dla typu QUESTION
    QLabel* titleLabel = nullptr;
    QLabel* questionLabel = nullptr;
    QLabel* yesButton = nullptr;
    QLabel* noButton = nullptr;
    QLabel* firstOptionLabel = nullptr;
    QLabel* secondOptionLabel = nullptr;
    QLabel* thirdOptionLabel = nullptr;

    // Dla typu SUBSTANCE_FILE_ERRORS_LIST
    SubstanceFileErrorsList *substanceFileErrorsList = nullptr;
    MaterialsChangesWidget<SubstancesParamsFileParam> *mcw = nullptr;
    QScrollArea *scrollArea = nullptr;
    // QLabel* titleLabel = nullptr;
    // QLabel* firstOptionLabel = nullptr;

    QTimer *waitingTimer = nullptr;
    DialogResult *exitCode = nullptr;

    // Funkcje sterujące
    void createDialog();
public:
    // Eventy
    void acceptButtonReleased();
    void cancelButtonReleased();
    void yesButtonReleased();
    void noButtonReleased();
    void firstOptionLabelReleased();
    void secondOptionLabelReleased();
    void thirdOptionLabelReleased();
    void fileDialogButtonReleased();
    void resetToDefaultPathButtonReleased();

public slots:
    void waitingTimerAction();

public:
    QLabel* getAcceptButton();
    QLabel* getCancelButton();
    QLabel* getYesButton();
    QLabel* getNoButton();
    QLabel* getFirstOptionLabel();
    QLabel* getSecondOptionLabel();
    QLabel* getThirdOptionLabel();
    bool getYesNoQuestion();
    QLabel* getResetToDefaultPathButton();
    QLabel* getFileDialogButton();
    QScrollArea* getScrollArea();

    DialogType getType();
    void setExitCode(DialogResult*);
    SubstanceFileErrorsList* getSubstanceFileErrorsList();
    bool checkFileSFEL(QFile*);
    MaterialsChangesWidget<SubstancesParamsFileParam>* getMCW();
};


#endif // MYDIALOG_HPP
