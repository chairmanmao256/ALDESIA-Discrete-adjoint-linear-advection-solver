#include "mesh.h"
#include "volVectorField.h"
#include <string>
#include <iostream>

// get the cell-center coordinate of every mesh cell and return it as a volVectorField
volVectorField mesh::C()
{
    // initialize the volVectorField
    std::string name_ = {"meshCell"};
    std::string BCtype_[4];
    double BCvalueX_[4] = {0.0,0.0,0.0,0.0};
    double BCvalueY_[4] = {0.0,0.0,0.0,0.0};
    double iniX = 0.0, iniY = 0.0;
    BCtype_[0] = "fixedValue";
    BCtype_[1] = "fixedValue";
    BCtype_[2] = "fixedValue";
    BCtype_[3] = "fixedValue";

    volVectorField center(nx, ny, name_, BCtype_, iniX, iniY, BCvalueX_, BCvalueY_);

    for(int j = 0; j < ny + 2; j++){
        for(int i = 0; i < nx + 2; i++){
            center[0][i][j] = xmin - dx/2.0 + double(i) * dx;
            center[1][i][j] = ymin - dy/2.0 + double(j) * dy;
        }
    }

    return center;
}