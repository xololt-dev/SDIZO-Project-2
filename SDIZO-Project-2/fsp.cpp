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

	int randValue = 9;
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
		e.destination = i;
		do {
			e.source = rand() % sideLength;
		} while (e.source == i);
		e.weight = 1 + rand() % randValue;
		
		iterList = neighborList.front();
		for (int i = 0; i < e.source; i++) iterList = iterList->next;

		iterList->data.push_back(Neighbor{ e.weight, e.destination, 0, 0 });
		neighborMatrix.addValue(e.source, e.destination, e.weight);
		edgesLeft--;
	}
	
	// ta czêœæ nie dokoñczona
	while (edgesLeft) {
		// wyznaczenie pierwszego wierzcho³ka
		do {
			random = rand() % sideLength;
			iterList = neighborList.front();
			for (int i = 0; i < random; i++) iterList = iterList->next;
		} while (iterList->data.size() >= sideLength - 1);

		List<int> exists;
		for (int i = 0; i < sideLength; i++) {
			if (i != random) exists.push_back(i);
		}

		ListMember<Neighbor>* inner = iterList->data.front();
		
		// zostawiæ tylko mo¿liwoœci do wybrania
		while (inner) {
			exists.deleteFromList(inner->data.destination);
			inner = inner->next;
		}

		// wyznaczenie celu
		ListMember<int>* intCount = exists.front();
		int dest = -1;
		do {
			dest = rand() % exists.size();
			for (int i = 0; i < dest; i++) intCount = intCount->next;

			dest = intCount->data;
		} while (dest == random);

		int weight = 1 + rand() % randValue;

		// wstawianie
		iterList->data.push_back(Neighbor{ weight, dest, 0, 0 });
		neighborMatrix.addValue(random, dest, weight);

		edgesLeft--;
	}
}

void FSP::dijkstraMatrix(bool display) {
	// setup
	int numberOfVertex = neighborMatrix.size();

	// wierzcholki pozostale = q, s = wierzcholki odwiedzone
	List<int> q, s;
	for (int i = 0; i < numberOfVertex; i++) q.push_back(i);

	int* d = new int[numberOfVertex];
	int* p = new int[numberOfVertex];
	for (int i = 0; i < numberOfVertex; i++) {
		p[i] = -1;
		if (i) d[i] = INT_MAX;
		else d[i] = 0;
	}
	
	// wlasciwy algorytm
	while (!q.empty()) {
		// znajdz najmniejszy koszt dojscia
		int lowestIndex = 0;
		int lowestAmount = INT_MAX;
		for (int i = 0; i < numberOfVertex; i++) {
			if (d[i] < lowestAmount) {
				ListMember<int>* frnt = s.front();
				bool input = 1;
				while (frnt) {
					if (frnt->data == i) {
						input = 0;
						break;
					}
					frnt = frnt->next;
				}
				if (input) {
					lowestIndex = i;
					lowestAmount = d[i];
				}
			}
		}

		// usun ze zbioru q, wstaw do s
		q.deleteFromList(lowestIndex);
		s.push_back(lowestIndex);

		for (int i = 0; i < numberOfVertex; i++) {
			if (i != lowestIndex) {
				int weight = neighborMatrix.valueInPosition(lowestIndex, i);
				if (d[i] > lowestAmount + weight && weight) {
					d[i] = lowestAmount + weight;
					p[i] = lowestIndex;
				}
			}
		}
	}
	
	if (display) {
		std::cout << "u ";
		for (int i = 0; i < numberOfVertex; i++) {
			std::cout << i << " ";
		}
		std::cout << "\nd[u] ";
		for (int i = 0; i < numberOfVertex; i++) {
			std::cout << d[i] << " ";
		}
		std::cout << "\np[u] ";
		for (int i = 0; i < numberOfVertex; i++) {
			std::cout << p[i] << " ";
		}
		std::cout << "\n";
	}

	// release resources
	delete[] d;
	delete[] p;
}

void FSP::dijkstraList(bool display) {
	// setup
	int numberOfVertex = neighborList.size();

	// wierzcholki pozostale = q, s = wierzcholki odwiedzone
	List<int> q, s;
	for (int i = 0; i < numberOfVertex; i++) q.push_back(i);

	int* d = new int[numberOfVertex];
	int* p = new int[numberOfVertex];
	for (int i = 0; i < numberOfVertex; i++) {
		p[i] = -1;
		if (i) d[i] = INT_MAX;
		else d[i] = 0;
	}

	// wlasciwy algorytm
	while (!q.empty()) {
		// znajdz najmniejszy koszt dojscia
		int lowestIndex = 0;
		int lowestAmount = INT_MAX;
		for (int i = 0; i < numberOfVertex; i++) {
			if (d[i] < lowestAmount) {
				ListMember<int>* frnt = s.front();
				bool input = 1;
				while (frnt) {
					if (frnt->data == i) {
						input = 0;
						break;
					}
					frnt = frnt->next;
				}
				if (input) {
					lowestIndex = i;
					lowestAmount = d[i];
				}
			}
		}

		// usun ze zbioru q, wstaw do s
		q.deleteFromList(lowestIndex);
		s.push_back(lowestIndex);

		ListMember<List<Neighbor>>* iterator = neighborList.front();
		int i = 0;
		while (iterator) {
			if (i == lowestIndex) {
				ListMember<Neighbor>* innerIter = iterator->data.front();
				while (innerIter) {
					int weight = innerIter->data.weight;
					int index = innerIter->data.destination;// neighborMatrix.valueInPosition(lowestIndex, i);
					if (d[index] > lowestAmount + weight && weight) {
						d[index] = lowestAmount + weight;
						p[index] = lowestIndex;
					}
					innerIter = innerIter->next;
				}
				break;
			}
			iterator = iterator->next;
			i++;
		}
	}

	if (display) {
		std::cout << "u ";
		for (int i = 0; i < numberOfVertex; i++) {
			std::cout << i << " ";
		}
		std::cout << "\nd[u] ";
		for (int i = 0; i < numberOfVertex; i++) {
			std::cout << d[i] << " ";
		}
		std::cout << "\np[u] ";
		for (int i = 0; i < numberOfVertex; i++) {
			std::cout << p[i] << " ";
		}
		std::cout << "\n";
	}

	// release resources
	delete[] d;
	delete[] p;
}
