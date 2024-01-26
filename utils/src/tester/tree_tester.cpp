#include <collections/tree.h>
#include <iostream>

//template <typename TT, class Sorting = std::less<TT>>
//class Node {
//public:
//  Node(const TT& object) : _object{ object } {}
//
//  bool operator < (const Node<TT, Sorting>& another) const {
//    return _compare(this->_object, another._object);
//  }
//
//protected:
//  const TT& _object;
//  Sorting _compare;
//};


int ss_main(int argc, char** args) {
  InitializeLogger(std::cout);

  using namespace tree;
  int a = 8;
  int b = 2;
  Tree<int> mytree;
  mytree.root(a);
  mytree.root(b);
  std::cout << mytree.root() << std::endl;



  Tree<int> mytree2(b);
  std::cout << mytree2 << std::endl;

  int c = 1;
  mytree.addNode(c, a);
  int d = 11;
  mytree.addNode(d, a);
  
  int e = 12;
  int f = 13;
  int g = 25;
  int h = 15;
  int i = 5;
  int j = 35;
  int k = -5;
  mytree.addNode(b, a); 
  

  mytree.addNode(e, a);
  mytree.addNode(f, c);
  mytree.addNode(g, f);
  mytree.addNode(h, f);
  mytree.addNode(i, a);
  std::cout << mytree << std::endl;

  Tree<int> trre = mytree;
  std::cout << trre << std::endl;

  auto siblings_iter = mytree.siblings(g);

  while (siblings_iter.hasNext()) {
    std::cout << siblings_iter.next() << std::endl;
  }

  auto children_iter = mytree.children(a);

  while (children_iter.hasNext()) {
    std::cout << children_iter.next() << std::endl;
  }
  children_iter.begin();
  while (children_iter.hasNext()) {
    std::cout << children_iter.next() << std::endl;
  }

  Tree<int> subtree(mytree, c );
  std::cout << subtree << std::endl;

  Tree<int> sec_subtree;
  sec_subtree = mytree;
  std::cout << sec_subtree << std::endl;

  mytree.removeNode(b);
  std::cout << mytree << std::endl;
  mytree.removeNode(c, true);
  std::cout << mytree << std::endl;
  mytree.addNode(k, a);
  std::cout << mytree << std::endl;
  
  mytree.removeNode(d, true);
  std::cout << mytree << std::endl;
  mytree.addNode(j, e);
  std::cout << mytree << std::endl;


  std::cout << sec_subtree << std::endl;
  std::cout << sec_subtree.parent(h) << std::endl;

  
  std::cout << trre.parent(f) << std::endl;

  std::cout << mytree.parent(j) << std::endl;
  

  return 0;
}