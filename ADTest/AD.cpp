#include <codi.hpp>
#include <iostream>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include <algorithm>

using Real = codi::RealForward;

void func(Real* x, Real* y, size_t len);
void LaplacianResidual(Real* x, Real* y, size_t nx, size_t ny);
void CDResidual(Real* x, Real* y, size_t nx, size_t ny, volVectorField& U, volScalarField& nu);
double* CDResidualFD(int nx, int ny, int xIndex, int yIndex, volScalarField& T, volVectorField& U, volScalarField& nu);
 
int main(int nargs, char** args) {
  // initialize the fields
  int nx = 3, ny = 3, xindex = 1, yindex = 2;
  std::string name = {"T"}, nameU = {"U"}, nameNu = {"nu"};
  std::string BCtype[4] = {"fixedGradient","fixedGradient","fixedGradient","fixedGradient"};
  double BCvalue[4] = {0.0,0.0,0.0,0.0};
  double Ini = 0.1, * deriv = nullptr;
  volScalarField T(nx, ny, name, BCtype, Ini, BCvalue);
  volVectorField U(nx, ny, nameU, BCtype, 0.0, 0.0, BCvalue, BCvalue);
  volScalarField nu(nx, ny, nameNu, BCtype, 0.1*Ini, BCvalue);
  volVectorField& U_ = U;
  volScalarField& nu_ = nu;
  volScalarField& T_ = T;

  // initialize the jacobian
  codi::Jacobian<double> jacobian(nx*ny,nx*ny);

  // initialize the "Real" class for derivative compuatation and set the value
  Real* y = new Real[(nx + 2) * (ny + 2)];
  Real* x = new Real[(nx + 2) * (ny + 2)];
  for (size_t j = 0; j < ny + 2; j++){
    for (size_t i = 0; i < nx + 2; i++){
      x[i + j * (ny + 2)] = T[i][j];
    }
  }

  // compute the full Jacobian.
  for (size_t j = 1; j < ny + 1; j++){
    for (size_t i = 1; i < nx + 1; i++){
      x[i + j * (nx + 2)].gradient() = 1.0;

      //func(x,y,(nx+2)*(ny+2));
      //LaplacianResidual(x, y, nx, ny);
      CDResidual(x, y, nx, ny, U_, nu_);

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

  // FD check
  deriv = CDResidualFD(nx, ny, xindex, yindex, T_, U_, nu_);

  // for(size_t i = 0; i < nx*ny; i++){
  //   std::cout<<"AD: "<<jacobian(i,xindex + (yindex-1) * nx - 1)<<"  FD: "<<deriv[i];
  //   std::cout<<"\n";
  // }

  for(size_t i = 0; i < nx*ny; i++){
    for(size_t j = 0; j < nx*ny; j++){
      std::cout<<jacobian(i,j)<<" ";
    }
    std::cout<<"\n";
  }
 
  delete [] y;
  delete [] x;
  delete [] deriv;

  return 0;
}

void func(Real* x, Real* y, size_t len){
  for (size_t i = 0; i < len; i++){
    y[i] = x[i] * x[i];
  }
}

void LaplacianResidual(Real* x, Real* y, size_t nx, size_t ny){
  // the index of N, S, E, W and P points in the 1D x array and 1D y array
  size_t nIndex = 0, sIndex = 0, eIndex = 0, wIndex = 0, pIndex = 0;

	for(size_t j = 1; j <= ny; j++){
    for(size_t i = 1; i <= nx; i++){
      // get the 1D index of N, S, W ,E and P points
      pIndex = i + j * (nx + 2);
      nIndex = i + (j + 1) * (nx + 2);
      sIndex = i + (j - 1) * (nx + 2);
      wIndex = i - 1 + j * (nx + 2);
      eIndex = i + 1 + j * (nx + 2);

      // compute the Laplacian residual (Scaled)
      y[pIndex] = -4.0 * x[pIndex] + x[nIndex] + x[sIndex] + x[wIndex] + x[eIndex];
    }
  }
}

void CDResidual(Real* x, Real* y, size_t nx, size_t ny, volVectorField& U, volScalarField& nu){
  // the index of N, S, E, W and P points in the 1D x array and 1D y array
  size_t nIndex = 0, sIndex = 0, eIndex = 0, wIndex = 0, pIndex = 0;
  double Fs, Fn, Fw, Fe, Ds, Dn, De, Dw, as, an, aw, ae, ap;
  double dx = 1.0/double(nx), dy = 1.0/double(ny);

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
      y[pIndex] = as * x[sIndex] + an * x[nIndex] + ae * x[eIndex] + aw * x[wIndex] - ap * x[pIndex];
    }
  }
}

double* CDResidualFD(int nx, int ny, int xIndex, int yIndex, volScalarField& T, volVectorField& U, volScalarField& nu){
  double Fs, Fn, Fw, Fe, Ds, Dn, De, Dw, as, an, aw, ae, ap, us, un, uw, ue;
  double dx = 1.0/double(nx), dy = 1.0/double(ny);
  double* col1 = new double[nx*ny],* col2 = new double[nx*ny],* deriv = new double[nx*ny];

	for(int j = 1; j <= ny; j++){
    for(int i = 1; i <= nx; i++){
      us = T[i][j-1]; un = T[i][j+1]; uw = T[i-1][j]; ue = T[i+1][j];
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

      col1[i + (j - 1) * nx - 1] = as * us + an * un + aw * uw + ae * ue - ap * T[i][j];
    }
  }

  T[xIndex][yIndex] += 1e-1;

  for(int j = 1; j <= ny; j++){
    for(int i = 1; i <= nx; i++){
      us = T[i][j-1]; un = T[i][j+1]; uw = T[i-1][j]; ue = T[i+1][j];
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

      col2[i + (j - 1) * nx - 1] = as * us + an * un + aw * uw + ae * ue - ap * T[i][j];
    }
  }

  T[xIndex][yIndex] -= 1e-1;

  for(int i = 0; i < nx*ny; i++){
    deriv[i] = (col2[i] - col1[i]) / 1e-1;
  }

  delete [] col1;
  delete [] col2;

  return deriv;
}