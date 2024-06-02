CXX = g++
CXXFLAGS = -Wall -std=c++11

all: servidor cliente

servidor: src/servidor.cpp src/game.cpp
	$(CXX) $(CXXFLAGS) -o servidor src/servidor.cpp src/game.cpp

cliente: src/cliente.cpp src/game.cpp
	$(CXX) $(CXXFLAGS) -o cliente src/cliente.cpp src/game.cpp

clean:
	rm -f servidor cliente
