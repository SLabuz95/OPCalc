#ifndef MAINMACROS_HPP
#define MAINMACROS_HPP

// Makro nazwy programu
#define APP_NAME "OPCalc"
#define APP_VERSION "ver.1.1"

// Makra nazw plikow
#define CONFIG_FILE "config.cnf"
#define NEW_CONFIG_FILE_TEXT ("[General]\ndefaultPath=\"" + QDir::toNativeSeparators(QDir::homePath()) + "\\\"\nautocadPath=\"\"\nnumbOfProj=0\nnumbOfRec=0\nsort=" + QString::number((uint)sortingType) + "\n[Projects]\n")
#define FIND_AUTOCAD_EXE(path) (((path.isEmpty() || !QFile(path).exists()) && !Config::findAutoCadExe(&path))? QDir::toNativeSeparators(QFileDialog::getOpenFileName(nullptr, QString("Wybierz plik wykonywalny AutoCad"), QDir::homePath(), QString())) : path)
#define CONFIG_BACK_UP_FILE "backup.cnf"
#define SETTING_FILE "setting.txt"
#define TEMP_FILE "temp.tmp"
#define PROCESS_INSTANCE_FILE "p.tmp"
#define PROJECT_FILE_EXTENSTION ".opcp"

// Makra nazw plików materiałów
#define FLANGE_MATERIALS_FILE "flangeMaterials.ini"
#define PIPE_MATERIALS_FILE "pipeMaterials.ini"
#define SUBSTANCES_FILE "fluids.ini"
#define RESOURCES_FOLDER_PATH "Recources\\"
#define RESOURCES_FILE_EXTENSTION ".rsc"

// Makra nazw paczek danych
#define GENERAL_CONFIG_PACK_NAME tr("General")
#define PROJECTS_CONFIG_PACK_NAME tr("Projects")

// Marka  szacujące  rozmiar (ilość znaków) generowanych dla paczki danych [General]
#define CALCULATE_GENERAL_PACK_NAME_SIZE(addonProj, addonRec) (75 + App::lengthForFileCounter(defaultPath) + App::lengthForFileCounter(autocadPath) + QString::number(numbOfProjects + addonProj).length() + QString::number(numbOfRecentProjects + addonRec).length() + QString::number((uint)sortingType).length())
//#define CALCULATE_NEW_PROJECT_CONFIG_SIZE (68 + (parent->getProjectInfo()))->getObjectName() != nullptr)? ((*(parent->getProjectInfo()))->getObjectName()->length() + 23) : ((*(parent->getProjectInfo()))->getObjectName()->length()) + ((*parent->getProjectInfo())->getPath())->length() + (*(parent->getProjectInfo()))->getProjectName()->length() + QString::number(((*(parent->getProjectInfo()))->isDefaultProj())? 1 : 0))
#endif // MAINMACROS_HPP

// Makra tekstu informującego
#define NO_DATA_INFORMATION ("[Brak]")

// Makra ścieżek do ikon lub obrazow
#define CURSOR_DOUBLE_CLICK_PIXMAP "Icons\\doubleClickCursorIcon.png"
#define MODIFY_BUTTON_ICON "Icons\\leftExtendArrowIcon.png"
#define ADD_BUTTON_ICON "Icons\\addIcon.png"
#define EDIT_ALL_BUTTON_ICON "Icons\\editIcon.png"
#define EDIT_BUTTON_ICON "Icons\\editIcon.png"
#define REMOVE_BUTTON_ICON "Icons\\removeIcon.png"
#define PRESSURE_LIST_EXPAND_BUTTON_ICON "Icons\\close_button.png"
#define PRESSURE_LIST_EXPAND_BUTTON_REV_ICON "Icons\\close_button.png"
#define MOVE_TO_ERROR_ICON "Icons\\moveToErrorIcon.png"

// Makra równań
#define POW_SQR(x) ((x)*(x))
#define REYNOLDS_NUMBER_EQ ((4 * qm) / ( 3.1415 * ((*projectInfo->getFluidType() == customDataPos || *projectInfo->getFluidDynamicViscosityCustom())? *projectInfo->getFluidDynamicViscosity()->getData() : *projectInfo->getFluidDynamicViscosityFromFile()->getData()) * (pipeDiameter * 0.001)))
#define MAX_PRES_CONV [](ProjectInfo *projectInfo)->double{switch(*projectInfo->getMaxUpstreamPressureUnits()){case 0: /*MPa*/ return MPASCAL_TO_PASCAL_CONV(*projectInfo->getMaxUpstreamPressure()->getData()); case 1: /*kPa*/ return KPASCAL_TO_PASCAL_CONV(*projectInfo->getMaxUpstreamPressure()->getData()); case 2: /*BAR*/ return BAR_TO_PASCAL_CONV(*projectInfo->getMaxUpstreamPressure()->getData());}return 0;}(projectInfo)
#define DIFF_PRES_CONV [](ProjectInfo *projectInfo)->double{switch(*projectInfo->getMaxDiffPressureUnits()){case 0 : /*kPa*/ return KPASCAL_TO_PASCAL_CONV(*projectInfo->getMaxDiffPressure()->getData()); case 1: /*mm H2O*/ return MMH2O_TO_PASCAL_CONV(*projectInfo->getMaxDiffPressure()->getData());}return 0;}(projectInfo)
#define KELWIN_TO_CELCIUS_CONV(temp) ((temp) - 273.15)
#define CELCIUS_TO_KELWIN_CONV(temp) ((temp) + 273.15)

// Współczynniki korekcyjne zmiany jednostki ciśnienia
#define AT_TO_PASCAL_CONV(pres) ((pres) * 98066.5)
#define BILINEAR_INTERPOLATION_EQ(wspPsz, wspTsz) prevMinTempData * (1 - wspTsz) * (1 - wspPsz) + prevMaxTempData * wspTsz * (1 - wspPsz) + minTempData * (1 - wspTsz) * wspPsz + maxTempData * wspPsz * wspTsz
#define LINEAR_INTERPOLATION_EQ(wspTsz) (minTempData + wspTsz * (maxTempData - minTempData))
#define CP_TO_PAS_CONV(cp) ((cp) * 0.001)
#define BAR_TO_PASCAL_CONV(bar) ((bar) * 100000)
#define MPASCAL_TO_PASCAL_CONV(mpa) ((mpa) * 1000000)
#define KPASCAL_TO_PASCAL_CONV(kpa) ((kpa) * 1000)
#define PSI_TO_PASCAL_CONV(psi) ((psi) * 6894.75729)
#define ATM_TO_PASCAL_CONV(atm) ((atm) * 101325)
#define GET_L_ONE_FOR_C_CALC [pipeDiameter](ProjectInfo *projectInfo)->double{switch(*projectInfo->getMeasurementType()){case 0: return 0; case 1: return 1; case 2: return 25.4 * pipeDiameter * 0.001;}return 0;}(projectInfo)
#define GET_L_TWO_PRIM_FOR_C_CALC [pipeDiameter](ProjectInfo *projectInfo)->double{switch(*projectInfo->getMeasurementType()){case 0: return 0; case 1: return 0.47; case 2: return 25.4 / (pipeDiameter * 0.001);}return 0;}(projectInfo)
#define TEMP_CONV(projectInfo) ([projectInfo]()->double{switch(*projectInfo->getTempUnits()){case 0: return *projectInfo->getTemp()->getData(); case 1: return KELWIN_TO_CELCIUS_CONV(*projectInfo->getTemp()->getData());}return 0;}())
#define MMH2O_TO_PASCAL_CONV(mmhto) ((mmhto) * 9.80638)

// Współczynniki korekcyjne zmiany jednostki ciśnienia
#define AT_TO_ATM_CORRECTION_FACTOR (0.967841105354086)
#define AT_TO_BAR_CORRECTION_FACTOR (0.980665)
#define AT_TO_PSI_CORRECTION_FACTOR (14.2233433071)
#define AT_TO_KPAS_CORRECTION_FACTOR (98.0665)
#define ATM_TO_AT_CORRECTION_FACTOR (1.0332274528)
#define BAR_TO_AT_CORRECTION_FACTOR (1.019716212978)
#define PSI_TO_AT_CORRECTION_FACTOR (0.07030695796402)
#define KPAS_TO_AT_CORRECTION_FACTOR (0.01019716212978)

// Makra domyslnych formatów parametrów projektu
#define DEF_FORM_PRESSURE_TAP_DIAMETER ("f2")
#define DEF_FORM_MAX_DIFF_PRESSURE ("f2")
#define DEF_FORM_FLANGE_DIAMETER ("f3")
#define DEF_FORM_PIPE_DIAMETER ("f2")
#define DEF_FORM_MAX_UPSTREAM_PRESSURE ("f2")
#define DEF_FORM_MAX_FLOW_VALUE ("f2")
#define DEF_FORM_FLOW ("f2")
#define DEF_FORM_TEMP ("f2")
#define DEF_FORM_FLUID_DENSITY ("f4")
#define DEF_FORM_DYNAMIC_VISCOSITY ("e3")
#define DEF_FORM_HEAT_CAPACITY_RATIO ("e2")
#define DEF_FORM_COMPRESIBILITY_FACTOR ("f2")
#define DEF_FORM_FLUID_DENSITY_FROM_FILE ("f2")
#define DEF_FORM_DYNAMIC_VISCOSITY_FROM_FILE ("e2")
#define DEF_FORM_CHOKE_POINT ("f4")
#define DEF_FORM_FLOW_FACTOR ("f3")
#define DEF_FORM_EXPANSTION_NUMBER ("f3")
#define DEF_FORM_REYNOLDS_NUMBER ("e2")
#define DEF_FORM_PRESSURE_LOSE ("f2")
#define DEF_FORM_PIPE_LINEAR_EXPANSTION ("f2")
#define DEF_FORM_FLANGE_LINEAR_EXPANSTION ("f2")
#define DEF_FORM_UPSTREAM_DEVICE_DISTANCE ("f2")
#define DEF_FORM_DOWNSTREAM_DISTANCE ("f2")
#define DEF_FORM_NONE ("f2")
