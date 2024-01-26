#include <collections/closed_list.h>

#include <iostream>

using namespace utils;
using namespace std;

class AA {
public:
  AA(int a) : a{ a } {
    cout << "AA CONSTRUCTOR CALLED\n";
  }

  AA(const AA& a) {
    cout << "AA COPY CONSTRUCTOR CALLED\n";
    this->a = a.a;
  }

  ~AA() {
    cout << "AA DESTROYED\n";
  }

  AA& operator=(const AA& b) {
    cout << "AA ASSIGNMENT CALLED\n";
    this->a = b.a;
    return *this;
  }

  int a;

  friend std::ostream& operator<< (std::ostream& os, const AA& a) {
    os << a.a;
    return os;
  }
};

int ____main(int argc, char** args) {
  cout << "size of AA: " << sizeof AA << endl;
  ClosedList<AA> list(3);
  cout << "Size: " << list.size() << endl;

  list.add(AA(2));
  cout << "Elements: " << list << endl;
  AA a(3);
  list.add(a);
  cout << "Elements: " << list << endl; 

  list.add(AA(4));
  cout << "Elements: " << list << endl;

  list.add(AA(5));
  cout << "Elements: " << list << endl;

  list.add(AA(6));
  cout << "Elements: " << list << endl;

  ClosedList<AA> list2(list);
  cout << "Elements list 2: " << list2 << endl;
  list2.add(AA(100));
  cout << "Elements list 2: " << list2 << endl;

  cout << "Elements list 1: " << list << endl;
  list = list2;
  cout << "Elements list 1: " << list << endl;
  cout << "Elements list 2: " << list2 << endl;

  list.add(AA(1));
  list2.add(AA(2));
  cout << "Elements list 1: " << list << endl;
  cout << "Elements list 2: " << list2 << endl;

  cout << "Elements Data: " << list.data() << endl;

  for (int i = 0; i < list.size(); i++) {
    cout << "Elements Data " << i << " : " << list.data() + i << endl;
    cout << "Elements Data " << i << " : " << *(list.data() + i) << endl;
  }
  return 0;
}

