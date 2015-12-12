#ifndef FIBONACCI_HEAP_CORRECTNESS_VISITOR_HPP
#define	FIBONACCI_HEAP_CORRECTNESS_VISITOR_HPP

#include "fibonacciheapvisitor.hpp"

#include <math.h>



static std::map<int,int> FibonacciNumbers;

template<class T> class FibonacciHeapCorrectnessVisitor : public FibonacciHeapVisitor<T> {

public:
	FibonacciHeapCorrectnessVisitor() {
		FibonacciNumbers[0] = 0;
		FibonacciNumbers[1] = 1;
	}

	virtual void visit(FibonacciHeapNode<T> *node) {
		if (node->parent == 0x0) {
			assert(node->marked == false);
		} 
		
		if (node->children.size() == 0) {
			_subtreeSizes[node] = 1;
		}

		_nodes.push_back(node);
	}

	bool check() {
		int max_rank = 0;
		for(int i = 0; i < _nodes.size(); ++i) {
			FibonacciHeapNode<T> *node = _nodes[i];
			
			const int rank = node->children.size();
			
			if (rank > max_rank) {
				max_rank = rank;
			}
			
			const int sizeSubtree = subtree(node);
			const int fkp2 = fibNum(rank + 2);

			assert(sizeSubtree >= fkp2);
		}

		if (_nodes.size() > 0) {
			assert(max_rank <= (log2(_nodes.size()) + 1));
		}
	}

private:
	int fibNum(const int &n) {
		int result = 0;
		if (FibonacciNumbers.count(n) > 0) {
			result = FibonacciNumbers[n];
		} else {
			result = fibNum(n - 1) + fibNum(n - 2);
			FibonacciNumbers[n] = result;
		}
		return result;
	}

	int subtree(FibonacciHeapNode<T> *node) {
		int result = 0;
		if (_subtreeSizes.count(node) > 0) {
			result = _subtreeSizes[node];
		} else {
			result = 1;
			DoublyLinkedListItem<FibonacciHeapNode<T> *> *child = node->children.begin();
			while (child != 0x0) {
				result += subtree(child->value);
				child = child->next;
			}
			_subtreeSizes[node] = result;
		}
		return result;
	}

	std::vector<FibonacciHeapNode<T> *> _nodes;

	std::map<FibonacciHeapNode<T> *, int> _subtreeSizes;

};
#endif	