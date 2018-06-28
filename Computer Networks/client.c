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
    return 0;
}
