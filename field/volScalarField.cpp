#include <string>
#include <iostream>
#include "volScalarField.h"

volScalarField::volScalarField(int nx_, int ny_, std::string name_, std::string* BCtype_, double value_, double* BCvalue_)
{
    // set the name and the BCtype
    name = name_;
    for (int i = 0; i < 4; i++){
        BCtype[i] = BCtype_[i];
        BCvalue[i] = BCvalue_[i];
    }

    // set the dimension
    nx = nx_;
    ny = ny_;

    // initialize the field, virtual cells are included for boundary treatment
    field = new double* [nx + 2];
    for (int i = 0; i < nx + 2; i++){
        field[i] = new double [ny + 2];
    }

    // set the INTERNAL field value to the initial value specified
    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            field[i][j] = value_;
        }
    }

    // correct the boundary value
    correctBoundary();
}

volScalarField::~volScalarField()
{
    for(int i = 0; i < nx + 2; i++){
        delete [] field[i];
    }
}

void volScalarField::correctBoundary()
{
    // set southern boundary
    if (BCtype[0].compare("fixedValue")==0){
        for(int i = 0; i < nx + 2; i++){
            field[i][0] = BCvalue[0];
        }
    }
    else if(BCtype[0].compare("fixedGradient")==0){
        for(int i = 0; i < nx + 2; i++){
            field[i][0] = field[i][1] - BCvalue[0];
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
            field[i][ny+1] = BCvalue[1];
        }
    }
    else if(BCtype[1].compare("fixedGradient")==0){
        for(int i = 0; i < nx + 2; i++){
            field[i][ny+1] = field[i][ny] + BCvalue[1];
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
            field[0][j] = BCvalue[2];
        }
    }
    else if(BCtype[2].compare("fixedGradient")==0){
        for(int j = 0; j < ny + 2; j++){
            field[0][j] = field[1][j] - BCvalue[2];
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
            field[nx+1][j] = BCvalue[3];
        }
    }
    else if(BCtype[3].compare("fixedGradient")==0){
        for(int j = 0; j < ny + 2; j++){
            field[nx+1][j] = field[nx][j] + BCvalue[3];
        }
    }
    else{
        std::cout<<"Wrong boundary type at easthern boundary!\n";
        std::cout<<"All options are:\n";
        std::cout<<"fixedValue\n"<<"fixedGradient\n";
    }
}

int volScalarField::getNeighbor(int i, int j, const std::string direction)
{
    // check the index range
    if (i < 1 || i > nx || j < 1 || j > ny){
        std::cout<<"You can only use internal points' index!\n";
        return -1;
    }

    // decide which direction is consulted
    if(direction.compare("south")==0){
        if (j == 1 && BCtype[0].compare("fixedGradient") == 0) {return i + j * (nx + 2);}
        else{return i + (j - 1) * (nx + 2);}
    }
    else if(direction.compare("north")==0){
        if (j == ny && BCtype[1].compare("fixedGradient") == 0) {return i + j * (nx + 2);}
        else{return i + (j + 1) * (nx + 2);}
    }
    else if(direction.compare("west")==0){
        if (i == 1 && BCtype[2].compare("fixedGradient") == 0) {return i + j * (nx + 2);}
        else{return i - 1 + j * (nx + 2);}
    }
    else if(direction.compare("east")==0){
        if (i == nx && BCtype[3].compare("fixedGradient") == 0) {return i + j * (nx + 2);}
        else{return i + 1 + j * (nx + 2);}
    }
    else{
        std::cout<<"Error! Use 'south, north, west, east' to define direction!"<<"\n";
        return -1;
    }
}