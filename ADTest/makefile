AD.out: AD.o ../field/volScalarField.o ../field/volVectorField.o
	g++ $^ -o $@

AD.o: AD.cpp
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g AD.cpp -o AD.o