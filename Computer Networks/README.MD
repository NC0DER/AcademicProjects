# Computer Networks
I made this project for the Computer Networks lab in university, it is designed for Linux only.  
The teaching goal of this project was to grasp `TCP/IP` protocol communication throught net sockets.

## Project Architecture

### 1. Introduction
Each of the 4 servers:
* Implements a data structure called Key-Value Store, which stores in memory pairs of keys and their corresponding values. 
* Outputs descriptive error messages for memory / networking issues.
* Handles the `SIGINT` signal, to release all memory / network resources without leaks, and then exit normally.

### 2. Customized Protocol
Besides the typical `TCP/IP` communication through sockets, we were requested to implement a specific protocol,  
similar to `TCP/IP`, which is described below:  
The client sends `get` commands to request the data of the server, _(if any exists)_ in the key-value store.  
The server responds with a response consisting of a `found token` and the data themselves, or else with a `not-found token`. 
The client receives either the found token and prints the data or the not-found token and prints a newline character.  
The client sends `put` commands to insert / overwrite data of a specific key of the server's key value store.  
The server doesn't respond in that case.  
The client sends an erroneous command.  
The server doesn't respond and terminates the specific connection.  
The client can send a single `get` / `put` command.  
The client can send multiple `get` / `put` commands in any order in a single large request.  
The server will respond, whenever necessary, in the correct order and manner.  
The fields between the send / received character sequences are null character separated, as instructed by our professor.  

**2.1. Example Character Sequences**
```
                  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+  
Client:           | g | C | i | t | y |\0 | p | I | d |\0 | 2 | 2 | 3 |\0 |  
                  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+  
                    ^          ^            ^        ^          ^
                 get token    key        put token  key       value
                 
                  +---+---+---+---+---+---+---+---+                   +---+
Server:           | f | A | t | h | e | n | s |\0 |          or       | n |
                  +---+---+---+---+---+---+---+---+                   +---+
                    ^            ^                                      ^
                 found token   value                             not-found token
```

### 3. Servers
`serv1` is a serial execution server, iterating serially through all client requests.  
`serv2` is a parallel execution server, implementing the **one process per client** request model.  
`serv3` is a parallel execution server, implementing the **preforking process** request model.  
`serv4` is a parallel execution server, implementing the **one thread per client** request model.  

`serv2` & `serv3` implement signal handling to clean up the child process.  
`serv3` **preforking** means that a predefined number of worker processes only, treat each clients request.  

## Example Usage
```bash
# Server
$ ./serv1 4444 #port number
$ ./serv2 4445
$ ./serv3 4446 #port number & number of processes
$ ./serv4 4447

# Client (you can use either localhost or your servers name / ip address)
$ ./client localhost 4444 put city Athens
$ ./client localhost 4444 get city
Athens
$ ./client localhost 4444 put country Greece get country
Greece
$ ./client localhost 4444 get city put ZipCode 10557 get country
Athens
Greece
$ ./client localhost 4444 get city get unknown_key get ZipCode
Athens

10557
```

## Project Build
By using any terminal to navigate inside the project's directory,  
the project can be build, with the provided makefile, in the following way:
```bash
$ make all
```
or if you wish to build a specific server and the client:
```bash
$ make client
$ make serv1
```
The project's executables can be cleaned up with:
```bash
$ make clean
```
