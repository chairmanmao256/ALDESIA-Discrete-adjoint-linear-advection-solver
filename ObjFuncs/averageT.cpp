#include <iostream>
#include <string>
#include <codi.hpp>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"
#include "averageT.h"

// since we want to get the gradient of the objective function, we use reverse mode AD
using RealR = codi::RealReverse;
using Tape = typename RealR::Tape;
using namespace std;

void averageT(RealR* x, RealR* y, size_t nx, size_t ny)
{
    y[0] = 0.0;
    for (size_t i = 0; i < nx * ny; i++){
        y[0] += x[i];
    }
    y[0] = y[0]/(nx * ny);

    cout<<"The average T is:"<<y[0]<<"\n";
}

codi::Jacobian<double> dFdW(volScalarField& T, volVectorField& U, volScalarField& nu, volScalarField& S, mesh& Mesh)
{
    // get the dimension info
    int nx = Mesh.getNx(), ny = Mesh.getNy();

    // initialize the AD variables
    RealR* x = new RealR[nx * ny];
    RealR* y = new RealR[1];
    codi::Jacobian<double> jacobian(1, nx * ny);

    cout<<"getM: "<<jacobian.getM()<<" getN: "<<jacobian.getN()<<"\n";

    for (size_t j = 1; j <= ny; j++){
        for (size_t i = 1; i <= nx; i++){
            x[i - 1 + (j - 1) * nx] = T[i][j];
        }
    }

    // initialize and activate tape recording
    Tape& tape = RealR::getTape();
    tape.setActive();

    // register input
    for (size_t i = 0; i < nx * ny; i++){
        tape.registerInput(x[i]);
    }

    // evaluate the obj
    averageT(x, y, nx, ny);

    // register output
    tape.registerOutput(y[0]);

    // set the tape passive
    tape.setPassive();

    // set the weight of output and evaluate the gradient
    y[0].gradient() = 1.0;
    tape.evaluate();
    for (size_t i = 0; i < nx * ny; i++){
        jacobian(0, i) = x[i].getGradient();
    }

    return jacobian;

    delete [] x;
    delete [] y;
}