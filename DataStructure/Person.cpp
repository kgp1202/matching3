//Person.cpp
#include "DataStructure/Person.h"
#include "BasicModule/MLog.h"
#include "Constant.h"


Person::Person(int socket, int distance)
: _socket(socket), _distance(distance), _waitAdvantage(0){}

Person::Person(int socket, int distance, int waitAdvantage)
: _socket(socket), _distance(distance), _waitAdvantage(waitAdvantage){}

Person::Person(boost::shared_ptr<Person> ptr)
: _socket(ptr->getSocket()), _distance(ptr->getDistance()), _waitAdvantage(0){}

//FOR DEBUG
#include <stdio.h>

void Person::print(){
	printf("socket : %d, distance : %d, waitAdvantage : %d\n", _socket, _distance,
		getPriority());
}
