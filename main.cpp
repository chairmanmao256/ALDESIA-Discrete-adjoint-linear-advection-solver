#include <iostream>
#include <string>
#include <codi.hpp>
#include "config/ConfigMap.h"
#include "field/volScalarField.h"
#include "field/volVectorField.h"
#include "field/mesh.h"
#include "Solver/CDSolver.h"
#include "writer/writePlt.h"
#include "writer/writeJac.h"
#include "Residual/CDSolverResidual.h"
#include "ObjFuncs/averageT.h"
#include "AdjointSolver/AdjointSolver.h"

codi::Jacobian<double> FDCheck(volScalarField& T, volScalarField&S, volScalarField& nu,
volVectorField& U, mesh& Mesh);

double avgT(int nx, int ny, volScalarField& T);

int main(int argc, char** argv)
{
    using namespace std;
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
            // if(center[0][i][j] <= 0.6 && center[0][i][j] >= 0.4 && center[1][i][j] <= 0.6 && center[1][i][j] >= 0.4){
            //     S[i][j] = 1.0;
            // }
            if (i<=3 && i>=2 && j<=3 && j>=2){
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

    // solve adjoint and get the gradient
    codi::Jacobian<double> grad = solveAdjoint(T_, S_, nu_, U_, Mesh_, tol, omega, maxIter);

    string fname = "gradient.dat";
    writeJac(grad, fname);

    // solve adjoint and get the gradient
    codi::Jacobian<double> gradFD = FDCheck(T_, S_, nu_, U_, Mesh_);

    string fname2 = "gradientFD.dat";
    writeJac(gradFD, fname2);

    return 0;
}

codi::Jacobian<double> FDCheck(volScalarField& T, volScalarField&S, volScalarField& nu,
volVectorField& U, mesh& Mesh)
{
    int nx = Mesh.getNx(), ny = Mesh.getNy();
    codi::Jacobian<double> gradFD(1, nx * ny);
    double ref = 0.0, perturb = 0.0;

    ref = avgT(nx, ny, T);

    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            S[i][j] += 0.001;
            CDSolver(T, nu, U, S, Mesh, 1.5, 1e-7, 5000);
            perturb = avgT(nx, ny, T);
            gradFD(0, i - 1 + (j - 1) * nx) = (perturb - ref) / 1e-3;
            S[i][j] -= 0.001;
        }
    }

    return gradFD; 
}

double avgT(int nx, int ny, volScalarField& T){
    double ref = 0.0;
    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            ref += T[i][j];
        }
    }
    ref = ref / (nx * ny);
    std::cout<<"avgT = "<<ref<<"\n";
    return ref;
}