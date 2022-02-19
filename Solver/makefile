all: CDSolver.o

CDSolver.o: CDSolver.cpp CDSolver.h ../field/volScalarField.h ../field/volVectorField.h ../field/mesh.h
	g++ -c -fPIC $< -o $@

clean:
	rm *.o