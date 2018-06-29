#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "keyvalue.h"

#define BACKLOG 5
#define BUFFERSIZE 2055

struct kvstore_node{
    char key[1025];
    char value[1025];
    struct kvstore_node *next_node;
};

struct kvstore_node* kvstore;
struct kvstore_node** ptr; //Pointer to reference the store

char *get(char *key){
    if(kvstore == NULL)
        return NULL; //store is empty, no element to return.
    for(ptr = &kvstore; *ptr != NULL; ptr = &(*ptr)->next_node){
        if(strncmp(key, (*ptr)->key, 1025) == 0) //Key found, return the value.
            return (*ptr)->value;
    }
    /*If for doesn't return, then there is no next node.
      No key has been found on record, and the record is finished. */
    return NULL;
}

void put(char *key, char *value){
    int found = 0;
    struct kvstore_node* new_node = NULL;

    for(ptr = &kvstore; *ptr != NULL; ptr = &(*ptr)->next_node){
        if(strncmp(key, (*ptr)->key, 1025) == 0){ //Key found, overwrite new value.
            memmove((*ptr)->value , value, 1025);
            (*ptr)->value[1024] = '\0'; //Null terminate for safety
            found = 1;
            break;
        }
    }
    if(found) return;
    /*If key is found, the call simply returns with no value(void put).
      Ptr currently points to last empty next_node.
      The new element is added to the end of the list. */
    new_node = malloc(1 * sizeof(*new_node));
    //After creating new node, assign the new value
    if(new_node == NULL){perror("Malloc of node failed: "); return;}
    memmove(new_node->key, key, 1025);
    new_node->key[1024] = '\0'; //Null terminate for safety
    memmove(new_node->value, value, 1025);
    new_node->value[1024] = '\0'; //Null terminate for safety
    new_node->next_node = NULL;
    *ptr = new_node;
    return; //New node successfully created return.
}

ssize_t writen(int fd, const void *vptr, size_t n){ //Wrapper function from Stevens book, page 78.
    size_t nleft;
    ssize_t nwriten;
    const char *ptr;
    ptr = vptr;
    nleft = n;
    while (nleft > 0){
        if ( ((nwriten = write(fd, ptr, nleft)) <= 0 ) ){
            if (errno == EINTR) nwriten = 0; /* and call write() again */
            else return -1; /* error */
            } 
        nleft -= nwriten;
        ptr += nwriten;
    }
    return n;
}

volatile sig_atomic_t uninterrupted = 1;
void handle_cleanup(int signal_flag)
{
    uninterrupted = 0;
}

