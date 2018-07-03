#include <iostream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstring>
#include <sstream>
#include "struct.h"
#include "avl.h"
#include <cstdlib>
#include <exception>

#ifdef __gnu_linux__
    #include <unistd.h>
#endif

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

void clear_screen();
void mpause();
int inputInt();
int str2int(std::string);
int copy2cstr(char * , const char * , int); //Buf_Size < strlen(destination) or else BUFFER OVERFLOW!!
void printChar(char[], int);
void printComp(Company * );
void swapComp(Company * , Company * );
void insort(Company * , int);
bool check_flags(std::ifstream * );
int BinarySearch(Company * , int, int, int);
int InterpolationSearch(Company * , int, int, int);

int max_id = 0;
int main(int argc, char * argv[]) {
    bool exit_flag = false;
    bool file_exists = false;

    std::string filename = "";
    AVLTree tree;
    AVLNode * root = NULL;

    Companies log;
    log.numberOfCompanies = 0;
    log.arr = NULL;

    std::cout << "Exiting...." << std::endl;
    if (log.arr) //free arr from previous paths of executions
    {
        free(log.arr -> employees);
        free(log.arr);
    }
    delete root;
    return 0;
}
