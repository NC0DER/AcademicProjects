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

    while (!exit_flag) {
        //Menu:
        clear_screen();
        std::cout << "\n\t\t ------------\n";
        std::cout << "\t\t|Menu Options|\n";
        std::cout << "\t\t ------------\n";

        std::cout << "\t1. Load companies from file\n";
        std::cout << "\t2. Save companies to file\n";
        std::cout << "\t3. Add a company\n";
        std::cout << "\t4. Delete a company by id\n";
        std::cout << "\t5. Display a company by id\n";
        std::cout << "\t6. Display companies\n";
        std::cout << "\t7. Display companies by surname search\n";
        std::cout << "\n\t 8.Exit Program\n" << std::endl;

        int choice = 0;
        std::cout << "Select a Menu Number, ";
        choice = inputInt();

        std::string line = "";
        line.reserve(2049);

        switch (choice) {
        case 1:
            {
                if (writefile.is_open()){
                    writefile.close();
                }
                readfile.open(filename.c_str(), std::ifstream:: in );
                readfile.seekg(0, readfile.beg);
                getline(readfile, line);
                log.numberOfCompanies = str2int(line);
                if (log.numberOfCompanies > 0) {
                    std::cout << "\n" << log.numberOfCompanies << " Companies have been found!\n";
                } else {
                    std::cout << "\n\nInvalid Amount of Companies, exiting.....\n";
                    mpause();
                    return 0;
                }
                if (log.arr) //free arr from previous paths of executions
                {
                    free(log.arr -> employees);
                    free(log.arr);
                    std::cout << "Deleting previous loaded record.\n" << std::endl;
                }
                log.arr = (Company * ) malloc(log.numberOfCompanies * sizeof(Company));
                if (log.arr) {
                    std::cout << "Allocating record.... \n\n";
                }
                for (int m = 0; m < log.numberOfCompanies; ++m) {
                    log.arr[m].id = 1;
                    log.arr[m].numberOfEmployees = 7;
                    for (int l = 0; l < 10; ++l) {
                        log.arr[m].summary[l] = '0' + l;
                        log.arr[m].title[l] = '0' + l;
                    }
                    log.arr[m].summary[10] = '\0';
                    log.arr[m].title[10] = '\0';
                    log.arr[m].employees = (Employee * ) malloc(7 * sizeof(Employee));
                    for (int l = 0; l < 7; ++l) {
                        for (int k = 0; k < 10; ++k) {
                            log.arr[m].employees[l].firstName[k] = '0' + k;
                            log.arr[m].employees[l].lastName[k] = '0' + k;
                        }
                        log.arr[m].employees[l].firstName[10] = '\0';
                        log.arr[m].employees[l].lastName[10] = '\0';
                    }

                }
                int i = 0;
                while ((i < log.numberOfCompanies) && !readfile.eof()) {
                    if (!getline(readfile, line, ';')) std::cout << "Failed";

                    log.arr[i].id = str2int(line);
                    std::cin.clear();
                    if (!getline(readfile, line, ';')) std::cout << "Failed";

                    copy2cstr(log.arr[i].title, (line.substr(0, 254)).c_str(), 255); //Convert String to Char* and save data

                    if (!getline(readfile, line, ';')) std::cout << "Failed";

                    copy2cstr(log.arr[i].summary, (line.substr(0, 2046)).c_str(), 2047);

                    if (!getline(readfile, line, ';')) std::cout << "Failed";
                    log.arr[i].numberOfEmployees = str2int(line);
                    std::cin.clear();

                    for (int j = 0; j < (log.arr[i].numberOfEmployees) - 1; ++j) //All except last one
                    {
                        if (!getline(readfile, line, ' ')) std::cout << "Failed";
                        copy2cstr(((log.arr[i].employees) + j) -> firstName, (line.substr(0, 54)).c_str(), 55);

                        if (!getline(readfile, line, ';')) std::cout << "Failed";
                        copy2cstr(((log.arr[i].employees) + j) -> lastName, (line.substr(0, 54)).c_str(), 55);
                    }

                    if (!getline(readfile, line, ' ')) std::cout << "Failed";
                    copy2cstr(((log.arr[i].employees) + (log.arr[i].numberOfEmployees - 1)) -> firstName, (line.substr(0, 54)).c_str(), 55);

                    int C = 0;
                    while ((readfile.peek() != ';') && (readfile.peek() != '\n')) {
                        log.arr[i].employees[log.arr[i].numberOfEmployees - 1].lastName[C] = readfile.get();
                        ++C;
                    }
                    if (readfile.peek() == ';' || readfile.peek() == '\n')
                        readfile.ignore(std::numeric_limits < std::streamsize > ::max(), '\n');
                    log.arr[i].employees[log.arr[i].numberOfEmployees - 1].lastName[C] = '\0';

                    //AVL record Creation
                    tree.Insert(log.arr[i].id, i, root);
                    ++i;
                }

                std::cout << "Loaded Companies from file" << std::endl;
                if (readfile.is_open())
                    readfile.close();
                mpause();
                for (int o = 0; o < log.numberOfCompanies; ++o) {
                    if (log.arr[o].id > max_id)
                        max_id = log.arr[o].id;
                }
                std::cout << "Sorting...\n";
                insort(log.arr, log.numberOfCompanies);
                mpause();
                break;
            }
        case 2:
            {
                if (!log.arr) {
                    std::cout << "\nThere is no record of companies loaded in memory.\nGoing Back in Menu...\n" << std::endl;
                    mpause();
                    break;
                }
                writefile.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
                std::string line = " ";
                std::stringstream temp;

                temp << log.numberOfCompanies;
                std::cin.clear();
                line = temp.str();
                temp.clear();
                temp.str("");
                writefile.write(line.c_str(), line.size());
                line = " ";
                writefile.put('\n');
                for (int l = 0; l < log.numberOfCompanies; ++l) {
                    temp << log.arr[l].id;
                    std::cin.clear();
                    line = temp.str();
                    temp.clear();
                    temp.str("");
                    writefile.write(line.c_str(), line.size());
                    line = " ";
                    writefile.put(';');
                    writefile.write(log.arr[l].title, strlen(log.arr[l].title));

                    writefile.put(';');
                    writefile.write(log.arr[l].summary, strlen(log.arr[l].summary));

                    writefile.put(';');
                    temp << log.arr[l].numberOfEmployees;
                    std::cin.clear();
                    line = temp.str();
                    temp.clear();
                    temp.str("");
                    writefile.write(line.c_str(), line.size());
                    line = " ";
                    writefile.put(';');
                    for (int k = 0; k < log.arr[l].numberOfEmployees - 1; ++k) {
                        writefile.write(log.arr[l].employees[k].firstName, strlen(log.arr[l].employees[k].firstName));
                        writefile.put(' ');
                        writefile.write(log.arr[l].employees[k].lastName, strlen(log.arr[l].employees[k].lastName));
                        writefile.put(';');
                    }
                    writefile.write(log.arr[l].employees[log.arr[l].numberOfEmployees - 1].firstName, strlen(log.arr[l].employees[log.arr[l].numberOfEmployees - 1].firstName));
                    writefile.put(' ');
                    writefile.write(log.arr[l].employees[log.arr[l].numberOfEmployees - 1].lastName, strlen(log.arr[l].employees[log.arr[l].numberOfEmployees - 1].lastName));
                    writefile.put('\n');
                }
                if (writefile.is_open())
                    writefile.close();
                std::cout << "\nSaved Record of all Companies to file" << std::endl;
                mpause();
                break;
            }
        case 3:
            {
                if (!(log.arr + 0)) {
                    std::cout << "\nThere is no record of companies loaded in memory.\nGoing Back in Menu...\n" << std::endl;
                    mpause();
                    break;
                }
                Companies temp;
                temp.arr = (Company * ) malloc((log.numberOfCompanies + 1) * sizeof(Company));
                if (temp.arr) {
                    std::cout << "Realloc Succesful\n\n";
                    for (int m = 0; m < log.numberOfCompanies; ++m) {
                        temp.arr[m].id = log.arr[m].id;
                        temp.arr[m].numberOfEmployees = log.arr[m].numberOfEmployees;
                        int l = 0;
                        for (l = 0; l < strlen(log.arr[m].summary) + 1; ++l) {
                            temp.arr[m].summary[l] = log.arr[m].summary[l];
                        }
                        for (l = 0; l < strlen(log.arr[m].title) + 1; ++l) {
                            temp.arr[m].title[l] = log.arr[m].title[l];
                        }
                        temp.arr[m].summary[strlen(log.arr[m].summary)] = '\0';
                        temp.arr[m].title[strlen(log.arr[m].title)] = '\0';
                        temp.arr[m].employees = (Employee * ) malloc(7 * sizeof(Employee));
                        for (l = 0; l < log.arr[m].numberOfEmployees; ++l) {
                            int k = 0;
                            for (k = 0; k < strlen(log.arr[m].employees[l].firstName) + 1; ++k) {
                                temp.arr[m].employees[l].firstName[k] = log.arr[m].employees[l].firstName[k];
                            }
                            for (k = 0; k < strlen(log.arr[m].employees[l].lastName) + 1; ++k) {
                                temp.arr[m].employees[l].lastName[k] = log.arr[m].employees[l].lastName[k];
                            }
                            temp.arr[m].employees[l].firstName[strlen(log.arr[m].employees[l].firstName)] = '\0';
                            temp.arr[m].employees[l].lastName[strlen(log.arr[m].employees[l].lastName)] = '\0';
                        }
                    }

                    ++log.numberOfCompanies;
                    temp.numberOfCompanies = (log.numberOfCompanies);
                    std::cout << "Initialization sucessful. New length of record = " << log.numberOfCompanies << std::endl;
                    free(log.arr -> employees);
                    free(log.arr);
                    log.arr = temp.arr;

                }

                log.arr[log.numberOfCompanies - 1].id = max_id + 1;
                ++max_id; //Update max_id
                //AVL record Add
                tree.Insert(log.arr[log.numberOfCompanies - 1].id, log.numberOfCompanies - 1, root);

                std::string tempst = " ";
                std::cout << "\nNew Company Name: ";
                getline(std::cin, tempst);
                copy2cstr(log.arr[log.numberOfCompanies - 1].title, (tempst.substr(0, 254).c_str()), 255);

                std::cout << "New Summary: ";
                getline(std::cin, tempst);
                copy2cstr(log.arr[log.numberOfCompanies - 1].summary, (tempst.substr(0, 2046).c_str()), 2047);

                std::cout << "New Number of Employees, ";
                int tempsi = 0;

                tempsi = inputInt();
                log.arr[log.numberOfCompanies - 1].numberOfEmployees = tempsi;
                log.arr[log.numberOfCompanies - 1].employees = (Employee * ) malloc(7 * sizeof(Employee));

                for (int l = 0; l < tempsi; ++l) {
                    std::cout << "\nEmployee " << l + 1 << " Name: ";
                    getline(std::cin, tempst);
                    copy2cstr(log.arr[log.numberOfCompanies - 1].employees[l].firstName, tempst.substr(0, tempst.size()).c_str(), tempst.size() + 1);
                    std::cout << "Employee " << l + 1 << " Surname: ";
                    getline(std::cin, tempst);
                    copy2cstr(log.arr[log.numberOfCompanies - 1].employees[l].lastName, tempst.substr(0, 54).c_str(), 55);

                }
                std::cout << "New Company created with the following info:" << std::endl;
                printComp(log.arr + log.numberOfCompanies - 1);
                mpause();
                break;
            }
        case 4:
            {
                if (!log.arr) {
                    std::cout << "\nThere is no record of companies loaded in memory.\nGoing Back in Menu...\n" << std::endl;
                    mpause();
                    break;
                }
                std::cout << "Deleting Company by id ->";
                int del = 0;
                del = inputInt();
                for (int l = 0; l < log.numberOfCompanies; ++l) //Linear Search Start
                {
                    if (del == log.arr[l].id) {
                        std::cout << "Company with id(" << log.arr[l].id << ") was found at " << l + 1 << "th access" << std::endl;
                        if (log.arr[l].id != log.arr[log.numberOfCompanies - 1].id) //If current != Last
                            swapComp(log.arr + l, log.arr + log.numberOfCompanies - 1); //Swap Current with Last one
                        printComp(log.arr + log.numberOfCompanies - 1);

                        std::cout << "Are you certain you want to delete this company from record?\n |(y)es / (n)o|";
                        std::string choice = " ";
                        getline(std::cin, choice);
                        if (choice == "yes" || choice == "(y)" || choice == "y") {
                            Companies temp;
                            temp.arr = (Company * ) malloc((log.numberOfCompanies - 1) * sizeof(Company));
                            if (temp.arr) {
                                std::cout << "\nRealloc Succesful\n\n";
                                for (int m = 0; m < log.numberOfCompanies - 1; ++m) {
                                    temp.arr[m].id = log.arr[m].id;
                                    temp.arr[m].numberOfEmployees = log.arr[m].numberOfEmployees;
                                    int l = 0;
                                    for (l = 0; l < strlen(log.arr[m].summary) + 1; ++l) {
                                        temp.arr[m].summary[l] = log.arr[m].summary[l];
                                    }
                                    for (l = 0; l < strlen(log.arr[m].title) + 1; ++l) {
                                        temp.arr[m].title[l] = log.arr[m].title[l];
                                    }
                                    temp.arr[m].summary[strlen(log.arr[m].summary)] = '\0';
                                    temp.arr[m].title[strlen(log.arr[m].title)] = '\0';
                                    temp.arr[m].employees = (Employee * ) malloc(7 * sizeof(Employee));
                                    for (l = 0; l < log.arr[m].numberOfEmployees; ++l) {
                                        int k = 0;
                                        for (k = 0; k < strlen(log.arr[m].employees[l].firstName) + 1; ++k) {
                                            temp.arr[m].employees[l].firstName[k] = log.arr[m].employees[l].firstName[k];
                                        }
                                        for (k = 0; k < strlen(log.arr[m].employees[l].lastName) + 1; ++k) {
                                            temp.arr[m].employees[l].lastName[k] = log.arr[m].employees[l].lastName[k];
                                        }
                                        temp.arr[m].employees[l].firstName[strlen(log.arr[m].employees[l].firstName)] = '\0';
                                        temp.arr[m].employees[l].lastName[strlen(log.arr[m].employees[l].lastName)] = '\0';
                                    }
                                }
                                --log.numberOfCompanies;
                                temp.numberOfCompanies = (log.numberOfCompanies);
                                //AVL Remove & Update Pos

                                root = tree.remove(root, log.arr[log.numberOfCompanies].id); //Delete current
                                tree.change_pos(log.arr[l].id, log.numberOfCompanies - 1, root); //Alter linear pos of last
                                std::cout << "Deletion successful. New length of record = " << log.numberOfCompanies << std::endl;
                                free(log.arr -> employees);
                                free(log.arr);
                                log.arr = temp.arr;
                                mpause();
                                std::cout << "Sorting...\n";
                                insort(log.arr, log.numberOfCompanies);
                                mpause();
                                goto Case4_End;
                            } //Linear Search end
                        } else goto Case4_End;
                    }
                }
                std::cout << "\nNo Company with the given id was found";
                mpause();
                Case4_End: break;
            }
        case 5:
            {
                if (!log.arr) {
                    std::cout << "\nThere is no record of companies loaded in memory.\nGoing Back in Menu...\n" << std::endl;
                    mpause();
                    break;
                }
                int subchoice = 0;
                int del = 0;
                int temp_pos = 0;
                do {
                    clear_screen();
                    std::cout << "----------------\n";
                    std::cout << "|Search Submenu|\n";
                    std::cout << "----------------\n";
                    std::cout << "\n\tChoose method: \n";
                    std::cout << "\t\t1. Linear Search\n";
                    std::cout << "\t\t2. AVL Access\n";
                    std::cout << "\t\t3. Binary Search\n";
                    std::cout << "\t\t4. Interpolation Search\n";
                    std::cout << "\t\t5. Exit" << std::endl;
                    subchoice = inputInt();
                } while (subchoice > 5);
                switch (subchoice) {
                case 1:
                    std::cout << "\nDisplaying Company by id ->";
                    del = inputInt();
                    for (int l = 0; l < log.numberOfCompanies; ++l) //Linear Search Start
                    {
                        if (del == log.arr[l].id) {
                            std::cout << "\nCompany was found at " << l + 1 << "th access\n";
                            printComp(log.arr + l);
                            mpause();
                            goto Case5_End;
                        }
                    }
                    std::cout << "\nNo Company with the given id was found";
                    mpause();
                    break;
                case 2:
                    std::cout << "\nAccessing Company by id ->";
                    del = inputInt();
                    temp_pos = tree.Access(del, root);
                    if (temp_pos >= 0) {
                        std::cout << "Company with the following info was found:" << std::endl;
                        printComp(log.arr + temp_pos);
                    } else std::cout << "\nNo Company with the given id was found";
                    mpause();
                    break;
                case 3:
                    std::cout << "Binary Searching by id ->";
                    del = inputInt();
                    temp_pos = BinarySearch(log.arr, del, 0, log.numberOfCompanies);
                    if (temp_pos == -1) {
                        std::cout << "\nNo Company with the given id was found";
                    } else {
                        std::cout << "Company with the following info was found:" << std::endl;
                        printComp(log.arr + temp_pos);
                    }
                    mpause();
                    break;
                case 4:
                    std::cout << "Interpolate Searching by id ->";
                    del = inputInt();
                    temp_pos = InterpolationSearch(log.arr, del, 0, log.numberOfCompanies);
                    if (temp_pos == -1) {
                        std::cout << "\nNo Company with the given id was found";
                    } else {
                        std::cout << "Company with the following info was found:" << std::endl;
                        printComp(log.arr + temp_pos);
                    }
                    mpause();
                    break;
                default:
                    break;
                }
                Case5_End: break;
            }
        case 6:
            {
                std::string temp = " ";
                if (log.arr == NULL) {
                    std::cout << "\nThere is no record of companies loaded in memory.\nGoing Back in Menu...\n" << std::endl;
                    mpause();
                    break;
                }
                for (int l = 0; l < log.numberOfCompanies; ++l) {
                    printComp(log.arr + l);
                    std::cout << "\nPress 0 to exit or enter to continue: ";
                    getline(std::cin, temp);
                    if (temp == "0")
                        goto Case6_End;
                    clear_screen();
                }
                Case6_End: break;
            }
        default:
            {
                break;
            }
        }
    }
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

int inputInt() //Modified for ints(+)!!
    {
        std::string in = " ";
        int returned = 0;
        std::cout << "Input a positive integer: ";
        while (true) {
            getline(std::cin, in );
            std::stringstream myStream( in );
            //Checking whether the stream can be safely assigned to int(thus is an int)
            if ((myStream >> returned) && (returned > 0))
                break;
            std::cin.clear(); //Clear Flags In case a control character has been caught
            std::cout << "\n" << "Invalid number, please try again" << std::endl << std::endl;
            std::cout << "Input a positive integer: ";
        }
        return returned;
    }

int str2int(std::string str) {
    int temp = 0;
    std::stringstream myStream(str);
    if ((myStream >> temp) && (temp > 0))
        return temp;
    else return -1;
}

int copy2cstr(char * destination,
    const char * source, int buf_size) {
    int i = 0;
    while ((i++ < buf_size) && ( * destination++ = * source++));
    if (i < buf_size) { * destination = 0;
        return i; //Current Position
    }
    return -1; //strlen(source)>buf_size
}

bool check_flags(std::ifstream * f) {
    if (f -> eof()) {
        std::cerr << "stream eofbit. error state";
        std::cout << "\n\t---End of File has been reached---\n";
        mpause();
        return false;
    }
    if (f -> fail()) {
        std::cerr << "stream failbit (or badbit). error state";
        std::cout << "\n\t---Error while failing to read/write (internal stream error)---\n";
        mpause();
        return false;
    }
    if (f -> bad()) {
        std::cerr << "stream badbit. error state";
        std::cout << "\n\t---Error while reading/writing data---\n";
        mpause();
        return false;
    }
    return true;
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
