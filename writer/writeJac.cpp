#include <codi.hpp>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

void writeJac(codi::Jacobian<double> jac, string filename)
{
    // open the file
    ofstream outfile;
    outfile.open(filename, ios::out);

    // write the jacobian data
    int M = jac.getM(), N = jac.getN();
    for (int i = 0; i < M; i++){
        outfile<<"[";
        for (int j = 0; j < N; j++){
            if (j < N - 1){
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