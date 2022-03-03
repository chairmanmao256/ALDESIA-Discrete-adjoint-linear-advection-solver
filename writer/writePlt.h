#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"

void writePlt(volScalarField& T, volScalarField& nu, volScalarField& S, volVectorField& U, mesh& Mesh, std::string filename);