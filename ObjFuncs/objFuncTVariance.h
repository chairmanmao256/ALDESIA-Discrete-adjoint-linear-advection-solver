#ifndef OBJFUNSTVARIANCE_H
#define OBJFUNCTVARIANCE_H
#include "objFuncs.h"
class objFuncTVariance : public objFuncs
{
    public:
    objFuncTVariance(volScalarField& T, volScalarField& S, volScalarField& nu, volVectorField& U,
    mesh& Mesh):objFuncs(T, S, nu, U, Mesh){}

    virtual void calcObjVal();

    virtual void evalObjForAD(RealR* xW, RealR* xX, RealR* y);
};
#endif