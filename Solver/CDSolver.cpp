#include <iostream>
#include <string>
#include <algorithm>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"
#include "CDSolver.h"

using namespace std;
// Solve the Convection-Diffusion Problem using SOR method

void CDSolver(volScalarField& T, volScalarField& nu, volVectorField& U, volScalarField& S, mesh& Mesh, double omega, double tol, int maxIter)
{
    // read the mesh information
    int nx = Mesh.getNx(), ny = Mesh.getNy();
    double dx = Mesh.dx, dy = Mesh.dy, vol = Mesh.vol;

    // Control parameter of the SOR solver
    double res = 1000.0, valOld, valNew;
    double us, un, ue, uw, ap, as, an, ae, aw, Fs, Fn, Fe, Fw, Ds, Dn, Dw, De;
    int step = 0;

    // SOR Starts
    while(res > tol && step < maxIter){
        step++;
        res = -1.0;
        for(int j = 1; j <= ny; j++){
            for(int i = 1; i <= nx; i++){
                valOld = T[i][j];
                us = T[i][j-1]; un = T[i][j+1]; uw = T[i-1][j]; ue = T[i+1][j];
                Fs = (U[1][i][j-1] + U[1][i][j]) / 2.0 * dx;
                Fn = (U[1][i][j+1] + U[1][i][j]) / 2.0 * dx;
                Fe = (U[0][i+1][j] + U[0][i][j]) / 2.0 * dy;
                Fw = (U[0][i-1][j] + U[0][i][j]) / 2.0 * dy;
                Ds = (nu[i][j] + nu[i][j-1]) / 2.0 / dy * dx;
                Dn = (nu[i][j] + nu[i][j+1]) / 2.0 / dy * dx;
                De = (nu[i][j] + nu[i+1][j]) / 2.0 / dx * dy;
                Dw = (nu[i][j] + nu[i-1][j]) / 2.0 / dx * dy;
                as = max(max(Fs, Ds + Fs / 2.0), 0.0);
                an = max(max(-Fn, Dn - Fn / 2.0), 0.0);
                ae = max(max(-Fe, De - Fe / 2.0), 0.0);
                aw = max(max(Fw, Dw + Fw / 2.0), 0.0);
                ap = Fe - Fw + Fn - Fs + as + an + ae + aw;
                valNew = (aw*uw + ae*ue + as*us + an*un + vol * S[i][j]) / ap;
                valNew = valNew * omega + valOld * (1.0 - omega);

                if(abs(valNew - valOld) > res) {res = abs(valNew - valOld);}
                T[i][j] = valNew;
            }
        }
        // print solver performance info
        if(step%100 == 0 || step == 1){
            cout<<"Iteration step: "<< step <<"\n";
            cout<<"      Residual: "<< res << "\n";
            cout<<"\n";
        }
    }
    cout<<"Computation ends after "<<step<<" iterations.\n";
    cout<<"The residual documented at last is: "<<res<<"\n";
}