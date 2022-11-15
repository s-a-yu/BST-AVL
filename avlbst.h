#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{
  
}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight(AVLNode<Key, Value> *n);
    void rotateLeft (AVLNode<Key, Value> *n); 
    void insertFix(AVLNode<Key, Value> *p, AVLNode<Key, Value> *n);
    void removeFix(AVLNode<Key, Value> *n, int diff); 

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // TODO
  AVLNode<Key, Value> *exist = static_cast<AVLNode<Key, Value>*>(this->internalFind(new_item.first));

  //if empty tree set new_item as root, and set balance to 0
  if(this->root_ == nullptr){
    this->root_ = new AVLNode <Key, Value> (new_item.first, new_item.second, nullptr);
    static_cast<AVLNode<Key, Value>*>(this->root_) -> setBalance(0);
    return;
  }
  else if(exist != nullptr){
    exist -> setValue(new_item.second);
    return; 
  }
  else{
    
    AVLNode <Key, Value>* newNode = new AVLNode <Key, Value> (new_item.first, new_item.second, nullptr);
    AVLNode <Key, Value> *compareNode = static_cast<AVLNode<Key, Value>*>(this->root_); 

    newNode -> setBalance(0);
    newNode -> setRight(nullptr);
    newNode -> setLeft(nullptr);
  
    while(compareNode != nullptr){
      if(newNode -> getKey() > compareNode -> getKey()){
        //go to the right subtree
        if(compareNode -> getRight() != nullptr){
          compareNode = compareNode -> getRight();
        }
        else{
          //at an empty spot
          compareNode -> setRight(newNode);
          newNode -> setParent(compareNode);
          if((newNode -> getParent()) -> getBalance() == 1 || (newNode -> getParent()) -> getBalance() == -1){
            (newNode -> getParent()) -> setBalance(0);
            return;
          }
          else if((newNode -> getParent()) -> getBalance() == 0){
            //update b(p)
            (newNode ->getParent()) -> setBalance(1);
            //call insertFix
            insertFix(newNode->getParent(), newNode);
          }
          break; 
        }
      }
      else if(newNode -> getKey() < compareNode -> getKey()){
        //go to the left subtree
        
        if(compareNode -> getLeft() != nullptr){
          compareNode = compareNode -> getLeft();
        }
        else{
          //at an empty spot
          compareNode -> setLeft(newNode);
          newNode -> setParent(compareNode);

          if((newNode -> getParent()) -> getBalance() == -1 || (newNode -> getParent()) -> getBalance() == 1){
            (newNode -> getParent()) -> setBalance(0);
            return;
          }
          else if((newNode -> getParent()) -> getBalance() == 0){
            //update b(p)
            (newNode ->getParent()) -> setBalance(-1);
            //call insertFix
            AVLNode<Key,Value>* parent = newNode->getParent();
            insertFix(parent, newNode);
          }
          break;
        }
      }
    }
  }

}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value> *p, AVLNode<Key, Value> *n){
  if(p == nullptr || p->getParent() == nullptr){
    return;
  }

  AVLNode<Key, Value> *g = p -> getParent();

  //if p is left child of g 
  if(g -> getLeft() == p){
    g -> updateBalance(-1);
    
    //case 1: b(g) == 0, return
    if(g -> getBalance() == 0){
      return;
    }
    //case 2: b(g) == -1, insertFix (g,p)
    else if(g -> getBalance() == -1){
      insertFix(g, p);
      //return;
    }
    //case 3: b(g) == -2
    else if(g->getBalance() == -2){
      //zig-zig
      if(n == p -> getLeft()){
        rotateRight(g);
        g -> setBalance(0);
        p -> setBalance(0);
      }
      //zig-zag
      else if(n == p -> getRight()){
        rotateLeft(p);
        rotateRight(g);
        if(n->getBalance() == -1){
          p -> setBalance(0);
          g -> setBalance(1);
          n -> setBalance(0);
        }
        else if(n->getBalance() == 0){
          p -> setBalance(0);
          g -> setBalance(0);
          n -> setBalance(0);
        }
        else if(n->getBalance() == 1){
          p -> setBalance(-1);
          g -> setBalance(0);
          n -> setBalance(0);
        }
      }
    }
  }
  //p is the right child of g
  else if(g -> getRight() == p){
    g -> updateBalance(1);

    //case 1: b(g) == 0, return
    if(g -> getBalance() == 0){
      return;
    }
    //case 2: b(g) == 1, insertFix (g,p)
    else if(g -> getBalance() == 1){
      insertFix(g, p);
      //return; 
    }
    //case 3: b(g) == 2
    else if(g->getBalance() == 2){
      //zig-zig
      if(n == p->getRight()){
        rotateLeft(g);
        g -> setBalance(0);
        p -> setBalance(0);
      }
      //zig-zag
      else if(n == p->getLeft()){
        rotateRight(p);
        rotateLeft(g);
        if(n->getBalance() == 1){
          p -> setBalance(0);
          g -> setBalance(-1);
          n -> setBalance(0);
        }
        else if(n->getBalance() == 0){
          p -> setBalance(0);
          g -> setBalance(0);
          n -> setBalance(0);
        }
        else if(n->getBalance() == -1){
          p -> setBalance(1);
          g -> setBalance(0);
          n -> setBalance(0);
        }
      }
    }
  }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value> *n){
  AVLNode <Key, Value> *c = n -> getLeft();
  AVLNode <Key, Value> *g = n -> getParent();
  bool cR = false;
  AVLNode<Key,Value> *cRight = nullptr; 

  if(c -> getRight() != nullptr){
    cRight = c -> getRight();
    cR = true;
  }

  //n is the root
  if(n == this -> root_){
    this->root_ = c;
    c -> setParent(nullptr);
  }
  else{
    c -> setParent(g); 
    if(n == g->getLeft()){
      //n is left child of g 
      g -> setLeft(c);
    }
    else if(n == g->getRight()){
      //n is right child of g 
      g -> setRight(c); 
    }
  }

  c -> setRight(n);
  n -> setParent(c);
  n -> setLeft(NULL); 
  if(cR == true){
    n -> setLeft(cRight); 
    cRight -> setParent(n); 
  }

}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value> *n){
  AVLNode <Key, Value> *c = n -> getRight();
  AVLNode <Key, Value> *g = n -> getParent();
  bool cL = false;
  AVLNode<Key,Value> *cLeft = nullptr; 
  
  if(c -> getLeft() != nullptr){
    cLeft = c -> getLeft();
    cL = true;
  }

  //n is the root
  if(n == this -> root_){
    this->root_ = c;
    c -> setParent(nullptr);
  }
  else{
    c -> setParent(g); 
    if(n == g->getLeft()){
      //n is left child of g 
      g -> setLeft(c);
    }
    else if(n == g->getRight()){
      //n is right child of g 
      g -> setRight(c); 
    }
  }

  c -> setLeft(n);
  n -> setParent(c);
  n -> setRight(NULL); 
   
  if(cL == true){
    cLeft->setParent(n);
    n -> setRight(cLeft); 
  }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
  //tree is empty
  if(this -> root_ == nullptr){
    return;
  }
  //there is only root node
  if((this->root_) -> getLeft() == nullptr && (this->root_) -> getRight() == nullptr && (this->root_) -> getKey() == key){
    delete this->root_;
    this->root_ = nullptr;
    return; 
  }

  //need to check if the node exists
  AVLNode <Key, Value> *find = static_cast<AVLNode<Key, Value>*>(this->internalFind(key)); 

  //node does not exist
  if(find == nullptr){
    return;
  }
  //otherwise node does exist

  //if 2 children swap with pred
  if(find -> getLeft() != nullptr && find -> getRight() != nullptr){
    AVLNode <Key, Value> *pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(find));
    nodeSwap(find, pred); 
  }
  
  int diff;
  //keep track of node's children and parent
  AVLNode <Key, Value> *parentNode = find -> getParent(); 
  AVLNode <Key, Value> *child = nullptr; 

  if(find -> getLeft() != nullptr){
    child = find -> getLeft(); 
  }
  else if(find -> getRight() != nullptr){
    child = find -> getRight(); 
  }
 
  if(parentNode == nullptr){
    //promote child to root
    this->root_ = child; 
  }
  else{
    if(parentNode -> getRight() == find){
      //find is right child
      parentNode -> setRight(child); 
      diff = -1; 
      
    }
    else if(parentNode -> getLeft() == find){
      //find is left child
      parentNode -> setLeft(child); 
      diff = 1;
    }
    //parentNode -> updateBalance(diff);
  }

  //change child to updated place
  if(child != nullptr){
    //child exists!
    child -> setParent(parentNode); 
  }
  find->setParent(nullptr);
  delete find; 
  //"patch tree" by calling removeFix(p, diff)

  removeFix(parentNode, diff); 

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value> *n, int diff){
  if(n == nullptr){
    return;
  }
  int ndiff;

  AVLNode<Key,Value> *p = n->getParent();
  if(p!=nullptr){
    if(p->getLeft() == n){
      //n is left child
      ndiff = 1;
    }
    else if(p->getRight() == n){
      //n is right child
      ndiff = -1;
    }
  }

  if(diff == -1){
    //case 1: b(n) + diff == -2
    if(n->getBalance() + diff == -2){
      AVLNode<Key,Value> *c = n -> getLeft();
      //case 1a: b(c) == -1 zig-zig case
      if(c->getBalance() == -1){
        rotateRight(n);
        n -> setBalance(0);
        c -> setBalance(0);
        removeFix(p, ndiff);
      }
      //case 1b: b(c) == 0 zig-zig case
      else if(c -> getBalance() == 0){
        rotateRight(n);
        n -> setBalance(-1);
        c -> setBalance(1);
      }
      //case 1c: b(c) == +1 zig-zag case
      else if(c -> getBalance() == 1){
        AVLNode<Key,Value> *g = c->getRight();
        rotateLeft(c);
        rotateRight(n);
        if(g -> getBalance() == 1){
          n -> setBalance(0);
          c -> setBalance(-1);
          g -> setBalance(0);
        }
        else if(g -> getBalance() == 0){
          n -> setBalance(0);
          c -> setBalance(0);
          g -> setBalance(0);
        }
        else if(g -> getBalance() == -1){
          n -> setBalance(1);
          c -> setBalance(0);
          g -> setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    //case 2: b(n) + diff == -1
    else if(n -> getBalance() + diff == -1){
      n -> setBalance(-1);
    }
    //case 3: b(n) + diff == 0
    else if(n -> getBalance() +diff == 0){
      n -> setBalance(0);
      removeFix(p, ndiff);
    }
    //**** mirrored cases ***************
    //mirrored case 1
    else if(n->getBalance() + diff == 2){
      AVLNode<Key,Value> *c = n -> getRight();

      //case 1a: b(c) == 1 zig-zig case
      if(c->getBalance() == 1){
        rotateLeft(n);
        n -> setBalance(0);
        c -> setBalance(0);
        removeFix(p, ndiff);
      }
      //case 1b: b(c) == 0 zig-zig case
      else if(c -> getBalance() == 0){
        rotateLeft(n);
        n -> setBalance(1);
        c -> setBalance(-1);

      }
      //case 1c: b(c) == -1 zig-zag case
      else if(c -> getBalance() == -1){
        AVLNode<Key,Value> *g = c->getRight();
        rotateRight(c);
        rotateLeft(n);
        if(g -> getBalance() == -1){
          n -> setBalance(0);
          c -> setBalance(1);
          g -> setBalance(0);
        }
        else if(g -> getBalance() == 0){
          n -> setBalance(0);
          c -> setBalance(0);
          g -> setBalance(0);
        }
        else if(g -> getBalance() == 1){
          n -> setBalance(-1);
          c -> setBalance(0);
          g -> setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    //mirrored case 2
    else if(n -> getBalance() + diff == 1){
      n -> setBalance(1);
    }
    //mirrored case 3
    else if(n -> getBalance() +diff == 0){
      n -> setBalance(0);
      removeFix(p, ndiff);
    }
  }
  //___________________________________________________
  else if(diff == 1){
    //case 1: b(n) + diff == 2
    if(n->getBalance() + diff == 2){
      AVLNode<Key,Value> *c = n -> getRight();
      //case 1a: b(c) == 1 zig-zig case
      if(c->getBalance() == 1){
        rotateLeft(n);
        n -> setBalance(0);
        c -> setBalance(0);
        removeFix(p, ndiff);
      }
      //case 1b: b(c) == 0 zig-zig case
      else if(c -> getBalance() == 0){
        rotateLeft(n);
        n -> setBalance(1);
        c -> setBalance(-1);
      }
      //case 1c: b(c) == -1 zig-zag case
      else if(c -> getBalance() == -1){
        AVLNode<Key,Value> *g = c->getLeft();
        rotateRight(c);
        rotateLeft(n);
        if(g -> getBalance() == -1){
          n -> setBalance(0);
          c -> setBalance(1);
          g -> setBalance(0);
        }
        else if(g -> getBalance() == 0){
          n -> setBalance(0);
          c -> setBalance(0);
          g -> setBalance(0);
        }
        else if(g -> getBalance() == 1){
          n -> setBalance(-1);
          c -> setBalance(0);
          g -> setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    //case 2: b(n) + diff == 1
    else if(n -> getBalance() + diff == 1){
      n -> setBalance(1);
    }
    //case 3: b(n) + diff == 0
    else if(n -> getBalance() +diff == 0){
      n -> setBalance(0);
      removeFix(p, ndiff);
    }
    //**** mirrored cases ***************
    //mirrored case 1
    else if(n->getBalance() + diff == -2){
      AVLNode<Key,Value> *c = n -> getRight();
      //case 1a: b(c) == -1 zig-zig case
      if(c->getBalance() == -1){
        rotateRight(n);
        n -> setBalance(0);
        c -> setBalance(0);
        removeFix(p, ndiff);
      }
      //case 1b: b(c) == 0 zig-zig case
      else if(c -> getBalance() == 0){
        rotateRight(n);
        n -> setBalance(-1);
        c -> setBalance(1);
      }
      //case 1c: b(c) == 1 zig-zag case
      else if(c -> getBalance() == 1){
        AVLNode<Key,Value> *g = c->getLeft();
        rotateLeft(c);
        rotateRight(n);
        if(g -> getBalance() == 1){
          n -> setBalance(0);
          c -> setBalance(-1);
          g -> setBalance(0);
        }
        else if(g -> getBalance() == 0){
          n -> setBalance(0);
          c -> setBalance(0);
          g -> setBalance(0);
        }
        else if(g -> getBalance() == -1){
          n -> setBalance(1);
          c -> setBalance(0);
          g -> setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    //mirrored case 2
    else if(n -> getBalance() + diff == -1){
      n -> setBalance(-1);
    }
    //mirrored case 3
    else if(n -> getBalance() + diff == 0){
      n -> setBalance(0);
      removeFix(p, ndiff);
    }
  }

}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
