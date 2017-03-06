//AddDSCommand.cpp
#include "Constant.h"
#include "Command/AddDSCommand.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/Person.h"
#include "boost/shared_array.hpp"
#include <unistd.h>

AddDSCommand::AddDSCommand(int acceptedSocket)
: _acceptedSocket(acceptedSocket) {}

AddDSCommand::~AddDSCommand(){}


void AddDSCommand::execute(DataStructure* ds){
	if(ds == NULL)
		MLog::criticalLog("execute() in AddDsCommand.cpp\n ds is NULL\n");

	//Data 수신
	boost::shared_array<char> buf(new char[1024]);
	int nread = read(_acceptedSocket, buf.get(), BUF_SIZE);
	if(nread <= 0){
		MLog::criticalLog("execute() in AddDSCommand.cpp\n");
	}

	//DataStructure's add 호출
	ds->add(_acceptedSocket, buf);
}
