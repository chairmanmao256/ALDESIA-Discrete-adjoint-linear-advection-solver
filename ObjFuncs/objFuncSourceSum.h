#ifndef OBJFUNSAVGT_H
#define OBJFUNCAVGT_H
#include "objFuncs.h"
class objFuncSourceSum : public objFuncs
{
    public:
    objFuncSourceSum(volScalarField& T, volScalarField& S, volScalarField& nu, volVectorField& U,
    mesh& Mesh):objFuncs(T, S, nu, U, Mesh){isDVOnly = true;}

    virtual void calcObjVal();

    virtual void evalObjForAD(RealR* xW, RealR* xX, RealR* y);
};

#endif