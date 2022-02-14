all: DerivObj.o objFuncs.o objFuncAvgT.o objFuncSourceSum.o

DerivObj.o: DerivObj.cpp DerivObj.h
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g DerivObj.cpp -o DerivObj.o

objFuncs.o: objFuncs.cpp objFuncs.h
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g objFuncs.cpp -o objFuncs.o

objFuncAvgT.o: objFuncAvgT.cpp objFuncAvgT.h objFuncs.h
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g objFuncAvgT.cpp -o objFuncAvgT.o

objFuncSourceSum.o: objFuncSourceSum.cpp objFuncSourceSum.h objFuncs.h
	g++ -c -I ~/coding/AD_learn/CoDiPack/include -std=c++11 -g objFuncSourceSum.cpp -o objFuncSourceSum.o

clean:
	rm *.o