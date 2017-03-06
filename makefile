C=g++
CPPFLAGS=-I/usr/local/boost -I /home/kim/dev -I /home/kim/dev/matching3
LIB = /home/kim/dev/BasicModule
OFs = DataStructure/DataStructure.o DataStructure/MiniDataStructure.o DataStructure/MiniPerson.o DataStructure/Person.o Command/AddDSCommand.o Command/CheckMDSCommand.o Command/CommandQueue.o
LIBOFs = $(LIB)/MLog.o

main : main.cpp $(OFs)
	g++ -c main.cpp $(CPPFLAGS)
	g++ -g -o main main.o $(OFs) $(CPPFLAGS) $(LIBOFs)

test : test.cpp $(LIB)/MLog.cpp


