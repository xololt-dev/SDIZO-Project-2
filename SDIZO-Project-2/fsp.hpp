// Find Shortest Path
#pragma once

// #include "util.hpp"
#include "graph.hpp"

class FSP : public Graph{
public:
	void readFromFile(std::string FileName);
	void generateGraph(int sideLength, int density);

	void dijkstraMatrix();
	void dijkstraList();
	void fordBellmanMatrix();
	void fordBellmanList();

private:
	
};