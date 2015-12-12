#ifndef FIBONACCI_HEAP_JSON_GRAPH_VISITOR_HPP
#define FIBONACCI_HEAP_JSON_GRAPH_VISITOR_HPP

#include "fibonacciheapvisitor.hpp"

#include <assert.h>
#include <sstream>
#include <set>

template<class T> class FibonacciHeapJSONGraphVisitor : public FibonacciHeapVisitor<T> {

public:

	virtual void visit(FibonacciHeapNode<T> *node) {
		assert(_jsonIds.count(node) == 0);
		_nodes.push_back(node);
		const int jsonId = _nodes.size() -1;
		_jsonIds[node] = jsonId;
		_isRoot[jsonId] = node->parent == 0x0;
	}

	std::string getJSONGraph() {
		std::string json = "{\n\t\"nodes\": [\n";

		if (_nodes.size()) {
			FibonacciHeapNode<T> *node = _nodes[0];
			json += "\t\t" + toString(node);
		}
		for (int i = 1; i < _nodes.size(); ++i) {
			FibonacciHeapNode<T> *node = _nodes[i];
			json += ",\n\t\t" + toString(node);
		}

		json += "\n\t],";
		json += "\n\t\"edges\": [\n";

		if (_edges.size()) {
			const std::pair<int,int> &edge = _edges[0];
			json += "\t\t" + toString(edge);
		}

		for (int i = 1; i < _edges.size(); ++i) {
			const std::pair<int,int> &edge = _edges[i];
			json += ",\n\t\t" + toString(edge);
		}
		
		json += "\t]\n";
		json += "}";

		return json;
	}

private:
	std::map<FibonacciHeapNode<T> *, int> _jsonIds;

	std::map<int, bool> _isRoot;
	
	std::vector<FibonacciHeapNode<T> *> _nodes;
	
	std::vector<std::pair<int,int> > _edges;

	std::set<std::pair<int,int> > _createdEdges;

	std::string toString(FibonacciHeapNode<T> *node) {
		std::string result = "{";
		result += "\"caption\": \"" + ttos<T>(node->value) + "\", ";
		int jsonId = _jsonIds[node];
		result += "\"id\": " + ttos<int>(jsonId);	
		if (_isRoot[jsonId]) {
			result += ", \"root\": true";	
		}
		result += "}";
		return result;
	}

	std::string toString(const std::pair<int,int> &edge) const {
		std::string result = "{\"source\": " + ttos<int>(edge.first) + ", \"target\": " + ttos<int>(edge.second) + "}";
		return result;
	}

	void createEdge(const int &startJsonId, const int &endJsonId) {
		const std::pair<int,int> edgeSE = std::make_pair(startJsonId, endJsonId);
		const std::pair<int,int> edgeES = std::make_pair(endJsonId, startJsonId);

		if (_createdEdges.count(edgeSE) || _createdEdges.count(edgeES)) {
			return;
		}

		_edges.push_back(edgeSE);
		_createdEdges.insert(edgeSE);
		_createdEdges.insert(edgeES);
	}

	void createEdges() {
		for (int i = 0; i < _nodes.size(); ++i) {
			const FibonacciHeapNode<T> *node = _nodes[i];
			int jsonId = _jsonIds[node];
			if (_isRoot[jsonId]) {
				DoublyLinkedListItem<FibonacciHeapNode<T> *> *listItem = node->listItem;
				const DoublyLinkedListItem<FibonacciHeapNode<T> *> *leftRoot = listItem->previous;
				const DoublyLinkedListItem<FibonacciHeapNode<T> *> *rightRoot = listItem->next;

				if (leftRoot != 0x0) {
					const int leftRootJsonId = _jsonIds[leftRoot->value];
					createEdge(jsonId, leftRootJsonId);
				}
				if (rightRoot != 0x0) {
					const int rightRootJsonId = _jsonIds[rightRoot->value];
					createEdge(jsonId, rightRootJsonId);
				}
			}

			DoublyLinkedListItem<FibonacciHeapNode<T> *> *child = node->children.begin();
			while (child != 0x0) {
				const FibonacciHeapNode<T> *childNode = child->value; 
				const int childJsonId = _jsonIds[childNode];
				createEdge(jsonId, childJsonId);
			}

			/*
			if (node->parent != 0x0) {
				const FibonacciHeapNode<T> *parentNode = node->parent; 
				const int parentJsonId = _jsonIds[parentNode];
				createEdge(jsonId, parentJsonId);
			}
			*/
		}
	}

	template<class N> static std::string ttos(const N &val) {
	std::ostringstream ss;
	ss.clear();
	ss << val;
	return ss.str();
}

};

#endif