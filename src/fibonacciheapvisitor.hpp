#ifndef FIBONACCI_HEAP_VISITOR
#define FIBONACCI_HEAP_VISITOR

#include "fibonacciheapnode.hpp"

template<class T> class FibonacciHeapNode;

template<class T> class FibonacciHeapVisitor {

public:
	FibonacciHeapVisitor() : depth(0) {}

	int depth;

	virtual void visit(FibonacciHeapNode<T> *node) = 0;
};

#endif