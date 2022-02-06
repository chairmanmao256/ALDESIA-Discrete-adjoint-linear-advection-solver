all: volScalarField.o volVectorField.o mesh.o

volScalarField.o: volScalarField.cpp volScalarField.h
	g++ -c $< -o $@

volVectorField.o: volVectorField.cpp volVectorField.h
	g++ -c $< -o $@

mesh.o: mesh.cpp mesh.h
	g++ -c $< -o $@

clean:
	rm *.o