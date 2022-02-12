#include <codi.hpp>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"
#include "../writer/writePlt.h"
#include "../writer/writeJac.h"
#include "../Residual/CDSolverResidual.h"
#include "../ObjFuncs/DerivObj.h"

codi::Jacobian<double> solveAdjoint(volScalarField& T, volScalarField& S, volScalarField& nu,
volVectorField& U, mesh& Mesh, double tol, double omega, int maxIter, objFuncs& obj);