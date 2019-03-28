
using namespace std;

// Experiment with C++ template meta programming
// First simple interpreter of numeric exps...

template<int> class EXP_Num;
template<class,class> class EXP_Add;

template<class> class Eval;

template<int n>
struct Eval<EXP_Num<n> > {
  enum {value = n};
};

template<class E1,class E2>
struct Eval<EXP_Add<E1,E2> > {
  enum {value = Eval<E1>::value + Eval<E2>::value};
};

#define num(n)	    EXP_Num<n>
#define add(a,b)    EXP_Add<a,b >

typedef
add(add(num(1),num(2)),add(num(3),num(4)))
prog;

#include <iostream>

int main() {
  int result = Eval<prog>::value; //compile time calculation
  cout << "***result = " << result << endl;
}
