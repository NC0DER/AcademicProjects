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

