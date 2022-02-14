#include "Aldesia.h"

int main(int argc, char** argv)
{
    using namespace std;
    if(argc != 3){
        cout<<"Wrong argument number!\n";
        cout<<"Usage: ./main.out <inputFileName> <the number of design variables>\n";
    }

    // initialize Aldesia
    std::string filename = argv[1];
    Aldesia solver(filename);

    // initialize the DV vector
    int nDV = atoi(argv[2]);
    double* DVList = new double [nDV];
    for (int i = 0; i < nDV; i++) {DVList[i] = 0.0;}

    // set the design variables
    DVList[5] = 1.0; DVList[6] = 1.0; DVList[9] = 1.0; DVList[10] = 1.0;
    for (int i = 0; i < nDV; i++){
        solver.setDesignVariable(i, DVList[i]);
    }

    solver.solvePrimal();

    //double objVal = solver.calcObj("averageTempreture");
    double objVal = solver.calcObj("sourceSum");

    solver.writePrimal();

    //solver.solveDA("averageTempreture");
    solver.solveDA("sourceSum");

    delete [] DVList;
    return 0;
}