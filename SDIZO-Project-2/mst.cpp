// Minimal Spanning Tree
// Kruskal
// Prim
#include "prim_mst.hpp"

#include <string>
#include <fstream>
#include <iostream>
/*
Prim_MST::Prim_MST() {
	/*
	verticesChecked = verticesNotChecked = nullptr;
	edgesCollection = edgesMST = nullptr;
	prioQueue = nullptr;
	
}
*/
/*
Prim_MST::~Prim_MST() {
	verticesChecked.clear();
	verticesNotChecked.clear();
	edgesCollection.clear();
	edgesMST.clear();
	while (!prioQueue.empty()) prioQueue.pop();
	/*
	delete verticesChecked;
	delete verticesNotChecked;
	delete edgesCollection;
	delete edgesMST;
	delete prioQueue;

	verticesChecked = nullptr;
	verticesNotChecked = nullptr;
	edgesCollection = nullptr;
	edgesMST = nullptr;
	prioQueue = nullptr;
	
}*/

// za�adowa� liczb� kraw�dzi, liczb� wierzcho�k�w i ka�d� kraw�d�
void Prim_MST::readFromFile(std::string FileName) {
	std::fstream file;
	file.open(FileName, std::ios::in);

	if (file.good()) {
		int edgesAmount = 0, vertexAmount = 0;
		int temp = 0;

		// clear the values
		/*
		if (edgesCollection) {
			delete edgesCollection;
			edgesCollection = nullptr;
		}
		if (edgesMST) {
			delete edgesMST;
			edgesMST = nullptr;
		}
		if (verticesNotChecked) {
			delete verticesNotChecked;
			verticesNotChecked = nullptr;
		}
		if (verticesChecked) {
			delete verticesChecked;
			verticesChecked = nullptr;
		}
		if (prioQueue) {
			delete prioQueue;
			prioQueue = nullptr;
		}
		*/
		verticesChecked.clear();
		verticesNotChecked.clear();
		edgesCollection.clear();
		edgesMST.clear();
		while (!prioQueue.empty()) prioQueue.pop();

		// amount of edges in a graph
		file >> edgesAmount;
		// edgesCollection = new ListEdge;
		// edgesCollection.resize(edgesAmount);

		// amount of vertices in a graph
		file >> vertexAmount;
		// verticesNotChecked.resize(vertexAmount);
		/*
		verticesNotChecked = new List;
		verticesChecked = new List;
		edgesMST = new ListEdge;
		prioQueue = new HeapEdge;
		*/

		Edge tempEdge;
		// Zrobi� dodawanie po jednym elem.
		while (file >> temp) {
			tempEdge.source = temp;
			/*
			if (verticesNotChecked->listSize() < vertexAmount) {
				if (!(verticesNotChecked->IsValueInList(temp))) verticesNotChecked->push_backValue(temp);
			}*/
			if (verticesNotChecked.size() < vertexAmount) {
				bool dodaj = 1;
				for (auto const& i : verticesNotChecked) {
					if (i == temp) {
						dodaj = 0;
						break;
					}
				}
				if (!verticesNotChecked.size() || dodaj) verticesNotChecked.push_back(temp);
			}

			file >> temp;
			tempEdge.destination = temp;
			/*
			if (verticesNotChecked->listSize() < vertexAmount) {
				if (!(verticesNotChecked->IsValueInList(temp))) verticesNotChecked->push_backValue(temp);
			}
			*/
			if (verticesNotChecked.size() < vertexAmount) {
				bool dodaj = 1;
				for (auto const& i : verticesNotChecked) {
					if (i == temp) {
						dodaj = 0;
						break;
					}
				}
				if (dodaj) verticesNotChecked.push_back(temp);
			}

			file >> temp;
			tempEdge.weight = temp;

			edgesCollection.push_back(tempEdge);
		}
		verticesNotChecked.sort();
		file.close();
	}
	else std::cout << "Plik nie zostal otworzony!\n";
}

void Prim_MST::algorithm() {
	int numberOfVertex = verticesNotChecked.size() - 1;
	int vertexID = verticesNotChecked.front();
	std::list<int>::iterator begin = verticesNotChecked.begin();
	std::list<Edge>::iterator beginE = edgesCollection.begin();
	Edge temp;

	// przenoszenie wierzcho�k�w z not check do check
	verticesChecked.push_back(vertexID);
	for (begin = verticesNotChecked.begin(); begin != verticesNotChecked.end(); begin++) {
		if (*begin == vertexID) {
			verticesNotChecked.erase(begin);
			break;
		}
	}

	// dop�ki nie sprawdzimy ka�dego wierzcho�ka
	while (numberOfVertex) {
		// wype�nienie kolejki kraw�dziami
		if (!(edgesMST.size() + prioQueue.size() == edgesCollection.size())) {
			
			for (beginE = edgesCollection.begin(); beginE != edgesCollection.end(); beginE++) {
				bool doWstawienia = 1;

				std::list<Edge>::iterator beginS;
				for (beginS = edgesMST.begin(); beginS != edgesMST.end(); beginS++) {
					if (*beginS == *beginE) {
						doWstawienia = 0;
						break;
					}
				}

				if ((beginE->source == vertexID || beginE->destination == vertexID) && doWstawienia) {
					std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue = prioQueue;
					Edge e = { beginE->weight, beginE->source, beginE->destination };

					while (!prioQueue.empty()) {
						temp = prioQueue.top();
						// tempQueue.push(temp);
						prioQueue.pop();
						if (temp == e) {
							doWstawienia = 0;
							break;
						}
					}
					prioQueue = tempQueue;
					if (doWstawienia) prioQueue.push(e);
				}
			}
		}		

		Edge insertEdge = { INT_MAX, 0, 0 };
		int nmbrEdge = -1;
		// je�li lista nie pusta
		if (!edgesMST.empty()) {
			std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue = prioQueue;
			while (!prioQueue.empty()) {
				bool doWstawienia = 1;
				// wzi�cie kraw�dzi z heap
				temp = prioQueue.top();
				prioQueue.pop();

				if (temp.weight < insertEdge.weight) {
					nmbrEdge++;

					if (doWstawienia) {
						// trzeba sprawdzi� czy istnieje petla
						int vertexTakenIntoaccount = 0;
						for (begin = verticesChecked.begin(); begin != verticesChecked.end(); begin++) {
							if (temp.destination == *begin || temp.source == *begin) {
								vertexTakenIntoaccount++;
							}
							if (vertexTakenIntoaccount == 2) {
								doWstawienia = 0;
								break;
							}
						}
					}

					// nie powt�rzone
					if (doWstawienia) {
						insertEdge = temp;
					}
				}
				else break;
			}
			prioQueue = tempQueue;
		}
		else {
			nmbrEdge++;
			insertEdge = prioQueue.top();
		}

		// wstaw do drzewa
		edgesMST.push_back(insertEdge);

		if (!verticesNotChecked.empty()) {
			std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue;

			while (!prioQueue.empty()) {
				if (nmbrEdge != 0) {
					tempQueue.push(prioQueue.top());
				}
				prioQueue.pop();
				nmbrEdge--;
			}
			prioQueue = tempQueue;
			std::list<int> candidates = { insertEdge.source, insertEdge.destination };
			for (int v : verticesChecked) {
				for (begin = candidates.begin(); begin != candidates.end(); begin++) {
					if (v == *begin) {
						candidates.erase(begin);
						break;
					}
				}
			}
			if (candidates.empty() && !verticesNotChecked.empty()) vertexID = verticesNotChecked.front();
			else vertexID = candidates.front();

			// przenoszenie wierzcho�k�w z not check do check
			verticesChecked.push_back(vertexID);
			for (begin = verticesNotChecked.begin(); begin != verticesNotChecked.end(); begin++) {
				if (*begin == vertexID) {
					verticesNotChecked.erase(begin);
					break;
				}
			}
			numberOfVertex--;
		}
	}
}

void Prim_MST::displayMST() {
	if (edgesMST.empty()) std::cout << "Lista jest pusta." << std::endl;
	else {
		for (Edge e : edgesMST) {
			std::cout << e.source << " " << e.destination << " " << e.weight << "\n";
		}			
		std::cout << "\n" << std::endl;
	}
}