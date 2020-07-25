#ifndef CONCERTATIONREALTIME_NETWORK_H
#define CONCERTATIONREALTIME_NETWORK_H

#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <cstring>
#include <fcntl.h>
#include <cstdlib>

bool udpSend(const char *msg, int length){
	sockaddr_in servaddr{};
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0){
		perror("cannot open socket");
		return false;
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("192.168.45.105");
	servaddr.sin_port = htons(4210);
	if (sendto(fd, msg, length, 0, (sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		perror("cannot send message");
		close(fd);
		return false;
	}
	close(fd);
	return true;
}

#endif //CONCERTATIONREALTIME_NETWORK_H
