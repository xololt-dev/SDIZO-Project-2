#pragma once
// #include "dynamicCollections.hpp"
#include <list>
#include <queue>
#include <string>

struct Edge {
	int weight;
	int source;
	int destination;

	inline bool operator==(const Edge& rhs) {
		return (destination == rhs.destination && source == rhs.source && weight == rhs.weight);
	}
	inline bool operator!=(const Edge& rhs) {
		return (destination != rhs.destination || source != rhs.source || weight != rhs.weight);
	}
	inline bool operator<(const Edge& rhs) {
		return (weight < rhs.weight);
	}
	inline bool operator>(const Edge& rhs) {
		return (weight > rhs.weight);
	}
	inline bool operator<=(const Edge& rhs) {
		return (weight <= rhs.weight);
	}
	inline bool operator>=(const Edge& rhs) {
		return (weight >= rhs.weight);
	}
};

inline bool operator==(const Edge& lhs, const Edge& rhs) {
	return (lhs.destination == rhs.destination && lhs.source == rhs.source && lhs.weight == rhs.weight);
}
inline bool operator<(const Edge& lhs, const Edge& rhs) {
	return (lhs.weight < rhs.weight);
}
inline bool operator>(const Edge& lhs, const Edge& rhs) {
	return (lhs.weight > rhs.weight);
}

class MST {
public:
	void readFromFile(std::string FileName);
	void algorithmPrim();
	void displayMST();
	void displayList();
	void displayMatrix();

private:
	//MST();
	//~MST();
	/*
	List verticesChecked;		// tablica sprawdzonych
	List verticesNotChecked;	// tablica niesprawdzonych
	ListEdge edgesCollection;
	ListEdge edgesMST;
	HeapEdge prioQueue;
	*/
	
	std::list<int> verticesChecked;
	std::list<int> verticesNotChecked;
	std::list<Edge> edgesCollection;
	std::list<Edge> edgesMST;
	std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> prioQueue;
};