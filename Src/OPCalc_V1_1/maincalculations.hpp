#ifndef MAINCALCULATIONS_HPP
#define MAINCALCULATIONS_HPP
#include"projectinfo.hpp"
class MainCalculations{
  // Obliczanie parametrów materiałów
    static void calculateSubstanceParams(ProjectInfo*);
    static void calculateFlangeParams(ProjectInfo*);
    static void calculatePipeParams(ProjectInfo*);

  // Obliczanie parametrów zwężki
    static void calculateOrificeResult(ProjectInfo*);
};

#endif // MAINCALCULATIONS_HPP
