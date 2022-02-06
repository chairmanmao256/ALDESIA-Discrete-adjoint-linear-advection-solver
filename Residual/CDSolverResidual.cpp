#include <iostream>
#include <string>
#include <algorithm>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"
#include <codi.hpp>

using namespace std;
using Real = codi::RealForward;

void CDResidual(Real* x, Real* y, size_t nx, size_t ny, double dx, double dy, double vol, 
volVectorField& U, volScalarField& nu, volScalarField& S);

codi::Jacobian<double> CDSolverResidual(volScalarField& T, volScalarField& nu, volScalarField& S,
volVectorField& U, mesh& Mesh)
{
    // get the dimension of the mesh
    int nx = Mesh.getNx(), ny = Mesh.getNy();
    double dx = Mesh.dx, dy = Mesh.dy, vol = Mesh.vol;
    int pIndex, nIndex, sIndex, eIndex, wIndex;
    double Fs, Fn, Fw, Fe, Ds, Dn, De, Dw, as, an, ae, aw, ap;

    // initialize the FORWARD Real class for derivative computation, and assign the value
    Real* y = new Real[(nx + 2) * (ny + 2)];
    Real* x = new Real[(nx + 2) * (ny + 2)];
    
    for(size_t j = 0; j < ny + 2; j++){
        for(size_t i = 0; i < nx + 2; i++){
            x[i + j * (nx + 2)] = T[i][j];
        }
    }

    // compute the full Jacobian
    codi::Jacobian<double> jacobian(nx*ny,nx*ny);

    // compute the full Jacobian.
    for (size_t j = 1; j < ny + 1; j++){
        for (size_t i = 1; i < nx + 1; i++){
            x[i + j * (nx + 2)].gradient() = 1.0;

            CDResidual(x, y, nx, ny, dx, dy, vol, U, nu, S);

            for (size_t l = 1; l < ny + 1; l++){
                for (size_t m = 1; m < nx + 1; m++){
                    // compute the element of the Jacobian. only internal points are included
                    jacobian(m + (l-1) * nx - 1, i + (j-1) * nx - 1) 
                    = y[m + l * (nx + 2)].getGradient();

                    // std::cout<<"("<<m + (l - 1) * nx - 1<<", "
                    // << i + (j - 1) * nx - 1<<")" <<": "
                    // <<y[m + l * (nx + 2)].getGradient()<<"\n";
                }
            }

            x[i + j * (nx + 2)].gradient() = 0.0;
        }
    }

    delete [] y;
    delete [] x;

    cout<<"Jacobian computation completed!\n";

    return jacobian;
}

void CDResidual(Real* x, Real* y, size_t nx, size_t ny, double dx, double dy, double vol, 
volVectorField& U, volScalarField& nu, volScalarField& S){

    // the index of N, S, E, W and P points in the 1D x array and 1D y array
    size_t nIndex = 0, sIndex = 0, eIndex = 0, wIndex = 0, pIndex = 0;
    double Fs, Fn, Fw, Fe, Ds, Dn, De, Dw, as, an, aw, ae, ap;

    for(size_t j = 1; j <= ny; j++){
        for(size_t i = 1; i <= nx; i++){
            // get the 1D index of N, S, W ,E and P points
            pIndex = i + j * (nx + 2);
            nIndex = i + (j + 1) * (nx + 2);
            sIndex = i + (j - 1) * (nx + 2);
            wIndex = i - 1 + j * (nx + 2);
            eIndex = i + 1 + j * (nx + 2);

            // compute the convection-diffusion coefficients (Scaled)
            Fs = (U[1][i][j-1] + U[1][i][j]) / 2.0 * dx;
            Fn = (U[1][i][j+1] + U[1][i][j]) / 2.0 * dx;
            Fe = (U[0][i+1][j] + U[0][i][j]) / 2.0 * dy;
            Fw = (U[0][i-1][j] + U[0][i][j]) / 2.0 * dy;
            Ds = (nu[i][j] + nu[i][j-1]) / 2.0 / dy * dx;
            Dn = (nu[i][j] + nu[i][j+1]) / 2.0 / dy * dx;
            De = (nu[i][j] + nu[i+1][j]) / 2.0 / dx * dy;
            Dw = (nu[i][j] + nu[i-1][j]) / 2.0 / dx * dy;
            as = std::max(std::max(Fs, Ds + Fs / 2.0), 0.0);
            an = std::max(std::max(-Fn, Dn - Fn / 2.0), 0.0);
            ae = std::max(std::max(-Fe, De - Fe / 2.0), 0.0);
            aw = std::max(std::max(Fw, Dw + Fw / 2.0), 0.0);
            ap = Fe - Fw + Fn - Fs + as + an + ae + aw;

            // compute the residual
            y[pIndex] = vol*S[i][j] + as * x[sIndex] + an * x[nIndex] + ae * x[eIndex] + aw * x[wIndex] - ap * x[pIndex];
    }
  }
}