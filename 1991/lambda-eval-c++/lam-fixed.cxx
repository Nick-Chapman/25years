
// A lambda calculus interpreter in an OO style
// Each type of lambda expression knows how to :
//   evaluate, apply, force and recursively force,
//   value, index and print itself.

// ** Must find out how to declare the constructors for **
// ** a class as private - like an eiffel deferred class **

#include<string.h>
#include<iostream>
#include<cstdlib>
using namespace std;

void error(char* mes)
{
    cout << mes << "\n";
    exit(1);
}

class lam; // forward declaration

class binding
{
public:
    char* ident;
    lam* value;
    binding* next;
    binding( char* id, lam* v, binding* n ) {
	ident = id;
	value = v;
	next = n;
    }
};
   

class env
{
    binding* top;
public:
    env() { top = NULL; } // construct the empty environment
    env( env& e );
	
    void insert( char* id, lam* v ) {
	top = new binding( id, v, top );
    }
    
    lam* lookup( char* id ) {
	binding* b = top;
	while (b) {
	    if ( strcmp(id, b->ident) == 0 ) return b->value;
	    b = b->next;
	}
	cerr << id << " ";
	error("Identifier is unbound");
    }
};

env::env( env& e )
{
    top = NULL;
    binding* from = e.top;
    binding *to = NULL;
    binding *last = NULL;
    while (from) {
	last = to;
	to = new binding( from->ident, from->value, NULL );
	if (last)
	    last->next = to;
	else
	    top = to;
	from = from->next;
    }
}


class lam
{
    // lam() {}; // deferred
public:
    int value;
    virtual lam* eval( env e ) {
	error("This lambda term cannot be evaluated");
    }
    virtual lam* apply( lam* arg ) {
	error("This lamba term cannot be applied");
    }
    virtual lam* index(int i) {
	error("This lambda term cannot be indexed");
    }
    virtual lam* force() { return this; }
    virtual lam* rec_force() { return this; }
};


class constant : public lam
{
    // constant() {}; // deferred
public:
    // all constant eval to themselves
    lam* eval( env e ) { return this; }
};


class number : public constant
{
public:
    number(int i) { value = i; }
};


typedef int (*BIN_OP)(int, int);

class bin_primitive : public constant
{
    BIN_OP bin_op;
public:
    bin_primitive( BIN_OP b ) { bin_op = b; }
    lam* apply( lam* arg ) {
	lam* f = arg->rec_force();
	return new number( (*bin_op)( f->index(1)->value,
				     f->index(2)->value ) );
    }
};

// various binary primitives
int plus ( int a, int b ) { return a+b; }
int minus( int a, int b ) { return a-b; }
int times( int a, int b ) { return a*b; }
int mod  ( int a, int b ) { return a%b; }


class variable : public lam
{
    char* ident;
public:
    variable(char* id) { ident = id; }
    lam* eval( env e ) { return e.lookup( ident ); }
};


class application : public lam
{
    lam* function;
    lam* argument;
public:
    application(lam* f, lam* a) {
	function = f;
	argument = a;
    }
    lam* eval( env e ) {
	env* e2 = new env(e);
	return function->eval(e)->apply(argument->eval(*e2));
    }
};


class closure : public lam
{
    char* ident;
    lam* body;
    env environment;
public:
    closure( char* id, lam* b, env e ) {
	ident = id;
	body = b;
	environment = e;
    }
    lam* apply( lam* arg ) {
	environment.insert(ident, arg);
	return body->eval(environment);
    }
};


class abstraction : public lam
{
    char* ident;
    lam* body;
public:
    abstraction( char* id, lam* b ) { ident = id; body = b; }
    lam* eval( env e ) {
	return new closure( ident, body, e );
    }
};


class tuple_2 : public lam
{
    lam* component1;
    lam* component2;
public:
    tuple_2( lam* c1, lam* c2 ) {
	component1 = c1;
	component2 = c2;
    }
    lam* eval( env e ) {
	env* e2 = new env(e);
	return new tuple_2( component1->eval(e), component2->eval(*e2) );
    }
    lam* index( int i ) {
	if (i == 1) return component1;
	else if (i == 2) return component2;
	else error("Tuple_2's can only be indexed by 1 or 2");
    }
};


main()
{
    // constuct a very simple lambda expression
    // by hand, and try to evaluate it

    env e;

    /*
    // subtracts one number from another
    lam* the_exp =
	new application( new bin_primitive( &minus ),
			new tuple_2( new number(99),
				    new number(87) ));
    */
    /*
    // uses a hand constructed environment
    e.insert("y", new number(4));
    e.insert("x", new number(23));
    lam* the_exp =
	new application( new bin_primitive( &minus ),
			new tuple_2( new variable("x"),
				    new variable("y") ));
    */
    /*
    // uses an abstraction
    lam* the_exp =
	new application(
	    new abstraction(
		"x",
		 new application(
		     new bin_primitive( &times ),
		     new tuple_2(
			 new variable("x"),
			 new variable("x") ))),
	    new number(7) );
    */
    
    // higher order function
    lam* the_exp =
	new application(
	    new application(
		
		new abstraction(
		    "f",
		    new abstraction(
			"x",
			new application(
			   new variable("f"),
			   new application(
				new variable("f"),
				new variable("x"))))),
			    
		new abstraction(
		    "i",
		     new application(
			 new bin_primitive( &times ),
			 new tuple_2(
			     new variable("i"),
			     new variable("i"))))),
			
	    new number(7) );
    
    cout << the_exp->eval(e)->value << "\n";
    
}
