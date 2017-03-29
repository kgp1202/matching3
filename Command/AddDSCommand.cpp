//AddDSCommand.cpp
#include "Constant.h"
#include "Command/AddDSCommand.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/Person.h"
#include "boost/shared_array.hpp"
#include <unistd.h>
#include <string.h>

AddDSCommand::AddDSCommand(int acceptedSocket)
: _acceptedSocket(acceptedSocket) {}

AddDSCommand::~AddDSCommand(){}


void AddDSCommand::execute(DataStructure* ds){
	printf("Add\n");

	if(ds == NULL)
		MLog::criticalLog("execute() in AddDsCommand.cpp\n ds is NULL\n");

	//FOR DEBUG
	if(_buf != NULL){
		boost::shared_array<char> buf(new char[1024]);
		memcpy(buf.get(), _buf, strlen(_buf));
		buf[strlen(_buf)] = '\0';
		
		ds->add(_acceptedSocket, buf); 
		return;
	}

	//Data 수신
	boost::shared_array<char> buf(new char[1024]);
	int nread = read(_acceptedSocket, buf.get(), BUF_SIZE);
	if(nread <= 0){
		MLog::criticalLog("execute() in AddDSCommand.cpp\n");
	}
	buf[nread] = '\0';

	printf("at AddDSCommand : ");
	printf(" %s\n", buf.get());

	//DataStructure's add 호출
	ds->add(_acceptedSocket, buf);
}



//FOR DEBUG
AddDSCommand::AddDSCommand(int acceptedSocket, char* buf)
: _acceptedSocket(acceptedSocket), _buf(buf){
}
