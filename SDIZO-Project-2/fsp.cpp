#include "fsp.hpp"
#include <fstream>

void FSP::readFromFile(std::string FileName) {
	std::fstream file;
	file.open(FileName, std::ios::in);

	if (file.good()) {
		int edgesAmount = 0, vertexAmount = 0;
		int temp = 0;

		while (!prioQueue.empty()) prioQueue.pop();
		while (!prioQueueNew.empty()) prioQueueNew.pop();
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

			for (int i = 0; i < tempEdge.source; i++) iterList = iterList->next;
			iterList->data.push_back(Neighbor{ tempEdge.weight, tempEdge.destination, 0, 0 });

			neighborMatrix.addValue(tempEdge.source, tempEdge.destination, tempEdge.weight);
		}
		file.close();
	}
	else std::cout << "Plik nie zostal otworzony!\n";
}

void FSP::generateGraph(int sideLength, int density) {
	int random, edgesLeft = sideLength * (sideLength - 1) * density / 100;
	if (edgesLeft < sideLength || !sideLength || !density) return;

	// cleanup
	while (!prioQueue.empty()) prioQueue.pop();
	neighborList.clear();
	neighborMatrix.clear();

	int randValue = 1000;
	List<int> remainingVertexToPointTo;

	srand(time(NULL));

	neighborList.generateEmpty(sideLength);
	neighborMatrix.generateEmpty(sideLength);
	for (int i = 0; i < sideLength; i++) remainingVertexToPointTo.push_back(i);

	Edge e;
	ListMember<List<Neighbor>>* iterList = neighborList.front();
	/*
	for (int i = 0; i < sideLength; i++) {
		verticesNotChecked.push_back(i);
		if (i) {
			random = rand() % (verticesNotChecked.sideLength() - 1);
			e.weight = rand() % 100;
			e.source = i;
			e.destination = random;
			edgesCollection.push_back(e);
			edgesLeft--;
		}
	}*/
	// dojscie do kazdego wierzcholka
	for (int i = 0; i < sideLength; i++) {
		e.source = rand() % sideLength;
		e.destination = i;
		e.weight = 1 + rand() % randValue;
		
		iterList = neighborList.front();
		for (int i = 0; i < e.source; i++) iterList = iterList->next;

		iterList->data.push_back(Neighbor{ e.weight, e.destination, 0, 0 });
		neighborMatrix.addValue(e.source, e.destination, e.weight);
		edgesLeft--;
	}
	/*
	// ta czêœæ nie dokoñczona
	while (edgesLeft) {
		// wyznaczenie pierwszego wierzcho³ka
		do {
			random = rand() % sideLength;
			outsideL = neighborList.begin();
			for (int i = 0; i < random; i++) outsideL++;
		} while (outsideL->size() == sideLength - 1);

		std::list<int> exists;
		for (int i = 0; i < sideLength; i++) {
			if (i != random) exists.push_back(i);
		}

		insideL = outsideL->begin();

		// zostawiæ tylko mo¿liwoœci do wybrania
		while (insideL != outsideL->end()) {
			exists.remove(insideL->destination);
			insideL++;
		}

		// wyznaczenie celu
		int dest = rand() % exists.size();
		itt = exists.begin();
		for (int i = 0; i < dest; i++) itt++;

		dest = *itt;
		int weight = 1 + rand() % randValue;

		// wstawianie
		if (outsideL->begin()->destination != -1) {
			outsideL->push_back(Neighbor{ weight, dest });
		}
		else {
			*(outsideL->begin()) = Neighbor{ weight, dest };
		}
		outsideM = neighborMatrix.begin();
		for (int i = 0; i < random; i++) outsideM++;
		insideM = outsideM->begin();
		for (int i = 0; i < dest; i++) insideM++;
		*insideM = weight;

		outsideL = neighborList.begin();
		outsideM = neighborMatrix.begin();
		for (int i = 0; i < dest; i++) {
			outsideL++;
			outsideM++;
		}
		if (outsideL->begin()->destination != -1) {
			outsideL->push_back(Neighbor{ weight, random });
		}
		else {
			*(outsideL->begin()) = Neighbor{ weight, random };
		}
		insideM = outsideM->begin();
		for (int i = 0; i < random; i++) insideM++;
		*insideM = weight;

		edgesLeft--;
	}
	*/
}

/*
void FSP::dijkstraMatrix() {
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}
	
	int numberOfVertex = neighborMatrix.size();
	int vertexID = 0;

	Matrix outputMatrix;
	outputMatrix.clear();

	for (int j = 0; j < numberOfVertex; j++) {
		sub_list.push_back(0);
	}
	for (int i = 0; i < numberOfVertex; i++) {
		outputMatrix.push_back(sub_list);
	}

	numberOfVertex--;
}
*/

