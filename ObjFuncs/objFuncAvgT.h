#include "objFuncs.h"

#ifndef OBJFUNSAVGT_H
#define OBJFUNCAVGT_H
class objFuncAvgT : public objFuncs
{
    public:
    objFuncAvgT(volScalarField& T, volScalarField& S, volScalarField& nu, volVectorField& U,
    mesh& Mesh):objFuncs(T, S, nu, U, Mesh){}

    virtual void calcObjVal();

    virtual void evalObjForAD(RealR* xW, RealR* xX, RealR* y);
};
#endif