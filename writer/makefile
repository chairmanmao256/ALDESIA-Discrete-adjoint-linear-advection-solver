all: writePlt.o writeJac.o

writePlt.o: writePlt.cpp writePlt.h ../field/volScalarField.h ../field/volVectorField.h ../field/mesh.h
	g++ -c writePlt.cpp -o writePlt.o

writeJac.o: writeJac.cpp writeJac.h
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g writeJac.cpp -o writeJac.o

clean:
	rm *.o