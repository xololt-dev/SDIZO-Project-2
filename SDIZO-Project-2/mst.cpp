// Minimal Spanning Tree
// Kruskal
// Prim
#include "prim_mst.hpp"

#include <string>
#include <fstream>
#include <iostream>


// za³adowaæ liczbê krawêdzi, liczbê wierzcho³ków i ka¿d¹ krawêdŸ
void Prim_MST::readFromFile(std::string FileName) {
	std::fstream file;
	file.open(FileName, std::ios::in);

	if (file.good()) {
		int temp = 0;
		int i = 0;

		// clear the table
		if (cnt) {
			delete[] edgesCollection;
			delete[] verticesNotChecked;
			delete[] verticesChecked;
			verticesNotChecked = verticesChecked = nullptr;
			edgesCollection = nullptr;
		}

		// amount of edges in a graph
		file >> temp;
		edgesCollection = new Edge[temp];
		
		// space between (?)
		file >> temp;

		// amount of vertices in a graph
		file >> temp;
		verticesNotChecked = new int[temp];

		// Zrobiæ dodawanie po jednym elem.
		while (file >> temp) {
			tab[i] = temp;
			i++;
		}

		file.close();

		buildHeap();
	}
	else std::cout << "Plik nie zostal otworzony!\n";

	return 0;
}

void setEdges() {

}