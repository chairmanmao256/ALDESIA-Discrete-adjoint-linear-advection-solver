all: main.out

main.out: main.o ./field/volScalarField.o ./field/volVectorField.o ./field/mesh.o \
		./config/INIReader.o ./config/ini.o ./config/ConfigMap.o \
		./Solver/CDSolver.o \
		./Residual/CDSolverResidual.o \
		./writer/writePlt.o ./writer/writeJac.o \
		./ObjFuncs/objFuncs.o ./ObjFuncs/objFuncAvgT.o ./ObjFuncs/DerivObj.o \
		./AdjointSolver/AdjointSolver.o
	g++ $^ -o $@

main.o: main.cpp readValue.h \
		./config/ConfigMap.h \
		./field/volScalarField.h ./field/volVectorField.h ./field/mesh.h \
		./Solver/CDSolver.h \
		./Residual/CDSolverResidual.h\
		./writer/writePlt.h ./writer/writeJac.h \
		./ObjFuncs/objFuncs.h ./ObjFuncs/objFuncAvgT.h ./ObjFuncs/DerivObj.h ./ObjFuncs/selector.h\
		./AdjointSolver/AdjointSolver.h
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g $< -o $@

clean:
	rm *.o *.out