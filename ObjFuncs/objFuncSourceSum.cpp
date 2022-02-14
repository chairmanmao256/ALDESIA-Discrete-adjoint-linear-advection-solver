#include "objFuncSourceSum.h"

void objFuncSourceSum::calcObjVal()
{
    int nx = (*meshPtr).getNx(), ny = (*meshPtr).getNy();
    double cellVol = (*meshPtr).vol;
    objVal = 0.0;

    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            objVal += (*SPtr)[i][j] * cellVol;
        }
    }

    cout<<"The domain integration of S is:"<<objVal<<"\n";
}

void objFuncSourceSum::evalObjForAD(RealR* xW, RealR* xX, RealR* y)
{
    int nx = (*meshPtr).getNx(), ny = (*meshPtr).getNy();
    double cellVol = (*meshPtr).vol;

    y[0] = 0.0;
    for (size_t i = 0; i < nx * ny; i++){
        y[0] += xX[i] * cellVol;
    }
}