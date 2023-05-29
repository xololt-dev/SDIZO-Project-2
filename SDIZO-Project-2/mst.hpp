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
	void algorithmPrimMatrix(bool display = 0);
	void algorithmPrimList(bool display = 0);
	void algorithmKruskalMatrix(bool display = 0);
	void algorithmKruskalList(bool display = 0);

	void testFunc();
	
	void unionSet(int x, int y, int* rank, int* parent);
	int findSet(int x, int* parent);

private:

};