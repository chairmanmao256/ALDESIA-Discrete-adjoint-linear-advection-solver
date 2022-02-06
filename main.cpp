#include <iostream>
#include <string>
#include "config/ConfigMap.h"
#include "field/volScalarField.h"
#include "field/volVectorField.h"
#include "field/mesh.h"
#include "Solver/CDSolver.h"
#include "writer/writePlt.h"
#include "writer/writeJac.h"
#include "Residual/CDSolverResidual.h"
#include <codi.hpp>

using namespace std;

int main(int argc, char** argv)
{
    if(argc != 2){
        cout<<"Wrong argument number!\n";
        cout<<"Usage: ./main.out <inputFileName>\n";
    }

    // get the file name and initiate the dictionary
    std::string filename = argv[1];
    ConfigMap test(filename);

    // read Values
    #include "readValue.h"

    // print user input data information
    cout<<"* * * * * * Problem Description * * * * * *\n";
    cout<<"nx: "<<nx<<", ny: "<<ny<<"\n";
    cout<<"Scalar's name:     "<<dependentScalar<<"\n";
    cout<<"BC type, South:    "<<BCtype_[0]<<"\n"
        <<"         North:    "<<BCtype_[1]<<"\n"
        <<"         East:     "<<BCtype_[2]<<"\n"
        <<"         West:     "<<BCtype_[3]<<"\n";
    cout<<"Scalar initial:    "<<dpIni<<"\n";
    cout<<"Velocity initial:  ("<<UXIni<<", "<<UYIni<<")\n";
    cout<<"Diffusitivity:     "<<nuIni<<"\n";
    cout<<"Uniform source:    "<<SIni<<"\n";
    cout<<"* * * * * * * * * * * * * * * * * * * * *\n"<<"\n";

    cout<<"* * * * * * * Solver Setting * * * * * * *\n";
    cout<<"Relaxation of SOR: "<<omega<<"\n";
    cout<<"Absolute tolerance:"<<tol<<"\n";
    cout<<"Maximum iteration: "<<maxIter<<"\n"<<"\n";

    // Initialize the fields
    volScalarField T(nx, ny, dependentScalar, BCtype_, dpIni, BCvalue_);
    volScalarField nu(nx, ny, diffusitivity, BCtypeZeroGradient, nuIni, BCvalueZeroGradient);
    volScalarField S(nx, ny, source, BCtypeZeroGradient, SIni, BCvalueZeroGradient);
    volVectorField U(nx, ny, velocity, BCtypeVelocity_, UXIni, UYIni, BCvalueVelocityX_, BCvalueVelocityY_);
    mesh Mesh(nx, ny, box[0],box[1], box[2], box[3]);
    volVectorField center = Mesh.C();

    //Set the distribution of the source term
    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            if(center[0][i][j] <= 0.6 && center[0][i][j] >= 0.4 && center[1][i][j] <= 0.6 && center[1][i][j] >= 0.4){
                S[i][j] = 1.0;
            }
        }
    }

    // Create the Refs of the fields
    volScalarField& T_ = T;
    volScalarField& nu_ = nu;
    volScalarField& S_ = S;
    volVectorField& U_ = U;
    mesh& Mesh_ = Mesh;

    // Solve the Problem
    CDSolver(T_, nu_, U_, S_, Mesh_, omega, tol, maxIter);

    // write the outcome
    writePlt(T_, nu_, S_, U_, Mesh_);

    // compute the Jacobian
    codi::Jacobian<double> jac = CDSolverResidual(T_, nu_, S_, U_, Mesh_);

    // write the Jacobian
    writeJac(jac, Mesh_);

    return 0;
}