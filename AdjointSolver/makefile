all: AdjointSolver.o

AdjointSolver.o: AdjointSolver.cpp AdjointSolver.h
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g AdjointSolver.cpp -o AdjointSolver.o

clean:
	rm *.o