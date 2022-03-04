#include <codi.hpp>
#include <iostream>
#include <string>
#include "AdjointSolver.h"

using namespace std;

codi::Jacobian<double> solveAdjoint(volScalarField& T, volScalarField& S, volScalarField& nu,
volVectorField& U, mesh& Mesh, double tol, double omega, int maxIter, objFuncs& obj, bool isWrite)
{
    // initialize the SOR parameters
    double res = 1e4, valOld, valNew, product = 0.0, offDiag = 0.0;
    int step = 0, nx = Mesh.getNx(), ny = Mesh.getNy(), num;
    double* lambda = new double [nx * ny];
    double** dRdWSparse = new double* [5];
    int** sparseIndex = new int* [5];

    std::cout<<"Initializing sparse matrix...\n";
    // initialize the sparse matrix
    for (int i = 0; i < 5; i++){
        dRdWSparse[i] = new double [nx * ny];
        sparseIndex[i] = new int [nx * ny];
        for (int j = 0; j < nx * ny; j++){
            dRdWSparse[i][j] = 0.0;
            sparseIndex[i][j] = -1;
        }
    }

    for (int i = 0; i < nx * ny; i++){lambda[i] = 0.1;}

    // initialize the gradient Jacobian
    codi::Jacobian<double> grad(1, nx * ny);

    // if the objective function relies only on DV, use
    // DA to compute its gradient
    if (obj.getIsDVOnly())
    {
        std::cout<<"The objective depends on design variables only!\n";
        std::cout<<"Aldesia uses reverse mode AD to calcluate gradient.\n";
        grad = calcdFdX(T, U, nu, S, Mesh, obj);
        std::cout<<"Job finished!\n";
        return grad;
    }

    // calculate the Jacobians and write them

    // compute the Jacobian, graph-coloring is implemented
    codi::Jacobian<double> dRdW = calcdRdWColored(T, nu, S, U, Mesh);

    // dRdW file name
    string dRdWFile = {"dRdWColored.dat"};

    // compute the Jacobian, graph-coloring is implemented
    codi::Jacobian<double> dRdX = calcdRdXColored(T, nu, S, U, Mesh);

    //  dRdX file name
    string dRdXFile = {"dRdXColored.dat"};

    // compute the objective function F and dFdW
    codi::Jacobian<double> dFdW = calcdFdW(T, U, nu, S, Mesh, obj);

    // dFdW file name
    string dFdWFile = {"dFdW.dat"};

    // compute the objective function F and dFdX
    codi::Jacobian<double> dFdX = calcdFdX(T, U, nu, S, Mesh, obj);

    // dFdW file name
    string dFdXFile = {"dFdX.dat"};

    // if we write the Jacobian
    if(isWrite){
        writeJac(dRdW, dRdWFile);
        writeJac(dRdX, dRdXFile);
        writeJac(dFdW, dFdWFile);
        writeJac(dFdX, dFdXFile);
    }

    // set the sparse matrix
    for (int i = 0; i < nx * ny; i++){
        // set the diagnoals
        dRdWSparse[0][i] = dRdW(i,i);
        // set column index
        sparseIndex[0][i] = i;

        //set the off-diagnoals
        num = 1;
        for (int j = 0; j < nx * ny; j++){
            if (abs(dRdW(j, i)) > 1e-12 && i != j){
                dRdWSparse[num][i] = dRdW(j,i);
                sparseIndex[num][i] = j;
                num++;
            }
        }
    }

    // solve the linear equation using SOR

    while(step < maxIter && res > tol){
        step ++;
        res = -1.0;
        for (int i = 0; i < nx * ny; i++){

            // add up all the off diagnoal terms
            for (int j = 1; j < 5; j++){
                //if (j != i) {offDiag += dRdW(j, i) * lambda[j];}
                if (sparseIndex[j][i]!=-1){
                    offDiag += dRdWSparse[j][i] * lambda[sparseIndex[j][i]];
                }  
            }

            // SOR iteration
            valNew = omega * (dFdW(0, i) - offDiag) / dRdW(i,i) + (1.0 - omega) * lambda[i];
            if(abs(valNew - lambda[i]) > res) {res = abs(valNew - lambda[i]);}
            lambda[i] = valNew;

            // reset the offDiagnoal terms
            offDiag = 0.0;
        }
        if(step % 100 == 0 || step == 1){
            std::cout<<"Iteration step:   "<< step <<"\n";
            std::cout<<"Adjoint Residual: "<< res << "\n";
            std::cout<<"\n";
        }
    }
    std::cout<<"Adjoint computation ends after "<<step<<" iterations.\n";
    std::cout<<"The residual documented at last is: "<<res<<"\n";

    // calculate the gradient
    for (int i = 0; i < nx * ny; i++){

        // vector-matrix product, more abstraction might be needed
        for (int j = 0; j < nx * ny; j++){
            product += lambda[j] * dRdX(j, i);
        }
        grad(0,i) = dFdX(0,i) - product;

        // reset product to 0
        product = 0.0;
    }

    // deallocate the space
    delete [] lambda;
    for (int i = 0; i < 5; i++){
        delete [] dRdWSparse[i];
        delete [] sparseIndex[i];
    }

    return grad;
}