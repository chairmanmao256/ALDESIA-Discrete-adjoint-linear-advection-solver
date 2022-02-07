#include <codi.hpp>
#include <fstream>
#include <iomanip>
#include <string>
#include "../field/mesh.h"

using namespace std;

void writeJac(codi::Jacobian<double> jac, mesh& Mesh, string filename)
{
    // open the file
    ofstream outfile;
    outfile.open(filename, ios::out);

    // write the jacobian data
    int nx = Mesh.getNx(), ny = Mesh.getNy();
    for (int i = 0; i < nx * ny; i++){
        outfile<<"[";
        for (int j = 0; j < nx * ny; j++){
            if (j < nx*ny - 1){
                outfile<<fixed<<setprecision(5)<<jac(i,j)<<", ";
            }
            else{
                outfile<<fixed<<setprecision(5)<<jac(i,j)<<"],";
            }
        }
        outfile<<"\n";
    }

    outfile.close();
}