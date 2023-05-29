// Find Shortest Path
#pragma once

// #include "util.hpp"
#include "graph.hpp"

class FSP : public Graph{
public:
	void readFromFile(std::string FileName);
	void generateGraph(int sideLength, int density);

	void dijkstraMatrix(bool display = 0);
	void dijkstraList(bool display = 0);
	void fordBellmanMatrix(bool display = 0);
	void fordBellmanList(bool display = 0);
	void displayPath(int p, int* tab);

	void testFunc();

private:
	int startVertexIndex;
};