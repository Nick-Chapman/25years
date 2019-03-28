
using namespace std;

// Experiment with C++ template meta programming
// second: interpreter for lambda exps...

#include <iostream>

template<int> class EXP_Num;
template<class,class> class EXP_Add;
template<class,class> class EXP_App;
template<int,class> class EXP_Lam;
template<int> class EXP_Var;

#define num(n)	    EXP_Num<n>
#define add(a,b)    EXP_Add<a,b >
#define app(a,b)    EXP_App<a,b >
#define lam(x,b)    EXP_Lam<x,b >
#define var(x)      EXP_Var<x>

template<int> class VALUE_Num;
template<int x,class env, class exp> class VALUE_Closure;

template<class> class NUM_Value;
template<int n> struct NUM_Value<VALUE_Num<n> > { enum {compileTimeInt = n}; };
#define num_value(v) NUM_Value<v>::compileTimeInt

class ENV_Null;
template<int x, class value, class env> class ENV_Cons;

template<int x,class env> class Lookup;
#define lookup(x,env) typename Lookup<x,env>::result

template<int x,class value, class env>
struct Lookup<x,ENV_Cons<x,value,env> > {
  typedef value result;
};

template<int x,int y,class value, class env> //x != y
struct Lookup<x,ENV_Cons<y,value,env> > {
  typedef lookup(x,env) result;
};

template<class exp,class env> class Eval;
#define eval(exp,env) typename Eval<exp,env>::result

template<class closure,class value> class Apply;
#define apply(closure,value) typename Apply<closure,value>::result

template<int n,class env>
struct Eval<EXP_Num<n>,env> {
  typedef VALUE_Num<n> result;
};

template<class A,class B,class env>
struct Eval<EXP_Add<A,B>,env> {
  typedef VALUE_Num<num_value(eval(A,env)) + num_value(eval(B,env))> result;
};

template<int x,class B,class env>
struct Eval<EXP_Lam<x,B>,env> {
  typedef VALUE_Closure<x,env,B> result;
};

template<int x,class env>
struct Eval<EXP_Var<x>,env> {
  typedef lookup(x,env) result;
};

template<class A, class B,class env>
struct Eval<EXP_App<A,B>,env> {
  typedef apply(eval(A,env),eval(B,env)) result;
};

template<int x,class env, class exp, class value>
struct Apply<VALUE_Closure<x,env,exp>,value> {
  typedef ENV_Cons<x,value,env> extendedEnv;
  typedef eval(exp,extendedEnv) result;
};

template<class e> struct TopEval {
  enum {value = num_value(eval(e,ENV_Null))};
};
#define topEval(e) TopEval<e>::value

const int x = 1001;
const int y = 1002;
const int f = 1003;

typedef add(num(2),num(3)) prog1;
typedef app(lam(x,add(var(x),num(1))),num(41)) prog2;
typedef app(app(lam(x,lam(y,add(var(x),var(y)))),num(4)),num(3)) prog3;

typedef lam(x,add(var(x),num(1))) inc;    
typedef lam(f,lam(x,app(var(f),app(var(f),var(x))))) twice;
typedef lam(f,lam(x,app(var(f),app(var(f),app(var(f),var(x)))))) thrice;

typedef app(app(twice,inc),num(0)) prog4;
typedef app(app(app(twice,twice),inc),num(0)) prog5;
typedef app(app(app(app(twice,twice),twice),inc),num(0)) prog6;

typedef app(app(thrice,inc),num(100)) prog7;
typedef app(app(app(thrice,thrice),inc),num(0)) prog8;
typedef app(app(app(app(twice,twice),thrice),inc),num(0)) prog9;

int main() {
  int result = topEval(prog9);
  cout << "***result = " << result << endl;
}

