all: ConfigMap.o INIReader.o ini.o

ConfigMap.o: ConfigMap.cpp ConfigMap.h ./inih/INIReader.h
	g++ -c -fPIC $< -o $@

INIReader.o: ./inih/INIReader.cpp ./inih/INIReader.h ./inih/ini.h
	g++ -c -fPIC $< -o $@

ini.o: ./inih/ini.cpp ./inih/ini.h
	g++ -c -fPIC $< -o $@

clean: 
	rm *.o