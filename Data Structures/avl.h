#ifndef AVL_H
#define AVL_H
#include "AvL.h"
#include <string>
extern int max_id;
struct AVLNode{
    int id; //Elements are only Company Ids
    int pos; //absolute position in linear allocated record
    int height;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int x,int p)
    {
        id = x; left = right = NULL; height = 1; pos = p;
    }
};

#endif
