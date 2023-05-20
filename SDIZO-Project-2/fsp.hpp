// Find Shortest Path
#pragma once

// #include "util.hpp"
#include "graph.hpp"

class FSP : public Graph{
public:
	void dijkstraMatrix();
	void dijkstraList();
	void fordBellmanMatrix();
	void fordBellmanList();

private:
	
};