#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int calculateHeight(Node * root){
    if (root == nullptr){
        return 0; 
    }
    else{
        int lHeight = calculateHeight(root -> left);
        int rHeight = calculateHeight(root -> right); 
        if(lHeight > rHeight){
            return (lHeight + 1);
        }
        else {
            return (rHeight+1);
        }
    }
    return 0;
}

bool equalPaths(Node * root)
{
    // Add your code below
     // Add your code below
    if(root == nullptr){
        return true;
    }

    Node *leftChild = root -> left;
    Node *rightChild = root -> right;

    //no children
    if(leftChild == nullptr && rightChild == nullptr){
        return true;
    }
    //yes left child
    else if(leftChild != nullptr && rightChild == nullptr){
        return equalPaths(leftChild);
    }
    //yes right child 
    else if(leftChild == nullptr && rightChild != nullptr){
        return equalPaths(rightChild); 
    }
    //yes both children
    else if(calculateHeight(leftChild) == calculateHeight(rightChild)){
        return true;
    }


    return false; 

}

