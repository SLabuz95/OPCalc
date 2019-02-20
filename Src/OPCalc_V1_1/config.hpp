#ifndef CONFIG_HPP
#define CONFIG_HPP
#include"configpacknameenum.hpp"
#include"generalparamnames.hpp"
#include"projectsparamnames.hpp"
#include"sortingtypeenum.hpp"
#include"readingconfigfileenum.hpp"
#include"mainmacros.hpp"
#include <QString>
#include<QObject>
#include<QTextCodec>
#include<QFontMetrics>

class QFile;
class App;
class ConfigDataPosition;
class Config : public QObject{
public:
  // Konstruktor
  Config(App*);

  // Destruktor
  ~Config();

private:
  // Zmienne
  bool constructingFail = false;
  App *parent = nullptr;
  QString defaultPath = nullptr;
  QString autocadPath = nullptr;
  uint numbOfProjects = 0;
  ushort numbOfRecentProjects = 0;
  ConfigDataPosition *configDataPosition = nullptr;
  SortingType sortingType = THE_YOUNGEST;
  uint *progressBarValue = nullptr;
  uint *progressBarMax = nullptr;
  uint *progressBarMin = nullptr;
  // Metody

  QString readValueAtStart(QFile&, char&, unsigned long long*);
  bool resetAndRestoreConfig(QFile*);
  bool restoreConfig(QFile*);
  void checkProjectsAtStart(QFile*);
  void readAtStart(QFile*);
  bool createConfigFile();
  bool createConfigBackUpFile();
  bool checkConfigFile(QFile*);
  ConfigPackName checkConfigPackName(QString*);
  GeneralParamNames checkGeneralParamName(QString*);
  ProjectsParamNames checkProjectParamName(QString*);

  void checkDefaultPathStructure();
  void moveDefaultPath(QString);
public:
  bool rewriteGeneral();
  // Funkcje get
  bool isConstructingFail();
  QString getDefaultPath();
  QString getAutocadPath();
  App* getParent();
  uint getNumbOfProjects();
  ushort getNumbOfRecentProjects();
  uint getProgressBarValue();
  uint getProgressBarMax();
  uint getProgressBarMin();

  // Funkcje set
  void setConstructingFail(bool);
  void setDefaultPath(QString);
  void setAutocadPath(QString);
  void setNumbOfProjects(uint);
  void setNumbOfRecentProjects(ushort);

  //Metody ładujące dane dla elementów
public:
   void start();
   bool readForRecentList();
   bool readForProjectList(bool);
   bool readForProjectInfoPanel(uint, QString*, int&, QFontMetrics&);
   bool addNewProject();
   bool readRecentProject(ushort);
   bool readProject(uint, ushort**);
   bool rewriteRecents(uint, ushort*, short, bool, bool);
   bool rewriteProject(uint);
   int removeFromRecentList(ushort, bool);
   bool removeProjects(uint*, uint, ushort**, ushort*);
   int findPath(QString);
   // Funkcja odczytu danych
   static QString readValue(QFile*, char*);

   // Funkcja szukania Autocada
   static bool findAutoCadExe(QString*);
};

#endif // CONFIG_HPP
