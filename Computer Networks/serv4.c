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
