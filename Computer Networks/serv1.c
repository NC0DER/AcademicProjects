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

