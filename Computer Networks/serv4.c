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

    memset(&address_info,0,sizeof(address_info));
    address_info.ai_family = AF_UNSPEC;
    address_info.ai_socktype = SOCK_STREAM;
    address_info.ai_flags = AI_PASSIVE; //uses the IP of this computer

    if(getaddrinfo(NULL, port_number, &address_info, &server_info) != 0){ //returns nonzero on error
        perror("getaddrinfo failed: ");
        exit(EXIT_FAILURE);
    }
    i = 1;
    //Loop through all the results and connect to the first socket possible
    for(pntr = server_info; pntr != NULL; pntr = pntr->ai_next){
        sockfd = socket(pntr->ai_family, pntr->ai_socktype, pntr->ai_protocol);
        if (sockfd == -1){perror("Server Socket failed: "); continue;}
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(i), sizeof(int)) == -1){perror("Setsockopt failed: "); exit(EXIT_FAILURE);}
        if (bind(sockfd, pntr->ai_addr, pntr->ai_addrlen) == -1){close(sockfd); perror("Bind failed: "); continue;}
        break;
    }
    if (pntr == NULL){perror("Failed to bind to client "); exit(EXIT_FAILURE);}
    if (listen(sockfd, BACKLOG) == -1){perror("Listen Failed: "); exit(EXIT_FAILURE);}

    memset(&sigint_action,0,sizeof(sigint_action));
    sigint_action.sa_handler = handle_cleanup; //setting handler for SIGINT
    sigemptyset(&sigint_action.sa_mask); //garbage values on flags' memory cause seg_fault during sigaction.
    if (sigaction(SIGINT, &sigint_action, NULL) == -1){

    } //equivalent to signal(SIGINT,handle_cleanup);
    sigint_action.sa_flags = SA_RESTART;

    //GNU 24.3.5 Flags for sigaction
    //Most of the time, SA_RESTART is a good value to use for this field.

    if(pthread_attr_init(&thread_attr) != 0){
        perror("Thread attribute initialization failed");
        exit(EXIT_FAILURE);
    }
    if(pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) != 0){
        perror("Thread set detached state failed");
        exit(EXIT_FAILURE);
    }
