#include "objFuncTGradient.h"

void objFuncTGradient::calcObjVal()
{
    int nx = (*meshPtr).getNx(), ny = (*meshPtr).getNy();
    double dx = (*meshPtr).dx, dy = (*meshPtr).dy, vol = (*meshPtr).vol, gradx, grady;
    objVal = 0.0;

    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            // calculate y component
            if (j==ny){
                grady = ((*TPtr)[i][j] - (*TPtr)[i][j-1])/dy;
            }
            else if (j==1){
                grady = ((*TPtr)[i][j+1] - (*TPtr)[i][j])/dy;
            }
            else
            {
                // grady = ((*TPtr)[i][j+1] - (*TPtr)[i][j-1])/(2 * dy); // central difference will induce jumping gradient
                grady = ((*TPtr)[i][j] - (*TPtr)[i][j-1])/dy;
            }

            // calculate x component
            if (i==nx){
                gradx = ((*TPtr)[i][j] - (*TPtr)[i-1][j])/dx;
            }
            else if (i==1){
                gradx = ((*TPtr)[i+1][j] - (*TPtr)[i][j])/dx;
            }
            else
            {
                //gradx = ((*TPtr)[i+1][j] - (*TPtr)[i-1][j])/(2 * dx); // central difference will induce jumping gradient
                gradx = ((*TPtr)[i][j] - (*TPtr)[i-1][j])/dx;
            }

            objVal += (gradx * gradx + grady * grady) * vol;
        }
    }

    cout<<"The integral of gradient-T is:"<<objVal<<"\n";
}

void objFuncTGradient::evalObjForAD(RealR* xW, RealR* xX, RealR* y)
{
    int nx = (*meshPtr).getNx(), ny = (*meshPtr).getNy(), ii, jj;
    double dx = (*meshPtr).dx, dy = (*meshPtr).dy, vol = (*meshPtr).vol;
    RealR gradx, grady;

    y[0] = 0.0;
    for (size_t i = 0; i < nx * ny; i++){
        // get the (ii,jj) index of xW[i]
        ii = i % nx + 1;
        jj = (i - (ii - 1))/nx + 1;

        // calculate the y component
        if (jj == ny){
            grady = (xW[i] - xW[ii-1+(jj-2)*nx]) / dy;
        }
        else if (jj == 1){
            grady = (xW[ii-1+(jj)*nx] - xW[i]) / dy;
        }
        else
        {
            //grady = (xW[ii - 1 + jj*nx] - xW[ii - 1 + (jj - 2)*nx]) / (2 * dy); // central difference leads to jumping gradient
            grady = (xW[i] - xW[ii-1+(jj-2)*nx]) / dy;
        }

        // calculate the x component
        if (ii == nx){
            gradx = (xW[i] - xW[ii-2+(jj-1)*nx]) / dx;
        }
        else if (ii == 1){
            gradx = (xW[ii+(jj-1)*nx] - xW[i]) / dx;
        }
        else
        {
            // gradx = (xW[ii+(jj-1)*nx] - xW[ii-2+(jj-1)*nx]) / (2 * dx); // central difference leads to jumping gradient
            gradx = (xW[i] - xW[ii-2+(jj-1)*nx]) / dx;
        }
        y[0] += (gradx * gradx + grady * grady) * vol;
    }
}