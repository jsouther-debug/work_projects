#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <time.h>


#define MYHOST "localhost"
#define MYPORT "8080"
#define MAXTHREADS 1000
#define BYTES 5242880


void handle_connection(int acceptfd) {
	FILE *file = NULL;
    char buf[1024];
    long read;
    char *msg, *fpath, *v;
    read = recv(acceptfd, buf, 1024, 0);
    buf[read] = '\0';
    msg = (char*)malloc(sizeof buf + 1);
    fpath = ' ';
    v = ' ';
    strncpy(msg, buf, sizeof buf);
    msg = strtok(msg, ' ');
    fpath = strtok(fpath, ' ');    
	v = strtok(v, '\r\n');
    printf("%s\n", msg);
    printf("%s\n", fpath);
    printf("%s\n", v);

    file = fopen(fpath, 'r');
    if (file == NULL){
        exit(0); //file not found
    }

    fclose(file);
}



int main(int argc, char **argv) {
    if (argc != 3){
        printf("error wrong number of arguments.\n");
        return -1;
    }

    char* fpath = argv[1];
    char* port = argv[2];

    printf("arg1: %s\n", fpath);
    printf("arg2: %s\n", port);

    int socket_desc;
    struct addrinfo hints;
    struct addrinfo *address_resource;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; //Streaming protocol(TCP)
    hints.ai_flags = AI_PASSIVE; //Listen

    getaddrinfo(MYHOST, port, &hints, &address_resource);

    socket_desc =    socket(address_resource->ai_family,
                            address_resource->ai_socktype,
                            address_resource->ai_protocol);

    int enable = 1;
    if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))){
        printf("ERROR: setsockopt failed\n");
        return 1;
    }


    int tmp_ret = bind(socket_desc, address_resource->ai_addr, address_resource->ai_addrlen);
    if (tmp_ret != 0){
        printf("ERROR: %s\n", strerror(errno));
        return 1;
    }

    freeaddrinfo(address_resource);
    listen(socket_desc, 10); //number is how many connections the operating system will spin up
    int accept_desc;

	struct sockaddr_storage remote_addr;
	socklen_t remote_addr_s = sizeof remote_addr;

	while(1) {
		accept_desc = accept(socket_desc, (struct sockaddr *) &remote_addr, &remote_addr_s);

		if (!fork()) {
			// child process
			handle_connection(accept_desc);
			close(accept_desc);

			exit(0);
		}

		// parent process
		close(accept_desc);
	}

	return 0;

}