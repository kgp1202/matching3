C=g++
CPPFLAGS=-I/usr/local/boost -I /home/kim/dev -I /home/kim/dev/matching3
LIB = /home/kim/dev/BasicModule


test : test.cpp $(LIB)/MLog.cpp


