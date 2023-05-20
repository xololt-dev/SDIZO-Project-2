#pragma once

#include <list>
#include <queue>
#include <string>
#include "util.hpp"

class Graph {
public:
	void readFromFile(std::string FileName);
	void generateGraph(int sideLength, int density, bool directed);
	void displayMSTMatrix();
	void displayMSTList();
	void algorithmPrimListN();
	void displayList(bool mst = false);
	void displayMatrix(bool mst = false);
protected:
	Matrix neighborMatrix;
	Matrix outputMatrix;
	List<List<Neighbor>> neighborList;
	List<List<Neighbor>> outputList;
	std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> prioQueue;
};