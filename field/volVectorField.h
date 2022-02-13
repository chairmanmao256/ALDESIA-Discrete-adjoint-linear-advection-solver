#ifndef VOLVECTORFIELD_H
#define VOLVECTORFIELD_H
#include <iostream>
#include <string>

class volVectorField
{
    private:
    /// the name of the scalar field
    std::string name;

    /// the dimension of the INTERNAL domain
    int nx, ny;

    /// the two dimensional array that stores the scalar field value
    double*** field = new double**[2];

    /// the initial value of the field in x direction
    double iniValueX;

    /// the initial value of the field in y direction
    double iniValueY;

    /// the boundary condition data in x direction
    double BCvalueX[4];

    /// the boundary condition data in y direction
    double BCvalueY[4];


    /// the list of string that stores the boundary information
    std::string BCtype[4];

    public:
    // Constructor
    volVectorField(int nx_, int ny_, std::string name_, std::string* BCtype_, double iniValueX_, double iniValueY_, double* BCvalueX_, double* BCvalueY_);

    // Destructor
    ~volVectorField();

    // Impose boundary condition after every update
    void correctBoundary();

    // get value, operator is overloaded, use object[componentI][index1][index2] 
    // to access (for both write and read) field[componentI][index1][index2]
    double** operator[](int index) {return field[index];}

};



#endif