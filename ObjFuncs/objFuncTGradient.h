#ifndef OBJFUNSTGRADIENT_H
#define OBJFUNCTGRADIENT_H
#include "objFuncs.h"
class objFuncTGradient : public objFuncs
{
    public:
    objFuncTGradient(volScalarField& T, volScalarField& S, volScalarField& nu, volVectorField& U,
    mesh& Mesh):objFuncs(T, S, nu, U, Mesh){}

    virtual void calcObjVal();

    virtual void evalObjForAD(RealR* xW, RealR* xX, RealR* y);
};
#endif