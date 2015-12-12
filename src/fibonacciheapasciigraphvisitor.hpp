#ifndef FIBONACCI_HEAP_ASCII_GRAPH_VISITOR_HPP
#define FIBONACCI_HEAP_ASCII_GRAPH_VISITOR_HPP

#include "fibonacciheapvisitor.hpp"

#include <vector>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <map>

template<class T> class FibonacciHeapASCIIGraphVisitor : public FibonacciHeapVisitor<T> {

public:
	FibonacciHeapASCIIGraphVisitor() : 
	_nodeWith(6), // support -99999 to +99999
	_nodeVerticalSpacing(3),
	_nodeHorizontalSpacing(3),
	_previousNode(0x0),
	_canvas(std::vector<std::vector<char> >()),
	_positions(std::map<FibonacciHeapNode<T> *, Point>()),
	_nodesPerDepth(std::map<int, std::vector<FibonacciHeapNode<T> *> >()),
	_edges(std::vector<Edge>()),
	depth(0) {
    	initCanvas();
	}


	virtual void visit(FibonacciHeapNode<T> *node) {
		if (_nodesPerDepth.count(depth) == 0) {
			_nodesPerDepth[depth] = std::vector<FibonacciHeapNode<T> *>();
		}
		Point location(_nodesPerDepth[depth].size() * _nodeHorizontalSpacing, depth * _nodeVerticalSpacing);
	}

	std::string getASCIIGraph() const {
		std::string result;
		for (int i = _canvas.size() - 1; i >= 0; --i) {
			for (int j = 0; j < _canvas.at(i).size(); ++j) {
				result += _canvas[i][j];
			}
			result += '\n';
		}
		return result;
	}

	int depth;


private:
	struct Point {
		int x;
		int y;

		Point(const int &x, const int &y) : x(x), y(y) {}
	};

	struct Edge {
		FibonacciHeapNode<T> *start;
		FibonacciHeapNode<T> *end;
	};

	const int _nodeWith;

	const int _nodeVerticalSpacing;

	const int _nodeHorizontalSpacing;

	FibonacciHeapNode<T> *_previousNode;

	std::vector<std::vector<char> > _canvas;

	std::map<FibonacciHeapNode<T> *, Point> _positions;

	std::map<int, std::vector<FibonacciHeapNode<T> *> > _nodesPerDepth;

	std::vector<Edge> _edges;

	int _canvasWidth;

	int _canvasHeight;

	void initCanvas() {
		struct winsize w;
    	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    	_canvasHeight = w.ws_row;
    	_canvasWidth = w.ws_col - 1;

		_canvas = std::vector<std::vector<char> >(_canvasHeight);
		
		for (int i = 0; i < _canvasHeight; ++i) {
			_canvas[i] = std::vector<char>(_canvasWidth);
			for (int j = 0; j < _canvasWidth; ++j) {
				_canvas[i][j] = ' ';
			}
		}
	}

	template<typename N> static std::string toString(const N &val) {
		std::ostringstream ss;
		ss.clear();
		ss << val;
		return ss.str();
	}

};

#endif 