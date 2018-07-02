#ifndef STRUCT_H
#define STRUCT_H
struct Employee{
    char firstName[56];
    char lastName[56];
};

struct Company{
    int id;
    char title[256];
    char summary[2048];
    int numberOfEmployees;
    Employee *employees;
};

struct Companies{
    Company *arr;
    int numberOfCompanies;
};

#endif
