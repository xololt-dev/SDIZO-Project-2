#pragma once

struct Edge {
	int weight;
	int source;
	int destination;
};

class Prim_MST {
public:
	void readFromFile(std::string FileName);
	void algorithm();

private:
	int* verticesChecked;		// tablica sprawdzonych
	int* verticesNotChecked;	// tablica niesprawdzonych
	Edge* edgesCollection;

	// int weight;
};


void setEdges();