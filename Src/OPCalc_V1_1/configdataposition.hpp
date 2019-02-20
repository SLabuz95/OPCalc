#ifndef CONFIGDATAPOSITION_HPP
#define CONFIGDATAPOSITION_HPP
#include<QObject>
class Config;
class ConfigDataPosition{
public:
    // Konstruktor
    ConfigDataPosition(Config*);

    // Destruktor
    ~ConfigDataPosition();
private:
    // Zmienne przechowujące pozycje:
    // - Paczek danych pliku
    unsigned long long *packNamePosition = nullptr;
    // - Projektów domyślnej ścieżki
    unsigned long long *projectsPosition = nullptr;
    // - Projekty ostatnio otwierane
    unsigned long long *recentProjectsPosition = nullptr;
    // Parent
    Config *parent = nullptr;

public:
    // Tworzenie wstępnego rozmiaru tablic
    void createProjectsPosition(uint);
    void createRecentProjectsPosition(ushort);

    // Realokacja po zapisaniu danych
    void reallocProjectsPosition(unsigned long long*, uint);
    void reallocRecentProjectsPosition(unsigned long long*, uint);

    // Dodawanie nowych pozycji
    void addNewProjectPos(unsigned long long);
    void addNewRecentPos(unsigned long long);

    // Usuwanie pozycji
    void removeProjectPos(uint);
    void removeRecentPos(ushort);
    void removeForRecentList(ushort);
    // Funkcje get
    unsigned long long* getPackNamePosition();
    unsigned long long* getProjectsPosition();
    unsigned long long* getRecentProjectsPosition();
    Config* getParent();

    // Funkcje set
    void setProjectsPosition(unsigned long long*);
    void setRecentProjectsPosition(unsigned long long*);
    void setPackNamePositionAt(unsigned long long, uint);
    void setProjectsPositionAt(unsigned long long, uint);
    void setRecentProjectsPositionAt(unsigned long long, uint);
    void setParent(Config*);
};

#endif // CONFIGDATAPOSITION_HPP
