#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <string>
#include "volVectorField.h"
class mesh
{
    private:
    /// the dimension of the INTERNAL domain
    int nx, ny;

    /// the bound the domain
    double xmin, xmax, ymin, ymax;

    public:
    // Constructor
    mesh(int nx_, int ny_, double xmin_, double xmax_, double ymin_, double ymax_):
    nx(nx_),ny(ny_),xmin(xmin_),xmax(xmax_),ymin(ymin_),ymax(ymax_){}


    // Destructor
    ~mesh(){};

    // get cell-center's coordinate
    volVectorField C();

    // get the dimension of the INTERNAL domain
    int getNx() {return nx;}

    int getNy() {return ny;}

    // cell width and cell height
    double dx = (xmax - xmin)/double(nx);

    double dy = (ymax - ymin)/double(ny);

    // cell volume (area)
    double vol = (xmax - xmin)*(ymax - ymin)/double(nx*ny);
};


#endif