#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>  //System V semaphore
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "keyvalue.h"

#define BACKLOG 5
#define BUFFERSIZE 2055

void signal_child_handler(int signal_flag){
    while (waitpid(-1, NULL, WNOHANG) > 0); //Alternative parameter -1 instead of 0
}

volatile sig_atomic_t uninterrupted = 1;
void handle_cleanup(int signal_flag)
{
    uninterrupted = 0;
}

struct kvstore_node{
    char key[1025];
    char value[1025];
};

struct kvstore_node* kvstore = NULL;
size_t shared_mem_size;
int shared_mem_id, data_semaphore, connect_semaphore;
struct sembuf up;
struct sembuf down;

char *get(char *key){
    int i = 0;
    if(kvstore == NULL)
        return NULL; //kvstore is empty, no element to return.
    for(i = 0; i < 1000; ++i){
        if(kvstore[i].key[0] == '\0')
            break; //reached last node of current record which is empty
        else if(strncmp(key, kvstore[i].key, 1025) == 0) //Key found, return the value.
            return kvstore[i].value;
    }
    /*If for doesn't return, then there is no next node.
      No key has been found on record, and the record is finished. */
    return NULL;
}

void put(char *key, char *value){
    int i = 0, found = 0;
    for(i = 0; i < 1000; ++i){
        if(kvstore[i].key[0] == '\0')
            break; //reached last node of current record which is empty
        else if(strncmp(key, kvstore[i].key, 1025) == 0){ //Key found, overwrite new value.
            memmove(kvstore[i].value , value, 1025);
            kvstore[i].value[1024] = '\0'; //Null terminate for safety
            found = 1;
            break;
        }
    }
    if(found) return;
    /*If key is found, the call simply returns with no value(void put).
      i currently points to last empty next_node.
      The new element is added to the end of the list. */

    memmove(kvstore[i].key, key, 1025);
    kvstore[i].key[1024] = '\0'; //Null terminate for safety
    memmove(kvstore[i].value, value, 1025);
    kvstore[i].value[1024] = '\0'; //Null terminate for safety
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

int main(int argc, char *argv[]) {
    int i = 1, j = 0, numberof_procs = 0, sockfd = 0, new_sockfd = 0;
    ssize_t numberof_bytes = 0;
    size_t size = 0;
    pid_t *pid_table = NULL, pid;
    char *port_number = NULL;
    char *buf_token = NULL;
    char *returned = NULL;
    char *buffer = malloc(sizeof(*buffer) * BUFFERSIZE);
    struct sigaction sigint_action, sigchld_action;
    struct addrinfo address_info, *server_info, *pntr;
    struct sockaddr_storage client_address; // connector's address information
    socklen_t address_size;

    up.sem_num = 0; //only one semaphore used
    up.sem_op = 1; //semval +1 => semaphore up
    up.sem_flg = 0; //no special flags

    down.sem_num = 0; //only one semaphore used
    down.sem_op = -1; //semval -1 => semaphore down
    down.sem_flg = 0; //no special flags

    /*
      The same semaphore is used for both put() and get()
      In order to mutually exclude any child process, from calling put()/get() concurrently
      while another child process, also calls put()/get(), thus avoiding the race condition
      on this shared memory segment (key value kvstore)
   */
    if (argc != 3) {
        perror("Syntax: ./server <port_number> <number_of_processes>");
        free(buffer);
        exit(EXIT_FAILURE);
    }
    port_number = strdup(argv[1]);
    numberof_procs = atoi(argv[2]);
    if (numberof_procs <= 0) {
        perror("Syntax: number_of_processes should be a positive non-zero integer");
        exit(EXIT_FAILURE);
    }
    pid_table = malloc(numberof_procs * sizeof(*pid_table));
    data_semaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600); //Creating Semaphore for data handling
    if (data_semaphore == -1) {
        perror("Semget failed:");
        exit(EXIT_FAILURE);
    }
    connect_semaphore = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600); //Creating Semaphore for accept() call
    if (connect_semaphore == -1) {
        perror("Semget failed:");
        exit(EXIT_FAILURE);
    }
    if ((semctl(data_semaphore, 0, SETVAL, 1)) == -1) { //Test & Initialize the data semaphore to 1
        perror("Semctl failed:");
        exit(EXIT_FAILURE);
    }
    if ((semctl(connect_semaphore, 0, SETVAL, 1)) == -1) { //Test & Initialize the connect semaphore to 1
        perror("Semctl failed:");
        exit(EXIT_FAILURE);
    }
    shared_mem_size = (1000 * sizeof(*kvstore));

    //Creating and Attaching shared memory before creating parent and children processes
    if ((shared_mem_id = shmget(IPC_PRIVATE, shared_mem_size, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
        perror("Shmget Failed:");
        exit(EXIT_FAILURE);
    }
    if ((kvstore = shmat(shared_mem_id, NULL, 0)) == (struct kvstore_node *) -1) {
        perror("Shmat Failed:");
        exit(EXIT_FAILURE);
    }
    //Initializing node contents to '\0'
    for (i = 0; i < 1000; ++i) {
        memset(kvstore[i].key, 0, 1025);
        memset(kvstore[i].value, 0, 1025);
    }

