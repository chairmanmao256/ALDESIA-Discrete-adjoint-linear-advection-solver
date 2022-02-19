all: CDSolverResidual.o

CDSolverResidual.o: CDSolverResidual.cpp CDSolverResidual.h ResidualFunc.h ../field/volScalarField.h ../field/volVectorField.h ../field/mesh.h
	g++ -c -fPIC -I ../CoDiPackModule/include -std=c++11 -g CDSolverResidual.cpp -o CDSolverResidual.o

clean:
	rm *.o