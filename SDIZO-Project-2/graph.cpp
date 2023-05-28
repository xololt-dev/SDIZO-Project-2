#include <iostream>
#include <fstream>
#include "graph.hpp"

// za³adowaæ liczbê krawêdzi, liczbê wierzcho³ków i ka¿d¹ krawêdŸ
void Graph::readFromFile(std::string FileName) {
	std::fstream file;
	file.open(FileName, std::ios::in);

	if (file.good()) {
		int edgesAmount = 0, vertexAmount = 0;
		int temp = 0;

		if (!neighborMatrix.empty()) neighborMatrix.clear();
		if (!neighborList.empty()) neighborList.clear();

		// Iloœæ krawêdzi
		file >> edgesAmount;

		// Iloœæ wierzcho³ków
		file >> vertexAmount;
		neighborList.generateEmpty(vertexAmount);
		neighborMatrix.generateEmpty(vertexAmount);

		Edge tempEdge;
		ListMember<List<Neighbor>>* iterList = neighborList.front();
		// Zrobiæ dodawanie po jednym elem.
		while (file >> tempEdge.source) {
			file >> tempEdge.destination;
			file >> tempEdge.weight;
			iterList = neighborList.front();

			if (tempEdge.source > tempEdge.destination) {
				for (int i = 0; i < tempEdge.destination; i++) iterList = iterList->next;
				iterList->data.push_back(Neighbor{ tempEdge.weight, tempEdge.source});
				for (int i = 0; i < (tempEdge.source - tempEdge.destination); i++) iterList = iterList->next;
				iterList->data.push_back(Neighbor{ tempEdge.weight, tempEdge.destination });
			}
			else {
				for (int i = 0; i < tempEdge.source; i++) iterList = iterList->next;
				iterList->data.push_back(Neighbor{ tempEdge.weight, tempEdge.destination });
				for (int i = 0; i < (tempEdge.destination - tempEdge.source); i++) iterList = iterList->next;
				iterList->data.push_back(Neighbor{ tempEdge.weight, tempEdge.source });
			}

			neighborMatrix.addValue(tempEdge.source, tempEdge.destination, tempEdge.weight);
			neighborMatrix.addValue(tempEdge.destination, tempEdge.source, tempEdge.weight);
		}
		file.close();
	}
	else std::cout << "Plik nie zostal otworzony!\n";
}

void Graph::displayList(bool mst) {
	if (mst) {
		if (outputList.empty()) std::cout << "Lista jest pusta." << std::endl;
		else {
			int lengthMST = 0;
			ListMember<List<Neighbor>>* it = outputList.front();
			ListMember<Neighbor>* itN = it->data.front();
			std::cout << "u->[v|w]\n";
			int i = 0;
			while (it) {
				std::cout << i++;
				itN = it->data.front();

				while (itN) {
					std::cout << "->[" << itN->data.destination << "|" << itN->data.weight << "]";
					lengthMST += itN->data.weight;
					itN = itN->next;
				}
				it = it->next;
				std::cout << "\n";
			}
			std::cout << "\nRozmiar drzewa: " << lengthMST / 2 << "\n\n";
		}
	}
	else {
		if (neighborList.empty()) std::cout << "Lista jest pusta." << std::endl;
		else {
			int lengthMST = 0;
			ListMember<List<Neighbor>>* it = neighborList.front();
			ListMember<Neighbor>* itN = it->data.front();
			std::cout << "u->[v|w]\n";
			int i = 0;
			while (it) {
				std::cout << i++;
				itN = it->data.front();
				while (itN) {
					std::cout << "->[" << itN->data.destination << "|" << itN->data.weight << "]";
					lengthMST += itN->data.weight;
					itN = itN->next;
				}
				it = it->next;
				std::cout << "\n";
			}
			std::cout << "\n\n";
		}
	}	
}

void Graph::displayMatrix(bool mst) {
	if (mst) {
		if (outputMatrix.empty()) std::cout << "Macierz jest pusta." << std::endl;
		else outputMatrix.display(1);
	}
	else {
		if (neighborMatrix.empty()) std::cout << "Macierz jest pusta." << std::endl;
		else neighborMatrix.display();
	}
}