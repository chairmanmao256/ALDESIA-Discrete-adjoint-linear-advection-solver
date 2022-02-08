#include <iostream>
#include <string>
#include <codi.hpp>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"

using RealR = codi::RealReverse;

void averageT(RealR* x, RealR* y, size_t nx, size_t ny);

codi::Jacobian<double> dFdW(volScalarField& T, volVectorField& U, volScalarField& nu, volScalarField& S, mesh& Mesh);