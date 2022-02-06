#include "volVectorField.h"
#include <iostream>
#include <string>

volVectorField::volVectorField(int nx_, int ny_, std::string name_, std::string* BCtype_, double iniValueX_, double iniValueY_, double* BCvalueX_, double* BCvalueY_)
{
    // set the name and the BCtype
    name = name_;
    for (int i = 0; i < 4; i++){
        BCtype[i] = BCtype_[i];
        BCvalueX[i] = BCvalueX_[i];
        BCvalueY[i] = BCvalueY_[i];
    }

    // set the dimension
    nx = nx_;
    ny = ny_;

    // initialize the field array
    for (int k = 0; k < 2; k++){
        field[k] = new double* [nx + 2];
        for (int i = 0; i < nx + 2; i++){
            field[k][i] = new double [ny + 2];
        }
    }

    // set the INTERNAL field value to the initial value specified
    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            field[0][i][j] = iniValueX_;
            field[1][i][j] = iniValueY_;
        }
    }

    // correct the boundary value
    correctBoundary();
}

volVectorField::~volVectorField()
{
    for(int k = 0; k < 2; k++){
        for(int i = 0; i < nx + 2; i++){
            delete [] field[k][i];
        }
    }
}

void volVectorField::correctBoundary()
{
    // set southern boundary
    if (BCtype[0].compare("fixedValue")==0){
        for(int i = 0; i < nx + 2; i++){
            field[0][i][0] = BCvalueX[0];
            field[1][i][0] = BCvalueY[0];
        }
    }
    else if(BCtype[0].compare("fixedGradient")==0){
        for(int i = 0; i < nx + 2; i++){
            field[0][i][0] = field[0][i][1] - BCvalueX[0];
            field[1][i][0] = field[1][i][1] - BCvalueY[0];
        }
    }
    else{
        std::cout<<"Wrong boundary type at southern boundary!\n";
        std::cout<<"All options are:\n";
        std::cout<<"fixedValue\n"<<"fixedGradient\n";
    }

    // set northern boundary
    if (BCtype[1].compare("fixedValue")==0){
        for(int i = 0; i < nx + 2; i++){
            field[0][i][ny+1] = BCvalueX[1];
            field[1][i][ny+1] = BCvalueY[1];
        }
    }
    else if(BCtype[1].compare("fixedGradient")==0){
        for(int i = 0; i < nx + 2; i++){
            field[0][i][ny+1] = field[0][i][ny] + BCvalueX[1];
            field[1][i][ny+1] = field[1][i][ny] + BCvalueY[1];
        }
    }
    else{
        std::cout<<"Wrong boundary type at northern boundary!\n";
        std::cout<<"All options are:\n";
        std::cout<<"fixedValue\n"<<"fixedGradient\n";
    }

    // set western boundary
    if (BCtype[2].compare("fixedValue")==0){
        for(int j = 0; j < ny + 2; j++){
            field[0][0][j] = BCvalueX[2];
            field[1][0][j] = BCvalueY[2];
        }
    }
    else if(BCtype[2].compare("fixedGradient")==0){
        for(int j = 0; j < ny + 2; j++){
            field[0][0][j] = field[0][1][j] - BCvalueX[2];
            field[1][0][j] = field[1][1][j] - BCvalueY[2];
        }
    }
    else{
        std::cout<<"Wrong boundary type at westhern boundary!\n";
        std::cout<<"All options are:\n";
        std::cout<<"fixedValue\n"<<"fixedGradient\n";
    }

    // set eastern boundary
    if (BCtype[3].compare("fixedValue")==0){
        for(int j = 0; j < ny + 2; j++){
            field[0][nx+1][j] = BCvalueX[3];
            field[1][nx+1][j] = BCvalueY[3];
        }
    }
    else if(BCtype[3].compare("fixedGradient")==0){
        for(int j = 0; j < ny + 2; j++){
            field[0][nx+1][j] = field[0][nx][j] + BCvalueX[3];
            field[1][nx+1][j] = field[1][nx][j] + BCvalueY[3];
        }
    }
    else{
        std::cout<<"Wrong boundary type at easthern boundary!\n";
        std::cout<<"All options are:\n";
        std::cout<<"fixedValue\n"<<"fixedGradient\n";
    }
}