#include <iostream>
#include <string>
#include <codi.hpp>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"

using RealR = codi::RealReverse;

// objective function
void averageT(RealR* xW, RealR* xX, RealR* y, size_t nx, size_t ny);

// the Jacobian of objective function with respect to state variable
codi::Jacobian<double> calcdFdW(volScalarField& T, volVectorField& U, volScalarField& nu, volScalarField& S, mesh& Mesh);

// the Jacobian of objective function with respect to design variable
codi::Jacobian<double> calcdFdX(volScalarField& T, volVectorField& U, volScalarField& nu, volScalarField& S, mesh& Mesh);