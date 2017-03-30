#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

int main(){
	int sock[1000];

	for(int i = 0; i < 10; i++){
		sock[i] = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in sock_addr;

		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		sock_addr.sin_port = htons(5001);

		connect(sock[i], (struct sockaddr*)&sock_addr, sizeof(sock_addr));

		char* msg = "{\"distance\":\"30\"}";

		write(sock[i], msg, strlen(msg));

//		char* buf = new char[100];
//		int nread = read(sock[i], buf, sizeof(buf));
//		buf[nread] = '\0';
	
		//printf("%s", buf);
	}

//	close(sock[0]);
	sleep(1000000);
	return 0;
}
