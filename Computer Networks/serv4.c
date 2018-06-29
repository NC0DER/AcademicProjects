#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h> //Thread functions and mutexes
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "keyvalue.h"

#define BACKLOG 5
#define BUFFERSIZE 2055

volatile sig_atomic_t uninterrupted = 1;
void handle_cleanup(int signal_flag)
{
    uninterrupted = 0;
}

struct kvstore_node{
    char key[1025];
    char value[1025];
    struct kvstore_node *next_node;
};

struct kvstore_node* kvstore;
struct kvstore_node** ptr; //Pointer to reference the store
pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
/*
      The same mutex is used for both put() and get()
      In order to mutually exclude any thread, from calling put()/get() concurrently
      while another thread, also calls put()/get(), thus avoiding the race condition
      on this globally shared memory segment (key value store)
*/
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

int main(int argc, char *argv[]){
    int i = 1, sockfd = 0, new_sockfd = 0;
    char *port_number = NULL;
    pthread_t thread_id;
    pthread_attr_t thread_attr;
    struct sigaction sigint_action;
    struct addrinfo address_info, *server_info, *pntr;
    struct sockaddr_storage client_address; // connector's address information
    socklen_t address_size;

    struct kvstore_node* temp = NULL;
    kvstore = NULL;

    if(argc != 2){
        perror("Syntax: ./server <port_number> ");
        exit(EXIT_FAILURE);
    }

    port_number=strdup(argv[1]);

