#include "ConfigMap.h"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if(argc != 2){
        std::cout<<"Wrong argument number!\n";
        std::cout<<"Usage: ./main.out <inputFileName>\n";
    }

    std::string filename = argv[1];
    ConfigMap test(filename);
    
    double parameter = test.getFloat("float", "a1", 1.0);
    int nx = test.getInteger("dimension","nx",1);
    std::string southBCType = test.getString("boundaryType", "south", "fixedValue");

    std::cout<<"The parameter is: "<< parameter<<" .\n";
    std::cout<<"The nx is: "<< nx <<".\n";
    std::cout<<"The string is: "<<test.getString("boundaryType", "south", "fixedValue")<<". \n";
}