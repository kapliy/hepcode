// Binary search trees and depth-first vs breadth-first transversal

#include<iostream>
#include<queue>
#include<cassert>

struct Node {
  int data;
  Node* left;
  Node* right;
  Node() : data(-1),left(0),right(0) {};
  Node(int v) : data(v),left(0),right(0) {};
  ~Node() { delete left; delete right; }
  void Add(int v) {
    if(v<data) {
      if(!left) { left = new Node(v); return; }
      left->Add(v);
    } else {
      if(!right) { right = new Node(v); return; }
      right->Add(v);
    }
  }
  void PrintInOrder(int depth,const std::string& side) const {
    if(left) left->PrintInOrder(depth+1,"lft");
    std::cout << side << "(" << depth << ") = " << data << std::endl;
    if(right) right->PrintInOrder(depth+1,"rgt");
  }
  void PrintPreOrder(int depth,const std::string& side) const {
    std::cout << side << "(" << depth << ") = " << data << std::endl;
    if(left) left->PrintPreOrder(depth+1,"lft");
    if(right) right->PrintPreOrder(depth+1,"rgt");
  }
  void PrintPostOrder(int depth,const std::string& side) const {
    if(left) left->PrintPostOrder(depth+1,"lft");
    if(right) right->PrintPostOrder(depth+1,"rgt");
    std::cout << side << "(" << depth << ") = " << data << std::endl;
  }
  // breadth search is non-recursive!
  void PrintBreadth() const {
    std::queue<const Node*> q;
    std::queue<int> q_depth;
    std::queue<std::string> q_dir;
    q.push(this); q_depth.push(0); q_dir.push("top");
    while( !q.empty() ) {
      const Node *node = q.front(); q.pop();
      const int node_depth = q_depth.front(); q_depth.pop();
      const std::string node_dir = q_dir.front(); q_dir.pop();
      assert(node);
      std::cout << node_dir << "(" << node_depth << ") = " << node->data << std::endl;
      // enqueue children
      if(node->left) { q.push(node->left); q_depth.push(node_depth+1); q_dir.push("lft"); }
      if(node->right) { q.push(node->right); q_depth.push(node_depth+1); q_dir.push("rgt"); }
    }
  }
};

struct Tree {
  Node *top;
  Tree() : top(0) {};
  ~Tree() { delete top; }
  void Add(int v) {
    if(!top) {
      top = new Node(v);
      return;
    }
    top->Add(v);
  }
  void PrintInOrder() const {
    int depth = 0;
    top->PrintInOrder(depth,"top");
  }
  void PrintPreOrder() const {
    int depth = 0;
    top->PrintPreOrder(depth,"top");
  }
  void PrintPostOrder() const {
    int depth = 0;
    top->PrintPostOrder(depth,"top");
  }
  void PrintBreadth() const {
    top->PrintBreadth();
  }
};

int main() {
  Tree *t = new Tree();
  t->Add(5);
  t->Add(4);
  t->Add(7);
  t->Add(5);
  t->Add(1);
  t->Add(3);
  std::cout << "In-order:" << std::endl;
  t->PrintInOrder();
  std::cout << "Pre-order:" << std::endl;
  t->PrintPreOrder();
  std::cout << "Post-order:" << std::endl;
  t->PrintPostOrder();
  std::cout << "Breadth:" << std::endl;
  t->PrintBreadth();
}
