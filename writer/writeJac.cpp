#include <codi.hpp>
#include <fstream>
#include <iomanip>
#include "../field/mesh.h"

using namespace std;

void writeJac(codi::Jacobian<double> jac, mesh& Mesh)
{
    // open the file
    ofstream outfile;
    outfile.open("dRdT.dat", ios::out);

    // write the jacobian data
    int nx = Mesh.getNx(), ny = Mesh.getNy();
    for (int i = 0; i < nx * ny; i++){
        for (int j = 0; j < nx * ny; j++){
            outfile<<fixed<<setprecision(5)<<jac(i,j)<<" ";
        }
        outfile<<"\n";
    }

    outfile.close();
}