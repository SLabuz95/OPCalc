#include"appclass.hpp"
#include"appwindow.hpp"
#include"config.hpp"
#include"projectinfo.hpp"
#include<QTextStream>
#include"formatabledata.hpp"
#include"formatabledatafield.hpp"
#include"errormacroes.hpp"
#include"materialsinfofiles.hpp"
#include"projectmenager.hpp"
#include<QLineEdit>
#include<QFile>
#include"mydialog.hpp"
#include<QTextCodec>
#include<math.h>

// Konstruktor
ConfigThread::ConfigThread(Config* setCnf){
    cnf = setCnf;
}

void ConfigThread::run(){
    cnf->start();
    deleteLater();
}

App::App(int argc, char** argv)
    : QApplication(argc, argv){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    if(isOneInstanceOfProgram()){
        mainWin = new AppWindow(this);
        mainWin->show();
        configuration = new Config(this);
        ConfigThread *thread = new ConfigThread(configuration);
        configuration->moveToThread(thread);
        thread->start();
    }else{
        closeApp = true;
    }
}

// Destruktor
App::~App(){

}

void App::setMainWin(AppWindow *set){
    delete mainWin;
    mainWin = set;
}

void App::setConfiguration(Config *set){
    delete configuration;
    configuration = set;
}

void App::setProjectInfo(ProjectInfo **set){
    delete projectInfo;
    projectInfo = set;
}
/*
void App::setAppSetting(AppSetting *set){
    appSetting = set;
}
*/

void App::setProjectToShow(ProjectInfo* set){
    projectToShow = set;
}

AppWindow* App::getMainWin(){
    return mainWin;
}

Config* App::getConfiguration(){
    return configuration;
}

ProjectInfo** App::getProjectInfo(){
    return projectInfo;
}

ProjectInfo* App::getProjectToShow(){
    return projectToShow;
}
/*
AppSetting* App::getAppSetting(){
    return appSetting;
}
*/

InnerWinStatusCP* App::getStatusToUse(){
    return statusToUse;
}

void App::setStatusToUse(InnerWinStatusCP set){
    if(statusToUse == nullptr)
        statusToUse = new InnerWinStatusCP;
    *statusToUse = set;
}

void App::removeStatusToUse(){
    delete statusToUse;
    statusToUse = nullptr;
}

void App::addProjectInfo(){
    if(projectInfo != nullptr){
        uint size = (*projectInfo)->getPosition() + 1;
        ProjectInfo **temp = new ProjectInfo*[size];
        for(uint i = 1; i < size ; i++){
            (*(temp + i)) = *(projectInfo + i - 1);
          }
        (*temp) = new ProjectInfo(this, size);
        delete []projectInfo;
        projectInfo = temp;
        }else{
        projectInfo = new ProjectInfo*[1];
        *projectInfo = new ProjectInfo(this, 1);
    }
}

void App::removeProjectInfo(uint removeProject){
    if(projectInfo != nullptr){
        uint size = (*projectInfo)->getPosition();
        removeProject = size - removeProject;
            if(size > 1){
            ProjectInfo **temp = new ProjectInfo*[size - 1];
            for(uint i = 0; i < size; i++){
                if(i < removeProject){
                    (*(temp + i)) = *(projectInfo + i);
                    (*(temp + i))->setPosition((*(temp + i))->getPosition() - 1);
                }else{
                    if(i != removeProject )
                        *(temp + i - 1) = *(projectInfo + i);
                    else{
                        delete (*(projectInfo + i));
                        (*(projectInfo + i)) = nullptr;
                    }
                }
            }
            delete []projectInfo;
            projectInfo = temp;
        }else{
                delete *projectInfo;
                *projectInfo = nullptr;
                delete []projectInfo;
                projectInfo = nullptr;
              }
    }
}

bool App::compareStr(QString fromInnerWindowCP, QString fromProjectInfo){
    if(fromInnerWindowCP.length() != fromProjectInfo.length())
        return false;
    else{
        for(uint i = 0; i < (uint)fromInnerWindowCP.length(); i++)
            if(fromInnerWindowCP.at(i) != fromProjectInfo.at(i))
                return false;
    }
    return true;
}

uint App::lengthForFileCounter(QString str){
    uint counter = 0;
    QByteArray data = str.toUtf8();
    ushort i = 1;
    char tempChar = '\0';
    QTextStream textStr(data);
    while(!textStr.atEnd())
    {
        textStr.device()->getChar(&tempChar);
        if(tempChar > 0){
            counter++;
        }else{
        if(tempChar > -96){
            textStr.device()->getChar(&tempChar);
            i++;
        }else{
            if(tempChar > -112)
            {
                for( ; i < 4; i++)
                {
                    textStr.device()->getChar(&tempChar);

                }

            }else{
                if(tempChar > -120){
                    for( ; i < 5; i++){
                        textStr.device()->getChar(&tempChar);
                     }
                }else{
                    if(tempChar > -124){
                        for( ; i < 6; i++){
                            textStr.device()->getChar(&tempChar);

                        }
                    }else{
                        for( ; i < 7; i++){
                            textStr.device()->getChar(&tempChar);
                         }
                    }
                }

            }
        }
        counter += i;
        i = 1;
   }
 }
    return counter;
}

bool App::isReadyToCalculate(ProjectInfo *projectInfo, uint customDataPos){
    if(projectInfo->getPipeDiameter() != nullptr)
    {
        if(*projectInfo->getPipeDiameter()->getData() < 50 || *projectInfo->getPipeDiameter()->getData() > 1000)
            {
                MyDialog dlg(QUESTION_DIALOG, QString(ERROR_PIPE_DIAMETER_TITLE), QString(ERROR_PIPE_DIAMETER_DESCRIPTION), false, QString("Ok"), QString(), QString(), nullptr);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
                return false;
            }
    }else{
        MyDialog dlg(QUESTION_DIALOG, QString("Brak średnicy rurociągu"), QString("Podaj średnicę rurociągu"), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
    }
    if(projectInfo->getMaxDiffPressure() == nullptr){
        MyDialog dlg(QUESTION_DIALOG, QString("Brak ciśnienia różnicowego"), QString("Podaj ciśnienie różnicowe"), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
    }
    if(projectInfo->getMaxUpstreamPressure() == nullptr){
        MyDialog dlg(QUESTION_DIALOG, QString("Brak ciśnienia dopływowego"), QString("Podaj ciśnienie strony dopływowej"), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
    }
    double checkPressureQioutent = (MAX_PRES_CONV/(MAX_PRES_CONV - DIFF_PRES_CONV));
    if(checkPressureQioutent < 0.75){
        MyDialog dlg(QUESTION_DIALOG, QString(), QString("Róznica ciśnień musi być mniejsza niz ") + QString::number(1 - 1,333 - DIFF_PRES_CONV), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
    }
    if(*projectInfo->getFluidType() == customDataPos || *projectInfo->getFluidDensityCustom()){
        if(projectInfo->getFluidDensity() == nullptr){
            MyDialog dlg(QUESTION_DIALOG, QString("Brak gęstości płynu"), QString("Podaj gęstość płynu"), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }
    }else{
        if(projectInfo->getFluidDensityFromFile() == nullptr){
            MyDialog dlg(QUESTION_DIALOG, QString("Brak gęstości płynu"), QString("Podaj gęstość płynu"), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }
    }
    if(*projectInfo->getFluidType() == customDataPos || *projectInfo->getFluidDynamicViscosityCustom()){
        if(projectInfo->getFluidDynamicViscosity() == nullptr){
            MyDialog dlg(QUESTION_DIALOG, QString("Brak lepkości"), QString("Podaj lepkość dynamiczną płynu"), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }
    }else{
        if(projectInfo->getFluidDynamicViscosityFromFile() == nullptr){
            MyDialog dlg(QUESTION_DIALOG, QString("Brak lepkości"), QString("Podaj lepkość dynamiczną płynu"), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }
    }
    if(*projectInfo->getFluidType() != customDataPos){
        if(projectInfo->getSubstanceStat() != nullptr){
            if(App::compareStr(*projectInfo->getSubstanceStat(), "Gaz")){
                if(projectInfo->getFluidHeatCapacityRatio() == nullptr){
                    MyDialog dlg(QUESTION_DIALOG, QString("Brak wykładnika izentropy"), QString("Podaj wykładnik izentropy gazu"), false, QString("Ok"), QString(), QString(), nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    return false;
                }
                if(*projectInfo->getFluidType() == customDataPos && projectInfo->getCompresibilityFactor() == nullptr){
                    MyDialog dlg(QUESTION_DIALOG, QString("Brak współczynnika ściśliwości"), QString("Podaj współczynnik ściśliwości"), false, QString("Ok"), QString(), QString(), nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    return false;
                }
            }
        }else{
            MyDialog dlg(QUESTION_DIALOG, QString("Brak stanu substancji"), QString("Wybierz stan (fazę) substancji"), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }
    }else{
        if(projectInfo->getOtherSubstanceStat() != nullptr){
            if(App::compareStr(*projectInfo->getOtherSubstanceStat(), "Gaz")){
                if(projectInfo->getFluidHeatCapacityRatio() == nullptr){
                    MyDialog dlg(QUESTION_DIALOG, QString("Brak wykładnika izentropy"), QString("Podaj wykładnik izentropy gazu"), false, QString("Ok"), QString(), QString(), nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    return false;
                }
                if(*projectInfo->getFluidType() == customDataPos && projectInfo->getCompresibilityFactor() == nullptr){
                    MyDialog dlg(QUESTION_DIALOG, QString("Brak współczynnika ściśliwości"), QString("Podaj współczynnik ściśliwości"), false, QString("Ok"), QString(), QString(), nullptr);
                    dlg.setWindowModality(Qt::ApplicationModal);
                    dlg.show();
                    dlg.exec();
                    return false;
                }
            }
        }else{
            MyDialog dlg(QUESTION_DIALOG, QString("Brak stanu substancji"), QString("Wybierz stan (fazę) substancji"), false, QString("Ok"), QString(), QString(), nullptr);
            dlg.setWindowModality(Qt::ApplicationModal);
            dlg.show();
            dlg.exec();
            return false;
        }
    }
    if(projectInfo->getMaxFlowValue() == nullptr){
        MyDialog dlg(QUESTION_DIALOG, QString("Brak maksymalnej wartości przepływu"), QString("Podaj maksymalną wartość przepływu"), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        return false;
    }
    return true;
}

bool App::calculate(ProjectInfo *projectInfo, uint customDataPos){
        // Przeliczanie jednostek strumienia na strumien masy
        double qm = 0;
        double fluidDensity;
        if(*projectInfo->getFluidType() == customDataPos || *projectInfo->getFluidDensityCustom()){
            fluidDensity = *projectInfo->getFluidDensity()->getData();
        }else{
            fluidDensity = *projectInfo->getFluidDensityFromFile()->getData();
        }
        // Uwzględnianie rozszerzalności cieplnej dla rurociągu
        double pipeDiameter = *projectInfo->getPipeDiameter()->getData();
        double pipeLinearExpanstionFactor = 1;
        double *pipeLinearExpanstionValue = MaterialsInfoFiles::readAndInterpolateForMaterials(projectInfo, true);
        if(pipeLinearExpanstionValue != nullptr)
            pipeLinearExpanstionFactor = 1 + ((TEMP_CONV(projectInfo)) - ((*projectInfo->getPipeMaterial() <= 3)? 20 : ((*projectInfo->getPipeMaterial() < 6)? 0 : 0))) * *pipeLinearExpanstionValue ;
        delete pipeLinearExpanstionValue;
        pipeLinearExpanstionValue = nullptr;
        projectInfo->setPipeLinearExpanstionFactor([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_PIPE_LINEAR_EXPANSTION);}(pipeLinearExpanstionFactor));
        pipeDiameter *= pipeLinearExpanstionFactor;
        double maxP = MAX_PRES_CONV;
        // Przeliczanie jednostek przepływu
        switch(*projectInfo->getFlowUnits()){
        case 0:
            qm = *projectInfo->getMaxFlowValue()->getData() * fluidDensity * 0.000277;
            break;
        case 1:
            qm = *projectInfo->getMaxFlowValue()->getData();
            break;
        case 2:
            qm = *projectInfo->getMaxFlowValue()->getData() * 0.000277;
            break;
        case 3:
            // qm = ( Wzór korekcyjny: v[m^3] = ( vn * z * t[K] * 101.325) / (273.15 *  maxP * 0.001 ) ) * gestość / 3600
            qm =( *projectInfo->getMaxFlowValue()->getData() * *projectInfo->getCompresibilityFactor()->getData() * CELCIUS_TO_KELWIN_CONV(TEMP_CONV(projectInfo)) * 101.325/(273.15 * maxP * 0.001) ) * fluidDensity * 0.000277;
            break;
        }

        // Obliczanie liczby Reynoldsa
        projectInfo->setReynoldsNumber([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_REYNOLDS_NUMBER);}(REYNOLDS_NUMBER_EQ));

        // Obliczanie stałej iteracji
        bool isFluid;
        if(*projectInfo->getFluidType() != customDataPos)
            isFluid= !App::compareStr(*projectInfo->getSubstanceStat(), "Gaz");
        else
            isFluid= !App::compareStr(*projectInfo->getOtherSubstanceStat(), "Gaz");
        double dp = DIFF_PRES_CONV;
        double a = (((*projectInfo->getFluidType() == customDataPos || *projectInfo->getFluidDynamicViscosityCustom())? *projectInfo->getFluidDynamicViscosity()->getData() : *projectInfo->getFluidDynamicViscosityFromFile()->getData()) * *projectInfo->getReynoldsNumber()->getData()) / (pipeDiameter * 0.001 * sqrt(2 * dp * fluidDensity));

        // Alogrytm iteracji
        // Przyjmowanie parametrów
        double *beta = new double[3];
        *beta = 0.15;
        *(beta + 1) = 0.75;
        *(beta + 2) = 0;
        double *c = new double[3];
        double *eps = new double[3];
        double *x = new double[3];
        double *delta = new double[3];
        for(uint i = 0; i < 3; i++){
            *(x + i) = 0;
            *(delta + i) = 0;
            *(c + i) = 0;
            *(eps + i) = 0;
        }
        *x = (*beta * *beta) / sqrt(1 - pow(*beta, 4));
        if(isFluid)
            *eps = 1;
        else
            *eps = (1 - (0.351 + 0.256 * pow(*beta, 4) + 0.93 * pow(*beta, 8)) * (1 - pow((maxP - dp)/maxP, 1/(*projectInfo->getFluidHeatCapacityRatio()->getData()))));
        *c = 0.5961 + 0.0261 * pow(*beta, 2) - 0.216 * pow(*beta, 8);
        *c += 0.000521 * pow(1000000 * *beta / *projectInfo->getReynoldsNumber()->getData(), 0.7);
        *c += (0.0188 + 0.0063 * pow(19000 * *beta / *projectInfo->getReynoldsNumber()->getData(), 0.8)) * pow(*beta, 3.5) * pow(1000000 / *projectInfo->getReynoldsNumber()->getData(), 0.3);
        *c += (0.043 + 0.08 * pow(10, -10 * GET_L_ONE_FOR_C_CALC) - 0.123 * pow(10, -7 * GET_L_ONE_FOR_C_CALC)) * (1 - 0.11 * pow(19000 * *beta / *projectInfo->getReynoldsNumber()->getData(), 0.8)) * pow(*beta, 4) / (1 - pow(*beta, 4));
        *c -= 0.031 * ((2 * GET_L_TWO_PRIM_FOR_C_CALC / (1 - *beta)) - 0.8 * pow((2 * GET_L_TWO_PRIM_FOR_C_CALC / (1 - *beta)), 1.1)) * pow(*beta, 1.3);
        if(pipeDiameter < 71.12)
            *c += 0.011 * (0.75 - *beta) * (2.8 - pipeDiameter * 0.001/25.4);
        *delta =*x * *c * *eps - a;
        *(x + 1) = (*(beta + 1) * *(beta + 1)) / sqrt(1 - pow(*(beta + 1), 4));
        if(isFluid)
            *(eps + 1) = 1;
        else
            *(eps + 1) = (1 - (0.351 + 0.256 * pow(*(beta + 1), 4) + 0.93 * pow(*(beta + 1), 8)) * (1 - pow((maxP - dp)/maxP, 1/(*projectInfo->getFluidHeatCapacityRatio()->getData()))));
        *(c + 1) = 0.5961 + 0.0261 * pow(*(beta + 1), 2) - 0.216 * pow(*(beta + 1), 8);
        *(c + 1) += 0.000521 * pow(1000000 * *(beta + 1) / *projectInfo->getReynoldsNumber()->getData(), 0.7);
        *(c + 1) += (0.0188 + 0.0063 * pow(19000 * *(beta + 1) / *projectInfo->getReynoldsNumber()->getData(), 0.8)) * pow(*(beta + 1), 3.5) * pow(1000000 / *projectInfo->getReynoldsNumber()->getData(), 0.3);
        *(c + 1) += (0.043 + 0.08 * pow(10, -10 * GET_L_ONE_FOR_C_CALC) - 0.123 * pow(10, -7 * GET_L_ONE_FOR_C_CALC)) * (1 - 0.11 * pow(19000 * *(beta + 1) / *projectInfo->getReynoldsNumber()->getData(), 0.8)) * pow(*(beta +1 ), 4) / (1 - pow(*(beta + 1), 4));
        *(c + 1) -= 0.031 * ((2 * GET_L_TWO_PRIM_FOR_C_CALC / (1 - *(beta + 1))) - 0.8 * pow((2 * GET_L_TWO_PRIM_FOR_C_CALC / (1 - *(beta + 1))), 1.1)) * pow(*(beta + 1), 1.3);
        if(pipeDiameter < 71.12)
            *(c + 1) += 0.011 * (0.75 - *(beta + 1)) * (2.8 - pipeDiameter * 0.001/25.4);
        *(delta + 1) =*(x + 1) * *(c + 1) * *(eps + 1) - a;
        // Pętla iteracji
        do{
            *(x + 2) = (*(x + 1) - *(delta + 1) * (*(x + 1) - *x)/(*(delta + 1) - *delta));
            *(beta + 2) = pow((pow(*(x + 2), 2) / (1 + pow(*(x + 2), 2))), 0.25);
            *(c + 2) = 0.5961 + 0.0261 * pow(*(beta + 2), 2) - 0.216 * pow(*(beta + 2), 8);
            *(c + 2) += 0.000521 * pow(1000000 * *(beta + 2) / *projectInfo->getReynoldsNumber()->getData(), 0.7);
            *(c + 2) += (0.0188 + 0.0063 * pow(19000 * *(beta + 2) / *projectInfo->getReynoldsNumber()->getData(), 0.8)) * pow(*(beta + 2), 3.5) * pow(1000000 / *projectInfo->getReynoldsNumber()->getData(), 0.3);
            *(c + 2) += (0.043 + 0.08 * pow(10, -10 * GET_L_ONE_FOR_C_CALC) - 0.123 * pow(10, -7 * GET_L_ONE_FOR_C_CALC)) * (1 - 0.11 * pow(19000 * *(beta + 2) / *projectInfo->getReynoldsNumber()->getData(), 0.8)) * pow(*(beta + 2), 4) / (1 - pow(*(beta + 2), 4));
            *(c + 2) -= 0.031 * ((2 * GET_L_TWO_PRIM_FOR_C_CALC / (1 - *(beta + 2))) - 0.8 * pow((2 * GET_L_TWO_PRIM_FOR_C_CALC / (1 - *(beta + 2))), 1.1)) * pow(*(beta + 2), 1.3);
            if(pipeDiameter < 71.12)
                *(c + 2) += 0.011 * (0.75 - *(beta + 2)) * (2.8 - pipeDiameter * 0.001/25.4);
            if(isFluid)
                *(eps + 2) = 1;
            else
                *(eps + 2) = (1 - (0.351 + 0.256 * pow(*(beta + 2), 4) + 0.93 * pow(*(beta + 2), 8)) * (1 - pow((maxP - dp)/maxP, 1/(*projectInfo->getFluidHeatCapacityRatio()->getData()))));

            *(delta + 2) = *(x + 2) * *(c + 2) * *(eps + 2) - a;
            *delta = *(delta + 1);
            *(delta + 1) = *(delta + 2);
            *x = *(x + 1);
            *(x + 1) = *(x + 2);
            *c = *(c + 1);
            *(c + 1) = *(c + 2);
            *eps = *(eps + 1);
            *(eps + 1) = *(eps + 2);
            *beta = *(beta + 1);
            *(beta + 1) = *(beta + 2);
        }while(fabs((a - *(c + 2) * *(x + 2) * *(eps + 2))/a) > pow(10, -6));

        double *flangeLinearExpanstionValue = MaterialsInfoFiles::readAndInterpolateForMaterials(projectInfo, false);
        double flangeLinearExpanstionFactor = 1;
        if(flangeLinearExpanstionValue != nullptr)
            flangeLinearExpanstionFactor = 1 + ((TEMP_CONV(projectInfo)) - ((*projectInfo->getMeasurementDevice() <= 3)? 20 : ((*projectInfo->getMeasurementDevice() < 6)? 0 : 0))) * *flangeLinearExpanstionValue ;
        delete flangeLinearExpanstionValue;
        flangeLinearExpanstionValue = nullptr;
        projectInfo->setFlangeLinearExpanstionFactor([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_FLANGE_LINEAR_EXPANSTION);}(flangeLinearExpanstionFactor));

        double flangeDiameter = (pipeDiameter * pow((POW_SQR(*(x + 2)) / (1 + POW_SQR(*(x + 2)))), 0.25))/flangeLinearExpanstionFactor;
        // W przypadku wyników nie zgodnych z normą program wykonuje iteracje dla deltaP
        if(*(beta + 2) < 0.1 || *(beta + 2) > 0.75 || flangeDiameter < 12.5)
        {
            if(*(beta + 2) < 0.1){
                *(beta + 2) = 0.1;
                if(*(beta + 2) * pipeDiameter < 0.0125){
                    *(beta + 2) = 0.0125 / pipeDiameter;
                }
            }
            else if(*(beta + 2) > 0.75 ){
                *(beta + 2) = 0.75;
                if(*(beta + 2) * pipeDiameter < 0.0125)
                    *(beta + 2) = 0.0125 / pipeDiameter;
            }
            else{
                *(beta + 2) = 0.0125 / pipeDiameter;
                if(*(beta + 2) > 0.75){
                    *(beta + 2) = 0.75;
                }
            }
            flangeDiameter = *(beta + 2) * pipeDiameter;
            a = 8 * (1 - pow(*(beta + 2), 4))/fluidDensity * pow( qm/(*(c + 2) * 3.1415 * pow(flangeDiameter, 2)) , 2);
            *eps = 1;
            *x = pow(*eps, -2) * a;
            *delta = a - *x/ pow(*eps, -2);
            if(!isFluid)
                *(eps + 1) = (1 - (0.351 + 0.256 * pow(*(beta + 2), 4) + 0.93 * pow(*(beta + 2), 8)) * (1 - pow(0.75, 1/(*projectInfo->getFluidHeatCapacityRatio()->getData()))));
            else
                *(eps + 1) = 1;
            *(x + 1) = pow(*(eps + 1), -2) * a;
            *(delta + 1) = a - *(x + 1)/pow(*(eps + 1), -2);
            do{
                *(x + 2) = *(x + 1) - *(delta + 1)*(*(x + 1) - *(x + 2))/(*(delta + 1) - *(delta + 2));
                if(!isFluid)
                    *(eps + 2) = *(eps + 1) = (1 - (0.351 + 0.256 * pow(*(beta + 2), 4) + 0.93 * pow(*(beta + 2), 8)) * (1 - pow((maxP - *(x + 2))/maxP, 1/(*projectInfo->getFluidHeatCapacityRatio()->getData()))));
                else
                    *(eps + 2) = 1;
                *(delta + 2) = a - *(x + 2)/pow(*(eps + 2), -2);
                *x = *(x + 1);
                *delta = *(delta + 1);
                *eps = *(eps + 1);
                *(x + 1) = *(x + 2);
                *(delta + 1) = *(delta + 2);
                *(eps + 1) =*(eps + 2);
            }while(*(delta + 2)/a > pow(10, -12));
            if(*(beta + 2) == 0.75){
                MyDialog dlg(QUESTION_DIALOG, QString("Błąd ciśnienia"), QString("Wymagane większe niż ") + QString::number(*(x + 2) * 0.001, 'f', 2) + QString(" kPa."), false, QString("Ok"), QString(), QString(), nullptr);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
            }else{
                MyDialog dlg(QUESTION_DIALOG, QString("Błąd ciśnienia"), QString("Wymagane większe niż ") + QString::number(*(x + 2) * 0.001, 'f', 2) + QString(" kPa."), false, QString("Ok"), QString(), QString(), nullptr);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
            }
            delete []beta;
            delete []c;
            delete []eps;
            delete []x;
            delete []delta;
            return false;
        }
        // Zapisywanie wyników
        projectInfo->setFlangeDiameter([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_FLANGE_DIAMETER);}(flangeDiameter));
        projectInfo->setFlowFactor([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_FLOW_FACTOR);}(*(c + 2)));
        projectInfo->setChokePoint([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_CHOKE_POINT);}(*(beta + 2)));
        projectInfo->setExpanstionNumber([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_EXPANSTION_NUMBER);}(*(eps + 2)));
        delete []beta;
        delete []c;
        delete []eps;
        delete []x;
        delete []delta;
        double pressureLoss = (sqrt(1 - pow(*projectInfo->getChokePoint()->getData(), 4)*(1 - *projectInfo->getFlowFactor()->getData() * *projectInfo->getFlowFactor()->getData())) - *projectInfo->getFlowFactor()->getData() * POW_SQR(*projectInfo->getChokePoint()->getData()))/ (sqrt(1-pow(*projectInfo->getChokePoint()->getData(), 4) * (1 - POW_SQR(*projectInfo->getFlowFactor()->getData()))) + *projectInfo->getFlowFactor()->getData() * POW_SQR(*projectInfo->getChokePoint()->getData())) * dp;
        projectInfo->setPressureLoss([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_PRESSURE_LOSS);}(pressureLoss/1000));
        float *distanceFactor = App::getUpstreamDeviceDistanceFactor(projectInfo);
        projectInfo->setUpstreamDeviceDistance([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_UPSTREAM_DEVICE_DISTANCE);}(*distanceFactor * pipeDiameter * 0.001));
        projectInfo->setDownstreamDistance([](double set)->FormatableData<ProjectMenagerParam>*{return new FormatableData<ProjectMenagerParam>(set, PROJECT_MENAGER_DOWNSTREAM_DISTANCE);}(*(distanceFactor + 1) * pipeDiameter * 0.001));
        delete []distanceFactor;
        return true;
}

void App::quickSort(FormatableData<SubstancesParamsFileParam> ***data, int size){
    bool flag = true;
    while(flag){
        flag = false;
        for(int j = 0; j < size -1; j++){
            if(*((**(data + j))->getData()) > *((**(data + j + 1))->getData())){
                FormatableData<SubstancesParamsFileParam> *temp = **(data + j);
                **(data + j) = **(data + j + 1);
                **(data + j + 1) = temp;
                temp = *(*(data + j) + 1);
                (*(*(data + j) + 1)) = (*(*(data + j + 1) + 1));
                (*(*(data + j + 1) + 1)) = temp;
                flag = true;
            }
        }
    }
}

void App::quickSort(FormatableDataField<SubstancesParamsFileParam> **data, int size){
    bool flag = true;
    while(flag){
        flag = false;
        for(int j = 0; j < size -1; j++){
            if((*(data + j))->getDataUTextEdit()->text().toDouble() > (*(data + j + 1))->getDataUTextEdit()->text().toDouble()){
                FormatableDataField<SubstancesParamsFileParam> *temp = (*(data + j));
                (*(data + j)) = (*(data + j + 1));
                (*(data + j + 1)) = temp;
                flag = true;
            }
        }
    }
}

void App::safeCloseApp(){
    configuration->rewriteGeneral();
    mainWin->saveAllProjects();
    exit(0);
}

float* App::getUpstreamDeviceDistanceFactor(ProjectInfo* projectInfo){
    float *distance = new float[2];
    switch(*projectInfo->getUpstreamDevice()){
    case 0:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 6;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 16;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 22;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 42;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 44;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 44;
        break;
    case 1:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 10;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 10;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 18;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 18;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 18;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 18;
        break;
    case 2:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 10;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 10;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 22;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 42;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 44;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 44;
        break;
    case 3:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 19;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 44;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 44;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 44;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 44;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 44;
        break;
    case 4:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 34;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 50;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 75;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 65;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 60;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 75;
        break;
    case 5:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 3;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 9;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 19;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 29;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 36;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 44;
        break;
    case 6:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 7;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 30;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 30;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 30;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 44;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 44;
        break;
    case 7:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 5;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 5;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 8;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 9;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 12;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 13;
        break;
    case 8:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 6;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 12;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 20;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 26;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 28;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 36;
        break;
    case 9:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 12;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 12;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 12;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 14;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 18;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 24;
        break;
    case 10:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 30;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 30;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 30;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 30;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 30;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 30;
        break;
    case 11:
        if(*projectInfo->getChokePoint()->getData() <= 0.2) *distance = 3;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *distance = 3;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *distance = 3;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *distance = 3;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *distance = 3;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *distance = 3;
        break;
    default:
        *distance = 0;
        break;
    }
    if(*projectInfo->getChokePoint()->getData() <= 0.2) *(distance + 1) = 4;
        else if(*projectInfo->getChokePoint()->getData() <= 0.4) *(distance + 1) = 6;
        else if(*projectInfo->getChokePoint()->getData() <= 0.5) *(distance + 1) = 6;
        else if(*projectInfo->getChokePoint()->getData() <= 0.6) *(distance + 1) = 7;
        else if(*projectInfo->getChokePoint()->getData() <=0.67) *(distance + 1) = 7;
        else if(*projectInfo->getChokePoint()->getData() <= 0.75) *(distance + 1) = 8;

    return distance;
   }

QByteArray App::readCharUtf8(QFile* file, char *c){
    QByteArray byteArr = QByteArray();
    byteArr.append(*c);
    if(*c > -96)
    {
       file->getChar(c);
       byteArr.append(*c);
    }else{
         if(*c > -112){
             for(uint i = 0; i < 2; i++){
                file->getChar(c);
                byteArr.append(*c);
             }
         }else{
             if(*c > -120){
                 for(uint i = 0; i < 3; i++){
                     file->getChar(c);
                     byteArr.append(*c);
                  }
              }else{
                   if(*c > -124){
                     for(uint i = 0; i < 4; i++){
                         file->getChar(c);
                         byteArr.append(*c);
                      }
                    }else{
                          for(uint i = 0; i < 5; i++){
                               file->getChar(c);
                               byteArr.append(*c);
                           }
                    }
              }
         }
   }
  return byteArr;
}

bool App::isOneInstanceOfProgram(){
    QProcess process;
    process.setReadChannel(QProcess::StandardOutput);
    process.setReadChannelMode(QProcess::MergedChannels);
    process.start("wmic.exe process where name='OPCalc.exe' get processid");
    process.waitForStarted(1000);
    process.waitForFinished(-1);
    QString str = process.readAll();
    bool numb = false;
    uint len = (uint)str.length();
    short i = 0;
    for(uint j = 0; j < len; j++){
        if(str.at(j) >= 0x30 && str.at(j) <= 0x39){
            if(!numb){
                i++;
                if(i == 2){
                    exit(0);
                    return false;
                }
            }
            numb = true;
        }
        if(str.at(j) == '\n'){
            numb = false;
        }
    }
    return true;
}

bool App::isCloseApp(){
    return closeApp;
}
