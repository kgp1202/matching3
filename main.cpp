#include "Command/AddDSCommand.h"
#include "Command/CheckMDSCommand.h"
#include "Command/RecvCommand.h"
#include "Command/CommandQueue.h"
#include "DataStructure/DataStructure.h"
#include "DataStructure/Person.h"
#include "Constant.h"

#include <boost/shared_ptr.hpp>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

void* thread_work(void* input){
	printf("SS\n");
}

int main(){
	CommandQueue commandQueue = CommandQueue();
	DataStructure* ds = new DataStructure(&commandQueue);
	

	//Worker Thread init
	pthread_t* pid = new pthread_t[THREAD_SIZE];
	for(int i = 0; i < THREAD_SIZE; i++){
		if(pthread_create(&pid[i], NULL, &thread_work, NULL) < 0){
			MLog::criticalLog("main() in main.cpp\n");
		}
	}


	//server init	
	int serv_sock = socket(AF_INET, SOCK_STREAM, 0);

	{
		//set serv_sock NON_BLOCK
		int flags = fcntl(serv_sock, F_GETFL, 0);
		if(fcntl(serv_sock, F_SETFL, flags | O_NONBLOCK) < 0){
			MLog::criticalLog("main() in main.cpp\n");
		}
	}

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
	

	//epoll init
	int epoll_fd = epoll_create(CONCURRENT_MAX_NUMBER);
	struct epoll_event init_event;
	init_event.events = EPOLLIN;
	init_event.data.fd = serv_sock;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_sock, &init_event);


	//epoll
	struct epoll_event* events = (struct epoll_event*) malloc
				(sizeof(struct epoll_event) * CONCURRENT_MAX_NUMBER);
	int event_count, accepted_sock;
	int timeout = -1;
	while(true)
	{
		event_count = epoll_wait(epoll_fd, events, CONCURRENT_MAX_NUMBER, timeout);
		if(event_count == -1)
			continue;
		
		for(int i = 0; i < event_count; i++)
		{
			if(events[i].data.fd == serv_sock)
			{
				//accept ALL
				while(true){
					//serv_sock is NON_BLOCK
					accepted_sock = accept(serv_sock, NULL, NULL);
					if(accepted_sock < 0){
						break;
					}

					init_event.data.fd = accepted_sock;
					epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accepted_sock,
						&init_event);

					//ADDDSCommand push
					boost::shared_ptr<Command> addCommandPtr
						(new AddDSCommand(accepted_sock));
					commandQueue.push(addCommandPtr);
				}
			}
			else
			{
				//RecvCommand 	DS에 등록된 정보를 변경
				boost::shared_ptr<Command> recvCommandPtr
					(new RecvCommand(events[i].data.fd));
				commandQueue.push(recvCommandPtr);
			}
		}
	}

	return 0;	
}
