// Minimal Spanning Tree
// Kruskal
// Prim
#include "mst.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>

MST::~MST()
{
	verticesChecked.clear();
	verticesNotChecked.clear();
	edgesCollection.clear();
	edgesMST.clear();
	while (!prioQueue.empty()) prioQueue.pop();
	neighborMatrix.clear();
	neighborList.clear();
}

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

void MST::readFromFileNew(std::string FileName) {
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
		if (!neighborList.empty()) neighborList.clear();
		if (!neighborMatrix.empty()) neighborMatrix.clear();

		// Iloœæ krawêdzi
		file >> edgesAmount;

		// Iloœæ wierzcho³ków
		file >> vertexAmount;
		std::list<int> sub_list;
		for (int j = 0; j < vertexAmount; j++) {
			sub_list.push_back(0);
		}
		for (int i = 0; i < vertexAmount; i++) {
			neighborMatrix.push_back(sub_list);
			std::list<Neighbor> temp = { Neighbor{-1, -1 } };
			neighborList.push_back(temp);
		}

		Edge tempEdge;
		std::list<std::list<int>>::iterator itr = neighborMatrix.begin();
		std::list<int>::iterator itrSecond = itr->begin();
		std::list<std::list<Neighbor>>::iterator itrNeigh;
		// Zrobiæ dodawanie po jednym elem.
		while (file >> tempEdge.source) {
			file >> tempEdge.destination;
			file >> tempEdge.weight;
			itr = neighborMatrix.begin();
			itrNeigh = neighborList.begin();
			for (int i = 0; i < tempEdge.source; i++) {
				itr++;
				itrNeigh++;
			}

			if (itrNeigh->front().weight == -1 && itrNeigh->front().destination == -1) itrNeigh->front() = Neighbor{ tempEdge.weight, tempEdge.destination };
			else itrNeigh->push_back(Neighbor{ tempEdge.weight, tempEdge.destination });
			
			itrSecond = itr->begin();
			for (int i = 0; i < tempEdge.destination; i++) itrSecond++;
			
			*itrSecond = tempEdge.weight;

			// dodanie do drugiej czêœci

			itr = neighborMatrix.begin();
			itrNeigh = neighborList.begin();
			for (int i = 0; i < tempEdge.destination; i++) {
				itr++;
				itrNeigh++;
			}

			if (itrNeigh->front().weight == -1 && itrNeigh->front().destination == -1) itrNeigh->front() = Neighbor{ tempEdge.weight, tempEdge.source };
			else itrNeigh->push_back(Neighbor{ tempEdge.weight, tempEdge.source });

			itrSecond = itr->begin();
			for (int i = 0; i < tempEdge.source; i++) itrSecond++;

			*itrSecond = tempEdge.weight;
		}
		file.close();
	}
	else std::cout << "Plik nie zostal otworzony!\n";
}

void MST::generateGraph(int size, int density)
{	
	if (!size || !density) return;
	// cleanup
	verticesNotChecked.clear();
	verticesChecked.clear();
	edgesMST.clear();
	edgesCollection.clear();

	srand(time(NULL));

	int random, edgesLeft = size * (size - 1) * density / 200;
	Edge e;
	for (int i = 0; i < size; i++) {
		verticesNotChecked.push_back(i);
		if (i) {
			random = rand()%(verticesNotChecked.size() - 1);
			e.weight = rand() % 100;
			e.source = i;
			e.destination = random;
			edgesCollection.push_back(e);
			edgesLeft--;
		}
	}

	std::list<Edge>::iterator it = edgesCollection.begin();
	std::list<int>::iterator itt;
	while (edgesLeft) {
		random = rand() % verticesNotChecked.size();
		std::list<int> exists;
		
		for (it = edgesCollection.begin(); it != edgesCollection.end(); it++) {
			if (it->source == random) {
				exists.push_back(it->destination);
			}
			else if (it->destination == random) exists.push_back(it->source);
		}
		if (!(exists.size() == verticesNotChecked.size() - 1)) {
			int dest = 0;
			exists.sort();
			for (int e : exists) {
				if (dest != random && e != dest) break;
				while (e == dest || dest == random) dest++;
			}

			e.weight = rand() % 100;
			e.source = random;
			e.destination = dest;
			edgesCollection.push_back(e);
			edgesLeft--;
		}		
	} 
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

std::list<std::list<int>> MST::algorithmPrimMatrix() {
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}

	// auto start = std::chrono::system_clock::now();
	int numberOfVertex = neighborMatrix.size();
	int vertexID = 0;
	std::list<int> sub_list;
	std::list<std::list<int>> outputMatrix;
	outputMatrix.clear();

	for (int j = 0; j < numberOfVertex; j++) {
		sub_list.push_back(0);
	}
	for (int i = 0; i < numberOfVertex; i++) {
		outputMatrix.push_back(sub_list);
	}

	numberOfVertex--;

	// dopóki nie sprawdzimy ka¿dego wierzcho³ka
	while (numberOfVertex) {
		std::list<std::list<int>>::iterator outside = neighborMatrix.begin();

		// wype³nienie kolejki krawêdziami
		int i = 0;
		for (outside = neighborMatrix.begin(); outside != neighborMatrix.end(); ++outside) {
			std::list<int>::iterator inside;

			// jeœli jest to wierzcho³ek który chcemy dodaæ
			if (i == vertexID) {
				int j = 0;
				Edge temp;
				temp.source = i;
				for (inside = outside->begin(); inside != outside->end(); inside++) {
					// jesli po³¹czenie
					if (*inside) {
						bool doWstawienia = 1;
						std::list<Edge>::iterator beginS;
						temp.destination = j;
						temp.weight = *inside;

						// jesli bylo brane pod uwage
						std::list<std::list<int>>::iterator someiter = outputMatrix.begin();
						// int src = temp.source;
						// int dst = temp.destination;
						for (int i = 0; i < temp.source; i++) someiter++;

						std::list<int>::iterator someiterInner = someiter->begin();
						for (int i = 0; i < temp.destination; i++) someiterInner++;

						if (*someiterInner) {
							doWstawienia = 0;
						}
						/* jesli dobra implementacja to ta czesc nie powinna byæ potrzebna
						else {
							someiter = outputMatrix.begin();
							// src = temp.destination;
							// dst = temp.source;
							for (int i = 0; i < temp.destination; i++) someiter++;

							someiterInner = someiter->begin();
							for (int i = 0; i < temp.source; i++) someiterInner++;

							if (*someiterInner) {
								doWstawienia = 0;
							}
						}*/

						// jesli jest w kopcu
						if (doWstawienia) {
							Edge e;
							std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue = prioQueue;
							while (!prioQueue.empty()) {
								e = prioQueue.top();
								prioQueue.pop();
								if (temp == e) {
									doWstawienia = 0;
									break;
								}
							}
							prioQueue = tempQueue;
						}

						if (doWstawienia) prioQueue.push(temp);
					}
					j++;
				}
				break;
			}
			i++;
		}

		// wybranie krawêdzi
		bool petla = 0;
		int src, dest;
		Edge toAdd;
		do {
			toAdd = prioQueue.top();
			prioQueue.pop();

			std::list<std::list<int>>::iterator outsideSecond = outputMatrix.begin();
			src = toAdd.source;
			dest = toAdd.destination;
			petla = 0;

			outside = outputMatrix.begin();

			for (int i = 0; i < src; i++) outside++;
			for (int i = 0; i < dest; i++) outsideSecond++;

			std::list<int>::iterator inside = outside->begin();
			std::list<int>::iterator insideS = outsideSecond->begin();

			int totalSrc = 0, totalDest = 0;

			for (inside; inside != outside->end(); inside++) {
				totalSrc += *inside;
				totalDest += *insideS;
				
				if ((*inside > 0 && *insideS > 0) || (totalSrc && totalDest)) {
					petla = 1;
					break;
				}				
				insideS++;
			}

			// if (totalSrc && totalDest) petla = 1;
		} while (petla);

		// dodaj do matrycy
		addToMatrix(toAdd.source, toAdd.destination, toAdd.weight, outputMatrix);

		vertexID = toAdd.destination;
		numberOfVertex--;
	}
	while (!prioQueue.empty()) prioQueue.pop();

	// auto end = std::chrono::system_clock::now();
	// auto elapsed = end - start;
	// std::cout << elapsed.count() << '\n';

	return outputMatrix;
}

std::list<std::list<Neighbor>> MST::algorithmPrimList() {
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}

	// auto start = std::chrono::system_clock::now();
	int numberOfVertex = neighborList.size();
	int vertexEdgesToAdd = 0;
	std::list<Neighbor> sub_list = { Neighbor{ -1, -1 } };
	std::list<std::list<Neighbor>> outputList;
	outputList.clear();

	for (int i = 0; i < numberOfVertex; i++) {
		outputList.push_back(sub_list);
	}

	numberOfVertex--;

	// dopóki nie sprawdzimy ka¿dego wierzcho³ka
	while (numberOfVertex) {
		std::list<std::list<Neighbor>>::iterator outside = neighborList.begin();

		// wype³nienie kolejki krawêdziami
		int i = 0;
		for (outside = neighborList.begin(); outside != neighborList.end(); ++outside) {
			std::list<Neighbor>::iterator inside;

			// jeœli jest to wierzcho³ek który chcemy dodaæ
			if (i == vertexEdgesToAdd) {
				// int j = 0;
				Edge temp;
				temp.source = i;
				for (inside = outside->begin(); inside != outside->end(); inside++) {
					// jesli po³¹czenie
					if (inside->weight != -1) {
						bool doWstawienia = 1;
						std::list<Edge>::iterator beginS;
						temp.destination = inside->destination;
						temp.weight = inside->weight;

						// jesli bylo brane pod uwage
						std::list<std::list<Neighbor>>::iterator outerIter = outputList.begin();
						int src = temp.source;
						int dst = temp.destination;
						for (int i = 0; i < src; i++) outerIter++;

						std::list<Neighbor>::iterator someiterInner = outerIter->begin();

						for (someiterInner; someiterInner != outerIter->end(); someiterInner++) {
							if (someiterInner->destination == dst) {
								doWstawienia = 0;
								break;
							}
						}

						if (doWstawienia) {
							// jesli jest w kopcu
							Edge e;
							std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue = prioQueue;
							while (!prioQueue.empty()) {
								e = prioQueue.top();
								prioQueue.pop();
								if (temp == e) {
									doWstawienia = 0;
									break;
								}
							}
							prioQueue = tempQueue;
						}

						if (doWstawienia) prioQueue.push(temp);
					}
				}
				break;
			}
			i++;
		}

		// wybranie krawêdzi
		bool petla = 0;
		int src, dest;
		Edge toAdd;
		do {
			toAdd = prioQueue.top();
			prioQueue.pop();
			src = toAdd.source;
			dest = toAdd.destination;
			petla = 0;

			std::list<std::list<Neighbor>>::iterator outsideSecond = outputList.begin();
			
			outside = outputList.begin();

			for (int i = 0; i < src; i++) outside++;
			for (int i = 0; i < dest; i++) outsideSecond++;

			std::list<Neighbor>::iterator inside = outside->begin();
			std::list<Neighbor>::iterator insideS = outsideSecond->begin();

			if (inside->destination == -1 || insideS->destination == -1) {
				for (inside; inside != outside->end(); inside++) {
					for (insideS; insideS != outsideSecond->end(); insideS++) {
						if (inside->destination == insideS->destination && inside->destination != -1) {
							petla = 1;
							break;
						}
					}

					if (petla) break;
					else insideS = outsideSecond->begin();
				}
			}
			else petla = 1;
		} while (petla);

		// dodaj do matrycy
		addToList(toAdd.source, toAdd.destination, toAdd.weight, outputList);

		outside = outputList.begin();

		int currentVertex = vertexEdgesToAdd;
		vertexEdgesToAdd = -1;
		numberOfVertex = 0;
		for (int i = 0; i < outputList.size(); i++) {
			if (i == toAdd.destination && outside->size() == 1) {
				vertexEdgesToAdd = toAdd.destination;
			}
			if (outside->front().destination == -1) {
				if (vertexEdgesToAdd == -1) {
					vertexEdgesToAdd = i;
				}
				numberOfVertex++;
			}
			outside++;
		}
	}
	while (!prioQueue.empty()) prioQueue.pop();

	// auto end = std::chrono::system_clock::now();
	// auto elapsed = end - start;
	// std::cout << elapsed.count() << '\n';

	return outputList;
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
	
		// matrix
		verticesChecked.sort();
		std::list<int>::iterator it;
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
					for (Edge e : edgesMST) {
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
}

void MST::displayMSTMatrix(std::list<std::list<int>>& matrix) {
	if (matrix.empty()) std::cout << "Macierz jest pusta." << std::endl;
	else {
		int spacesNeeded = log10(neighborMatrix.size()) + 1;

		// nested_list`s iterator(same type as nested_list)
		// to iterate the nested_list
		std::list<std::list<int>>::iterator outside = matrix.begin();
		int i = 0;
		for (int s = -1; s < spacesNeeded; s++) std::cout << " ";

		while (outside != matrix.end()) {
			std::cout << i++ << " ";
			outside++;
		}
		std::cout << "\n";

		for (int s = 0; s < spacesNeeded; s++) std::cout << " ";
		std::cout << "|";

		i *= 2;
		for (--i; i > 0; i--) std::cout << "-";
		std::cout << "\n";

		// Print the nested_list
		for (outside = matrix.begin(); outside != matrix.end(); ++outside) {
			std::cout << i++;
			for (int s = 1; s < spacesNeeded; s++) std::cout << " ";
			std::cout << "|";

			// normal_list`s iterator(same type as temp_list)
			// to iterate the normal_list
			std::list<int>::iterator inside;

			// pointer of each list one by one in nested list
			// as loop goes on
			std::list<int>& inside_pointer = *outside;

			for (inside = inside_pointer.begin(); inside != inside_pointer.end(); inside++) {
				std::cout << *inside << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
}

void MST::displayMSTList(std::list<std::list<Neighbor>>& list) {
	if (list.empty()) std::cout << "Lista jest pusta." << std::endl;
	else {
		std::list<std::list<Neighbor>>::iterator it;
		std::list<Neighbor>::iterator itN;
		std::cout << "u->[v|w]\n";
		int i = 0;
		for (it = list.begin(); it != list.end(); it++) {
			std::cout << i++;

			for (Neighbor n : *it) {
				std::cout << "->[" << n.destination << "|" << n.weight << "]";
			}
			std::cout << "\n";
		}
		std::cout << std::endl;
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

void MST::displayList2() {
	if (neighborList.empty()) std::cout << "Lista jest pusta." << std::endl;
	else {
		std::list<std::list<Neighbor>>::iterator it;
		std::list<Neighbor>::iterator itN;
		std::cout << "u->[v|w]\n";
		int i = 0;
		for (it = neighborList.begin(); it != neighborList.end(); it++) {
			std::cout << i++;
			
			for (Neighbor n : *it) {
				std::cout << "->[" << n.destination << "|" << n.weight << "]";
			}
			std::cout << "\n";
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

void MST::displayMatrix2() {
	if (neighborMatrix.empty()) std::cout << "Macierz jest pusta." << std::endl;
	else {
		int spacesNeeded = log10(neighborMatrix.size()) + 1;

		// nested_list`s iterator(same type as nested_list)
		// to iterate the nested_list
		std::list<std::list<int>>::iterator outside = neighborMatrix.begin();
		int i = 0;
		for (int s = -1; s < spacesNeeded; s++) std::cout << " ";

		while (outside != neighborMatrix.end()) {
			std::cout << i++ << " ";
			outside++;
		}
		std::cout << "\n";
		
		for (int s = 0; s < spacesNeeded; s++) std::cout << " ";
		std::cout << "|";

		i *= 2;
		for (--i; i > 0; i--) std::cout << "-";
		std::cout << "\n";

		// Print the nested_list
		for (outside = neighborMatrix.begin(); outside != neighborMatrix.end(); ++outside) {
			std::cout << i++;
			for (int s = 1; s < spacesNeeded; s++) std::cout << " ";
			std::cout << "|";

			// normal_list`s iterator(same type as temp_list)
			// to iterate the normal_list
			std::list<int>::iterator inside;

			// pointer of each list one by one in nested list
			// as loop goes on
			std::list<int>& inside_pointer = *outside;

			for (inside = inside_pointer.begin(); inside != inside_pointer.end(); inside++) {
				std::cout << *inside << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
}

void MST::addToMatrix(int src, int dst, int weight, std::list<std::list<int>>& matrix) {
	std::list<std::list<int>>::iterator outside = matrix.begin();

	for (int i = 0; i < src; i++) outside++;

	std::list<int>::iterator inside = outside->begin();

	for (int i = 0; i < dst; i++) inside++;
	*inside = weight;

	outside = matrix.begin();

	for (int i = 0; i < dst; i++) outside++;

	inside = outside->begin();

	for (int i = 0; i < src; i++) inside++;
	*inside = weight;
}

void MST::addToList(int src, int dst, int weight, std::list<std::list<Neighbor>>& list) {
	std::list<std::list<Neighbor>>::iterator outside = list.begin();

	if (src > dst) {
		int temp = src;
		src = dst;
		dst = temp;
	}

	for (int i = 0; i < src; i++) outside++;

	if (outside->front().destination == -1) outside->front() = Neighbor{ weight, dst };
	else outside->push_back(Neighbor{ weight, dst });

	for (int i = 0; i < (dst - src); i++) outside++;

	if (outside->front().destination == -1) outside->front() = Neighbor{ weight, src };
	else outside->push_back(Neighbor{ weight, src });
}