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

    // objective function's name
    // objName = test.getString("ObjectiveFunction","name", "averageTempreture");

    // decide if we want to write down the Jacobian and the gradient
    if(test.getInteger("WriteJacobianAndGradient","Jacobian",0) > 0) {isWriteJac = true;}
    if(test.getInteger("WriteJacobianAndGradient","gradient",0) > 0) {isWriteGrad = true;}

    // print user input data's info
    cout<<"* * * * * * Problem Description * * * * * *\n";
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
    cout<<"* * * * * * * * * * * * * * * * * * * * *\n"<<"\n";

    cout<<"* * * * * * * Solver Setting * * * * * * *\n";
    cout<<"Relaxation of SOR: "<<omega<<"\n";
    cout<<"Absolute tolerance:"<<tol<<"\n";
    cout<<"Maximum iteration: "<<maxIter<<"\n"<<"\n";

    // Initialize the fields
    TPtr = new volScalarField(nx, ny, dependentScalar, BCtype_, dpIni, BCvalue_);
    nuPtr = new volScalarField(nx, ny, diffusitivity, BCtypeZeroGradient, nuIni, BCvalueZeroGradient);
    SPtr = new volScalarField(nx, ny, source, BCtypeZeroGradient, SIni, BCvalueZeroGradient);
    UPtr = new volVectorField(nx, ny, velocity, BCtypeVelocity_, UXIni, UYIni, BCvalueVelocityX_, BCvalueVelocityY_);
    MeshPtr = new mesh(nx, ny, box[0],box[1], box[2], box[3]);


    for (int i = 1; i <= nx; i++){
        for (int j = 1; j <= ny; j++){
            // if(center[0][i][j] <= 0.6 && center[0][i][j] >= 0.4 && center[1][i][j] <= 0.6 && center[1][i][j] >= 0.4){
            //     S[i][j] = 1.0;
            // }
            if (i<=3 && i>=2 && j<=3 && j>=2){
                (*SPtr)[i][j] = 1.0;
            }
        }
    }

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
}

void Aldesia::setObjMap()
{
    #include "createRef.h"

    objMap["averageTempreture"] = selector<objFuncAvgT>(T_, S_, nu_, U_, Mesh_);
}

void Aldesia::solvePrimal()
{
    #include "createRef.h"

    CDSolver(T_, nu_, U_, S_, Mesh_, omega, tol, maxIter);
}

void Aldesia::solveDA(string objName)
{
    // compute the gradient of a scalar function using Discrete-Adjoint method


    #include "createRef.h"

    // calculate the gradient of the function specified by the user
    objFuncs& obj_ = *objMap[objName];
    codi::Jacobian<double> gradJac = solveAdjoint(T_, S_, nu_, U_, Mesh_, tol, omega, maxIter, obj_, isWriteJac);

    // convert the Jacobian to the standard array
    for (int i = 0; i < gradJac.getN(); i++){
        grad[i] = gradJac(0,i);
    }

    // write the gradient
    if(isWriteGrad){
        writeJac(gradJac,"gradient.dat");
    }
}

void Aldesia::writePrimal()
{
    #include "createRef.h"
    writePlt(T_, nu_, S_, U_, Mesh_);
}