#ifndef __LL_HPP__
#define __LL_HPP__

#include <assert.h>

#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>

//YOUR CODE GOES HERE

template<typename T>
class LinkedList {
 private:
  class Node {
   public:
    T item;
    Node * next;
    Node * prev;
    Node(T data) : item(data), next(NULL), prev(NULL) {}
    Node(T data, Node * n1, Node * n2) : item(data), next(n1), prev(n2) {}
  };
  Node * head;
  Node * tail;
  int size;

 public:
  LinkedList() : head(NULL), tail(NULL), size(0){};

  LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(0) {
    Node * cur = rhs.head;
    while (cur != NULL) {
      this->addBack(cur->item);
      cur = cur->next;
    }
  }

  void addFront(const T & data) {
    head = new Node(data, head, NULL);
    if (tail == NULL)
      tail = head;
    else
      head->next->prev = head;
    size++;
  }

  void addBack(const T & data) {
    tail = new Node(data, NULL, tail);
    if (head == NULL)
      head = tail;
    else
      tail->prev->next = tail;
    size++;
  }

  bool remove(const T & data) {
    Node * cur = head;
    while (cur != NULL && cur->item != data) {
      cur = cur->next;
    }
    if (cur == NULL)
      return 0;
    if (this->size == 1) {
      head = NULL;
      tail = NULL;
      delete cur;
      size--;
      return 1;
    }
    if (cur->next == NULL) {
      cur->prev->next = NULL;
      tail = cur->prev;
    }
    else if (cur->prev == NULL) {
      cur->next->prev = NULL;
      head = cur->next;
    }
    else {
      cur->next->prev = cur->prev;
      cur->prev->next = cur->next;
    }
    delete cur;
    size--;
    return 1;
  }

  T & operator[](int index) {
    assert(index >= 0 && index < size);
    Node * cur = head;
    int i = 0;
    while (i != index) {
      i++;
      cur = cur->next;
    }
    return cur->item;
  }

  const T & operator[](int index) const {
    assert(index >= 0 && index < size);
    Node * cur = head;
    int i = 0;
    while (i != index) {
      i++;
      cur = cur->next;
    }
    return cur->item;
  }

  //find an item
  int find(const T & data) {
    int index = 0;
    Node * cur = head;
    while (cur != NULL) {
      if (cur->item == data) {
        return index;
      }
      index++;
      cur = cur->next;
    }
    return -1;
  }

  //getSize function
  int getSize() const { return size; }

  LinkedList & operator=(const LinkedList & rhs) {
    Node * tmp = new Node(rhs[0], NULL, NULL);
    Node * cur = tmp;
    for (int i = 1; i < rhs.getSize(); i++) {
      cur->next = new Node(rhs[i], NULL, cur);
      cur = cur->next;
    }
    while (head != NULL) {
      Node * temp1 = head->next;
      delete head;
      head = temp1;
    }
    head = tmp;
    tail = cur;
    size = rhs.getSize();
    return *this;
  }

  //destructor
  ~LinkedList() {
    while (head != NULL) {
      Node * tmp = head->next;
      delete head;
      head = tmp;
    }
  }
};

#endif
