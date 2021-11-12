#include <cstdio>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "set.h"

template<typename K>
class BstSet : public Set<K> {
 private:
  class Node {
   public:
    K key;
    Node * left;
    Node * right;
    Node(K k, Node * l_n, Node * r_n) : key(k), left(l_n), right(r_n) {}
  };
  Node * root;
  Node * copy_(Node * rhs) {
    if (!rhs) {
      return NULL;
    }
    Node * cur = new Node(rhs->key, NULL, NULL);
    cur->left = copy_(rhs->left);
    cur->right = copy_(rhs->right);
    return cur;
  }
  void delete_(Node * rhs) {
    if (!rhs)
      return;
    delete_(rhs->left);
    delete_(rhs->right);
    delete (rhs);
  }

 public:
  BstSet() : root(NULL){};
  BstSet(const BstSet & rhs) { root = copy_(rhs.root); }
  BstSet & operator=(const BstSet & rhs) {
    BstSet tem(rhs);
    std::swap(tem.root, root);
    return *this;
  }
  ~BstSet() { delete_(root); }
  virtual void add(const K & key) {
    if (!root) {
      root = new Node(key, NULL, NULL);
      return;
    }
    Node * cur = root;
    while (1) {
      if (key < cur->key) {
        if (!cur->left) {
          cur->left = new Node(key, NULL, NULL);
          break;
        }
        else
          cur = cur->left;
      }
      else if (key > cur->key) {
        if (!cur->right) {
          cur->right = new Node(key, NULL, NULL);
          break;
        }
        else
          cur = cur->right;
      }
      else {
        break;
      }
    }
  }
  bool contains(const K & key) const {
    Node * cur = root;
    while (1) {
      if (!cur) {
        return false;
      }
      if (key < cur->key) {
        cur = cur->left;
      }
      else if (key > cur->key) {
        cur = cur->right;
      }
      else {
        return true;
      }
    }
  }
  virtual void remove(const K & key) {
    Node ** cur = &root;
    while ((*cur) != NULL && (*cur)->key != key) {
      if (key > (*cur)->key) {
        cur = &(*cur)->right;
      }
      else {
        cur = &(*cur)->left;
      }
    }
    if ((*cur) == NULL)
      return;
    if ((*cur)->left == NULL) {
      Node * tmp = (*cur)->right;
      delete *cur;
      *cur = tmp;
    }
    else if ((*cur)->right == NULL) {
      Node * tmp = (*cur)->left;
      delete *cur;
      *cur = tmp;
    }
    else {
      Node ** tmp = &(*cur)->left;
      while ((*tmp)->right != NULL) {
        tmp = &(*tmp)->right;
      }
      (*cur)->key = (*tmp)->key;
      Node * lT = (*tmp)->left;
      delete *tmp;
      *tmp = lT;
    }
  }
};
