#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "objFuncs.h"
#include "objFuncAvgT.h"
#include "selector.h"
#include <iostream>
#include <map>
#include <string>

int main()
{
    int nx = 3, ny = 3, xindex = 1, yindex = 2;
    std::string name = {"T"}, nameU = {"U"}, nameNu = {"nu"};
    std::string BCtype[4] = {"fixedValue","fixedValue","fixedValue","fixedValue"};
    double BCvalue[4] = {0.0,0.0,0.0,0.0};
    double Ini = 0.1, * deriv = nullptr;
    volScalarField T(nx, ny, name, BCtype, Ini, BCvalue);
    volVectorField U(nx, ny, nameU, BCtype, 0.0, 0.0, BCvalue, BCvalue);
    volScalarField nu(nx, ny, nameNu, BCtype, 0.1*Ini, BCvalue);
    volScalarField S(nx, ny, nameNu, BCtype, 0.1*Ini, BCvalue);
    volVectorField& U_ = U;
    volScalarField& nu_ = nu;
    volScalarField& T_ = T;
    volScalarField& S_ = S;
    mesh Mesh(nx, ny, 0.0, 1.0, 0.0, 1.0);
    mesh& Mesh_ = Mesh;


    map<string, objFuncs*> myMap;
    myMap["averageTempreture"] = selector<objFuncAvgT>(T_, S_, nu_, U_, Mesh_);

    return 0;
}