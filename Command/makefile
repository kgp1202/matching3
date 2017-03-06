C=g++
CPPFLAGS=-I/usr/local/boost -I /home/kim/dev -I /home/kim/dev/matching3 -I /home/kim/dev/BasicModule
LIB = /home/kim/dev/BasicModule

AddDSCommand : AddDSCommand.cpp  
	g++ -g -c AddDSCommand.cpp $(CPPFLAGS)

commands : AddDSCommand.cpp CheckMDSCommand.cpp RecvCommand.cpp CommandQueue.cpp
	g++ -g -c AddDSCommand.cpp CheckMDSCommand.cpp RecvCommand.cpp CommandQueue.cpp $(CPPFLAGS)



main : main.cpp 

test : test.cpp $(LIB)/MLog.cpp


