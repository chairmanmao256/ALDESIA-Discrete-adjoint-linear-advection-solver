all: DerivObj.o objFuncs.o objFuncAvgT.o objFuncSourceSum.o objFuncTGradient.o objFuncTVariance.o

DerivObj.o: DerivObj.cpp DerivObj.h
	g++ -c -fPIC -I ../CoDiPackModule/include -std=c++11 -g DerivObj.cpp -o DerivObj.o

objFuncs.o: objFuncs.cpp objFuncs.h
	g++ -c -fPIC -I ../CoDiPackModule/include -std=c++11 -g objFuncs.cpp -o objFuncs.o

objFuncAvgT.o: objFuncAvgT.cpp objFuncAvgT.h objFuncs.h
	g++ -c -fPIC -I ../CoDiPackModule/include -std=c++11 -g objFuncAvgT.cpp -o objFuncAvgT.o

objFuncSourceSum.o: objFuncSourceSum.cpp objFuncSourceSum.h objFuncs.h
	g++ -c -fPIC -I ../CoDiPackModule/include -std=c++11 -g objFuncSourceSum.cpp -o objFuncSourceSum.o

objFuncTGradient.o: objFuncTGradient.cpp objFuncTGradient.h objFuncs.h
	g++ -c -fPIC -I ../CoDiPackModule/include -std=c++11 -g objFuncTGradient.cpp -o objFuncTGradient.o

objFuncTVariance.o: objFuncTVariance.cpp objFuncTVariance.h objFuncs.h
	g++ -c -fPIC -I ../CoDiPackModule/include -std=c++11 -g objFuncTVariance.cpp -o objFuncTVariance.o

clean:
	rm *.o