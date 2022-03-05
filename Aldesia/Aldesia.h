#ifndef ALDESIA_H
#define ALDESIA_H

#include <iostream>
#include <string>
#include <map>
#include <codi.hpp>
#include "../config/ConfigMap.h"
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"
#include "../Solver/CDSolver.h"
#include "../writer/writePlt.h"
#include "../writer/writeJac.h"
#include "../Residual/CDSolverResidual.h"
#include "../ObjFuncs/DerivObj.h"
#include "../ObjFuncs/objFuncs.h"
#include "../ObjFuncs/objFuncAvgT.h"
#include "../ObjFuncs/objFuncSourceSum.h"
#include "../ObjFuncs/objFuncTGradient.h"
#include "../ObjFuncs/selector.h"
#include "../AdjointSolver/AdjointSolver.h"

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

    /// the bounding box of initial non-zero S area
    double boxXmin = 0.0, boxXmax = 0.0, boxYmin = 0.0, boxYmax = 0.0;

    /// decide if we want to solve the adjoint
    bool solvePrimalOnly = false;

    /// decide if we want to write the Jacobian and the gradient
    bool isWriteJac = false, isWriteGrad = false;

    /// the number of primal and adjoint evaluations
    int nSolvePrimals = 0, nSolveAdjoints = 0;
    

    public:
    /// constructor
    Aldesia(string inputFile);

    /// destructor
    ~Aldesia();

    /// set the string-obj map
    void setObjMap();

    /// set the design variable
    void setDesignVariable(int oneDimesionalIndex, double val);

    /// give a one-dimensional index, check if it is in the box
    /// in which the user specifies non-zero S
    bool isInBox(int oneDimensionalIndex);

    /// solve the PDE
    void solvePrimal();

    /// solve adjoint
    void solveDA(string objName);

    /// calculate the objective function value
    double calcObj(string objName);

    /// write the .plt file of the primal solution
    void writePrimal();

    /// print the header
    void printHeader();

    /// get the gradient
    double getGrad(int i);

    /// update the number of adjoint solve
    void updateSolveAdjoint() {nSolveAdjoints++;}
};


#endif