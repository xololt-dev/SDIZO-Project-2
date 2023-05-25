#pragma once
// #include "util.hpp"
#include "graph.hpp"
#include <list>
#include <queue>
#include <string>

class MST : public Graph {
public:
	// MST();
	~MST();

	void readFromFile(std::string FileName);
	void generateGraph(int sideLength, int density);
	void algorithmPrimMatrix();
	std::list<std::list<Neighbor>> algorithmPrimListOld();
	void algorithmPrimList();

	void addToMatrix(int src, int dst, int weight, std::list<std::list<int>>& matrix);
	void addToList(int src, int dst, int weight, std::list<std::list<Neighbor>>& list);

private:

};