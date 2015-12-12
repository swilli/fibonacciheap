#ifndef FIBONACCI_HEAP_LIST_HPP
#define FIBONACCI_HEAP_LIST_HPP

#include "doublylinkedlist.hpp"
#include "fibonacciheapnode.hpp"

template<class T> class FibonacciHeapList : public DoublyLinkedList<FibonacciHeapNode<T> *> {};

#endif