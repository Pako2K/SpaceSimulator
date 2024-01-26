

#include <iostream>
#include <collections/mtree.h>

class INT : public utils::UniqueSortable<int, int>
{
public:
  INT(int i, int j) : utils::UniqueSortable<int, int>("INT", i, j), _key{ i }, _s_key{ j } {
  }


  friend std::ostream& operator<< (std::ostream& os, const INT& i) {
    os << i._key  << "," << i._s_key;
    return os;
  }

  char test;
protected:
  int _key;
  int _s_key;
};

class ClassC {
};



using namespace tree;

int main(int argc, char** args) {
  InitializeLogger(std::cout);

  MTree<INT> t;

  //INT neg = INT(-1, 4);
  //t.root(neg);
  //std::cout << t.root().matchingKey() << "\n";

  //MTree<int> t1;
  //MTree<ClassC> t2;
  //MTree<INT> t2(t);

  MTree<INT> t3(INT(1,4));
  //t3.root(INT(2, 2));
  std::cout << t3.root().matchingKey() << "\n";

  INT i(33, 2);
  MTree<INT> t4(std::move(i));

  INT* j = new INT(34, 1);
  MTree<INT> t5(j);
  std::cout << t5.root().matchingKey() << "\n";

  MTree<INT> t5b(new INT(35, 1));  
  std::cout << t5b.root().matchingKey() << "\n";

  MTree<INT> t6;
  t6.root(INT(11, 4));
  std::cout << t6.root().matchingKey() << "\n";
  //std::cout << " Parent: " << t6.parent(11).matchingKey() << "\n";


  INT ii(133, 2);
  MTree<INT> t7;
  t7.root(std::move(ii));
  std::cout << t7.root().matchingKey() << "\n";
  std::cout << t7;

  INT* jj = new INT(134, 1);
  MTree<INT> t8;
  t8.root(jj);

  std::cout << t8.root().matchingKey() << "\n";
  std::cout << t8;

  t8.addNode(INT(5, 2), 134);
  std::cout << t8;

  t8.addNode(new INT(6, 1), 134);
  t8.addNode(INT(8, 11), 6); 
  t8.addNode(INT(7, 21), 134);
  t8.addNode(INT(9, 31), 6);
  t8.addNode(INT(19, 131), 7);
  t8.addNode(INT(18, 131), 8);

  std::cout << t8;
  std::cout << "Parent: " << t8.parent(5) << "\n";


  auto siblings_iter = t8.siblings(8);

  while (siblings_iter.hasNext()) {
    std::cout << siblings_iter.next() << std::endl;
  }

  auto siblings_root_iter = t8.siblings(t8.root().matchingKey());

  while (siblings_root_iter.hasNext()) {
    std::cout << siblings_root_iter.next() << std::endl;
  }

  auto children_iter = t8.children(134);

  while (children_iter.hasNext()) {
    std::cout << children_iter.next() << std::endl;
  }
  children_iter.rewind();
  while (children_iter.hasNext()) {
    std::cout << children_iter.next() << std::endl;
  }

  auto tree_iter = t8.begin();
  std::cout << "TREE ITERATOR" << std::endl;
  while (tree_iter.hasNext()) {
    auto& h = tree_iter.next();
    std::cout << h << std::endl;
    //h.test = 'C';
  }

  tree_iter.rewind();
  while (tree_iter.hasNext()) 
    std::cout << *tree_iter++ << std::endl;
    

  const auto& t9 = t8;
  std::cout << "Find: " << t9.find(8) << "\n";
  std::cout << "Find: " << t8.find(19) << "\n";

  //t8.removeNode(8);
  t8.removeNode(8,true);
  std::cout << t8;
  //t8.removeNode(218);
  t8.removeNode(19);
  std::cout << t8;


  return 0;

}
