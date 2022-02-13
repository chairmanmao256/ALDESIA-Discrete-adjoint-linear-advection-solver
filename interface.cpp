#include "Aldesia.h"

int main(int argc, char** argv)
{
    using namespace std;
    if(argc != 2){
        cout<<"Wrong argument number!\n";
        cout<<"Usage: ./main.out <inputFileName>\n";
    }

    std::string filename = argv[1];
    Aldesia solver(filename);

    solver.solvePrimal();

    solver.writePrimal();

    solver.solveDA("averageTempreture");
}