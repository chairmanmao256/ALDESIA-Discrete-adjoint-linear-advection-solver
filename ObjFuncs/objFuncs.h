#include <iostream>
#include <codi.hpp>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"

using RealR = codi::RealReverse;
using namespace std;

#ifndef OBJFUNCS_H
#define OBJFUNCS_H
// base class of objective functions
class objFuncs{
    protected:
    // the dependent field pointer
    volScalarField* TPtr = nullptr;

    // the source field pointer
    volScalarField* SPtr = nullptr;

    // the diffusitivity field pointer
    volScalarField* nuPtr = nullptr;

    // the velocity vector field pointer
    volVectorField* UPtr = nullptr;

    // mesh pointer
    mesh* meshPtr = nullptr;

    // obj's value
    double objVal;

    public:
    // constructor
    objFuncs(volScalarField& T, volScalarField& S, volScalarField& nu, volVectorField& U,
    mesh& Mesh):TPtr(&T),SPtr(&S),nuPtr(&nu),UPtr(&U),meshPtr(&Mesh){}

    // destructor
    virtual ~objFuncs(){}

    // return the obj value
    double getObjVal();

    // calculate obj value
    virtual void calcObjVal() {objVal = 0.0;}

    // evaluate the obj, needed by reverse-mode AD
    virtual void evalObjForAD(RealR* xW, RealR* xX, RealR* y);

};

#endif