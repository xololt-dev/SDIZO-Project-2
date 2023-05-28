#pragma once

#include <string>
#include "util.hpp"

class Graph {
public:
	virtual void readFromFile(std::string FileName) = 0;
	virtual void generateGraph(int sideLength, int density) = 0;
	void displayList(bool mst = false);
	void displayMatrix(bool mst = false);
	void freeHeap() {
		while (!prioQueueNew.empty()) prioQueueNew.pop();
		prioQueueNew.resize();
	}

protected:
	Matrix neighborMatrix;
	Matrix outputMatrix;
	List<List<Neighbor>> neighborList;
	List<List<Neighbor>> outputList;
	Heap prioQueueNew;
};