#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"

void CDSolver(volScalarField& T, volScalarField& nu, volVectorField& U, volScalarField& S, mesh& Mesh, double omega, double tol, int maxIter);