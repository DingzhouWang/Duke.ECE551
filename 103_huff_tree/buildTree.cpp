#include "node.h"

Node * buildTree(uint64_t * counts) {
  //WRITE ME!
  priority_queue_t q;
  for (int i = 0; i < 257; i++) {
    if (counts[i] != 0) {
      q.push(new Node(i, counts[i]));
    }
  }
  while (q.size() > 1) {
    Node * l = q.top();
    q.pop();
    Node * r = q.top();
    q.pop();
    q.push(new Node(l, r));
  }
  return q.top();
}
