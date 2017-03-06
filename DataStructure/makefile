C=g++
CPPFLAGS=-I/usr/local/boost -I /home/kim/dev -I /home/kim/dev/matching3
LIB = /home/kim/dev/BasicModule

DataStructure : MiniDataStructure.cpp DataStructure.cpp MiniPerson.cpp Person.cpp
	g++ -g -c MiniDataStructure.cpp DataStructure.cpp MiniPerson.cpp Person.cpp  $(CPPFLAGS)

main : main.cpp	 

test : test.cpp $(LIB)/MLog.cpp


