#include <cstdio>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "map.h"

template<typename K, typename V>
class BstMap : public Map<K, V> {
 private:
  class Node {
   public:
    K key;
    V val;
    Node * left;
    Node * right;
    Node(K k, V v, Node * l_n, Node * r_n) : key(k), val(v), left(l_n), right(r_n) {}
  };
  Node * root;
  Node * copy_(Node * rhs) {
    if (!rhs)
      return NULL;
    Node * cur = new Node(rhs->key, rhs->val, NULL, NULL);
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
  BstMap() : root(NULL){};
  BstMap(const BstMap & rhs) { root = copy_(rhs.root); }
  BstMap & operator=(const BstMap & rhs) {
    BstMap tmp(rhs);
    std::swap(tmp.root, root);
    return *this;
  }
  ~BstMap() { delete_(root); }

  virtual void add(const K & key, const V & val) {
    if (root == NULL) {
      root = new Node(key, val, NULL, NULL);
      return;
    }
    Node * tmp = root;
    while (1) {
      if (key < tmp->key) {
        if (tmp->left == NULL) {
          tmp->left = new Node(key, val, NULL, NULL);
          break;
        }
        else
          tmp = tmp->left;
      }
      else if (key > tmp->key) {
        if (tmp->right == NULL) {
          tmp->right = new Node(key, val, NULL, NULL);
          break;
        }
        else
          tmp = tmp->right;
      }
      else if (key == tmp->key) {
        tmp->val = val;
        break;
      }
    }
  }
  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    Node * cur = root;
    while (true) {
      if (!cur) {
        throw std::invalid_argument("cannot find");
      }
      if (key > cur->key) {
        cur = cur->right;
      }
      else if (key < cur->key) {
        cur = cur->left;
      }
      else {
        return cur->val;
      }
    }
  }
  virtual void remove(const K & key) {
    Node ** cur = &root;
    while (*cur && *cur->key != key) {
      if (key > *cur->key) {
        cur = cur->right;
      }
      else {
        cur = cur->left;
      }
    }
    if (!*cur)
      return;
    if (!*cur->left) {
      Node * tem = *cur->right;
      delete *cur;
      *cur = tem;
    }
    else if (!*cur->right) {
      Node * tem = *cur->left;
      delete *cur;
      *cur = tem;
    }
    else {
      Node ** tem = cur->left;
      while (*tem->right) {
        tem = tem->right;
      }
      *cur->key = *tem->key;
      *cur->val = *tem->val;
      Node * T = *tem->left;
      delete *tem;
      *tem = T;
    }
  }
};
