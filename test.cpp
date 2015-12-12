#include "src/fibonacciheap.hpp"

#include <queue>
#include <assert.h>
#include <iostream>
#include <time.h> 

bool smaller(const int &val1, const int &val2) {
	return val1 < val2;
}

void test0() {
	const int infty = 1000000;
	FibonacciHeap<int> heap(smaller, -infty);
	heap.push(5);
	heap.push(4);
	heap.push(3);
	heap.push(2);
	heap.push(1);
	const int top = heap.pop();
	std::cout << heap.top() << std::endl;
	//std::cout << heap << std::endl;
}

void test1() {
	srand(time(0));

	const int nTests = 10000000;

	const int infty = 1000000;
	FibonacciHeap<int> heap(smaller, -infty);

	std::priority_queue<int, std::vector<int>, std::greater<int> > solution;

	int sum_push = 0;
	int sum_pop = 0;
	for (int i = 0; i < nTests; ++i) {
		const int item = (rand() % infty) -  infty/2; 
		const int decision = ((sum_push - sum_pop) > 0 ? (rand() % 2) : 0);

		//std::cout << "[action: " << (decision ? "pop" : "push") << "]\t[value: " << item << "]\t[push-pop: " << (sum_push - sum_pop) << "]\t";
		if (decision) {
			heap.pop();
			solution.pop();
			sum_pop++;
		} else {
			heap.push(item);
			solution.push(item);
			sum_push++;
		}
		
		if ((sum_push - sum_pop) > 0) {
			//std::cout << "[fibheap: " << heap.top() << "][stlheap: " << solution.top() << "]";
			assert(heap.top() == solution.top());
		}
		//std::cout << std::endl;
	}
	std::cout << heap.toJSON();
}

void test2() {
	const int infty = 100000;
	FibonacciHeap<int> heap(smaller, -infty);
	heap.push(0);
	heap.push(1);
	std::cout << heap;
}

void test3() {
	const int infty = 100000;
	FibonacciHeap<int> heap(smaller, -infty);
	for (int i = 10; i > -1; --i) {
		heap.push(i);
	}
	int top = heap.pop();
	heap.pop();
	std::cout << heap.toJSON();
}

void test4() {
	srand(time(0));
	const int infty = 100000;
	FibonacciHeap<int> heap(smaller, -infty);
	for (int i = 0; i < 10000000; ++i) {
		heap.push(1 + (rand() % (infty-1)));
	}
	for (int i = 0; i < 100; ++i) {
		heap.pop();
	}
	std::cout << heap.top() << std::endl;
}

void test45() {
	const int infty = 100000;
	std::priority_queue<int> queue;
	for (int i = 0; i < 10000000; ++i) {
		queue.push(1 + (rand() % (infty-1)));
	}
	for (int i = 0; i < 10; ++i) {
		queue.pop();
	}
	std::cout << queue.top() << std::endl;
}

void test5() {
	const int infty = 100000;
	FibonacciHeap<int> heap(smaller, -infty);

	heap.push(5);
	heap.push(4);
	heap.push(3);
	heap.pop();
	heap.push(3);
	heap.push(2);
	heap.push(1);
	heap.push(0);
	heap.pop();
	heap.pop();
	std::cout << heap.toJSON();
}

void test6() {
	const int infty = 100000;
	FibonacciHeap<int> heap(smaller, -infty);

	FibonacciHeapNode<int> *node5 = heap.push(5);
	FibonacciHeapNode<int> *node4 = heap.push(4);
	FibonacciHeapNode<int> *node3 = heap.push(3);
	FibonacciHeapNode<int> *node2 = heap.push(2);
	FibonacciHeapNode<int> *node1 = heap.push(1);
	FibonacciHeapNode<int> *node0 = heap.push(0);
	heap.pop();
	heap.remove(node4);
	std::cout << heap.toJSON();
	assert(node5->marked == false);
	assert(node3->marked == false);
	assert(node2->marked == false);
	assert(node1->marked == false);
}

void test7() {
	const int infty = 100000;
	FibonacciHeap<int> heap(smaller, -infty);
	FibonacciHeapNode<int> *node0 = heap.push(0);
	heap.remove(node0);
	assert(heap.size() == 0);
	std::cout << heap.toJSON();
}

void test8() {
	const int infty = 100000;
	FibonacciHeap<int> heap(smaller, -infty);

	std::vector<FibonacciHeapNode<int> *> nodes;
	for (int i = 0; i <= 10; ++i) {
		nodes.push_back(heap.push(i));
	}

	heap.pop();
	heap.remove(nodes[6]);
	
	for (int i = 1; i <= 10; ++i) {
		if (i == 5) {
			assert(nodes[i]->marked == true);
		} else {
			assert(nodes[i]->marked == false);
		}
	}

	heap.remove(nodes[8]);
	
	for (int i = 1; i <= 10; ++i) {
		if (i == 7 || i == 5) {
			assert(nodes[i]->marked == true);
		} else {
			assert(nodes[i]->marked == false);
		}
	}

	heap.remove(nodes[7]);
	
	for (int i = 1; i <= 10; ++i) {
		if (i == 0) {
			assert(nodes[i]->marked == true);
		} else {
			assert(nodes[i]->marked == false);
		}
	}

	std::cout << heap.toJSON();
}

void test9() {
	const int infty = 100000;
	FibonacciHeap<int> heap(smaller, -infty);

	std::vector<FibonacciHeapNode<int> *> nodes;
	for (int i = 0; i <= 20; ++i) {
		nodes.push_back(heap.push(i));
	}

	heap.pop();
	heap.remove(nodes[14]);

	for (int i = 1; i <= 20; ++i) {
		if (i == 13) {
			assert(nodes[i]->marked == true);
		} else {
			assert(nodes[i]->marked == false);
		}
	}
	
	heap.remove(nodes[10]);

	for (int i = 1; i <= 20; ++i) {
		if (i == 13 || i == 9) {
			assert(nodes[i]->marked == true);
		} else {
			assert(nodes[i]->marked == false);
		}
	}

	heap.remove(nodes[15]);

	for (int i = 1; i <= 20; ++i) {
		if (i == 30) {
			assert(nodes[i]->marked == true);
		} else {
			assert(nodes[i]->marked == false);
		}
	}

	std::cout << heap.toJSON();
}

void test10() {
	srand(time(0));

	const int nTests = 100000;

	const int infty = 1000000;
	FibonacciHeap<int> heap(smaller, -infty);
	std::set<FibonacciHeapNode<int> *> inHeap;

	int sum_push = 0;
	int sum_pop = 0;
	int sum_remove = 0;
	for (int i = 0; i < nTests; ++i) {
		const int item = (rand() % infty) -  infty/2; 
		const int decision = ((sum_push - sum_pop - sum_remove) > 0 ? (rand() % 2) : 0);
		const int pop_or_remove = rand() % 2;

		//std::cout << "[action: " << (decision ? (pop_or_remove ? "pop" : "remove") : "push") << "]\t[value: " << item << "]\t[push-pop-remove: " << (sum_push - sum_pop - sum_remove) << "]\t";
		if (decision) {
			if (pop_or_remove) {
				FibonacciHeapNode<int> *node = heap.popNode();
				inHeap.erase(inHeap.find(node));
				delete node;
				sum_pop++;	
			} else {
				std::set<FibonacciHeapNode<int> *>::iterator it = inHeap.begin();
				const int randItem = rand() % inHeap.size();
				for (int j = 0; j < randItem; ++j) {
					it++;
				}
				FibonacciHeapNode<int> *node = *it;
				heap.remove(node);
				inHeap.erase(it);
				sum_remove++;
			}
		} else {
			inHeap.insert(heap.push(item));
			sum_push++;
		}
		
		if ((sum_push - sum_pop - sum_remove) > 0) {
			//std::cout << "[fibheap: " << heap.top() << "]";
		}
		//std::cout << std::endl;
	}
	std::cout << heap.toJSON();
}

void test11() {
	/*
	[action: push]	[value: -178233]	[push-pop-remove: 0]	[fibheap: -178233]
[action: push]	[value: -342396]	[push-pop-remove: 1]	[fibheap: -342396]
[action: push]	[value: 431913]	[push-pop-remove: 2]	[fibheap: -342396]
[action: remove]	[value: 137413]	[push-pop-remove: 3]	[fibheap: -342396]
*/
const int nTests = 10000000;

const int infty = 1000000;
FibonacciHeap<int> heap(smaller, -infty);

heap.push(-178233);
FibonacciHeapNode<int> *node = heap.push(-342396);
heap.push(431913);
heap.remove(node);
}

void test12() {
	const int infty = 1000000;
	FibonacciHeap<int> heap1(smaller, -infty);
	heap1.push(5);

	FibonacciHeap<int> heap2(smaller, -infty);
	heap2.push(1);

	heap2.unite(heap1);
}

void test13() {
	srand(time(0));

	const int nTests = 1000000;

	const int infty = 1000000;
	FibonacciHeap<int> heap(smaller, -infty);
	std::set<FibonacciHeapNode<int> *> inHeap;

	int sum_push = 0;
	int sum_pop = 0;
	int sum_remove = 0;
	for (int i = 0; i < nTests; ++i) {
		const int item = (rand() % infty) -  infty/2; 
		const int decision = ((sum_push - sum_pop - sum_remove) > 0 ? (rand() % 2) : 0);
		const int pop_or_remove = rand() % 2;

		if (decision) {
			if (pop_or_remove) {
				FibonacciHeapNode<int> *node = heap.popNode();
				inHeap.erase(inHeap.find(node));
				delete node;
				sum_pop++;	
			} else {
				std::set<FibonacciHeapNode<int> *>::iterator it = inHeap.begin();
				const int randItem = rand() % inHeap.size();
				for (int j = 0; j < randItem; ++j) {
					it++;
				}
				FibonacciHeapNode<int> *node = *it;
				heap.remove(node);
				inHeap.erase(it);
				sum_remove++;
			}
		} else {
			inHeap.insert(heap.push(item));
			sum_push++;
		}
		
		heap.check();
	}

}

int main(int argc, char *argv[]) {
	/*test0();
	test1();
	test2();
	test3();
	test4();
	test45();
	test5();
	test6();
	test7();
	test8();
	test9();
	test10();
	test11();
	test12();
	test10();
	*/
	test13();
	return 0;
}