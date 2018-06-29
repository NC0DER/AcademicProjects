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
    
    if (argc < 5) {
        perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
        exit(EXIT_FAILURE);
    }
    else if((argc < 6) && (strcmp(argv[3],"put") == 0)){ //call with only 1 get request 5 arguments in-total
        perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
        exit(EXIT_FAILURE);
    }
    /*
    Case 1: ... get \n  (No operand after last get request)
    Case 2: ... put \n  (No operand after last put request)
    Case 3: ... put <key> \n (No value operand after last put request)
    Case 4: ... <operand> Line 28: if
    (An operand that isn't get/put and doesn't belong to a request)              
    */
    if ((strcmp(argv[argc - 1],"get") == 0) || (strcmp(argv[argc - 1],"put") == 0) || (strcmp(argv[argc - 2],"put") == 0)){
        perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
        exit(EXIT_FAILURE);
    }
    if((strcmp(argv[argc - 3],"get") == 0) || (strcmp(argv[argc - 4],"put") == 0) ){ 
            perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
            exit(EXIT_FAILURE);
    }
    server_name = strdup(argv[1]);//Strdup allocates memory for the duplicated string
    port_number = strdup(argv[2]);

    //Client Initialization
    memset(&address_info,0,sizeof(address_info));
    address_info.ai_family = AF_UNSPEC;
    address_info.ai_socktype = SOCK_STREAM;
    address_info.ai_protocol = 0; //?
    if(getaddrinfo(server_name, port_number, &address_info, &server_info) != 0){ //returns nonzero on error
        perror("getaddrinfo failed: ");
        exit(EXIT_FAILURE);
    }
    //Loop through all the results and connect to the first socket possible
    for(ptr = server_info; ptr != NULL; ptr = ptr->ai_next){
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sockfd == -1){perror("Client Socket failed: "); continue;}
        if (connect(sockfd, ptr->ai_addr, ptr->ai_addrlen) == -1){close(sockfd); perror("Connect failed: "); continue;}
        break; //Connected successfully on a socket
    }
    if (ptr == NULL) {perror("Failed to connect to host: "); exit(EXIT_FAILURE);}

    //Traversing through all get/put requests
    for(i = 3; i < argc; i = i + 2){ //every request is at least 2 operands long
        if (strcmp(argv[i], "get") == 0){
            if ((strcmp(argv[i + 1], "get") == 0) || (strcmp(argv[i + 1], "put") == 0)){
				perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
				exit(EXIT_FAILURE);
            }
            else{
                buffer[0] = 'g';
                for (j = 1; j < BUFFERSIZE; ++j){
                    buffer[j] = argv[i + 1][j-1];
                    if(buffer[j] == '\0') break; //After null-termination exit
                }
                buffer[j] = '\0';
                numberof_bytes = writen(sockfd, buffer, (size_t) (j + 1));
                if(numberof_bytes < 0){
                    perror("Failed to send data to host: ");
                     //Cleanup of allocated memory
                    free(server_name);
                    free(port_number);
                    free(buffer);
                    //Cleanup of net structs/sockets
                    freeaddrinfo(server_info);
                    close(sockfd);
                    exit(EXIT_FAILURE);
                }
                memset(buffer, 0 , BUFFERSIZE);
                do{ //Reading Block
                    int count = 0; //count '\0' occurences
                    memset(buf_token, 0 , BUFFERSIZE);
                    numberof_bytes = read(sockfd, buf_token, BUFFERSIZE);
                    if(numberof_bytes < 0){
                        perror("Failed to receive data from client: ");
                        //Cleanup of allocated memory
                        free(port_number);
                        free(buffer);
                        free(buf_token);
                        //Cleanup of net structs/sockets
                        freeaddrinfo(server_info);
                        close(sockfd);
                        exit(EXIT_FAILURE);//EOF reached, server closed connection
                    }
                    for(k = 0, j = 0; k < numberof_bytes; ++k, ++j){
                        buffer[k] = buf_token[j];
                        if((buffer[k] == '\0') &&(count < 2)){
                            ++count;
                        }
                    }
                    if((buffer[k] == '\0') && (buffer[0] == 'f')){break;} //found: Read until the first '\0'
                    else if(buffer[0] == 'n'){break;} //not found: just break out of the reading loop
                    else if((buffer[0] != 'f') && (buffer[0] != 'n')){break;} //Server did not follow protocol exit.
                }while(numberof_bytes != 0);
                if(buffer[0] == 'f'){
                    printf("%s\n",buffer+1); //will print "<returned-key>\n" without 'f'
                }else if(buffer[0] == 'n'){
                    printf("\n");
                }
                else break; //go to cleanup then close connection and exit
            }
        }
        else if(strcmp(argv[i],"put") == 0){
            if((strcmp(argv[i+1],"get") == 0) || (strcmp(argv[i+1],"put") == 0)){
                    perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
                    exit(EXIT_FAILURE);
            }
            if((strcmp(argv[i+2],"get") == 0) || (strcmp(argv[i+2],"put") == 0)){
                    perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
                    exit(EXIT_FAILURE);
            }
            else{
                buffer[0] = 'p';
                for (j = 1; j < BUFFERSIZE; ++j){
                    buffer[j] = argv[i + 1][j-1];
                    if(buffer[j] == '\0') break;
                }
                buffer[j] = '\0'; //'p' + key + '\0'
                ++j;
				for (k = 0 ; j < BUFFERSIZE; ++j, ++k){
                    buffer[j] = argv[i + 2][k];
                    if(buffer[j] == '\0') break;
                }
                buffer[j] = '\0'; //'p' + key + '\0' + value + '\0'
                numberof_bytes = writen(sockfd, buffer, (size_t) (j + 1));
                if(numberof_bytes < 0){
                    perror("Failed to send data to host: ");
                     //Cleanup of allocated memory
                    free(server_name);
                    free(port_number);
                    free(buffer);
                    //Cleanup of net structs/sockets
                    freeaddrinfo(server_info);
                    close(sockfd);
                    exit(EXIT_FAILURE);
                }
                 ++i; //put request is 1 operand longer of get request
            }
        }  
        else{ //No get/put command
            perror("Syntax: ./client <server_name> <port_number> (put <key> <value> || get <key>)+ ");
            //Cleanup of allocated memory
            free(server_name);
            free(port_number);
            free(buffer);
            free(buf_token);
            //Cleanup of net structs/sockets
            freeaddrinfo(server_info);
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }    
    //Cleanup of allocated memory
    free(server_name);
    free(port_number);
    free(buffer);
    free(buf_token);
    //Cleanup of net structs/sockets
    freeaddrinfo(server_info);
    close(sockfd);
    return 0;
}
