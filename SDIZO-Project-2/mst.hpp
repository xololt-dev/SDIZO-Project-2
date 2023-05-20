#pragma once
#include "util.hpp"
#include <list>
#include <queue>
#include <string>

class MST {
public:
	// MST();
	~MST();

	void readFromFile(std::string FileName);
	void generateGraph(int sideLength, int density);
	std::list<std::list<int>> algorithmPrimMatrix();
	std::list<std::list<Neighbor>> algorithmPrimList();
	void displayMSTMatrix(std::list<std::list<int>>& matrix);
	void displayMSTList(std::list<std::list<Neighbor>>& list);
	void displayList();
	void displayMatrix();

	void addToMatrix(int src, int dst, int weight, std::list<std::list<int>>& matrix);
	void addToList(int src, int dst, int weight, std::list<std::list<Neighbor>>& list);

private:
	// zmieniæ na tablicê [][]
	std::list<std::list<int>> neighborMatrix;
	std::list<std::list<Neighbor>> neighborList;
	List<List<ListMember<Neighbor>>> myList;
	std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> prioQueue;
};