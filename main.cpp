#include "Command/AddDSCommand.h"
#include "Command/CheckMDSCommand.h"
#include "Command/RecvCommand.h"
#include "Command/CommandQueue.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/Person.h"
#include "Constant.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){
	//FOR DEBUG
	CommandQueue* cq = new CommandQueue();
	
	DataStructure ds(cq);

	char* msg = "{\"distance\":\"123\"}";
	boost::shared_array<char> sp(new char[30]);
	memcpy(sp.get(), msg, sizeof(msg));
	ds.add(4, sp);
	ds.add(5, sp);
	
	Person* pt = new Person(4, 245);
	ds.changeDS(pt);


	/*
	//BEFORE
	CommandQueue* commandQueue = new CommandQueue();
	
	int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1){
		MLog::criticalLog("main() in main.cpp\n socket\n");
	}

	struct sockaddr_in serv_addr;
	bzero((char*)&serv_addr, sizeof(serv_addr));
		
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT_NUMBER);

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
		MLog::criticalLog("main() in main.cpp\n bind\n");
	}

	listen(serv_sock, LISTEN_NUMBER);
	
	//epoll & accept


	*/


	return 0;	
}
