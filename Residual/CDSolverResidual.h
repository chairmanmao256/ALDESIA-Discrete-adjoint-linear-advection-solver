#include <iostream>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"
#include <codi.hpp>

codi::Jacobian<double> CDSolverResidual(volScalarField& T, volScalarField& nu, volScalarField& S,
volVectorField& U, mesh& Mesh);

codi::Jacobian<double> CDSolverResidualColored(volScalarField& T, volScalarField& nu, volScalarField& S,
volVectorField& U, mesh& Mesh);