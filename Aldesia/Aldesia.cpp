#include "Aldesia.h"

Aldesia::Aldesia(string inputFile)
{
    // get the file name and initiate the dictionary
    ConfigMap test(inputFile);

    // parameters to set up the problem
    int nx, ny;
    double box[4],dx,dy,dpIni, UXIni, UYIni, nuIni, SIni;
    double BCvalue_[4];
    double BCvalueZeroGradient[4] = {0.0, 0.0, 0.0, 0.0};
    double BCvalueVelocityX_[4],BCvalueVelocityY_[4];
    std::string BCtype_[4];
    std::string BCtypeZeroGradient[4] = {"fixedGradient","fixedGradient","fixedGradient","fixedGradient"};
    std::string BCtypeVelocity_[4];
    std::string dependentScalar, diffusitivity, source;
    std::string velocity;
    std::string objName;

    // domain dimension
    nx = test.getInteger("dimension","nx",10);
    ny = test.getInteger("dimension","ny",10);
    box[0] = test.getFloat("bound","xmin",0.0);
    box[1] = test.getFloat("bound","xmax",1.0);
    box[2]= test.getFloat("bound","ymin",0.0);
    box[3] = test.getFloat("bound","ymax",1.0);

    // dependent scalar's boundary value
    BCvalue_[0] = test.getFloat("boundaryValue","southValue",0.0);
    BCvalue_[1] = test.getFloat("boundaryValue","northValue",0.0);
    BCvalue_[2] = test.getFloat("boundaryValue","westValue",0.0);
    BCvalue_[3] = test.getFloat("boundaryValue","eastValue",0.0);

    // velocity boundary value
    BCvalueVelocityX_[0] = test.getFloat("boundaryValueVelocityX","southValue",0.0);
    BCvalueVelocityX_[1] = test.getFloat("boundaryValueVelocityX","northValue",0.0);
    BCvalueVelocityX_[2] = test.getFloat("boundaryValueVelocityX","westValue",0.0);
    BCvalueVelocityX_[3] = test.getFloat("boundaryValueVelocityX","eastValue",0.0);
    BCvalueVelocityY_[0] = test.getFloat("boundaryValueVelocityY","southValue",0.0);
    BCvalueVelocityY_[1] = test.getFloat("boundaryValueVelocityY","northValue",0.0);
    BCvalueVelocityY_[2] = test.getFloat("boundaryValueVelocityY","westValue",0.0);
    BCvalueVelocityY_[3] = test.getFloat("boundaryValueVelocityY","eastValue",0.0);

    // fields' name
    dependentScalar = test.getString("fieldName","dependentScalar","T");
    velocity = test.getString("fieldName","velocity","U");
    diffusitivity = test.getString("fieldName","diffusitivity","nu");
    source = test.getString("fieldName","source","S");

    // initial S distribution
    boxXmin = test.getFloat("initialSDistribution", "boxXmin", 0.0);
    boxXmax = test.getFloat("initialSDistribution", "boxXmax", 0.0);
    boxYmin = test.getFloat("initialSDistribution", "boxYmin", 0.0);
    boxYmax = test.getFloat("initialSDistribution", "boxYmax", 0.0);

    // dependent scalar's boundary type
    BCtype_[0] = test.getString("boundaryType","south","fixedValue");
    BCtype_[1] = test.getString("boundaryType","north","fixedValue");
    BCtype_[2] = test.getString("boundaryType","west","fixedValue");
    BCtype_[3] = test.getString("boundaryType","east","fixedValue");

    // velocity's boundary type
    BCtypeVelocity_[0] = test.getString("boundaryTypeVelocity","south","fixedGradient");
    BCtypeVelocity_[1] = test.getString("boundaryTypeVelocity","north","fixedGradient");
    BCtypeVelocity_[2] = test.getString("boundaryTypeVelocity","west","fixedGradient");
    BCtypeVelocity_[3] = test.getString("boundaryTypeVelocity","east","fixedGradient");

    // fields' initial value
    dpIni = test.getFloat("fieldValue","dependentInitial",0.0);
    UXIni = test.getFloat("fieldValue","UX",0.0);
    UYIni = test.getFloat("fieldValue","UY",0.0);
    nuIni = test.getFloat("fieldValue","nu",0.05);
    SIni = test.getFloat("fieldValue","S",0.0);

    // linear equation solver parameters
    omega = test.getFloat("SolverParameters","omega",1.5);
    tol = test.getFloat("SolverParameters","tolerance",1e-4);
    maxIter = test.getInteger("SolverParameters","maxIter", 3000);

    // decide if we want to solve the adjoint
    if(test.getInteger("AdjointOnOrOff","solvePrimalOnly", 0) > 0) {solvePrimalOnly = true;}

    // decide if we want to write down the Jacobian and the gradient
    if(test.getInteger("WriteJacobianAndGradient","Jacobian",0) > 0) {isWriteJac = true;}
    if(test.getInteger("WriteJacobianAndGradient","gradient",0) > 0) {isWriteGrad = true;}

    // print the header
    printHeader();

    cout<<"+----------------------------------------------------------+"<<endl;
    cout<<"|         Aldesia is now reading the parameters!           |"<<endl;
    cout<<"+----------------------------------------------------------+"<<endl<<endl;

    // print user input data's info
    cout<<"* * * * * * * * * * Problem Description * * * * * * * * * * *\n";
    cout<<"nx: "<<nx<<", ny: "<<ny<<"\n";
    cout<<"Scalar's name:     "<<dependentScalar<<"\n";
    cout<<"BC type, South:    "<<BCtype_[0]<<"\n"
        <<"         North:    "<<BCtype_[1]<<"\n"
        <<"         East:     "<<BCtype_[2]<<"\n"
        <<"         West:     "<<BCtype_[3]<<"\n";
    cout<<"Scalar initial:    "<<dpIni<<"\n";
    cout<<"Velocity initial:  ("<<UXIni<<", "<<UYIni<<")\n";
    cout<<"Diffusitivity:     "<<nuIni<<"\n";
    cout<<"Uniform source:    "<<SIni<<"\n";
    cout<<"* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"<<"\n";

    cout<<"* * * * * * * * * * * * Solver Setting * * * * * * * * * * * *\n";
    cout<<"Relaxation of SOR: "<<omega<<"\n";
    cout<<"Absolute tolerance:"<<tol<<"\n";
    cout<<"Maximum iteration: "<<maxIter<<"\n"<<"\n";
    cout<<"* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"<<"\n";

    // Initialize the fields
    TPtr = new volScalarField(nx, ny, dependentScalar, BCtype_, dpIni, BCvalue_);
    nuPtr = new volScalarField(nx, ny, diffusitivity, BCtypeZeroGradient, nuIni, BCvalueZeroGradient);
    SPtr = new volScalarField(nx, ny, source, BCtypeZeroGradient, SIni, BCvalueZeroGradient);
    UPtr = new volVectorField(nx, ny, velocity, BCtypeVelocity_, UXIni, UYIni, BCvalueVelocityX_, BCvalueVelocityY_);
    MeshPtr = new mesh(nx, ny, box[0],box[1], box[2], box[3]);

    // set the objective function
    setObjMap();

    // initialize the gradient array
    grad = new double [nx * ny];
}

Aldesia::~Aldesia()
{
    delete TPtr;
    delete nuPtr;
    delete SPtr;
    delete UPtr;
    delete MeshPtr;
    delete [] grad;

    // delete *objFuncs one by one
    delete objMap["averageTempreture"];
    delete objMap["souceSum"];
}

void Aldesia::setObjMap()
{
    #include "createRef.h"

    objMap["averageTempreture"] = selector<objFuncAvgT>(T_, S_, nu_, U_, Mesh_);
    objMap["sourceSum"] = selector<objFuncSourceSum>(T_, S_, nu_, U_, Mesh_);
}

void Aldesia::setDesignVariable(int oneDimensionalIndex, double val)
{   
    // get the 2-d index corresponds to oneDimensionalIndex
    int i, j;
    i = oneDimensionalIndex % (*MeshPtr).getNx() + 1;
    j = (oneDimensionalIndex - i + 1) / (*MeshPtr).getNx() + 1;

    // set the design variable to val
    (*SPtr)[i][j] = val;
}

bool Aldesia::isInBox(int oneDimensionalIndex)
{
    // get the 2-d index corresponds to oneDimensionalIndex
    int i, j;
    i = oneDimensionalIndex % (*MeshPtr).getNx() + 1;
    j = (oneDimensionalIndex - i + 1) / (*MeshPtr).getNx() + 1;

    //cout<<"checking: ("<<(*MeshPtr).getCellX(i,j)<<", "<<(*MeshPtr).getCellY(i,j)<<")\n";

    // check if the cell is in the box
    if ((*MeshPtr).getCellX(i,j) <= boxXmax && (*MeshPtr).getCellX(i,j) >= boxXmin
      &&(*MeshPtr).getCellY(i,j) <= boxYmax && (*MeshPtr).getCellY(i,j) >= boxYmin)
    {
        return true;
    }
    else{
        return false;
    }
}

void Aldesia::solvePrimal()
{
    #include "createRef.h"
    
    cout<<"+----------------------------------------------------------+"<<endl;
    cout<<"|             Aldesia is now solving the PDE!              |"<<endl;
    cout<<"+----------------------------------------------------------+"<<endl<<endl;

    nSolvePrimals++;

    CDSolver(T_, nu_, U_, S_, Mesh_, omega, tol, maxIter);

    cout<<endl;
}

void Aldesia::solveDA(string objName)
{
    // compute the gradient of a scalar function using Discrete-Adjoint method
    if (solvePrimalOnly) {
        cout<<"Aldesia found that the user has specified 'solvePrimalOnly'\n"
            <<"in the input file, so the adjoint will not be solved.\n";
        return;
    }

    #include "createRef.h"

    cout<<"+----------------------------------------------------------+"<<endl;
    cout<<"|         Aldesia is now calculating the gradient!         |"<<endl;
    cout<<"+----------------------------------------------------------+"<<endl;
    cout<<"+----------------------------------------------------------+"<<endl;
    cout<<"|                 Objective function name:                 |"<<endl;
    cout<<"+----------------------------------------------------------+"<<endl;
    cout<<"|              "<<objName<<"               |"<<endl<<endl;

    // calculate the gradient of the function specified by the user
    objFuncs& obj_ = *objMap[objName];
    if(!obj_.getIsDVOnly())  {nSolveAdjoints++;}

    codi::Jacobian<double> gradJac = 
    solveAdjoint(T_, S_, nu_, U_, Mesh_, tol, omega, maxIter, obj_, isWriteJac);

    // convert the Jacobian to the standard array
    for (int i = 0; i < gradJac.getN(); i++){
        grad[i] = gradJac(0,i);
    }

    // write the gradient
    if(isWriteGrad){
        writeJac(gradJac,"gradient.dat");
    }

    cout<<endl;
}

double Aldesia::calcObj(string objName)
{
    // obj's value should be calculated first!
    (*objMap[objName]).calcObjVal();

    cout<<endl;

    return (*objMap[objName]).getObjVal();
}

void Aldesia::writePrimal()
{
    #include "createRef.h"
    string filename = to_string(nSolvePrimals)+".plt";
    writePlt(T_, nu_, S_, U_, Mesh_, filename);
}

void Aldesia::printHeader()
{
    cout<<"+----------------------------------------------------------+"<<endl;
    cout<<"|               Nice to see you! I'm Aldesia!              |"<<endl;
    cout<<"|                                                          |"<<endl;
    cout<<"|  I was born in AeroLab, School of Aerospace engineering, |"<<endl;
    cout<<"|                    Tsinghua University.                  |"<<endl;
    cout<<"|                                                          |"<<endl;
    cout<<"|I'm a toy program who helps the students get familiar with|"<<endl;
    cout<<"|             CFD and Discrete-Adjoint method!             |"<<endl;
    cout<<"|                                                          |"<<endl;
    cout<<"|                   Now, let's do our work!                |"<<endl;
    cout<<"+----------------------------------------------------------+"<<endl<<endl;
}