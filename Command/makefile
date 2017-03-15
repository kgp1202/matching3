C=g++
CPPFLAGS=-I/usr/local/boost -I /home/kim/dev -I /home/kim/dev/matching3 -I /home/kim/dev/BasicModule
LIB = /home/kim/dev/BasicModule

Commands : AddDSCommand.cpp CheckMDSCommand.cpp RecvCommand.cpp CommandQueue.cpp
	g++ -g -c AddDSCommand.cpp CheckMDSCommand.cpp RecvCommand.cpp CommandQueue.cpp $(CPPFLAGS)

test : test.cpp CommandQueue.cpp
	g++ -o test test.cpp CommandQueue.cpp $(CPPFLAGS) -pthread


