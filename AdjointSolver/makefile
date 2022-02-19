all: AdjointSolver.o

AdjointSolver.o: AdjointSolver.cpp AdjointSolver.h
	g++ -c -fPIC -I ../CoDiPackModule/include -std=c++11 -g AdjointSolver.cpp -o AdjointSolver.o

clean:
	rm *.o