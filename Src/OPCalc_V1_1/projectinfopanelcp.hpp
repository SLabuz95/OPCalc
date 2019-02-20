#ifndef PROJECTINFOPANELCP_HPP
#define PROJECTINFOPANELCP_HPP
#include<QLabel>
#include<QScrollArea>

class InnerWindowCP;
class ProjectInfoPanelCP : public QLabel{
    Q_OBJECT
public:
    // Konstruktor
    ProjectInfoPanelCP(InnerWindowCP*);

    // Destruktor
    ~ProjectInfoPanelCP();
private:
    //Parent
    InnerWindowCP *parent = nullptr;

public:
    // Uzupełnianie
    void generateInformationAboutProject(uint);
};

#endif // PROJECTINFOPANELCP_HPP
