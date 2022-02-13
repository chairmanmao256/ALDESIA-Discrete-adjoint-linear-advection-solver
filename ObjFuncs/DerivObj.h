#include <iostream>
#include <string>
#include <codi.hpp>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"
#include "objFuncs.h"

using RealR = codi::RealReverse;

// the Jacobian of objective function with respect to state variable
codi::Jacobian<double> calcdFdW(volScalarField& T, volVectorField& U, volScalarField& nu, 
volScalarField& S, mesh& Mesh, objFuncs& obj);

// the Jacobian of objective function with respect to design variable
codi::Jacobian<double> calcdFdX(volScalarField& T, volVectorField& U, volScalarField& nu,
volScalarField& S, mesh& Mesh, objFuncs& obj);