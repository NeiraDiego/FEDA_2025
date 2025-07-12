all: main

main: main.cpp captura.cpp
	g++ -std=c++17 main.cpp captura.cpp calculos.cpp salidas.cpp -o main.out

clean:
	rm -f main.out

