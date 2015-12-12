#ifndef	FIBONACCI_HEAP_NODE_HPP
#define FIBONACCI_HEAP_NODE_HPP

#include "doublylinkedlist.hpp"
#include "fibonacciheapvisitor.hpp"

template<class T> class FibonacciHeapNode {

public:
	FibonacciHeapNode<T> *parent;

	T value;

	DoublyLinkedList<FibonacciHeapNode<T> *> children;

	bool marked;

	DoublyLinkedListItem<FibonacciHeapNode<T> *> *listItem;

	FibonacciHeapNode(FibonacciHeapNode<T> *parent, const T &value, const bool &marked) : 
	parent(parent), 
	value(value),
	children(DoublyLinkedList<FibonacciHeapNode<T> *>()), 
	marked(marked), 
	listItem(0x0) {

	}	

	void accept(FibonacciHeapVisitor<T> &visitor) {
		DoublyLinkedListItem<FibonacciHeapNode<T> *> *child = children.begin();
		if (children.size()) {
			visitor.depth++;
		}
		while (child != 0x0) {
			child->value->accept(visitor);
			child = child->next;
		}
		if (children.size()) {
			visitor.depth--;
		}
		visitor.visit(this);
	}
};

#endif