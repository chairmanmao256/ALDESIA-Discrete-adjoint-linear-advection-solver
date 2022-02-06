#ifndef VOLSCALARFIELD_H
#define VOLSCALARFIELD_H
#include <string>
class volScalarField
{
    private:
    /// the name of the scalar field
    std::string name;

    /// the dimension of the INTERNAL domain
    int nx, ny;

    /// the two dimensional array that stores the scalar field value
    double** field = nullptr;

    /// the initial value of the field
    double iniValue;

    /// the boundary condition data
    double BCvalue[4];

    /// the list of string that stores the boundary information
    std::string BCtype[4];

    public:
    // Constructor
    volScalarField(int nx_, int ny_, std::string name_, std::string* BCtype_, double value_, double* BCvalue_);

    // Destructor
    ~volScalarField();

    // Impose boundary condition after every update
    void correctBoundary();

    // get value, operator is overloaded, use object[index1][index2] 
    // to access (for both write and read) field[index1][index2]
    double* operator[](int index) {return field[index];}

};


#endif