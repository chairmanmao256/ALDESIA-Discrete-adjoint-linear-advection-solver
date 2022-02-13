int nx, ny;
double box[4],dx,dy,dpIni, UXIni, UYIni, nuIni, SIni, omega, tol, maxIter;
double BCvalue_[4];
double BCvalueZeroGradient[4] = {0.0, 0.0, 0.0, 0.0};
double BCvalueVelocityX_[4],BCvalueVelocityY_[4];
std::string BCtype_[4];
std::string BCtypeZeroGradient[4] = {"fixedGradient","fixedGradient","fixedGradient","fixedGradient"};
std::string BCtypeVelocity_[4];
std::string dependentScalar, diffusitivity, source;
std::string velocity;
std::string objName;

nx = test.getInteger("dimension","nx",10);
ny = test.getInteger("dimension","ny",10);
box[0] = test.getFloat("bound","xmin",0.0);
box[1] = test.getFloat("bound","xmax",1.0);
box[2]= test.getFloat("bound","ymin",0.0);
box[3] = test.getFloat("bound","ymax",1.0);

BCvalue_[0] = test.getFloat("boundaryValue","southValue",0.0);
BCvalue_[1] = test.getFloat("boundaryValue","northValue",0.0);
BCvalue_[2] = test.getFloat("boundaryValue","westValue",0.0);
BCvalue_[3] = test.getFloat("boundaryValue","eastValue",0.0);

BCvalueVelocityX_[0] = test.getFloat("boundaryValueVelocityX","southValue",0.0);
BCvalueVelocityX_[1] = test.getFloat("boundaryValueVelocityX","northValue",0.0);
BCvalueVelocityX_[2] = test.getFloat("boundaryValueVelocityX","westValue",0.0);
BCvalueVelocityX_[3] = test.getFloat("boundaryValueVelocityX","eastValue",0.0);
BCvalueVelocityY_[0] = test.getFloat("boundaryValueVelocityY","southValue",0.0);
BCvalueVelocityY_[1] = test.getFloat("boundaryValueVelocityY","northValue",0.0);
BCvalueVelocityY_[2] = test.getFloat("boundaryValueVelocityY","westValue",0.0);
BCvalueVelocityY_[3] = test.getFloat("boundaryValueVelocityY","eastValue",0.0);

dependentScalar = test.getString("fieldName","dependentScalar","T");
velocity = test.getString("fieldName","velocity","U");
diffusitivity = test.getString("fieldName","diffusitivity","nu");
source = test.getString("fieldName","source","S");

BCtype_[0] = test.getString("boundaryType","south","fixedValue");
BCtype_[1] = test.getString("boundaryType","north","fixedValue");
BCtype_[2] = test.getString("boundaryType","west","fixedValue");
BCtype_[3] = test.getString("boundaryType","east","fixedValue");

BCtypeVelocity_[0] = test.getString("boundaryTypeVelocity","south","fixedGradient");
BCtypeVelocity_[1] = test.getString("boundaryTypeVelocity","north","fixedGradient");
BCtypeVelocity_[2] = test.getString("boundaryTypeVelocity","west","fixedGradient");
BCtypeVelocity_[3] = test.getString("boundaryTypeVelocity","east","fixedGradient");

dpIni = test.getFloat("fieldValue","dependentInitial",0.0);
UXIni = test.getFloat("fieldValue","UX",0.0);
UYIni = test.getFloat("fieldValue","UY",0.0);
nuIni = test.getFloat("fieldValue","nu",0.05);
SIni = test.getFloat("fieldValue","S",0.0);

omega = test.getFloat("SolverParameters","omega",1.5);
tol = test.getFloat("SolverParameters","tolerance",1e-4);
maxIter = test.getInteger("SolverParameters","maxIter", 3000);

objName = test.getString("ObjectiveFunction","name", "averageTempreture");