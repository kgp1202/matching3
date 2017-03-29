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
: _socket(socket) {}

void RecvCommand::execute(DataStructure* ds){
	using boost::property_tree::ptree;
	using boost::property_tree::read_json;

	if(ds == NULL)
		MLog::criticalLog("execute() in RecvCommand.cpp\n ds is NULL\n");

	//FOR DEBUG
	if(_distance != 0){
		ds->changeDS(_socket, _distance);
		return;
	}

	//Data 수신
	boost::shared_array<char> buf(new char[BUF_SIZE]);
	int nread = read(_socket, buf.get(), BUF_SIZE);
	
	if(nread == 0){	//Client의 연결 종료
		ds->removeClient(_socket);
	}
	else if(nread < 0){
		MLog::criticalLog("execute() in RecvCommand.cpp\n nread is 0\n");
	}
	else{
		//데이터 입력 받음
		ptree tree;
		std::istringstream is(buf.get());
		read_json(is, tree);
		
		int distance = atoi(tree.get<std::string>("distance").c_str());

		//Person 객체 수정 
		//Person* changedPerson = new Person(_socket, distance);
		ds->changeDS(_socket, distance);
	}
}

//FOR DEBUG
RecvCommand::RecvCommand(int socket, int distance)
: _socket(socket), _distance(distance) {}
