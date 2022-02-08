all: main.out

main.out: main.o ./field/volScalarField.o ./field/volVectorField.o ./config/INIReader.o ./config/ini.o ./config/ConfigMap.o ./field/mesh.o ./Solver/CDSolver.o ./writer/writePlt.o ./Residual/CDSolverResidual.o ./writer/writeJac.o ./ObjFuncs/averageT.o
	g++ $^ -o $@

main.o: main.cpp readValue.h ./config/ConfigMap.h ./field/volScalarField.h ./field/volVectorField.h ./field/mesh.h ./Solver/CDSolver.h ./writer/writePlt.h ./Residual/CDSolverResidual.h ./ObjFuncs/averageT.h
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g $< -o $@

clean:
	rm *.o *.out