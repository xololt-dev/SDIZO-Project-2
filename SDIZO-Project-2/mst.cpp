// Minimal Spanning Tree
// Kruskal
// Prim
#include "mst.hpp"

#include <string>
#include <fstream>
#include <iostream>

// za³adowaæ liczbê krawêdzi, liczbê wierzcho³ków i ka¿d¹ krawêdŸ
void MST::readFromFile(std::string FileName) {
	std::fstream file;
	file.open(FileName, std::ios::in);

	if (file.good()) {
		int edgesAmount = 0, vertexAmount = 0;
		int temp = 0;


		verticesChecked.clear();
		verticesNotChecked.clear();
		edgesCollection.clear();
		edgesMST.clear();
		while (!prioQueue.empty()) prioQueue.pop();

		// Iloœæ krawêdzi
		file >> edgesAmount;

		// Iloœæ wierzcho³ków
		file >> vertexAmount;


		Edge tempEdge;
		// Zrobiæ dodawanie po jednym elem.
		while (file >> temp) {
			tempEdge.source = temp;

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

void MST::generateGraph(int size, int density)
{

}

void MST::algorithmPrim() {
	if (verticesNotChecked.empty()) {
		std::copy(verticesChecked.begin(), verticesChecked.end(), std::back_inserter(verticesNotChecked));
		verticesNotChecked.sort();
		verticesChecked.clear();
		edgesMST.clear();
	}
	int numberOfVertex = verticesNotChecked.size() - 1;
	int vertexID = verticesNotChecked.front();
	std::list<int>::iterator begin = verticesNotChecked.begin();
	std::list<Edge>::iterator beginE = edgesCollection.begin();
	Edge temp;

	// przenoszenie wierzcho³ków z not check do check
	verticesChecked.push_back(vertexID);
	for (begin = verticesNotChecked.begin(); begin != verticesNotChecked.end(); begin++) {
		if (*begin == vertexID) {
			verticesNotChecked.erase(begin);
			break;
		}
	}

	// dopóki nie sprawdzimy ka¿dego wierzcho³ka
	while (numberOfVertex) {
		// wype³nienie kolejki krawêdziami
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
		// jeœli lista nie pusta
		if (!edgesMST.empty()) {
			std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue = prioQueue;
			while (!prioQueue.empty()) {
				bool doWstawienia = 1;
				// wziêcie krawêdzi z heap
				temp = prioQueue.top();
				prioQueue.pop();

				if (temp.weight < insertEdge.weight) {
					nmbrEdge++;

					if (doWstawienia) {
						// trzeba sprawdziæ czy istnieje petla
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

					// nie powtórzone
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

			// przenoszenie wierzcho³ków z not check do check
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
	while (!prioQueue.empty()) prioQueue.pop();
}

void MST::displayMST() {
	if (edgesMST.empty()) std::cout << "Lista MST jest pusta." << std::endl;
	else {
		int weightTotal = 0;
		for (Edge e : edgesMST) {
			std::cout << e.source << " " << e.destination << " " << e.weight << "\n";
			weightTotal += e.weight;
		}
		std::cout << "Waga drzewa rozpinajacego: " << weightTotal << std::endl;
	}
}

void MST::displayList() {
	if (edgesCollection.empty()) std::cout << "Lista jest pusta." << std::endl;
	else {
		std::list<int>::iterator it;
		if (verticesChecked.empty()) {
			for (it = verticesNotChecked.begin(); it != verticesNotChecked.end(); it++) {
				std::cout << *it << ": ";
				for (Edge e : edgesCollection) {
					if (e.source == *it) std::cout << e.destination << " ";
					else if (e.destination == *it) std::cout << e.source << " ";
				}
				std::cout << "\n";
			}
		}
		else {
			verticesChecked.sort();

			for (it = verticesChecked.begin(); it != verticesChecked.end(); it++) {
				std::cout << *it << ": ";
				for (Edge e : edgesCollection) {
					if (e.source == *it) std::cout << e.destination << " ";
					else if (e.destination == *it) std::cout << e.source << " ";
				}
				std::cout << "\n";
			}
		}
		std::cout << std::endl;
	}
}

void MST::displayMatrix() {
	if (edgesCollection.empty()) std::cout << "Lista jest pusta." << std::endl;
	else {
		std::list<int>::iterator it;
		if (verticesChecked.empty()) {
			std::cout << "   ";
			for (it = verticesNotChecked.begin(); it != verticesNotChecked.end(); it++) {
				std::cout << *it << " ";
			}
			std::cout << "\n";
			std::cout << " |";
			for (it = verticesNotChecked.begin(); it != verticesNotChecked.end(); it++) {
				std::cout << "--";
			}
			std::cout << "\n";
			for (it = verticesNotChecked.begin(); it != verticesNotChecked.end(); it++) {
				std::cout << *it << "| ";
				std::list<int>::iterator its;
				for (its = verticesNotChecked.begin(); its != verticesNotChecked.end(); its++) {
					if (its != it) {
						int exists = 0;
						for (Edge e : edgesCollection) {
							if (e.source == *it && e.destination == *its) {
								exists = 1;
								break;
							}
							else if (e.destination == *it && e.source == *its) {
								exists = 1;
								break;
							}
						}
						std::cout << exists << " ";
					}
					else std::cout << "0 ";
				}
				std::cout << "\n";
			}
		}
		else {
			verticesChecked.sort();

			std::cout << "   ";
			for (it = verticesChecked.begin(); it != verticesChecked.end(); it++) {
				std::cout << *it << " ";
			}
			std::cout << "\n";
			std::cout << " |";
			for (it = verticesChecked.begin(); it != verticesChecked.end(); it++) {
				std::cout << "--";
			}
			std::cout << "\n";
			for (it = verticesChecked.begin(); it != verticesChecked.end(); it++) {
				std::cout << *it << "| ";
				std::list<int>::iterator its;
				for (its = verticesChecked.begin(); its != verticesChecked.end(); its++) {
					if (its != it) {
						int exists = 0;
						for (Edge e : edgesCollection) {
							if (e.source == *it && e.destination == *its) {
								exists = 1;
								break;
							}
							else if (e.destination == *it && e.source == *its) {
								exists = 1;
								break;
							}
						}
						std::cout << exists << " ";
					}
					else std::cout << "0 ";
				}
				std::cout << "\n";
			}
		}
		std::cout << std::endl;
	}
}

MST::~MST()
{
	verticesChecked.clear();
	verticesNotChecked.clear();
	edgesCollection.clear();
	edgesMST.clear();
	while (!prioQueue.empty()) prioQueue.pop();
}
