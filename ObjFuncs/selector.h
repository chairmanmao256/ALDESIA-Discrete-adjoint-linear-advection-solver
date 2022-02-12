#include "objFuncs.h"

template <typename child>
objFuncs* selector(volScalarField& T, volScalarField& S, volScalarField& nu, volVectorField& U,
mesh& Mesh){
    return new child(T, S, nu, U, Mesh);
}
