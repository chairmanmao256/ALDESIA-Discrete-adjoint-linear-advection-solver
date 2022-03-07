#include "objFuncTVariance.h"

void objFuncTVariance::calcObjVal()
{
    int nx = (*meshPtr).getNx(), ny = (*meshPtr).getNy();
    double dx = (*meshPtr).dx, dy = (*meshPtr).dy, vol = (*meshPtr).vol, avg = 0.0;
    objVal = 0.0;


    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            avg += (*TPtr)[i][j];
        }
    }
    avg = avg / double(nx * ny);

    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            objVal += ((*TPtr)[i][j] - avg) * ((*TPtr)[i][j] - avg);
        }
    }
    objVal = objVal / double(nx * ny);    

    cout<<"The variance of T is:"<<objVal<<"\n";
}

void objFuncTVariance::evalObjForAD(RealR* xW, RealR* xX, RealR* y)
{
    int nx = (*meshPtr).getNx(), ny = (*meshPtr).getNy(), ii, jj;
    double dx = (*meshPtr).dx, dy = (*meshPtr).dy, vol = (*meshPtr).vol;
    RealR avg = 0.0;

    y[0] = 0.0;
    for (size_t i = 0; i < nx * ny; i++){
        avg += xW[i];
    }
    avg = avg / double(nx * ny);

    for (size_t i = 0; i < nx * ny; i++){
        y[0] += (xW[i] - avg) * (xW[i] - avg);
    }

    y[0] = y[0] / double(nx * ny);

}