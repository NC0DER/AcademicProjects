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

    memset(&address_info, 0, sizeof(address_info));
    address_info.ai_family = AF_UNSPEC;
    address_info.ai_socktype = SOCK_STREAM;
    address_info.ai_flags = AI_PASSIVE; //uses the IP of this computer

    if (getaddrinfo(NULL, port_number, &address_info, &server_info) != 0) { //returns nonzero on error
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

    memset(&sigint_action, 0, sizeof(sigint_action));
    sigint_action.sa_handler = handle_cleanup; //setting handler for SIGINT
    sigemptyset(&sigint_action.sa_mask); //garbage values on flags' memory cause seg_fault during sigaction.
    if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
        perror("Action for Sigint failed: ");
        exit(EXIT_FAILURE);
    } //equivalent to signal(SIGINT,handle_cleanup);
    sigint_action.sa_flags = SA_RESTART;

    //GNU 24.3.5 Flags for sigaction
    //Most of the time, SA_RESTART is a good value to use for this field.

    memset(&sigchld_action, 0, sizeof(sigchld_action));
    sigchld_action.sa_handler = signal_child_handler; // reap all dead processes
    sigemptyset(&sigchld_action.sa_mask); //setting handler for SIGCHLD
    sigchld_action.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sigchld_action, NULL) == -1) {
        perror("Action for Sigchld failed: ");
        exit(EXIT_FAILURE);
    } //equivalent to signal(SIGCHLD,signal_child_handler);




    for(;uninterrupted;); //Wait for sigint here
    /*
      After sigint all child process are getting Sigint too
      Since they inherit the same signal handler from main()
      They cleanup and exit
    */

    for(i = 0; i < numberof_procs; ++i){
        kill(pid_table[i],SIGINT);
        kill(pid_table[i],SIGINT);//Second ctrl ^c for persistent processes
    }
    for(i = 0; i < numberof_procs; ++i)
        kill(pid_table[i], SIGKILL); //Finally Kill all Children

    while((pid = waitpid(-1, NULL, WNOHANG)) > 0); //Wait all Child processes to exit()
    if(port_number != NULL){free(port_number);}
    if(buffer != NULL){free(buffer);}
    if(buf_token != NULL){free(buf_token);}
    if(pid_table != NULL){free(pid_table);}
    //Cleanup of net structs/sockets
    freeaddrinfo(server_info);
    close(sockfd);
    if(new_sockfd > 0){
        close(new_sockfd);  //Close the connection in case of interrupt above
        new_sockfd = -1;
    }
    /*
        Cleanup of every store field and store itself.
        The content of the node is deleted, then the node itself.
        Deleting nodes from start to end.
    */
    if(shmdt(kvstore) == -1){ //Detaching from shared memory segment
        perror("Shmdt Failed: ");
        exit(EXIT_FAILURE);
    }
    shmctl(shared_mem_id, IPC_RMID, NULL); //Deleting the shared memory segment
    semctl(data_semaphore, 0, IPC_RMID); //Deleting the data semaphore
    semctl(connect_semaphore, 0, IPC_RMID); //Deleting the connect semaphore
    return 0;
}
