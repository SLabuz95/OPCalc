#ifndef MATERIALSINFOFILES_HPP
#define MATERIALSINFOFILES_HPP
#include<QObject>
#include"flangematerialsfileparams.hpp"
#include"pipematerialsfileparams.hpp"
#include"substancesparamsfileparamenum.hpp"
#include"substancesfileparams.hpp"
#include"config.hpp"

template <typename ParamName> class MaterialsChangesWidget;
class InnerWindowCEP;
class QFile;
class ProjectInfo;
class MaterialsInfoFiles{
public:
    // Operacje na materiałach kryz
    static bool readFlangeMaterials(InnerWindowCEP*);
    static bool readMaterials(MaterialsChangesWidget<FlangeMaterialsFileParams>*);
    static bool addMaterial(MaterialsChangesWidget<FlangeMaterialsFileParams>*);
    static bool editMaterial(MaterialsChangesWidget<FlangeMaterialsFileParams>*);
    static bool removeMaterial(MaterialsChangesWidget<FlangeMaterialsFileParams>*);
    static bool readMaterialParams(MaterialsChangesWidget<FlangeMaterialsFileParams>*);
    static int checkMaterialsNames(MaterialsChangesWidget<FlangeMaterialsFileParams>*);
    static bool mergeFlangeMaterialsFiles(QFile&, QFile&, Config&);

    // Operacje na matriałach rurociągów
    static bool readPipeMaterials(InnerWindowCEP*);
    static bool readMaterials(MaterialsChangesWidget<PipeMaterialsFileParams>*);
    static bool addMaterial(MaterialsChangesWidget<PipeMaterialsFileParams>*);
    static bool editMaterial(MaterialsChangesWidget<PipeMaterialsFileParams>*);
    static bool removeMaterial(MaterialsChangesWidget<PipeMaterialsFileParams>*);
    static bool readMaterialParams(MaterialsChangesWidget<PipeMaterialsFileParams>*);
    static int checkMaterialsNames(MaterialsChangesWidget<PipeMaterialsFileParams>*);
    static bool mergePipeMaterialsFiles(QFile&, QFile&, Config&);

    // Operacje na substancjach
    static bool readSubstances(InnerWindowCEP*);
    static bool readMaterials(MaterialsChangesWidget<SubstancesParamsFileParam>*);
    static bool readSubstanceForInterpolate(InnerWindowCEP*,QString*, QString*, QString*, QString*, uint);
    static bool addMaterial(MaterialsChangesWidget<SubstancesParamsFileParam>*);
    static bool editMaterial(MaterialsChangesWidget<SubstancesParamsFileParam>*);
    static bool removeMaterial(MaterialsChangesWidget<SubstancesParamsFileParam>*);
    static bool readSubstanceParams(QString, ProjectInfo*);
    static bool readMaterialParams(MaterialsChangesWidget<SubstancesParamsFileParam>*);
    static int checkMaterialsNames(MaterialsChangesWidget<SubstancesParamsFileParam>*);
    static bool rewriteSubstancesPaths(Config*, bool);

    // Operacje na plikach materiałów
    static FlangeMaterialsFileParams checkFlangeMaterialsFileParams(QString);
    static PipeMaterialsFileParams checkPipeMaterialsFileParams(QString);
    static SubstancesFileParams checkSubstancesFileParams(QString);

    static bool readSubstanceForFluidStat(InnerWindowCEP*);
    static int readAndInterpolate(QFile*, InnerWindowCEP*, bool);
    static int readAndInterpolateForWaterFluid(QFile*, InnerWindowCEP*, bool);
    static double* readAndInterpolateForMaterials(ProjectInfo*, bool);
    static SubstancesParamsFileParam checkFileParam(QString*);
    static QString readValue(QFile*, char*);
    static int getNumbOfMaterials(QFile*, Config&);

    static bool nameCheck(QString&);
};

#endif // MATERIALSINFOFILES_HPP
