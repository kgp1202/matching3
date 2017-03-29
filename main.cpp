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
#include <utility>

void* thread_work(void* param){
        std::pair<CommandQueue*, DataStructure*>* thread_param =
                (std::pair<CommandQueue*, DataStructure*>*) param;

        CommandQueue* commandQueue = thread_param->first;
        DataStructure* ds = thread_param->second;


	boost::shared_ptr<Command> commandPtr;
	while(true){
		commandPtr = commandQueue->pop();
		if(commandPtr == NULL){
			MLog::writeLog("thread_work() in main.cpp\n commandPtr is NULL\n");

			continue;
		}		

		if(commandPtr.get() == NULL){
			MLog::writeLog("thread_work() in main.cpp\n");
			continue;
		}
		
		commandPtr->execute(ds);
	}
}

int main(){
	CommandQueue* commandQueue = new CommandQueue();
	DataStructure* ds = new DataStructure(commandQueue);
	

	//Worker Thread init
	std::pair<CommandQueue*, DataStructure*>* thread_param = 
		new std::pair<CommandQueue*, DataStructure*> (commandQueue, ds);
	pthread_t* pid = new pthread_t[THREAD_SIZE];
	for(int i = 0; i < THREAD_SIZE; i++){
		if(pthread_create(&pid[i], NULL, &thread_work, thread_param) < 0){
			MLog::criticalLog("main() in main.cpp\n");
		}
	}

/*
	//FOR DEBUG	
	int sock;
	int d;
	char* distance = new char[10];
	char* front = "{\"distance\":\"";
	char* back = "\"}\0";
	char* buf = new char[30];
	char input;
	while(true){
		printf("Add : A, CheckMDS : C, Recv : R, print : P\n");

		scanf("%c", &input);

		boost::shared_ptr<Command> commandPtr;
		switch(input){
		case 'A':	
			printf("Input socket : ");
			scanf("%d", &sock);
			printf("Input distance : ");
			scanf("%s", distance);
			
			memset(buf, 0, 30);
			
			memcpy(buf, front, strlen(front));
			memcpy(buf + strlen(buf), distance, strlen(distance));
			memcpy(buf + strlen(buf), back, strlen(back));
		
	
			commandPtr.reset(new AddDSCommand(sock, buf));
			commandQueue->push(commandPtr);
			break;
		case 'C':
			commandPtr.reset(new CheckMDSCommand());
			commandQueue->push(commandPtr);
			break;
		case 'R':
			printf("Input socket : ");
			scanf("%d", &socket);
			printf("Input distance : ");
			scanf("%d", &d);
		
			printf("%d %d\n", sock, d);
			commandPtr.reset(new RecvCommand(sock, d));
			commandQueue->push(commandPtr);
			break;
		case 'P':
			ds->print();
			break;	
		}
	}
*/	
	
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
	init_event.events = EPOLLIN | EPOLLET;
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
		printf("EPOLL\n");

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
						continue;
					}

					init_event.data.fd = accepted_sock;
					epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accepted_sock,
						&init_event);

					printf("%d\n", accepted_sock);

					//이 부분을 RecvCommand에 넣어야 한다.
					//@@@@@@@@@@@@@@@수정 필요@@@@@@@@@@@@@@@@
					//ADDDSCommand push
					boost::shared_ptr<Command> addCommandPtr
						(new AddDSCommand(accepted_sock));
					commandQueue->push(addCommandPtr);
				}
			}
			else
			{
				//RecvCommand 	DS에 등록된 정보를 변경
				boost::shared_ptr<Command> recvCommandPtr
					(new RecvCommand(events[i].data.fd));
				commandQueue->push(recvCommandPtr);
			}
		}
	}

	return 0;	
}
