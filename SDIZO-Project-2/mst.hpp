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
		return (((destination == rhs.destination && source == rhs.source) || (destination == rhs.source && source == rhs.destination)) && weight == rhs.weight);
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

struct Neighbor {
	int weight;
	int destination;
};

class MST {
public:
	// MST();
	~MST();

	void readFromFile(std::string FileName);
	void readFromFileNew(std::string FileName);
	void generateGraph(int size, int density);
	void algorithmPrim();
	std::list<std::list<int>> algorithmPrimMatrix();
	std::list<std::list<Neighbor>> algorithmPrimList();
	void displayMST();
	void displayMSTMatrix(std::list<std::list<int>>& matrix);
	void displayMSTList(std::list<std::list<Neighbor>>& list);
	void displayList();
	void displayList2();
	void displayMatrix();
	void displayMatrix2();

	void addToMatrix(int src, int dst, int weight, std::list<std::list<int>>& matrix);
	void addToList(int src, int dst, int weight, std::list<std::list<Neighbor>>& list);

private:
	std::list<std::list<int>> neighborMatrix;
	std::list<std::list<Neighbor>> neighborList;
	std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> prioQueue;

	std::list<int> verticesChecked;
	std::list<int> verticesNotChecked;
	std::list<Edge> edgesCollection;
	std::list<Edge> edgesMST;
};