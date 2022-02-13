#include "objFuncAvgT.h"

void objFuncAvgT::calcObjVal()
{
    int nx = (*meshPtr).getNx(), ny = (*meshPtr).getNy();
    objVal = 0.0;

    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            objVal += (*TPtr)[i][j];
        }
    }

    objVal = objVal / double(nx * ny);
    cout<<"The average T is:"<<objVal<<"\n";
}

void objFuncAvgT::evalObjForAD(RealR* xW, RealR* xX, RealR* y)
{
    int nx = (*meshPtr).getNx(), ny = (*meshPtr).getNy();

    y[0] = 0.0;
    for (size_t i = 0; i < nx * ny; i++){
        y[0] += xW[i];
    }
    y[0] = y[0]/(nx * ny);
}