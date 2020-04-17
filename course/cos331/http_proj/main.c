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
#define BYTES 5242880
char * docroot;
int rd;

void handle_connection(int acceptfd) {
	FILE *file = NULL;
    char buf[1024];
    long read;
    char *token[1024];
    char *msg, *fpath, *v;
    read = recv(acceptfd, buf, 1024, 0);
    buf[read] = '\0';
    msg = (char*)malloc(sizeof buf + 1);
    fpath = ' ';
    v = ' ';
    strncpy(msg, buf, sizeof buf);
    if (read < 0)
    {
        fprintf(stderr, ("recv() error\n"));
    }
    else if (!read)
    {
        fprintf(stderr, ("client disconnect\n"));
    }
    else
    {
        
        printf("%s", msg);
        token[0] = strtok(msg, " \t\n");        

        if ((strncmp(token[0], "GET\0", 4) == 0) || (strncmp(token[0], "HEAD\0", 5) == 0)) {
            
            token[1] = strtok (NULL, " \t");
            token[2] = strtok (NULL, " \t\n");

            if (strncmp(token[1], "HTTP/1.1", 8) != 0 && strncmp(token[2], "HTTP/1.1", 8) != 0) {
                write(acceptfd, "HTTP/1.1 400 Bad Requests\n", 25);
            }
            else { 
                
                strncpy(fpath, docroot, sizeof docroot);
                strncpy(&fpath[strlen(docroot)], token[1], sizeof token[1]);

                if ((file = open(fpath, O_RDONLY)) != -1 ) { //this means the file exists

                    write(acceptfd, "HTTP/1.1 200 OK\r\n", 17);
                    write(acceptfd, "Content-Type: ", 14);

                    char * extenstion = strrchr(fpath, '.');

                    if(strncmp(extenstion, ".jpg", 4) == 0){
                        write(acceptfd, "image/jpeg\r\n", 12);
                    }
                    
                    else if(strncmp(extenstion, ".png", 4) == 0){
                        write(acceptfd, "image/png\r\n", 11);
                    }

                    else if(strncmp(extenstion, ".gif", 4) == 0){
                        write(acceptfd, "image/gif\r\n", 11);
                    }

                    else if(strncmp(extenstion, ".pdf", 4) == 0){
                        write(acceptfd, "application/pdf\r\n", 17);
                    }

                    else if(strncmp(extenstion, ".js", 3) == 0){
                        write(acceptfd, "application/javascript\r\n", 24);
                    }

                    else if(strncmp(extenstion, ".html", 5) == 0){
                        write(acceptfd, "text/html\r\n", 11);
                    }

                    else if(strncmp(extenstion, ".txt", 4) == 0){
                        write(acceptfd, "text/plain\r\n", 12);
                    }

                    else if(strncmp(extenstion, ".css", 4) == 0){
                        write(acceptfd, "text/css\r\n", 10);
                    }

                    else {
                        write(acceptfd, "\r\n", 2);
                    }

                    //todo get dates

                    //todo content length

                    //while ((rd = read(file, buf, BYTES)) > 0){
                        //load file
                    //}


                }
            } 

        }
    }
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
