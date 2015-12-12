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
	createEdges();
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
		const Edge &edge = _edges[0];
		json += "\t\t" + toString(edge);
	}

	for (int i = 1; i < _edges.size(); ++i) {
		const Edge &edge = _edges[i];
		json += ",\n\t\t" + toString(edge);
	}

	json += "\n\t]";
	json += "\n}";

	return json;
}

private:
	class Edge {
	public:
		int startId;
		int endId;
		std::string type;

		Edge(const int &startId, const int &endId, const std::string &type) : startId(startId), endId(endId), type(type) {}
	};

	std::map<FibonacciHeapNode<T> *, int> _jsonIds;

	std::map<int, bool> _isRoot;
	
	std::vector<FibonacciHeapNode<T> *> _nodes;
	
	std::vector<Edge> _edges;

	std::string toString(FibonacciHeapNode<T> *node) {
		std::string result = "{";
		result += "\"caption\": \"" + ttos<T>(node->value) + "\", ";
		int jsonId = _jsonIds[node];
		result += "\"id\": " + ttos<int>(jsonId);	
		if (_isRoot[jsonId]) {
			result += ", \"root\": true";	
			result += ", \"type\": \"root\"";
		}
		result += "}";
		return result;
	}

	std::string toString(const Edge &edge) const {
		std::string result = "{\"source\": " + ttos<int>(edge.startId) + ", \"target\": " + ttos<int>(edge.endId) + ", \"caption\": \"" + edge.type + "\"}";
		return result;
	}

	void createEdge(const int &startJsonId, const int &endJsonId, const std::string &type) {
		const Edge edge(startJsonId, endJsonId, type);
		_edges.push_back(edge);
	}

	void createEdges() {
		for (int i = 0; i < _nodes.size(); ++i) {
			FibonacciHeapNode<T> *node = _nodes[i];
			int jsonId = _jsonIds[node];

			DoublyLinkedListItem<FibonacciHeapNode<T> *> *listItem = node->listItem;
			const DoublyLinkedListItem<FibonacciHeapNode<T> *> *leftSibling = listItem->previous;
			const DoublyLinkedListItem<FibonacciHeapNode<T> *> *rightSibling = listItem->next;

			if (leftSibling != 0x0) {
				int leftSiblingJsonId = _jsonIds[leftSibling->value];
				createEdge(jsonId, leftSiblingJsonId, "prev");
			}
			if (rightSibling != 0x0) {
				int rightSiblingJsonId = _jsonIds[rightSibling->value];
				createEdge(jsonId, rightSiblingJsonId, "next");
			}

			DoublyLinkedListItem<FibonacciHeapNode<T> *> *child = node->children.begin();
			while (child != 0x0) {
				FibonacciHeapNode<T> *childNode = child->value; 
				int childJsonId = _jsonIds[childNode];
				createEdge(jsonId, childJsonId, "child");
				child = child->next;

				int parentJsonId = _jsonIds[childNode->parent];
				createEdge(childJsonId, parentJsonId, "parent");
			}
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