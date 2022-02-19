all: volScalarField.o volVectorField.o mesh.o

volScalarField.o: volScalarField.cpp volScalarField.h
	g++ -c -fPIC $< -o $@

volVectorField.o: volVectorField.cpp volVectorField.h
	g++ -c -fPIC $< -o $@

mesh.o: mesh.cpp mesh.h
	g++ -c -fPIC $< -o $@

clean:
	rm *.o