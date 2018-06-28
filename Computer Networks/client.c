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

