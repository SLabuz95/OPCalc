#ifndef APPCLASS_HPP
#define APPCLASS_HPP
#include<QThread>
#include"dyplomproject.hpp"
#include"innerwinstatuscpenum.hpp"
#include"substancesparamsfileparamenum.hpp"
class Config;
class QFile;
class QObject;
class ProjectInfo;
template <typename ParamName> class FormatableData;
template <typename ParamName> class FormatableDataField;
class AppWindow;

class ConfigThread : public QThread{
    Config* cnf = nullptr;
public:
    ConfigThread(Config*);
    void run() override;
};

class App : public QApplication{
    Q_OBJECT
public:
    // Konstruktor
    App(int, char**);
    // Destruktor
    ~App();
private:

    // Główne okno programu
    AppWindow *mainWin = nullptr;

    // Klasa pliku konfiguracyjnego
    Config *configuration = nullptr;

   // Klasa aktulanie otwartych projektów
    ProjectInfo** projectInfo = nullptr;
    ProjectInfo* projectToShow = nullptr;

    // Zapamiętanie stanów
    InnerWinStatusCP* statusToUse = nullptr;

   // Klasa ustawień

   // Flaga zamknięcia aplikacji
    bool closeApp = false;

   // AppSetting* appSetting = nullptr;

    // Funkcje Set
public:
    void setMainWin(AppWindow*);
    void setConfiguration(Config*);
    void setProjectInfo(ProjectInfo**);
    //void setAppSetting(AppSetting*);
    void setProjectToShow(ProjectInfo*);
    void setStatusToUse(InnerWinStatusCP);
    void removeStatusToUse();

    // Funkcje Get
public:
    AppWindow* getMainWin();
    Config* getConfiguration();
    ProjectInfo** getProjectInfo();
    ProjectInfo* getProjectToShow();
    InnerWinStatusCP* getStatusToUse();
    //AppSetting* getAppSetting();

    // Funkcje dodawania i usuwania otwartych projektów
    void addProjectInfo();
    void removeProjectInfo(uint);

    // Funkcje obsługi wątku
    void sendConfigToThread();

   static bool compareStr(QString, QString);
   static uint lengthForFileCounter(QString);
   static bool isReadyToCalculate(ProjectInfo*, uint);
   static bool calculate(ProjectInfo*, uint);
   static void quickSort(FormatableData<SubstancesParamsFileParam>***, int);
   static void quickSort(FormatableDataField<SubstancesParamsFileParam>**, int);
   void safeCloseApp();
   static float* getUpstreamDeviceDistanceFactor(ProjectInfo*);
   static QByteArray readCharUtf8(QFile*, char*);
   bool isOneInstanceOfProgram();
   bool isCloseApp();
};
#endif // APPCLASS_HPP
