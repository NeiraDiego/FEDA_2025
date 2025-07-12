all: main

main: main.cpp captura.cpp
	g++ -std=c++17 main.cpp captura.cpp -o main

clean:
	rm -f main

