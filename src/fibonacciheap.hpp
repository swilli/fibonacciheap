#ifndef FIBONACCI_HEAP_HPP
#define	FIBONACCI_HEAP_HPP

#include "log.hpp"
#include "vector.hpp"
#include "fibonacciheaplist.hpp"
#include "fibonacciheapasciigraphvisitor.hpp"
#include "fibonacciheapjsongraphvisitor.hpp"
#include "fibonacciheapcorrectnessvisitor.hpp"

#include <string>

template<class T> class FibonacciHeap {

public:
	FibonacciHeap(bool (&function)(const T &t1, const T &t2), const T &top_infinity) : 
	_merged(0),
	_size(0), 
	_top(0),
	_heaps(FibonacciHeapList<T>()), 
	_condition(function), 
	_top_infinity(top_infinity) {

	}

	~FibonacciHeap() {
		if (_merged) {
			return;
		}

		DoublyLinkedListItem<FibonacciHeapNode<T> *> *iterRoot = _heaps.begin();
		while (iterRoot != 0x0) {
			DoublyLinkedListItem<FibonacciHeapNode<T> *> *root = iterRoot;
			iterRoot = iterRoot->next;
			destroyNode(root->value);
			delete root;
		}
	}

	FibonacciHeapNode<T> *push(const T &item) {
		FibonacciHeapNode<T> *node = new FibonacciHeapNode<T>(0x0, item, false);
		DoublyLinkedListItem<FibonacciHeapNode<T> *> *newHeap = _heaps.push_back(node);
		node->listItem = newHeap;

		if(_size == 0) {
			_top = node;
		} else {
			if (_condition(node->value, _top->value)) {
				_top = node;
			}
		}

		_size++;

		return node;
	}

	FibonacciHeapNode<T> *popNode() {
		FibonacciHeapNode<T> *result = _top;

		_heaps.remove(result->listItem);
		delete result->listItem;

		DoublyLinkedListItem<FibonacciHeapNode<T> *> *iterChildListItem = result->children.begin();

		while (iterChildListItem != 0x0) {
			DoublyLinkedListItem<FibonacciHeapNode<T> *> *childListItem = iterChildListItem;

			iterChildListItem = iterChildListItem->next;

			FibonacciHeapNode<T> *childNode = childListItem->value;

			childNode->parent = 0x0;
			childNode->marked = false;

			_heaps.append(childListItem);
		}

		if (_heaps.size() > 0) {
			consolidate();

			DoublyLinkedListItem<FibonacciHeapNode<T> *> *heap = _heaps.begin();
			DoublyLinkedListItem<FibonacciHeapNode<T> *> *newTop = 0x0;

			while (heap != 0x0) {
				if (newTop == 0x0) {
					newTop = heap;
				} else {
					if (_condition(heap->value->value, newTop->value->value)) {
						newTop = heap;	
					}
				}

				heap = heap->next;
			}

			_top = newTop->value;
			_top->listItem = newTop;

		} else {
			_top = 0x0;
		}

		_size--;

		return result;
	}

	T pop() {
		const FibonacciHeapNode<T> *top = popNode();
		const T result = top->value;
		delete top;
		return result; 
	}

	bool remove(FibonacciHeapNode<T> *node) {
		decreaseNode(node);
		pop();
	}


	T top() const {
		return _top->value;
	}

	int size() const {
		return _size;
	}

	friend std::ostream& operator<<(std::ostream& os, const FibonacciHeap<T>& heap) {
		os << heap.toString();
		return os;
	}

	void unite(FibonacciHeap<T> &heap) {
		if (heap.size() > 0) {
			heap._merged = true;
			DoublyLinkedListItem<FibonacciHeapNode<T> *> *iterRoot = heap._heaps.begin();
			while (iterRoot != 0x0) {
				DoublyLinkedListItem<FibonacciHeapNode<T> *> *root = iterRoot;
				iterRoot = iterRoot->next;
				_heaps.append(root);
			}
			_size += heap._size;
		}
	}

	std::string toJSON() const {
		DoublyLinkedListItem<FibonacciHeapNode<T> *> *root = _heaps.begin();
		if (root != 0x0) {
			FibonacciHeapJSONGraphVisitor<T> v;	
			while (root != 0x0) {
				root->value->accept(v);
				root = root->next;
			}
			return v.getJSONGraph();
		}
		return std::string("{}");
	}

	void check() const {
		FibonacciHeapCorrectnessVisitor<T> v;	
		DoublyLinkedListItem<FibonacciHeapNode<T> *> *root = _heaps.begin();
		if (root != 0x0) {
			while (root != 0x0) {
				root->value->accept(v);
				root = root->next;
			}
			v.check();
		}
	}

private:
	bool _merged;

	int _size;

	const T &_top_infinity;

	FibonacciHeapNode<T> *_top;

	FibonacciHeapList<T> _heaps;

	bool (&_condition)(const T &t1, const T &t2);

	void consolidate() {
		bool done;

		if (_size == 0) {
			return;
		}

		Vector<DoublyLinkedListItem<FibonacciHeapNode<T> *> *> ranks(math::log(_size) + 2);

		do {
			done = true;

			DoublyLinkedListItem<FibonacciHeapNode<T> *> *iterRoot = _heaps.begin();
			while (iterRoot != 0x0) {
				DoublyLinkedListItem<FibonacciHeapNode<T> *> *root = iterRoot;
				
				iterRoot = iterRoot->next;

				FibonacciHeapNode<T> *rootNode = root->value;
				const int rank = rootNode->children.size();

				if (ranks[rank] != 0x0) {
					if (ranks[rank] != root) {
						DoublyLinkedListItem<FibonacciHeapNode<T> *> *previousRoot = ranks[rank];
						FibonacciHeapNode<T> *previousRootNode = previousRoot->value; 

						if (_condition(rootNode->value, previousRootNode->value)) {
							_heaps.remove(previousRoot);
							rootNode->parent = 0x0;
							rootNode->marked = false;
							previousRootNode->parent = rootNode;
							rootNode->children.append(previousRoot);
						} else {
							_heaps.remove(root);
							previousRootNode->parent = 0x0;
							previousRootNode->marked = false;
							rootNode->parent = previousRootNode;
							previousRootNode->children.append(root);
						}

						ranks[rank] = 0x0;
						done = false;
						break;
					}
				} else {
					ranks[rank] = root;
				}
			}
		} while (!done);
	}

	void decreaseNode(FibonacciHeapNode<T> *node) {
		node->value = _top_infinity;

		if (node->parent != 0x0) {
			FibonacciHeapNode<T> *parent = node->parent;

			if (_condition(node->value, parent->value)) {
				parent->children.remove(node->listItem);

				node->marked = false;
				node->parent = 0x0;

				_heaps.append(node->listItem);

				if (!parent->marked) {
					if (parent->parent != 0x0) {
						parent->marked = true;
					}
				} else {

					FibonacciHeapNode<T> *iterNode = parent;
					while (iterNode->parent != 0x0) {
						parent = iterNode->parent;

						if (iterNode->marked) {
							parent->children.remove(iterNode->listItem);

							iterNode->marked = false;
							iterNode->parent = 0x0;

							_heaps.append(iterNode->listItem);
						} else {
							iterNode->marked = true;
							break;
						}

						iterNode = parent;
					}
				}
			}
		}

		if (_condition(node->value, _top->value)) {
			_top = node;
		}
	}

	void destroyNode(FibonacciHeapNode<T> *node) {
		DoublyLinkedListItem<FibonacciHeapNode<T> *> *iterChild = node->children.begin();
		while (iterChild != 0x0) {
			DoublyLinkedListItem<FibonacciHeapNode<T> *> *child = iterChild;
			iterChild = iterChild->next;
			destroyNode(child->value);
			delete child;
		}
		delete node;
	}

	std::string toString() const {
		DoublyLinkedListItem<FibonacciHeapNode<T> *> *root = _heaps.begin();
		if (root != 0x0) {
			FibonacciHeapASCIIGraphVisitor<T> v;	
			while (root != 0x0) {
				root->value->accept(v);
				root = root->next;
			}
			return v.getASCIIGraph();
		}
		return std::string("");
	}

};


#endif