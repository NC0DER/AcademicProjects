#include <iostream>
#include "AvL.h"

int AVLTree::height(AVLNode* ptr)
{
    return ptr ? ptr->height : 0;
}

int AVLTree::hb(AVLNode* ptr) //Height Balance Factor
{
    return height(ptr->right) - height(ptr->left);
}

void AVLTree::fixheight(AVLNode* ptr)
{
    ptr->height = ((height(ptr->left) > height(ptr->right)) ? height(ptr->left) : height(ptr->right)) + 1;
}

AVLNode* AVLTree::findmin(AVLNode*& ptr)
{
    AVLNode* current = ptr;
    /* iterate left to find the mininum leaf */
    while (current->left != NULL)
        current = current->left;
    return current;
}

AVLNode* AVLTree::removemin(AVLNode*& ptr)
{
    if (ptr->left == 0)
        return ptr->right;
    ptr->left = removemin(ptr->left);
    return balanceCase(ptr);
}

int AVLTree::deletemin(AVLNode*& ptr)
{
    int temp = 0;
    if (ptr->left == NULL) {
        temp = ptr->id;
        ptr = ptr->right;
        return temp;
    }
    else {
        temp = deletemin(ptr->left);
        return temp;
    }
}

