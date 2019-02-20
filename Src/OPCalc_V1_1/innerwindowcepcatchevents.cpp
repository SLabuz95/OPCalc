#include"innerwindowcepcatchevents.hpp"
#include"innerwindowcep.hpp"
#include<QEvent>
#include<QKeyEvent>
#include"formatabledatafield.hpp"
#include"formatabledata.hpp"
#include<QLineEdit>
#include"formatchangepopup.hpp"
#include"mydialog.hpp"
#include"appwindow.hpp"
#include"windowmacros.hpp"
#include"modifybuttonpopup.hpp"
#include<QComboBox>
#include"resultstabbar.hpp"
#include<QAbstractItemView>
#include<QCheckBox>
#include"resultsgraphicpage.hpp"
#include"appclass.hpp"
#include"resultsbasicdatapage.hpp"
#include"resultsadditiondatapage.hpp"
#include"config.hpp"
#include"resultsraportpage.hpp"
InnerWindowCEPCatchEvents::InnerWindowCEPCatchEvents(InnerWindowCEP *setParent){
    parent = setParent;
}

InnerWindowCEPCatchEvents::~InnerWindowCEPCatchEvents(){

}

bool InnerWindowCEPCatchEvents::eventFilter(QObject *obj, QEvent *ev){
    switch((uint)*parent->getInnerWinStatusCEP()){
    case (uint)FLANGE:
    {
        switch(ev->type()){
        case QEvent::KeyPress:
        {
            QKeyEvent *evK = static_cast<QKeyEvent*>(ev);
            if(evK->matches(QKeySequence::Copy) || evK->matches(QKeySequence::Cut) || evK->matches(QKeySequence::Paste))
                return true;
            if(obj == parent->getPressuteTapDiameterFDF()->getDataUTextEdit())
            {
                if(parent->pressureTapDiameterFDFCheck(static_cast<QKeyEvent*>(ev)))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
            if(obj == parent->getMaxDiffPressureFDF()->getDataUTextEdit()){

                if(parent->FDFCheck(static_cast<QKeyEvent*>(ev), parent->getMaxDiffPressureFDF(), PROJECT_MENAGER_MAX_DIFF_PRESSURE))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
        }
            break;
        case QEvent::MouseButtonRelease:
            if(obj == ptrToFormatableDataField&& static_cast<QMouseEvent*>(ev)->button() == Qt::LeftButton ){
                if(FormatableData<ProjectMenagerParam>::checkStringFormat(ptrToFormatableDataField->getDataUTextEdit()->text(), false)){
                    // Obiekt jest tworzony, ale sam usuwa pamieć z wykorzystaniem deleteLater. Nie interesuje mnie jego adres, tylko istnienie
                    FormatChangePopUp<ProjectMenagerParam> *formatPopUp = new FormatChangePopUp<ProjectMenagerParam>(ptrToFormatableDataField);
                    QPoint pos = ptrToFormatableDataField->mapToGlobal(QPoint(0,ptrToFormatableDataField->height()));
                    formatPopUp->move(pos.x(), pos.y());
                    formatPopUp->show();
                }else{
                    MyDialog dlg(QUESTION_DIALOG,QString("Zmiana formatu"),QString("Nieprawidłowy format tekstu"), false, QString("Ok"), QString(), QString(), parent);
                    dlg.move(parent->getParent()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg.width()) * 0.5, (APP_WINDOW_HEIGHT - dlg.height()) * 0.5)));
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                }
                return true;
            }
            if(parent->getModifyButtonPopUp() != nullptr){
                if(parent->getModifyButtonPopUp()->getAddButton() == obj){
                    parent->getModifyButtonPopUp()->addButtonReleased();
                    return true;
                }
                if(parent->getModifyButtonPopUp()->getRemoveButton() == obj){
                    parent->getModifyButtonPopUp()->removeButtonReleased();
                    return true;
                }
                if(parent->getModifyButtonPopUp()->getEditButton() == obj){
                    parent->getModifyButtonPopUp()->editButtonReleased();
                    return true;
                }
            }
            if(obj == parent->getMeasurementDeviceMaterialModifyButton()){
                parent->measurementDeviceMaterialModifyButtonReleased();
                return true;
            }
            break;
        case QEvent::Enter:
            if(obj == parent->getPressuteTapDiameterFDF() || obj == parent->getMaxDiffPressureFDF()){
                ptrToFormatableDataField = static_cast<FormatableDataField<ProjectMenagerParam>*>(obj);
                if(FormatableData<ProjectMenagerParam>::checkStringFormat(ptrToFormatableDataField->getDataUTextEdit()->text(), false)){
                    FormatableData<ProjectMenagerParam> *tempFD = nullptr;
                    if(ptrToFormatableDataField->getFormatableData()){
                        ptrToFormatableDataField->getFormatableData()->setData(ptrToFormatableDataField->getDataUTextEdit()->text());
                    }else{
                        tempFD = new FormatableData<ProjectMenagerParam>(ptrToFormatableDataField->getDataUTextEdit()->text(), (obj == parent->getPressuteTapDiameterFDF())? PROJECT_MENAGER_PRESSURE_TAP_DIAMETER : PROJECT_MENAGER_MAX_DIFF_PRESSURE, false);
                        ptrToFormatableDataField->setFormatableData(tempFD);
                        delete tempFD;
                    }
                }
                ptrToFormatableDataField->createChangeFormatButton(FLANGE_PANEL_TEXTEDIT_LABEL_WIDTH);
                ptrToFormatableDataField->getChangeFormatButton()->installEventFilter(this);
                return true;
            }
            if(obj == parent->getMeasurementDeviceMaterialModifyButton())
                parent->getMeasurementDeviceMaterialModifyButton()->setStyleSheet("background-color: rgba(240,240,240, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
            break;
        case QEvent::Leave:
            if(obj == parent->getMeasurementDeviceMaterialList()->view()){
                return true;
            }
            if(obj == parent->getMeasurementDeviceMaterialModifyButton() && !parent->getModifyButtonPopUp())
            {
                parent->getMeasurementDeviceMaterialModifyButton()->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
                return true;
            }
            if(obj == parent->getModifyButtonPopUp()){
                parent->getModifyButtonPopUp()->deleteLater();
                parent->getMeasurementDeviceMaterialModifyButton()->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
                parent->setModifyButtonPopUpNullPtr();
                return true;
            }
            if(obj == parent->getPressuteTapDiameterFDF() || obj == parent->getMaxDiffPressureFDF()){
                static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->deleteChangeFormatButton();
                ptrToFormatableDataField = nullptr;
                return true;
            }
            break;
        default:
            break;
        }
    }
        break;
    case (uint)PIPE:
    {
        switch(ev->type()){
        case QEvent::KeyPress:
        {
            QKeyEvent *evK = static_cast<QKeyEvent*>(ev);
            if(evK->matches(QKeySequence::Copy) || evK->matches(QKeySequence::Cut) || evK->matches(QKeySequence::Paste))
                return true;
            if(obj == parent->getPipeDiameterFDF()->getDataUTextEdit()){
                if(parent->FDFCheck(static_cast<QKeyEvent*>(ev), parent->getPipeDiameterFDF(), PROJECT_MENAGER_PIPE_DIAMETER))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
                break;
            }
            if(obj == parent->getMaxUpstreamPressureFDF()->getDataUTextEdit())
            {
                if(parent->FDFCheck(static_cast<QKeyEvent*>(ev), parent->getMaxUpstreamPressureFDF(), PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
                break;
            }
        }
            break;
        case QEvent::MouseButtonRelease:
            if(obj == parent->getPipeMaterialModifyButton()){
                parent->pipeMaterialModifyButtonReleased();
                return true;
            }
            if(obj == ptrToFormatableDataField&& static_cast<QMouseEvent*>(ev)->button() == Qt::LeftButton ){
                if(FormatableData<ProjectMenagerParam>::checkStringFormat(ptrToFormatableDataField->getDataUTextEdit()->text(), false)){
                    // Obiekt jest tworzony, ale sam usuwa pamieć z wykorzystaniem deleteLater. Nie interesuje mnie jego adres, tylko istnienie
                    FormatChangePopUp<ProjectMenagerParam> *formatPopUp = new FormatChangePopUp<ProjectMenagerParam>(ptrToFormatableDataField);
                    QPoint pos = ptrToFormatableDataField->mapToGlobal(QPoint(0,ptrToFormatableDataField->height()));
                    formatPopUp->move(pos.x(), pos.y());
                    formatPopUp->show();
                }else{
                    MyDialog dlg(QUESTION_DIALOG,QString("Zmiana formatu"),QString("Nieprawidłowy format tekstu"), false, QString("Ok"), QString(), QString(), parent);
                    dlg.move(parent->getParent()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg.width()) * 0.5, (APP_WINDOW_HEIGHT - dlg.height()) * 0.5)));
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                }
                return true;
            }
            if(parent->getModifyButtonPopUp() != nullptr){
                if(parent->getModifyButtonPopUp()->getAddButton() == obj){
                    parent->getModifyButtonPopUp()->addButtonReleased();
                    return true;
                }
                if(parent->getModifyButtonPopUp()->getRemoveButton() == obj){
                    parent->getModifyButtonPopUp()->removeButtonReleased();
                    return true;
                }
                if(parent->getModifyButtonPopUp()->getEditButton() == obj){
                    parent->getModifyButtonPopUp()->editButtonReleased();
                    return true;
                }
            }
            break;
        case QEvent::Enter:
            if(obj == parent->getPipeDiameterFDF() || obj == parent->getMaxUpstreamPressureFDF()){
                ptrToFormatableDataField = static_cast<FormatableDataField<ProjectMenagerParam>*>(obj);
                if(FormatableData<ProjectMenagerParam>::checkStringFormat(ptrToFormatableDataField->getDataUTextEdit()->text(), false)){
                    FormatableData<ProjectMenagerParam> *tempFD = nullptr;
                    if(ptrToFormatableDataField->getFormatableData()){
                        ptrToFormatableDataField->getFormatableData()->setData(ptrToFormatableDataField->getDataUTextEdit()->text());
                    }else{
                        tempFD = new FormatableData<ProjectMenagerParam>(ptrToFormatableDataField->getDataUTextEdit()->text(), (obj == parent->getPipeDiameterFDF())? PROJECT_MENAGER_PIPE_DIAMETER : PROJECT_MENAGER_MAX_UPSTREAM_PRESSURE, false);
                        ptrToFormatableDataField->setFormatableData(tempFD);
                        delete tempFD;
                    }
                }
                ptrToFormatableDataField->createChangeFormatButton(PIPE_PANEL_TEXTEDIT_WIDTH);
                ptrToFormatableDataField->getChangeFormatButton()->installEventFilter(this);
                return true;
            }
            if(obj == parent->getPipeMaterialModifyButton())
                parent->getPipeMaterialModifyButton()->setStyleSheet("background-color: rgba(240,240,240, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
            ev->ignore();
            break;
        case QEvent::Leave:
            if(obj == parent->getPipeMaterialModifyButton()&&!parent->getModifyButtonPopUp()){
                parent->getPipeMaterialModifyButton()->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
            }
            if(obj == parent->getModifyButtonPopUp()){
                parent->getModifyButtonPopUp()->deleteLater();
                parent->getPipeMaterialModifyButton()->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
                parent->setModifyButtonPopUpNullPtr();
                return true;
            }
            if(obj == parent->getPipeDiameterFDF() || obj == parent->getMaxUpstreamPressureFDF()){
                static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->deleteChangeFormatButton();
                ptrToFormatableDataField = nullptr;
                return true;
            }
            break;
        default:
            break;
        }
    }
        break;
    case (uint)FLOW:
    {
        switch(ev->type()){
        case QEvent::KeyPress:
        {
            QKeyEvent *evK = static_cast<QKeyEvent*>(ev);
            if(evK->matches(QKeySequence::Copy) || evK->matches(QKeySequence::Cut) || evK->matches(QKeySequence::Paste))
                return true;
            if(obj == parent->getMaxFlowValueFDF()->getDataUTextEdit()){
                if(parent->FDFCheck(static_cast<QKeyEvent*>(ev), parent->getMaxFlowValueFDF(), PROJECT_MENAGER_MAX_FLOW_VALUE))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
                break;
            }
            if(obj == parent->getFlowFDF()->getDataUTextEdit()){
                if(parent->FDFCheck(static_cast<QKeyEvent*>(ev), parent->getFlowFDF(), PROJECT_MENAGER_FLOW))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
                break;
            }
            if(obj == parent->getTempFDF()->getDataUTextEdit()){
                if(parent->temperatureFDFCheck(static_cast<QKeyEvent*>(ev)))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
                break;
            }
        }
            break;
        case QEvent::MouseButtonRelease:
            if(obj == parent->getFluidTypeModifyButton()){
                parent->fluidTypeModifyButtonReleased();
                return true;
            }
            if(obj == ptrToFormatableDataField && static_cast<QMouseEvent*>(ev)->button() == Qt::LeftButton ){
                if(FormatableData<ProjectMenagerParam>::checkStringFormat(ptrToFormatableDataField->getDataUTextEdit()->text(), (obj == parent->getTempFDF())? true : false)){
                    // Obiekt jest tworzony, ale sam usuwa pamieć z wykorzystaniem deleteLater. Nie interesuje mnie jego adres, tylko istnienie
                    FormatChangePopUp<ProjectMenagerParam> *formatPopUp = new FormatChangePopUp<ProjectMenagerParam>(ptrToFormatableDataField);
                    QPoint pos = ptrToFormatableDataField->mapToGlobal(QPoint(0,ptrToFormatableDataField->height()));
                    formatPopUp->move(pos.x(), pos.y());
                    formatPopUp->show();
                }else{
                    MyDialog dlg(QUESTION_DIALOG,QString("Zmiana formatu"),QString("Nieprawidłowy format tekstu"), false, QString("Ok"), QString(), QString(), parent);
                    dlg.move(parent->getParent()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg.width()) * 0.5, (APP_WINDOW_HEIGHT - dlg.height()) * 0.5)));
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                }
                return true;
            }
            if(parent->getModifyButtonPopUp() != nullptr){
                if(parent->getModifyButtonPopUp()->getAddButton() == obj){
                    parent->getModifyButtonPopUp()->addButtonReleased();
                    return true;
                }
                if(parent->getModifyButtonPopUp()->getRemoveButton() == obj){
                    parent->getModifyButtonPopUp()->removeButtonReleased();
                    return true;
                }
                if(parent->getModifyButtonPopUp()->getEditButton() == obj){
                    parent->getModifyButtonPopUp()->editButtonReleased();
                    return true;
                }
            }
            break;
        case QEvent::Enter:
            if(obj == parent->getMaxFlowValueFDF() || obj == parent->getFlowFDF() || obj == parent->getTempFDF()){
                ptrToFormatableDataField = static_cast<FormatableDataField<ProjectMenagerParam>*>(obj);
                if(FormatableData<ProjectMenagerParam>::checkStringFormat(ptrToFormatableDataField->getDataUTextEdit()->text(), (parent->getTempFDF() == obj)? true : false)){
                    FormatableData<ProjectMenagerParam> *tempFD = nullptr;
                    ProjectMenagerParam tempFDParam = NONE_PARAM;
                    if(ptrToFormatableDataField->getFormatableData()){
                        ptrToFormatableDataField->getFormatableData()->setData(ptrToFormatableDataField->getDataUTextEdit()->text());
                    }else{
                        if(obj == parent->getMaxFlowValueFDF())
                            tempFDParam = PROJECT_MENAGER_MAX_FLOW_VALUE;
                        else
                            if(obj == parent->getFlowFDF())
                                tempFDParam = PROJECT_MENAGER_FLOW;
                            else
                                tempFDParam = PROJECT_MENAGER_TEMP;
                        tempFD = new FormatableData<ProjectMenagerParam>(ptrToFormatableDataField->getDataUTextEdit()->text(), tempFDParam, false);
                        ptrToFormatableDataField->setFormatableData(tempFD);
                        delete tempFD;
                    }
                }
                ptrToFormatableDataField->createChangeFormatButton(FLOW_PANEL_TEXTEDIT_WIDTH);
                ptrToFormatableDataField->getChangeFormatButton()->installEventFilter(this);
                return true;
            }
            if(obj == parent->getFluidTypeModifyButton())
                parent->getFluidTypeModifyButton()->setStyleSheet("background-color: rgba(240,240,240, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
                break;
        case QEvent::Leave:
            if(obj == parent->getFluidTypeModifyButton() && !parent->getModifyButtonPopUp()){
                parent->getFluidTypeModifyButton()->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
            }
            if(obj == parent->getModifyButtonPopUp()){
                parent->getModifyButtonPopUp()->deleteLater();
                parent->getFluidTypeModifyButton()->setStyleSheet("background-color: rgba(190,190,190, 0.6); border-radius: 4px 4px 0px 0px; border: 1px solid rgba(190,190,190,0.6);");
                parent->setModifyButtonPopUpNullPtr();
                return true;
            }
            if(obj == parent->getMaxFlowValueFDF() || obj == parent->getFlowFDF() || obj == parent->getTempFDF()){
                static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->deleteChangeFormatButton();
                ptrToFormatableDataField = nullptr;
                return true;
            }
            break;
        default:
            break;
        }
    }
        break;
    case (uint)FLUID:
        switch(ev->type()){
        case QEvent::MouseButtonRelease:
            if(obj == ptrToFormatableDataField && static_cast<QMouseEvent*>(ev)->button() == Qt::LeftButton ){
                if(FormatableData<ProjectMenagerParam>::checkStringFormat(ptrToFormatableDataField->getDataUTextEdit()->text(), false)){
                    // Obiekt jest tworzony, ale sam usuwa pamieć z wykorzystaniem deleteLater. Nie interesuje mnie jego adres, tylko istnienie
                    FormatChangePopUp<ProjectMenagerParam> *formatPopUp = new FormatChangePopUp<ProjectMenagerParam>(ptrToFormatableDataField);
                    QPoint pos = ptrToFormatableDataField->mapToGlobal(QPoint(0,ptrToFormatableDataField->height()));
                    formatPopUp->move(pos.x(), pos.y());
                    formatPopUp->show();
                }else{
                    MyDialog dlg(QUESTION_DIALOG,QString("Zmiana formatu"),QString("Nieprawidłowy format tekstu"), false, QString("Ok"), QString(), QString(), parent);
                    dlg.move(parent->getParent()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg.width()) * 0.5, (APP_WINDOW_HEIGHT - dlg.height()) * 0.5)));
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                }
                return true;
            }
            if(parent->getSubstanceTypeFluidCheckBox() == obj){
                parent->substanceTypeFluidCheckBoxReleased();
                return true;
            }
            if(parent->getSubstanceTypeGaseCheckBox() == obj){
                parent->substanceTypeGaseCheckBoxReleased();
                return true;
            }
            break;
        case QEvent::KeyPress:
        {
            QKeyEvent *evK = static_cast<QKeyEvent*>(ev);
            if(evK->matches(QKeySequence::Copy) || evK->matches(QKeySequence::Cut) || evK->matches(QKeySequence::Paste))
                return true;
            if(obj == parent->getSubstanceNameTextEdit()){
                if(parent->substanceNameTextEditCheck(static_cast<QKeyEvent*>(ev)))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
            if(obj == parent->getWorkDensityFDF()->getDataUTextEdit()){
                if(parent->workDensityFDFCheck(static_cast<QKeyEvent*>(ev)))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
            if(obj == parent->getDynamicViscosityFDF()->getDataUTextEdit()){
                if(parent->dynamicViscosityFDFCheck(static_cast<QKeyEvent*>(ev)))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
            if(obj == parent->getHeatCapacityRatioFDF()->getDataUTextEdit()){
                if(parent->heatCapacityRatioFDFCheck(static_cast<QKeyEvent*>(ev)))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
            if(obj == parent->getCompresibilityFactorFDF()->getDataUTextEdit()){
                if(parent->compresibilityFactorFDFCheck(static_cast<QKeyEvent*>(ev)))
                    return true;
                else
                    return QObject::eventFilter(obj, ev);
            }
        }
            break;
        case QEvent::Enter:
            if(obj == parent->getWorkDensityFDF() || obj == parent->getDynamicViscosityFDF() || obj == parent->getHeatCapacityRatioFDF() || obj == parent->getCompresibilityFactorFDF()){
                ptrToFormatableDataField = static_cast<FormatableDataField<ProjectMenagerParam>*>(obj);
                if(FormatableData<ProjectMenagerParam>::checkStringFormat(ptrToFormatableDataField->getDataUTextEdit()->text(), false)){
                    FormatableData<ProjectMenagerParam> *tempFD = nullptr;
                    ProjectMenagerParam tempFDParam = NONE_PARAM;
                    if(ptrToFormatableDataField->getFormatableData()){
                        ptrToFormatableDataField->getFormatableData()->setData(ptrToFormatableDataField->getDataUTextEdit()->text());
                    }else{
                        if(obj == parent->getWorkDensityFDF())
                            tempFDParam = PROJECT_MENAGER_WORK_DENSITY;
                        else
                            if(obj == parent->getDynamicViscosityFDF())
                                tempFDParam = PROJECT_MENAGER_WORK_DYNAMIC_VISCOSITY;
                            else
                                if(obj == parent->getHeatCapacityRatioFDF())
                                    tempFDParam = PROJECT_MENAGER_WORK_HEAT_CAPACITY_RATIO;
                                else
                                    tempFDParam = PROJECT_MENAGER_COMPRESIBILITY_FACTOR;
                        tempFD = new FormatableData<ProjectMenagerParam>(ptrToFormatableDataField->getDataUTextEdit()->text(), tempFDParam, false);
                        ptrToFormatableDataField->setFormatableData(tempFD);
                        delete tempFD;
                    }
                }
                ptrToFormatableDataField->createChangeFormatButton(FLUID_PANEL_TEXTEDIT_WIDTH);
                ptrToFormatableDataField->getChangeFormatButton()->installEventFilter(this);
                return true;
            }
            break;
        case QEvent::Leave:
            if(obj == parent->getWorkDensityFDF() || obj == parent->getDynamicViscosityFDF() || obj == parent->getHeatCapacityRatioFDF() || obj == parent->getCompresibilityFactorFDF()){
                static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->deleteChangeFormatButton();
                ptrToFormatableDataField = nullptr;
                return true;
            }
            break;
        default:
            break;
        }
        break;
    case (uint)RESULT:
    {
        switch(ev->type()){
        case QEvent::MouseButtonRelease:
            if(obj == parent->getResultsTabBar()->getGraphicTab())
            {
                parent->getResultsTabBar()->graphicTabReleased();
                return true;
            }
            if(obj == parent->getResultsTabBar()->getBasicTab()){
                parent->getResultsTabBar()->basicTabReleased();
                return true;
            }
            if(obj == parent->getResultsTabBar()->getAdditionalTab()){
                parent->getResultsTabBar()->additionalTabReleased();
                return true;
            }
            if(obj == parent->getResultsTabBar()->getRaportTab()){
                parent->getResultsTabBar()->raportTabReleased();
                return true;
            }
            switch(*parent->getResultsTabBar()->getActualTab()){
            case RESULTS_GRAPHIC_TAB:
                if(obj == parent->getResultsGraphicPage()->getGenerateGraphicFileButton()){
                    parent->getResultsGraphicPage()->generateGraphicFileButtonReleased();
                    return true;
                }
                if(obj == parent->getResultsGraphicPage()->getGenerateAutoCadFileButton()){
                    parent->getResultsGraphicPage()->generateAutoCadFileButtonReleased();
                    return true;
                }
                if(obj == parent->getResultsGraphicPage()->getGeneralTabButton()){
                    parent->getResultsGraphicPage()->generalTabButtonReleased();
                    return true;
                }
                if(obj == parent->getResultsGraphicPage()->getOrificePlateTabButton()){
                    parent->getResultsGraphicPage()->orificePlateTabButtonReleased();
                    return true;
                }
                break;
            case RESULTS_BASIC_TAB:
                if(obj == ptrToFormatableDataField){
                    if(!ptrToFormatableDataField->getDataULabel()->text().isEmpty()){
                        // Obiekt jest tworzony, ale sam usuwa pamieć z wykorzystaniem deleteLater. Nie interesuje mnie jego adres, tylko istnienie
                        FormatChangePopUp<ProjectMenagerParam> *formatPopUp = new FormatChangePopUp<ProjectMenagerParam>(ptrToFormatableDataField);
                        QPoint pos = ptrToFormatableDataField->mapToGlobal(QPoint(0,ptrToFormatableDataField->height()));
                        formatPopUp->move(pos.x(), pos.y());
                        formatPopUp->show();
                    }else{
                        MyDialog dlg(QUESTION_DIALOG,QString("Zmiana formatu"),QString("Nieprawidłowy format tekstu"), false, QString("Ok"), QString(), QString(), parent);
                        dlg.move(parent->getParent()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg.width()) * 0.5, (APP_WINDOW_HEIGHT - dlg.height()) * 0.5)));
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                    }
                    return true;
                }
                break;
            case RESULTS_ADDITIONAL_TAB:
                if(obj == ptrToFormatableDataField){
                    if(!ptrToFormatableDataField->getDataULabel()->text().isEmpty()){
                        // Obiekt jest tworzony, ale sam usuwa pamieć z wykorzystaniem deleteLater. Nie interesuje mnie jego adres, tylko istnienie
                        FormatChangePopUp<ProjectMenagerParam> *formatPopUp = new FormatChangePopUp<ProjectMenagerParam>(ptrToFormatableDataField);
                        QPoint pos = ptrToFormatableDataField->mapToGlobal(QPoint(0,ptrToFormatableDataField->height()));
                        formatPopUp->move(pos.x(), pos.y());
                        formatPopUp->show();
                    }else{
                        MyDialog dlg(QUESTION_DIALOG,QString("Zmiana formatu"),QString("Nieprawidłowy format tekstu"), false, QString("Ok"), QString(), QString(), parent);
                        dlg.move(parent->getParent()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg.width()) * 0.5, (APP_WINDOW_HEIGHT - dlg.height()) * 0.5)));
                        dlg.setWindowModality(Qt::ApplicationModal);
                        dlg.show();
                        dlg.exec();
                    }
                    return true;
                }
                break;
            case RESULTS_RAPORT_TAB:
                if(obj == parent->getResultsRaportPage()->getSaveButton()){
                    parent->getResultsRaportPage()->saveButtonReleased();
                    return true;
                }
                break;
            }
            break;
        case QEvent::Enter:
            if(obj == parent->getResultsTabBar()->getGraphicTab())
            {
                parent->getParent()->getStatusBar()->showMessage(QString("Przejdź do zakładki \"Grafika\""));
                return true;
            }
            if(obj == parent->getResultsTabBar()->getBasicTab()){
                parent->getParent()->getStatusBar()->showMessage(QString("Przejdź do zakładki \"Dane podstawowe\""));
                return true;
            }
            if(obj == parent->getResultsTabBar()->getAdditionalTab()){
                parent->getParent()->getStatusBar()->showMessage(QString("Przejdź do zakładki \"Dane dodatkowe\""));
                return true;
            }
            if(obj == parent->getResultsTabBar()->getRaportTab()){
                parent->getParent()->getStatusBar()->showMessage(QString("Przejdź do zakładki \"Raport\""));
                return true;
            }
            switch(*parent->getResultsTabBar()->getActualTab()){
            case RESULTS_GRAPHIC_TAB:
                if(obj == parent->getResultsGraphicPage()->getGenerateGraphicFileButton()){
                    parent->getParent()->getStatusBar()->showMessage(QString("Wygeneruj plik graficzny"));
                    return true;
                }
                if(obj == parent->getResultsGraphicPage()->getGenerateAutoCadFileButton()){
                    parent->getParent()->getStatusBar()->showMessage(QString("Wygeneruj projekt w AutoCad\t Ścieżka: ") + ((parent->getParent()->getApplication()->getConfiguration()->getAutocadPath().isEmpty())? "[Brak]" : parent->getParent()->getApplication()->getConfiguration()->getAutocadPath()));
                    return true;
                }
                if(obj == parent->getResultsGraphicPage()->getGeneralTabButton()){
                    parent->getParent()->getStatusBar()->showMessage(QString("Przejdź do zakładki \"Układ\""));
                    return true;
                }
                if(obj == parent->getResultsGraphicPage()->getOrificePlateTabButton()){
                     parent->getParent()->getStatusBar()->showMessage(QString("Przejdź do zakładki \"Zwężka\""));
                    return true;
                }
                break;
            case RESULTS_BASIC_TAB:
                if(obj == parent->getResultsBasicDataPage()->getChokePointDataLabel() || obj == parent->getResultsBasicDataPage()->getExpanstionNumberDataLabel() || obj == parent->getResultsBasicDataPage()->getFlangeDiameterDataLabel() || obj == parent->getResultsBasicDataPage()->getFlowFactorDataLabel() || obj == parent->getResultsBasicDataPage()->getPressureLossDataLabel() || obj == parent->getResultsBasicDataPage()->getReynoldsNumberDataLabel()){
                    static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->createChangeFormatButton(RESULT_PANEL_BASIC_RIGHT_ELEMENT_WIDTH);
                    static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->getChangeFormatButton()->installEventFilter(this);
                    ptrToFormatableDataField = static_cast<FormatableDataField<ProjectMenagerParam>*>(obj);
                    return true;
                }
                break;
            case RESULTS_ADDITIONAL_TAB:
                if(obj == parent->getResultsAdditionDataPage()->getDownstreamDistanceDataLabel() || obj == parent->getResultsAdditionDataPage()->getFlangeLinearExpanstionFactorDataLabel() || obj == parent->getResultsAdditionDataPage()->getPipeLinearExpanstionFactorDataLabel() || obj == parent->getResultsAdditionDataPage()->getUpstreamDeviceDistanceDataLabel()){
                    static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->createChangeFormatButton(RESULT_PANEL_ADDITIONAL_RIGHT_ELEMENT_WIDTH);
                    static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->getChangeFormatButton()->installEventFilter(this);
                    ptrToFormatableDataField = static_cast<FormatableDataField<ProjectMenagerParam>*>(obj);
                    return true;
                }
                break;
            default:
                return true;
            }
            break;
        case QEvent::Leave:
            parent->getParent()->getStatusBar()->showMessage("Gotowe");
            switch(*parent->getResultsTabBar()->getActualTab()){
            case RESULTS_BASIC_TAB:
                if(obj == parent->getResultsBasicDataPage()->getChokePointDataLabel() || obj == parent->getResultsBasicDataPage()->getExpanstionNumberDataLabel() || obj == parent->getResultsBasicDataPage()->getFlangeDiameterDataLabel() || obj == parent->getResultsBasicDataPage()->getFlowFactorDataLabel() || obj == parent->getResultsBasicDataPage()->getPressureLossDataLabel() || obj == parent->getResultsBasicDataPage()->getReynoldsNumberDataLabel()){
                    static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->deleteChangeFormatButton();
                    ptrToFormatableDataField = nullptr;
                    return true;
                }
                break;
            case RESULTS_ADDITIONAL_TAB:
                if(obj == parent->getResultsAdditionDataPage()->getDownstreamDistanceDataLabel() || obj == parent->getResultsAdditionDataPage()->getFlangeLinearExpanstionFactorDataLabel() || obj == parent->getResultsAdditionDataPage()->getPipeLinearExpanstionFactorDataLabel() || obj == parent->getResultsAdditionDataPage()->getUpstreamDeviceDistanceDataLabel()){
                    static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->deleteChangeFormatButton();
                    ptrToFormatableDataField = nullptr;
                    return true;
                }
                break;
            default:
                break;
            }
            return true;
        default:
            break;
        }
    }
        break;
    }
    if(ev->type() == QEvent::Destroy && obj == parent->getMaterialsChangesWidgetPtr())
    {
        parent->reloadList();
    }
    return QObject::eventFilter(obj, ev);
}

void InnerWindowCEPCatchEvents::mouseButtonReleasedSetFalse(){
    mouseButtonReleased = false;
}
