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

class AVLTree
{
    public:
        int Access(int, AVLNode*&);
        void change_pos(int, int, AVLNode*&);
        void Insert(int, int, AVLNode*&);
        AVLNode* remove(AVLNode*&, int);
        void empty(AVLNode*&);
        int height(AVLNode*);
        int hb(AVLNode*); //Height Balance Factor
        void fixheight(AVLNode*);
        AVLNode* rotateleft(AVLNode*& ptr);
        AVLNode* rotateright(AVLNode*& ptr);
        AVLNode* balanceCase(AVLNode*& ptr);
        AVLNode* findmin(AVLNode*& ptr);
        int deletemin(AVLNode*& ptr);
        AVLNode* removemin(AVLNode*& ptr);
        void preorder(AVLNode*& ptr);
};
#endif
