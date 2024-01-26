#include <collections/unique_sortable.h>
#include <iostream>
#include <string>

#include <logger.h>


using namespace utils;

class MyClass : public UniqueSortable<std::string, double> {
public:
  int g;

  MyClass(std::string key1, double key2, int v) : UniqueSortable("MyClass", key1, key2), g{ v }{}

  MyClass(const MyClass& cl) = delete;
  MyClass(MyClass&& cl) : UniqueSortable(std::move(cl)), g{ cl.g } {};

  std::string key1() { return matchingKey(); }
  double key2() { return sortingKey(); }
};

class ClassB: public UniqueSortable<int, double> {
public:
  int g;

  ClassB(int key1, double key2, int v) : UniqueSortable("MyClass", key1, key2), g{ v }{}

  int key1() { return matchingKey(); }
  double key2() { return sortingKey(); }
};

class ClassC : public UniqueSortable<std::string, double, std::greater<double> > {
public:

  ClassC(std::string key1, double key2, int v) : UniqueSortable("MyClass", key1, key2) {}

  //ClassC(const ClassC& c): UniqueSortable("MyClass", c.key1(), c.key2()) {}

  std::string key1() const { return matchingKey(); }
  double key2() const { return sortingKey(); }
};




int v__main(int argc, char** args) {
  InitializeLogger(std::cout);

  MyClass a("A", 2.0, 7);


//  MyClass aa("A", 2.0, 7);
  MyClass b("B", 2.0, 7);
  std::cout << b.g << " " << b.key1() << " " << b.key2() << std::endl;

  MyClass* c = new MyClass("C", 2.3, 1);
    
  MyClass d = std::move(*c);
  std::cout << d.g << " " << d.key1() << " " << d.key2() << std::endl;
//  std::cout << c->g << " " << c->key1() << " " << c->key2() << std::endl;
  delete c;

  MyClass* e = new MyClass(std::move(d));
  std::cout << e->g << " " << e->key1() << " " << e->key2() << std::endl;
  //std::cout << d.g << " " << d.key1() << " " << d.key2() << std::endl;
    

  MyClass cc(std::move(b));
//  MyClass cc2(std::move(b));
  std::cout << cc.g << " " << cc.key1() << " " << cc.key2() << std::endl;
//  std::cout << b.g << " " << b.key1() << " " << b.key2() << std::endl;

  MyClass bb(MyClass("D", 2.3, 1));
  
 // std::cout << "compare a < d : " << (a < d) << std::endl;
//  std::cout << "compare d < b : " << (d < b) << std::endl;
  std::cout << "compare a < cc : " << (a < cc) << std::endl;
//  std::cout << "compare b < a : " << (b < a) << std::endl;

  ClassB b1(2, 4, 1);
//  ClassC c1("A", 4.2, 1);
  ClassC c2("AAA", 4.2, 1);
  ClassC c3("Abb", 3.2, 1);
  ClassC c4("AAAv", 6.2, 1);
  
  std::cout << "compare c2 < c3 : " << (c2 < c3) << std::endl;
  std::cout << "compare c3 < c4 : " << (c3 < c4) << std::endl;


  return 0;
}
