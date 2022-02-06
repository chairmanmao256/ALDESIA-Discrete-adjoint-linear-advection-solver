#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "../field/volScalarField.h"
#include "../field/volVectorField.h"
#include "../field/mesh.h"
#include "writePlt.h"

// write the fields into a .plt file that can be read by Tecplot

using namespace std;

void writePlt(volScalarField& T, volScalarField& nu, volScalarField& S, volVectorField& U, mesh& Mesh)
{
    int nx = Mesh.getNx(), ny = Mesh.getNy();
    volVectorField center = Mesh.C();

    //open the file
    ofstream outfile;
    outfile.open("result.plt", ios::out);

    //write the header
    outfile<<"variables=x,y,u,v,T,nu,S\n";
    outfile<<" zone i="<<nx<<",j="<<ny<<",f=point\n";

    //write the data
    for (int j = 1; j <= ny; j++){
        for (int i = 1; i <= nx; i++){
            outfile<<fixed<<setprecision(5)<<center[0][i][j]<<"    "
            <<center[1][i][j]<<"    "<<U[0][i][j]<<"    "<<U[1][i][j]<<"    "
            <<T[i][j]<<"    "<<nu[i][j]<<"    "<<S[i][j]<<"\n";
        }
    }

    outfile.close();
}