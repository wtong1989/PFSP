program : src/pfspinstance.h src/flowshop.cpp src/pfspinstance.cpp

	g++ -O3 -c ./src/flowshop.cpp -o src/flowshop.o
	g++ -O3 -c ./src/pfspinstance.cpp -o src/pfspinstance.o

	g++ -O3 -c ./src/localsearch.cpp -o src/localsearch.o
	g++ -O3 -c ./src/utils.cpp -o src/utils.o
	g++ -O3 -c ./src/construction.cpp -o src/construction.o
	g++ -O3 -c ./src/stochasticls.cpp -o src/stochasticls.o

	g++ -O3 src/flowshop.o src/pfspinstance.o src/localsearch.o src/utils.o src/construction.o src/stochasticls.o -o flowshopWCT

clean:
	rm src/*.o flowshopCWT
