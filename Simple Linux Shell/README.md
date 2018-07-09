# Simple Linux Shell
This project was part of my assignments in Operating Systems lab course in university, it is targeted for Linux only.  
The teaching goal of this exercise was to get accustomed with various linux system functions such as `fork()`, `exec()`, `chdir()`
by building a minified shell that supports the basic features of a regular shell _(e.g Bash)_ with **less than 300 LoC.**

## Features
- [x] Single command support
- [x] Multiple parameters support
- [x] Built-in commands support `cd`, `exit`
- [x] Pipe'd commands support

## Example Usage
```bash
$ pwd
$ cd ..
$ clear
$ exit
$ ls - l
$ ls -l | tail -n 10
$ ps -aux | grep ./myshell
```

## Compilation && Execution
```bash
$ gcc myshell.c -o myshell  
$ ./myshell
```
