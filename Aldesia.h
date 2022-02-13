#ifndef ALDESIA_H
#define ALDESIA_H

#include <iostream>
#include <string>
#include <map>
#include <codi.hpp>
#include "config/ConfigMap.h"
#include "field/volScalarField.h"
#include "field/volVectorField.h"
#include "field/mesh.h"
#include "Solver/CDSolver.h"
#include "writer/writePlt.h"
#include "writer/writeJac.h"
#include "Residual/CDSolverResidual.h"
#include "ObjFuncs/DerivObj.h"
#include "ObjFuncs/objFuncs.h"
#include "ObjFuncs/objFuncAvgT.h"
#include "ObjFuncs/selector.h"
#include "AdjointSolver/AdjointSolver.h"

using namespace std;

class Aldesia
{
    private:
    /// pointer to dependent scalar variable
    volScalarField* TPtr;

    /// pointer to the source field (design variable)
    volScalarField* SPtr;

    /// pointer to the diffusitivity field
    volScalarField* nuPtr;

    /// pointer to the velocity field
    volVectorField* UPtr;

    /// pointer to the mesh
    mesh* MeshPtr;

    /// the string-obj map
    map<string, objFuncs*> objMap;

    /// the gradient of the objective function
    double* grad = nullptr;

    /// linear equation solver's paramters
    double omega, tol, maxIter;

    /// decide if we want to write the Jacobian and the gradient
    bool isWriteJac = false, isWriteGrad = false;
    

    public:
    /// constructor
    Aldesia(string inputFile);

    /// destructor
    ~Aldesia();

    /// set the string-obj map
    void setObjMap();

    /// solve the PDE
    void solvePrimal();

    /// solve adjoint
    void solveDA(string objName);

    /// write the .plt file of the primal solution
    void writePrimal();

};


#endif