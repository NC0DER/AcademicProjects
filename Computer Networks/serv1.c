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

int main(int argc, char *argv[]){
    int i = 1, j = 0, sockfd = 0, new_sockfd = 0;
    ssize_t numberof_bytes = 0;
    size_t size = 0;
    char *port_number = NULL;
    char *buf_token = NULL;
    char *returned = NULL;
    char *buffer = malloc(sizeof(*buffer) * BUFFERSIZE);
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
    sigint_action.sa_handler = handle_cleanup; //setting handler for Sigaction
    sigemptyset(&sigint_action.sa_mask); //garbage values on flags' memory cause seg_fault during sigaction.
    sigaction(SIGINT, &sigint_action, NULL); //equivalent to signal(SIGINT,handle_cleanup);
    sigint_action.sa_flags = SA_RESTART; //GNU 24.3.5 Flags for sigaction
    // Most of the time, SA_RESTART is a good value to use for this field.
    for( ; uninterrupted ; ){ //Run only if no interrupt is received
        int leftover = 0, escape = 0;
        address_size = sizeof(client_address);
        new_sockfd = accept(sockfd, (struct sockaddr *)&client_address, &address_size);
        if (new_sockfd == -1){
            if (errno == EINTR){ //Caught Interrupting Signal!! (Sigint/Sigkill)
                break; //escape the main loop, go to the cleanup section
            }else{ //Any other error (network related)
                perror("Accept: failed");
                continue;
            }
        }
        numberof_bytes = 1;
        /*
          When read returns 0, client has closed connection
          When read/write return -1, the connection has failed,
          So we continue; to the next iteration of accepting a client
        */
        for(;(numberof_bytes > 0);){
            //Treat requests here
            buf_token = malloc(sizeof(*buf_token)*(BUFFERSIZE));
            if(leftover == 0){
                memset(buffer, 0 , BUFFERSIZE); //clearing out the array
                do{ //Reading Block
                    int count = 0; //count '\0' occurences
                    memset(buf_token, 0 , BUFFERSIZE);
                    numberof_bytes = read(new_sockfd, buf_token, BUFFERSIZE);
                    if(numberof_bytes < 0){
                        perror("Failed to receive data from client: ");
                        //Cleanup of allocated memory
                        free(buf_token);
                        buf_token = NULL;
                        numberof_bytes = 0;
                        escape = 1;
                        close(new_sockfd); //Terminate the connection early
                        break; //step out of the reading loop
                    }
                    buffer[numberof_bytes - 1] = '\0'; //Terminating just in case of broken command
                    for(i = 0, j = 0; i < numberof_bytes; ++i,++j){
                        buffer[i] = buf_token[j];
                        if((buffer[i] == '\0') &&(count < 2)){
                            ++count;
                        }
                    }
                    if((buffer[i] == '\0') && (buffer[0] == 'g')){break;} //Read until the first '\0'
                    else if((buffer[i] == '\0') && (buffer[0] == 'p') && (count == 2)){break;} //or Read until the second '\0
                    else if((buffer[0] != 'g') && (buffer[0] != 'p')){break;} //Client did not followed protocol exit.
                }while(numberof_bytes != 0);
            }
            if(escape == 1) continue;
            //When leftover is found, the above reading method is skipped
            i = 0;
            if (buffer[0] == 'g'){
                int count = 0;
                memset(buf_token, 0, BUFFERSIZE);
                ++i; //index past 'g'
                for (j = 0; j < BUFFERSIZE ; ++i, ++j){
                    buf_token[j] = buffer[i]; //Assign first element after 'g'
                    if (buf_token[j] == '\0') break;
                }
                ++i; //after '\0'

                size = strlen(buf_token) + 1; //size + '\0'
                returned = get(buf_token);
			    if(returned == NULL){ //Not Found
				    numberof_bytes = writen(new_sockfd, "n", 1); //string not found, only send "n\0", should be replaced by just 'n'
                    if(numberof_bytes < 0){
                        perror("Failed to send data to client: ");
                        //Cleanup of allocated memory
                        free(buf_token);
                        buf_token = NULL;
                        numberof_bytes = 0;
                        if(new_sockfd > 0){
                            if(new_sockfd > 0){
                                close(new_sockfd);  //Terminate the connection early
                                new_sockfd = -1;
                            }
                            new_sockfd = -1;
                        }
                        continue;
                    }
                }
                else{ //Found
                    size = strlen(returned) + 2; // 'f' +'\0'
                    char *freturned = malloc(size * sizeof(*freturned));
                    freturned[0] = 'f';
                    for(j = 0; j < size; ++j)
                        freturned[j+1] = returned[j];
                    numberof_bytes = writen(new_sockfd, freturned, size);
                    if(numberof_bytes < 0){
                        perror("Failed to send data to client: ");
                        //Cleanup of allocated memory
                        free(buf_token);
                        buf_token = NULL;
                        free(freturned);
                        freturned = NULL;
                        numberof_bytes = 0;
                        if(new_sockfd > 0){
                            close(new_sockfd);  //Terminate the connection early
                            new_sockfd = -1;
                        }
                        continue;
                    }
                    if(freturned != NULL){
                        free(freturned);
                        freturned = NULL;
                    }
                }
                if (i < BUFFERSIZE) {
                    if (buffer[i] == 'g') { //handle leftover on buffer
                        for (j = 0; ((i < BUFFERSIZE) && (j < BUFFERSIZE)); ++i, ++j)
                            buffer[j] = buffer[i];

                        leftover = 1; //set leftover to 1 thus skipping read method
                    } else if (buffer[i] == 'p') {
                        for (j = 0; ((i < BUFFERSIZE) && (j < BUFFERSIZE)); ++i, ++j)
                            buffer[j] = buffer[i];

                        leftover = 1; //set leftover to 1 thus skipping read method
                    } else leftover = 0; //allow read method to be executed
                }
            }
            else if (buffer[0] == 'p'){
                int size2 = 0;
                int count = 0;
                char* temp_str = NULL;
                memset(buf_token, 0, BUFFERSIZE); //Empty for first element
                ++i; //index past 'p'
                for (j = 0; ((i < BUFFERSIZE) && (j < BUFFERSIZE)) ; ++i, ++j){
                    buf_token[j] = buffer[i]; //Assign first element after 'p'
                    if (buf_token[j] == '\0') break;
                }
                ++i; //after '\0'

                size = strlen(buf_token) + 1; //size + '\0'
                temp_str = strdup(buf_token); //keep first element
                memset(buf_token, 0, BUFFERSIZE); //Empty for second element
                for (j = 0; ((i < BUFFERSIZE) && (j < BUFFERSIZE)) ; ++i, ++j){
                    buf_token[j] = buffer[i]; //Assign second element after first
                    if (buf_token[j] == '\0') break;
                }
                ++i; //after '\0'

                size2 = strlen(buf_token) + 1; //size + '\0'
                put(temp_str,buf_token); //No sends to client here
			    free(temp_str);

                if (i < BUFFERSIZE) {
                    if (buffer[i] == 'g') { //handle leftover on buffer
                        for (j = 0; ((i < BUFFERSIZE) && (j < BUFFERSIZE)); ++i, ++j)
                            buffer[j] = buffer[i];

                        leftover = 1; //set leftover to 1 thus skipping read method
                    } else if (buffer[i] == 'p') {
                        for (j = 0; ((i < BUFFERSIZE) && (j < BUFFERSIZE)); ++i, ++j)
                            buffer[j] = buffer[i];

                        leftover = 1; //set leftover to 1 thus skipping read method
                    } else leftover = 0; //allow read method to be executed
                }
            }
            else if ((buffer[0] != 'p') && (buffer[0] != 'g')){
                //Protocol Broken here
                free(buf_token);
                buf_token = NULL;
                numberof_bytes = 0;
                if(new_sockfd > 0){
                    close(new_sockfd);  //Terminate the connection early
                    new_sockfd = -1;
                }
                continue;
            }
            if(buf_token != NULL){
                free(buf_token);
                buf_token = NULL;
            } //Release temporary memory used for each request commmand (buffer token)
        }
        if(new_sockfd > 0){
            close(new_sockfd);  //Succesfully close the connection
            new_sockfd = -1;
        }
    }
    //Cleanup of allocated memory
    if(port_number != NULL){free(port_number);}
    if(buffer != NULL){free(buffer);}
    if(buf_token != NULL){free(buf_token);}
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
    for( ptr = &kvstore; *ptr != NULL; ){
        temp = *ptr; //Hold the current node
        ptr = &(*ptr)->next_node; //Point to next node
        free(temp); //Release the current node
    }
    return 0;
}
