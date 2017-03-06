//RecvCommand.cpp
#include "Constant.h"
#include "Command/RecvCommand.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/Person.h"
#include "BasicModule/MLog.h"
#include "boost/shared_array.hpp"

RecvCommand::RecvCommand(int socket)
: _socket(socket) {}

void RecvCommand::execute(DataStructure* ds){
	if(ds == NULL)
		MLog::criticalLog("execute() in RecvCommand.cpp\n ds is NULL\n");

	//Data 수신
	boost::shared_array<char> buf(new char[BUF_SIZE]);
	int nread = read(_socket, buf.get(), BUF_SIZE);
	
	if(nread <= 0){
		MLog::criticalLog("execute() in RecvCommand.cpp\n nread is 0\n");
	}

	//데이터 입력 받음@@@@@@@@@@@@@@@@@@@




	//Person 객체 수정 
	Person* changedPerson = new Person(_socket, distance);
	ds->changeDS(changedPerson);
}
