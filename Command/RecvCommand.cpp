//RecvCommand.cpp
#include "Constant.h"
#include "Command/RecvCommand.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/Person.h"
#include "BasicModule/MLog.h"
#include "boost/shared_array.hpp"

#include <string.h>
#include <sstream>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

RecvCommand::RecvCommand(int socket)
: _socket(socket), _distance(0) {}

//데이터를 읽어서 이를 DS로 넘겨준다.
void RecvCommand::execute(DataStructure* ds){
	if(ds == NULL)
		MLog::criticalLog("execute() in RecvCommand.cpp\n ds is NULL\n");

	//FOR DEBUG
	if(_distance != 0){
		printf("?\n");
		ds->changeDS(_socket, _distance);
		return;
	}

	printf("%d : ", _socket);

	//Data 수신
	boost::shared_array<char> buf(new char[BUF_SIZE]);
	int nread = read(_socket, buf.get(), BUF_SIZE);
	
	if(nread == 0){	//Client의 연결 종료
		printf("Remove\n");
		ds->removeClient(_socket);
	}
	else if(nread < 0){
		MLog::criticalLog("execute() in RecvCommand.cpp\n nread is 0\n");
	}
	else{
		printf("Add\n");
		buf[nread] = '\0';
		ds->add(_socket, buf);
	}
}

//FOR DEBUG
RecvCommand::RecvCommand(int socket, int distance)
: _socket(socket), _distance(distance) {}
