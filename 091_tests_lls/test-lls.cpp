#include <assert.h>

#include <cstdlib>
#include <iostream>

#include "il.hpp"

class Tester {
 public:
  // testing for default constructor is done for you
  void testDefCtor() {
    IntList il;
    assert(il.head == NULL);
    assert(il.tail == NULL);
    assert(il.getSize() == 0);
  }
  // example of another method you might want to write
  void testAddFront() {
    // WRITE ME
    IntList il;
    il.addFront(1);
    assert(il.head != NULL);
    assert(il.head->data == 1);
    assert(il.head->prev == NULL);
    assert(il.head->next == NULL);
    assert(il.head == il.tail);

    IntList::Node * cur = il.head;
    il.addFront(5);
    assert(il.head != NULL);
    assert(il.head->data == 5);
    assert(il.head->next == cur);
    assert(cur->prev == il.head);
  }
  void testRemove() {
    IntList il = IntList();
    assert(il.head == NULL);
    assert(il.tail == NULL);
    assert(il.getSize() == 0);
    //il.addBack(0);
    il.addBack(1);
    il.addBack(2);
    il.addFront(0);
    IntList il_ = il;
    assert(il_.remove(1));
    assert(il_.head->data == 0);
    assert(il_.head->next->data == 2);
    assert(il_.tail->data == 2);
    assert(il_.tail->prev->data == 0);
    assert(il_.head->next == il_.tail);
    assert(il_.tail->prev == il_.head);
    assert(il_.getSize() == 2);
    assert(il_[0] == 0);
    assert(il_[1] == 2);

    assert(il.head->data == 0);
    assert(il.head->next->data == 1);
    assert(il.tail->data == 2);
    assert(il.tail->prev->data == 1);
    assert(il.head->next == il.tail->prev);
    assert(il.tail->prev->prev == il.head);
    assert(il.getSize() == 3);
    assert(il[0] == 0);
    assert(il[1] == 1);
    assert(il[2] == 2);

    assert(!il.remove(7));
    assert(il.remove(0));

    assert(il.head->data == 1);
    assert(il.head->next->data == 2);
    assert(il.tail->data == 2);
    assert(il.tail->prev->data == 1);
    assert(il.head->next == il.tail);
    assert(il.tail->prev == il.head);
    assert(il.getSize() == 2);
    assert(il[0] == 1);
    assert(il[1] == 2);

    assert(il.remove(2));
    assert(il.head->data == 1);
    assert(il.head->next == NULL);
    assert(il.tail->data == 1);
    assert(il.tail->prev == NULL);
    assert(il.head == il.tail);
    //assert(il.tail->prev == il.head);
    assert(il.getSize() == 1);
    assert(il[0] == 1);

    assert(il.remove(1));
    assert(il.head == NULL);
    assert(il.tail == NULL);
    assert(il.getSize() == 0);
    assert(!il.remove(0));
  }

  void testThreeRules() {
    IntList il1 = IntList();
    IntList il2 = il1;
    assert(il2.head == NULL);
    assert(il2.tail == NULL);
    assert(il2.getSize() == 0);

    il1.addBack(1);
    IntList il = IntList(il1);
    assert(il.head->data == 1);
    assert(il.head->next == NULL);
    assert(il.tail->data == 1);
    assert(il.tail->prev == NULL);
    assert(il.tail == il.head);

    il1.addBack(2);
    il = IntList(il1);
    assert(il.head->data == 1);
    assert(il.head->next->data == 2);
    assert(il.tail->data == 2);
    assert(il.tail->prev->data == 1);
    assert(il.head->next == il.tail);
    assert(il.tail->prev == il.head);

    IntList il3 = il;
    assert(il3.head->data == 1);
    assert(il3.head->next->data == 2);
    assert(il3.tail->data == 2);
    assert(il3.tail->prev->data == 1);
    assert(il3.tail == il3.head->next);
    assert(il3.tail->prev == il3.head);
    assert(il3.getSize() == 2);
  }
  // many more tester methods
  void testAddBack() {
    IntList il;
    il.addBack(1);
    IntList::Node * N1 = il.tail;
    assert(il.head != NULL);
    assert(il.tail->data == 1);
    assert(il.head->prev == NULL && il.tail->next == NULL);
    assert(il.getSize() == 1);
    il.addBack(2);
    IntList::Node * N2 = il.tail;
    assert(il.tail != NULL);
    assert(il.tail->data == 2);
    assert(N1->next == il.tail);
    assert(il.tail->prev == N1);
    il.addFront(3);
    IntList::Node * N3 = il.head;
    assert(il.head != NULL);
    assert(il.tail == N2);
    assert(il.getSize() == 3);
    assert(il[0] == 3);
    assert(il[1] == 1);
    assert(il[2] == 2);
    // Check N1
    assert(N1->data == 1);
    assert(N1->next == N2);
    assert(N1->prev == N3);
    // Check N2
    assert(N2->data == 2);
    assert(N2->next == NULL);
    assert(N2->prev == N1);
    // Check N3
    assert(N3->data == 3);
    assert(N3->next == N1);
    assert(N3->prev == NULL);
  }
};

int main(void) {
  Tester t;
  t.testDefCtor();
  t.testAddFront();
  t.testAddBack();
  t.testRemove();
  t.testThreeRules();
  // write calls to your other test methods here
  return EXIT_SUCCESS;
}
