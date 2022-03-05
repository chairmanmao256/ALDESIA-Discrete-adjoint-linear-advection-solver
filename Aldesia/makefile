all: interface.out

interface.out: interface.o \
		Aldesia.o\
		../field/volScalarField.o ../field/volVectorField.o ../field/mesh.o \
		../config/INIReader.o ../config/ini.o ../config/ConfigMap.o \
		../Solver/CDSolver.o \
		../Residual/CDSolverResidual.o \
		../writer/writePlt.o ../writer/writeJac.o \
		../ObjFuncs/objFuncs.o ../ObjFuncs/objFuncAvgT.o ../ObjFuncs/objFuncSourceSum.o ../ObjFuncs/objFuncTGradient.o ../ObjFuncs/DerivObj.o \
		../AdjointSolver/AdjointSolver.o
	g++ $^ -o $@


interface.o: interface.cpp
	g++ -c -I ../CoDiPackModule/include -std=c++11 -g $< -o $@

Aldesia.o: Aldesia.cpp createRef.h \
		../config/ConfigMap.h \
		../field/volScalarField.h ../field/volVectorField.h ../field/mesh.h \
		../Solver/CDSolver.h \
		../Residual/CDSolverResidual.h\
		../writer/writePlt.h ../writer/writeJac.h \
		../ObjFuncs/objFuncs.h ../ObjFuncs/objFuncAvgT.h ../ObjFuncs/objFuncSourceSum.h ../ObjFuncs/objFuncTGradient.h ../ObjFuncs/DerivObj.h ../ObjFuncs/selector.h\
		../AdjointSolver/AdjointSolver.h
	g++ -c -I ../CoDiPackModule/include -std=c++11 -g $< -o $@

clean:
	rm *.o *.out