#include"materialschangeswidgetcatchevents.hpp"
#include<QEvent>
#include<QKeyEvent>
#include"materialschangeswidgetstatsenum.hpp"
#include"materialschangeswidgettypeenum.hpp"
#include"materialschangeswidget.hpp"
#include"mylist.hpp"
#include"flangematerialsfileparams.hpp"
#include"pipematerialsfileparams.hpp"
#include"substancesparamsfileparamenum.hpp"
#include"formatabledatafield.hpp"
#include<QRadioButton>
#include<QCheckBox>
#include"windowmacros.hpp"
#include"formatchangepopup.hpp"
#include"mydialog.hpp"
#include"config.hpp"
#include"appclass.hpp"
#include"appwindow.hpp"
#include<QComboBox>
#include<QAbstractItemView>
#include<QLabel>

template <typename ParamName>
MaterialsChangesWidgetCatchEvents<ParamName>::MaterialsChangesWidgetCatchEvents(MaterialsChangesWidget<ParamName> *setParent){
    parent = setParent;
}

template <typename ParamName>
MaterialsChangesWidgetCatchEvents<ParamName>::~MaterialsChangesWidgetCatchEvents(){
    // Brak
}

template <typename ParamName>
bool MaterialsChangesWidgetCatchEvents<ParamName>::eventFilter(QObject *obj, QEvent *ev){
    switch(ev->type()){
    case QEvent::Timer:
        if(obj == parent->getTimer()){
            parent->reloadTimerTask();
            return true;
        }
        break;
    case QEvent::MouseButtonRelease:
        if(obj == ptrToFormatableDataField&& static_cast<QMouseEvent*>(ev)->button() == Qt::LeftButton){
            if(!ptrToFormatableDataField->getDataUTextEdit()->text().isEmpty()){
                // Obiekt jest tworzony, ale sam usuwa pamieć z wykorzystaniem deleteLater. Nie interesuje mnie jego adres, tylko istnienie
                FormatChangePopUp<ParamName> *formatPopUp = new FormatChangePopUp<ParamName>(ptrToFormatableDataField);
                QPoint pos = ptrToFormatableDataField->mapToGlobal(QPoint(0,ptrToFormatableDataField->height()));
                formatPopUp->move(pos.x(), pos.y());
                formatPopUp->show();
            }else{
                MyDialog dlg(QUESTION_DIALOG,QString("Zmiana formatu"),QString("Nieprawidłowy format tekstu"), false, QString("Ok"), QString(), QString(), parent);
                dlg.move(parent->getConfig()->getParent()->getMainWin()->mapToGlobal(QPoint((APP_WINDOW_WIDTH - dlg.width()) * 0.5, (APP_WINDOW_HEIGHT - dlg.height()) * 0.5)));
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
            }
            return true;
        }

    if(obj == parent->getCloseButton()){
        parent->closeButtonReleased();
        return true;
    }
    if(obj == parent->getOkButton()){
        if(parent->getPressureList() && parent->getPressureList()->getEnteringData())
            return true;
        else
            parent->okButtonReleased();
        return true;
    }
    if(obj == parent->getCancelButton()){
        parent->cancelButtonReleased();
        return true;
    }
    switch(*parent->getMaterialsChangesWidgetStats()){
        case EDIT:
        if(obj == parent->getBackButton()){
            parent->backButtonReleased();
            return true;
        }
        break;
    default:
        break;
    }
    switch(parent->getType()){
    case SUBSTANCE_TYPE:
    if(obj == parent->getFluidTypeCheckBox()){
        parent->fluidCheckBoxReleased();
        return true;
    }
    if(obj == parent->getGaseTypeCheckBox())
    {
        parent->gaseCheckBoxReleased();
        return true;
    }
    if(obj == parent->getAddArrayRow()){
        parent->addArrayRowReleased();
        return true;
    }
    if(obj == parent->getTempDependencyButton()){
        if(parent->getPressureList() && parent->getPressureList()->getEnteringData())
            return true;
        if(parent->tempDependencyButtonReleased())
            break;
        else
            return true;
    }
    if(obj == parent->getTempPresDependencyButton()){
        if(parent->getPressureList() && parent->getPressureList()->getEnteringData())
            return true;
        if(parent->tempPresDependencyButtonReleased())
            break;
        else
            return true;
    }
    if(obj == parent->getDensityTabLabel()){
        if(parent->getPressureList() && parent->getPressureList()->getEnteringData())
            return true;
        else
            parent->densityTabLabelReleased();
        return true;
    }
    if(obj == parent->getDynamicViscosityTabLabel()){
        if(parent->getPressureList() && parent->getPressureList()->getEnteringData())
            return true;
        else
            parent->dynamicViscosityTabLabelReleased();
        return true;
    }
    if(obj == parent->getPressureListAddButton()){
        if(parent->getPressureList() && parent->getPressureList()->getEnteringData())
            return true;
        else
            parent->pressureListAddButtonReleased();
        return true;
    }
    if(obj == parent->getPressureListEditAllButton()){
        if(parent->getPressureList() && parent->getPressureList()->getEnteringData())
            return true;
        else
            parent->pressureListEditAllButtonReleased();
        return true;
    }
    if(obj == parent->getPressureListExpandButton()){
        parent->pressureListExpandButtonReleased();
        return true;
    }
    if(obj == parent->getPressureListEditConfirmedButton()){
        parent->pressureListEditConfirmedButtonReleased();
        return true;
    }
    if(obj == parent->getPressureListEditCancelledButton()){
        parent->pressureListEditCancelledButtonReleased();
        return true;
    }
        break;
    default:
        break;
    }
        break;
    case QEvent::KeyPress:
        switch(*parent->getMaterialsChangesWidgetStats()){
        case CHOOSING:
            break;
        case REMOVE:
            break;
        default:
            if(obj == parent->getNameTextEdit()->getDataUTextEdit()){
                if(parent->nameTextEditKeyPressed(static_cast<QKeyEvent*>(ev)))
                return true;
                else
                    break;
            }
            switch(parent->getType()){
            case FLANGE_MATERIAL_TYPE:
                if(obj == parent->getLinearExpanstionFDF()->getDataUTextEdit()){
                    if(parent->linearExpanstionFDFKeyPressed(static_cast<QKeyEvent*>(ev)))
                        return true;
                    else
                        break;
                }
                if(obj == parent->getFiducialTemperatureFDF()->getDataUTextEdit()){
                    if(parent->fiducialTemperatureFDFKeyPressed(static_cast<QKeyEvent*>(ev)))
                        return true;
                    else
                        break;
                }
                break;
            case PIPE_MATERIAL_TYPE:
                if(obj == parent->getLinearExpanstionFDF()->getDataUTextEdit()){
                    if(parent->linearExpanstionFDFKeyPressed(static_cast<QKeyEvent*>(ev)))
                        return true;
                    else
                        break;
                }
                if(obj == parent->getFiducialTemperatureFDF()->getDataUTextEdit()){
                    if(parent->fiducialTemperatureFDFKeyPressed(static_cast<QKeyEvent*>(ev)))
                        return true;
                    else
                        break;
                }
                break;
            case SUBSTANCE_TYPE:
                if(obj == parent->getKTextEdit()){
                    if(parent->kTextEditKeyPressed(static_cast<QKeyEvent*>(ev)))
                        return true;
                }
                break;
            }
            break;
        }
        break;
    case QEvent::Enter:
    if(parent->getType() != SUBSTANCE_TYPE){
        if(obj == parent->getLinearExpanstionFDF() || obj == parent->getFiducialTemperatureFDF())
        {
            static_cast<FormatableDataField<ParamName>*>(obj)->createChangeFormatButton(MATERIALS_CHANGES_RIGHT_ELEMENTS_WIDTH - 25);
            static_cast<FormatableDataField<ParamName>*>(obj)->getChangeFormatButton()->installEventFilter(this);
            ptrToFormatableDataField = static_cast<FormatableDataField<ParamName>*>(obj);
            return true;
        }
    }else{
        if(obj == parent->getPressureListEditConfirmedButton()){
            parent->pressureListEditConfirmedButtonHighlight();
            return true;
        }
        if(obj == parent->getPressureListEditCancelledButton()){
            parent->pressureListEditCancelledButtonHighlight();
            return true;
        }
        if(obj == parent->getPressureListAddButton()){
            parent->getPressureListAddButton()->setStyleSheet("background-color: rgba(225, 225, 225, 0.6);");
            return true;
        }
        if(obj == parent->getPressureListEditAllButton()){
            parent->getPressureListEditAllButton()->setStyleSheet("background-color: rgba(225, 225, 225, 0.6);");
            return true;
        }
    }
        break;
    case QEvent::Leave:
        if(parent->getType() != SUBSTANCE_TYPE){
            if(obj == parent->getLinearExpanstionFDF() || obj == parent->getFiducialTemperatureFDF()){
                static_cast<FormatableDataField<ProjectMenagerParam>*>(obj)->deleteChangeFormatButton();
                ptrToFormatableDataField = nullptr;
                return true;
            }
        }else{
            if(parent->getPressureUnitList() && obj == parent->getPressureUnitList()->view() && !parent->getPressureList()->getEnteringData()){
                parent->pressureUnitListLeft();
                return true;
            }
            if(obj == parent->getPressureListEditConfirmedButton()){
                parent->pressureListEditConfirmedButtonLowlight();
                return true;
            }
            if(obj == parent->getPressureListEditCancelledButton()){
                parent->pressureListEditCancelledButtonLowlight();
                return true;
            }
            if(obj == parent->getPressureListAddButton()){
                parent->getPressureListAddButton()->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                return true;
            }
            if(obj == parent->getPressureListEditAllButton()){
                parent->getPressureListEditAllButton()->setStyleSheet("background-color: rgba(245, 245, 245, 0.6);");
                return true;
            }
        }
        break;
    case QEvent::Destroy:
        if(obj == parent->getErrorDlg()){
            parent->setErrorDlg(nullptr);
            break;
        }
        if(obj == parent->getErrorDenDlg()){
            parent->setErrorDenDlg(nullptr);
            break;
        }
        if(obj == parent->getErrorViscoDlg()){
            parent->setErrorViscoDlg(nullptr);
            break;
        }
        break;
    default:
        break;
     }
    return QObject::eventFilter(obj, ev);
}

template class MaterialsChangesWidgetCatchEvents<FlangeMaterialsFileParams>;
template class MaterialsChangesWidgetCatchEvents<PipeMaterialsFileParams>;
template class MaterialsChangesWidgetCatchEvents<SubstancesParamsFileParam>;
