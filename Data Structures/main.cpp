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

void mpause() {
    std::cout << "\n\tPress Enter <-' to continue..." << std::endl;
    std::cin.ignore(std::numeric_limits < std::streamsize > ::max(), '\n');
}

void clear_screen() {
    #ifdef _WIN32
        std::system("cls");
    #endif
    #ifdef __gnu_linux__
        std::system("clear");
    #endif
}

void printChar(char array[], int size) {
    for (int i = 0; i < size; ++i)
        if (array[i] >= 32 && array[i] <= 126) //a-z, A-Z, 0-9, space, symbols(,]~+
            std::cout << array[i];
    std::cout << std::endl;
}

void printComp(Company * arg) {
    std::cout << "\nCompany Name: " << arg -> title << "\nId: " << arg -> id << "\nSummary: " << arg -> summary << "\nNumber of employeers: " << arg -> numberOfEmployees << std::endl;
    for (int j = 0; j < arg -> numberOfEmployees; ++j)
        std::cout << "Employee " << j + 1 << " " << ((arg -> employees) + j) -> firstName << " " << ((arg -> employees) + j) -> lastName << std::endl;
}

void swapComp(Company * arg1, Company * arg2) {
    if ((arg1 == NULL) || (arg2 == NULL)) {
        std::cout << "\nAt least one Company is not initialized\n";
        return;
    }
    if (arg1 -> id == arg2 -> id) {
        std::cout << "\nCan't switch same Company\n";
        return;
    }
    Company temp; //Initialization & Allocation of intermediate
    temp.id = 1;
    temp.numberOfEmployees = 7;
    for (int l = 0; l < 10; ++l) {
        temp.summary[l] = '0' + l;
        temp.title[l] = '0' + l;
    }
    temp.summary[10] = '\0';
    temp.title[10] = '\0';
    temp.employees = (Employee * ) malloc(7 * sizeof(Employee));
    for (int l = 0; l < 7; ++l) {
        for (int k = 0; k < 10; ++k) {
            temp.employees[l].firstName[k] = '0' + k;
            temp.employees[l].lastName[k] = '0' + k;
        }
        temp.employees[l].firstName[10] = '\0';
        temp.employees[l].lastName[10] = '\0';
    }

    temp.id = arg1 -> id;
    temp.numberOfEmployees = arg1 -> numberOfEmployees;
    copy2cstr(temp.summary, arg1 -> summary, strlen(arg1 -> summary) + 1);
    temp.summary[strlen(arg1 -> summary) + 1] = '\0';
    copy2cstr(temp.title, arg1 -> title, strlen(arg1 -> title) + 1);
    temp.title[strlen(arg1 -> title) + 1] = '\0';
    for (int i = 0; i < arg1 -> numberOfEmployees; ++i) {
        copy2cstr(temp.employees[i].firstName, arg1 -> employees[i].firstName, strlen(arg1 -> employees[i].firstName) + 1);
        temp.employees[i].firstName[strlen(arg1 -> employees[i].firstName) + 1] = '\0';
        copy2cstr(temp.employees[i].lastName, arg1 -> employees[i].lastName, strlen(arg1 -> employees[i].lastName) + 1);
        temp.employees[i].lastName[strlen(arg1 -> employees[i].lastName) + 1] = '\0';
    }
    arg1 -> id = arg2 -> id;
    arg1 -> numberOfEmployees = arg2 -> numberOfEmployees;
    copy2cstr(arg1 -> summary, arg2 -> summary, strlen(arg2 -> summary) + 1);
    arg1 -> summary[strlen(arg2 -> summary) + 1] = '\0';
    copy2cstr(arg1 -> title, arg2 -> title, strlen(arg2 -> title) + 1);
    arg1 -> title[strlen(arg2 -> title) + 1] = '\0';
    for (int i = 0; i < arg2 -> numberOfEmployees; ++i) {
        copy2cstr(arg1 -> employees[i].firstName, arg2 -> employees[i].firstName, strlen(arg2 -> employees[i].firstName) + 1);
        arg1 -> employees[i].firstName[strlen(arg2 -> employees[i].firstName) + 1] = '\0';
        copy2cstr(arg1 -> employees[i].lastName, arg2 -> employees[i].lastName, strlen(arg2 -> employees[i].lastName) + 1);
        arg1 -> employees[i].lastName[strlen(arg2 -> employees[i].lastName) + 1] = '\0';
    }
    arg2 -> id = temp.id;
    arg2 -> numberOfEmployees = temp.numberOfEmployees;
    copy2cstr(arg2 -> summary, temp.summary, strlen(temp.summary) + 1);
    arg2 -> summary[strlen(temp.summary) + 1] = '\0';
    copy2cstr(arg2 -> title, temp.title, strlen(temp.title) + 1);
    arg2 -> title[strlen(temp.title) + 1] = '\0';
    for (int i = 0; i < temp.numberOfEmployees; ++i) {
        copy2cstr(arg2 -> employees[i].firstName, temp.employees[i].firstName, strlen(temp.employees[i].firstName) + 1);
        arg2 -> employees[i].firstName[strlen(temp.employees[i].firstName) + 1] = '\0';
        copy2cstr(arg2 -> employees[i].lastName, temp.employees[i].lastName, strlen(temp.employees[i].lastName) + 1);
        arg2 -> employees[i].lastName[strlen(temp.employees[i].lastName) + 1] = '\0';
    }
}

void insort(Company * arg, int size) //Company record is nearly sorted-> Ideal Case O(n) for insertion sort
    {
        int k = 0;
        int count = 1;

        for (int l = 1; l < size; ++l) {
            k = l;
            while (k > 0 && (arg[k].id < arg[k - 1].id)) {
                swapComp(arg + k, arg + k - 1);
                --k;
                --count;
            }
            ++count;
        }
        if (count == size) {
            std::cout << "\nRecord is already sorted.\n";
        } else std::cout << "\nNearly sorted Record was sorted.\n";
    }

int BinarySearch(Company * Arr, int x, int min, int max) {
    if (Arr == NULL) {
        std::cout << "\nRecord is empty\n";
        return -1;
    }
    int mid = 0;
    while (max >= min) {
        mid = min + ((max - min) / 2); //overflow protection from (max+min)/2

        if (Arr[mid].id > x) //Lower half of the array [min...mid]
        {
            max = mid - 1;
        } else if (Arr[mid].id < x) {
            min = mid + 1; //Upper half of the array [mid...max]
        } else if (Arr[mid].id == x)
            return mid;
    }
    return -1; //Key not found
}

int InterpolationSearch(Company * Arr, int x, int low, int high) {
    if ((high - low + 1) < 0 || !Arr) // size = high - low + 1
    {
        std::cout << "\nRecord is empty\n";
        return -1;
    }
    high = high - 1; //High has input total size
    int mid = 0;

    while (!(Arr[high].id == Arr[low].id || x < Arr[low].id || Arr[high].id < x)) {
        mid = low + (x - Arr[low].id) * ((high - low) / (Arr[high].id - Arr[low].id));
        if (Arr[mid].id < x){
            low = mid + 1;
        } else if (x < Arr[mid].id){
            high = mid - 1;
        } else return mid;
    }
    if (x == Arr[low].id) {
        return low;
    } else return -1;
}
