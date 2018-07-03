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

    switch (argc) {
    case 1:
        {
            std::cout << "No arguments specified. Running with default:data.csv" << std::endl;
            filename = "data.csv";
            break;
        }
    case 2:
        {
            if (strstr(argv[1], ".csv")) {
                file_exists = 1;
                filename = argv[1];
                std::cout << "File = " << filename << " has valid extension of .csv\n\n";
            } else {
                std::cout << "Inappropriate extension, correct usage: *.csv\n" << "Please run the app again with a valid argument!\n\n";
                return 0;
            }
            break;
        }
    default:
        {
            std::cout << "More than one arguments specified." << "Please run the app again with a valid argument!\n\n";
            return 0;
            break;
        }
    }
    mpause();
    std::ifstream readfile;
    std::ofstream writefile;

    std::cout << "Exiting...." << std::endl;
    if (log.arr) //free arr from previous paths of executions
    {
        free(log.arr -> employees);
        free(log.arr);
    }
    delete root;
    return 0;
}
