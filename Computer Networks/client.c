#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFFERSIZE 2055

ssize_t writen(int fd, const void *vptr, size_t n){ //Wrapper function from Stevens book, page 78.
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    ptr = vptr;
    nleft = n;
    while (nleft > 0){
        if ( ((nwritten = write(fd, ptr, nleft)) <= 0 ) ){
            if (errno == EINTR) nwritten = 0; /* and call write() again */
            else return -1; /* error */
        } 
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

int main(int argc, char *argv[]){
    int i = 0, j = 0, k = 0, sockfd = 0;
    ssize_t numberof_bytes = 0;
    char *server_name = NULL;
    char *port_number = NULL;
    char *buffer = malloc(sizeof(*buffer) * BUFFERSIZE);
    char *buf_token = malloc(sizeof(*buf_token) * BUFFERSIZE);
    struct addrinfo address_info, *server_info, *ptr;
    
    if (argc < 5) {
        perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
        exit(EXIT_FAILURE);
    }
    else if((argc < 6) && (strcmp(argv[3],"put") == 0)){ //call with only 1 get request 5 arguments in-total
        perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
        exit(EXIT_FAILURE);
    }
    /*
    Case 1: ... get \n  (No operand after last get request)
    Case 2: ... put \n  (No operand after last put request)
    Case 3: ... put <key> \n (No value operand after last put request)
    Case 4: ... <operand> Line 28: if
    (An operand that isn't get/put and doesn't belong to a request)              
    */
    if ((strcmp(argv[argc - 1],"get") == 0) || (strcmp(argv[argc - 1],"put") == 0) || (strcmp(argv[argc - 2],"put") == 0)){
        perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
        exit(EXIT_FAILURE);
    }
    if((strcmp(argv[argc - 3],"get") == 0) || (strcmp(argv[argc - 4],"put") == 0) ){ 
            perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
            exit(EXIT_FAILURE);
    }
    server_name = strdup(argv[1]);//Strdup allocates memory for the duplicated string
    port_number = strdup(argv[2]);

    return 0;
}
