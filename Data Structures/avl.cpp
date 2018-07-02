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

AVLNode* AVLTree::rotateleft(AVLNode*& ptr)
{
    AVLNode* child = ptr->right;
    ptr->right = child->left;
    child->left = ptr;
    fixheight(ptr);
    fixheight(child);
    return child;
}

AVLNode* AVLTree::rotateright(AVLNode*& ptr)
{
    AVLNode* child = ptr->left;
    ptr->left = child->right;
    child->right = ptr;
    fixheight(ptr);
    fixheight(child);
    return child;
}

AVLNode* AVLTree::balanceCase(AVLNode*& ptr)
{
    fixheight(ptr);
    if (hb(ptr) == 2) { //RightLeft or RightRight Case
        if (hb(ptr->right) < 0) //RightLeft
            ptr->right = rotateright(ptr->right); //Right Rotation of next node from critical
        return rotateleft(ptr); //In every case a left rotation will be done
    }
    if (hb(ptr) == -2) { //LeftRight or LeftLeft Case
        if (hb(ptr->left) > 0) //LeftRight
            ptr->left = rotateleft(ptr->left); //Left Rotation of next node from critical
        return rotateright(ptr); //In every case a right rotation will be done
    }
    return ptr; //Insertion didn't destroy balance
}

int AVLTree::Access(int x, AVLNode*& ptr)
{
    if (ptr == NULL) {
        std::cout << "ID not in AVL." << std::endl;
        return -1;
    }
    else {
        if (x > ptr->id)
            Access(x, ptr->right);
        else if (x < ptr->id)
            Access(x, ptr->left);
        else if ( x == ptr->id) {
            std::cout << "ID Found: " << ptr->id << std::endl;
            return ptr->pos;
        }
        else {
            std::cout << "ID not in AVL." << std::endl;
            return -1;
        }
    }
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

void AVLTree::preorder(AVLNode*& ptr)
{
    if (ptr == NULL) return;
    std::cout << (ptr->id) << "\t";
    preorder(ptr->left);
    preorder(ptr->right);
}
