#include"formatchangepopupcatchevents.hpp"
#include"projectmenagerparamenum.hpp"
#include"flangematerialsfileparams.hpp"
#include"pipematerialsfileparams.hpp"
#include"formatchangepopup.hpp"
#include"substancesparamsfileparamenum.hpp"
#include<QLabel>
#include<QComboBox>
#include<QAbstractItemView>
#include<QEvent>
#include<QFrame>
template <typename ParamName>
FormatChangePopUpCatchEvents<ParamName>::FormatChangePopUpCatchEvents(FormatChangePopUp<ParamName> *parent)
    : parent(parent){}

template <typename ParamName>
FormatChangePopUpCatchEvents<ParamName>::~FormatChangePopUpCatchEvents(){}

template <typename ParamName>
bool FormatChangePopUpCatchEvents<ParamName>::eventFilter(QObject *obj, QEvent *ev){
    switch(ev->type()){
    case QEvent::MouseButtonRelease:
        if(obj == parent->getCloseButton()){
            static_cast<QFrame*>(parent)->deleteLater();
            return true;
        }
        if(obj == parent->getDecrementButton()){
            parent->decrement();
            return true;
        }
        if(obj == parent->getIncrementButton()){
            parent->increment();
            return true;
        }
        break;
    case QEvent::Leave:
        if(obj == parent->getFormatList()->view()){
            parent->changeFormatType();
        }
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, ev);
}

template class FormatChangePopUpCatchEvents<ProjectMenagerParam>;
template class FormatChangePopUpCatchEvents<FlangeMaterialsFileParams>;
template class FormatChangePopUpCatchEvents<PipeMaterialsFileParams>;
template class FormatChangePopUpCatchEvents<SubstancesParamsFileParam>;
