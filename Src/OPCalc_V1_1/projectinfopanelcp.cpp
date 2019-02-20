#include"projectinfopanelcp.hpp"
#include"windowmacros.hpp"
#include"innerwindowcp.hpp"
#include"appclass.hpp"
#include"appwindow.hpp"
#include"config.hpp"
#include<QScrollBar>

ProjectInfoPanelCP::ProjectInfoPanelCP(InnerWindowCP *setParent){
    parent = setParent;    
    setTextFormat(Qt::RichText);
    setStyleSheet("font-family: \"Comic Sans MS\"; font-weight: 450; font-size: 14px;");
    setText(PROJECT_INFO_PANEL_START_TEXT);
    setAlignment(Qt::AlignCenter);    
}

ProjectInfoPanelCP::~ProjectInfoPanelCP(){

}

void ProjectInfoPanelCP::generateInformationAboutProject(uint numberOfProject){
    setAlignment(Qt::AlignVCenter);    
    setStyleSheet("QLabel{line-height: 3;}");
    QString text;
    QFont f("Arial",12, QFont::Normal, false);
    setFont(f);
    QFontMetrics fM = fontMetrics();
    int maxWidth = 0;
    if(!parent->getParent()->getApplication()->getConfiguration()->readForProjectInfoPanel(numberOfProject, &text, maxWidth, fM))
        return;
    setText(text);
    resize(maxWidth, height());
    show();
}
