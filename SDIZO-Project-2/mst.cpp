// Minimal Spanning Tree
// Kruskal
// Prim

// Prim mo¿e inny sposób na liczenie kroków do koñca bêdzie szybszy
// Poprawiæ macierz na tablicê[][]
#include "mst.hpp"

#include <fstream>
#include <iostream>
#include <chrono>

MST::~MST()
{
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
			std::list<Neighbor> temp = { Neighbor{ -1, -1 } };
			neighborList.push_back(temp);
		}
		myList.generateEmpty(vertexAmount);

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

void MST::generateGraph(int sideLength, int density)
{
	if (!sideLength || !density) return;
	// cleanup
	while (!prioQueue.empty()) prioQueue.pop();
	neighborList.clear();
	neighborMatrix.clear();

	int randValue = 1000;

	srand(time(NULL));

	std::list<int> sub_list;
	for (int j = 0; j < sideLength; j++) {
		sub_list.push_back(0);
	}
	for (int i = 0; i < sideLength; i++) {
		neighborMatrix.push_back(sub_list);
		std::list<Neighbor> temp = { Neighbor{ -1, -1 } };
		neighborList.push_back(temp);
	}

	int random, edgesLeft = sideLength * (sideLength - 1) * density / 200;
	std::list<std::list<Neighbor>>::iterator outsideL = neighborList.begin();
	std::list<std::list<int>>::iterator outsideM = neighborMatrix.begin();
	std::list<Neighbor>::iterator insideL;
	std::list<int>::iterator insideM;
	Edge e;
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
	for (int i = 0; i < sideLength - 1; i++) {
		if (i) {
			e.source = i;
			e.destination = rand() % i;
			e.weight = 1 + rand() % randValue;

			outsideL = neighborList.begin();
			outsideM = neighborMatrix.begin();
			for (int j = 0; j < e.destination; j++) {
				outsideL++;
				outsideM++;
			}

			if (outsideL->begin()->destination == -1) {
				*(outsideL->begin()) = Neighbor{ e.weight, e.source };
			}
			else {
				outsideL->push_back(Neighbor{ e.weight, e.source });
			}

			insideM = outsideM->begin();
			for (int j = 0; j < e.source; j++) insideM++;

			*insideM = e.weight;

			for (int j = 0; j < (e.source - e.destination); j++) {
				outsideL++;
				outsideM++;
			}

			if (outsideL->begin()->destination == -1) {
				*(outsideL->begin()) = Neighbor{ e.weight, e.destination };
			}
			else {
				outsideL->push_back(Neighbor{ e.weight, e.destination });
			}

			insideM = outsideM->begin();
			for (int j = 0; j < e.destination; j++) insideM++;

			*insideM = e.weight;
			edgesLeft--;
		}
		else {
			e.source = i;
			e.destination = i + 1;
			e.weight = 1 + rand() % randValue;

			outsideL = neighborList.begin();
			outsideM = neighborMatrix.begin();
			for (int j = 0; j < e.source; j++) {
				outsideL++;
				outsideM++;
			}

			if (outsideL->begin()->destination == -1) {
				*(outsideL->begin()) = Neighbor{ e.weight, e.destination };
			}
			else {
				outsideL->push_back(Neighbor{ e.weight, e.destination });
			}

			insideM = outsideM->begin();
			for (int j = 0; j < e.destination; j++) insideM++;

			*insideM = e.weight;

			outsideL++;
			
			if (outsideL->begin()->destination == -1) {
				*(outsideL->begin()) = Neighbor{ e.weight, e.source };
			}
			else {
				outsideL->push_back(Neighbor{ e.weight, e.source });
			}

			outsideM++;
			insideM = outsideM->begin();
			for (int j = 0; j < e.source; j++) insideM++;
			
			*insideM = e.weight;

			edgesLeft--;
			i++;
		}		
	}

	// ta czêœæ nie dokoñczona

	std::list<int>::iterator itt;
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
}

std::list<std::list<int>> MST::algorithmPrimMatrix() {
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}

	auto start = std::chrono::system_clock::now();
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

						// jesli jest w kopcu
						if (doWstawienia) {
							Edge e;
							std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue = prioQueue;
							while (!prioQueue.empty()) {
								e = prioQueue.top();
								prioQueue.pop();
								if (e.weight > temp.weight) break;
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

	auto end = std::chrono::system_clock::now();
	auto elapsed = end - start;
	std::cout << elapsed.count() << '\n';

	return outputMatrix;
}

std::list<std::list<Neighbor>> MST::algorithmPrimList() {
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}

	auto start = std::chrono::system_clock::now();
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
							// Edge e;
							std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue = prioQueue;
							while (!prioQueue.empty()) {
								//e = prioQueue.top();
								prioQueue.pop();
								if (prioQueue.top().weight > temp.weight) break;
								if (temp == prioQueue.top()) {
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

		//int currentVertex = vertexEdgesToAdd;
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

	auto end = std::chrono::system_clock::now();
	auto elapsed = end - start;
	std::cout << elapsed.count() << '\n';

	return outputList;
}

void MST::displayMSTMatrix(std::list<std::list<int>>& matrix) {
	if (matrix.empty()) std::cout << "Macierz jest pusta." << std::endl;
	else {
		int lengthMST = 0;
		int spacesNeeded = log10(neighborMatrix.size()) + 1;

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

		for (outside = matrix.begin(); outside != matrix.end(); ++outside) {
			std::cout << i++;
			for (int s = 1; s < spacesNeeded; s++) std::cout << " ";
			std::cout << "|";

			std::list<int>::iterator inside = outside->begin();

			for (inside; inside != outside->end(); inside++) {
				std::cout << *inside << " ";
				lengthMST += *inside;
			}
			std::cout << "\n";
		}
		std::cout << "\nRozmiar drzewa: " << lengthMST/2 << "\n\n";
	}
}

void MST::displayMSTList(std::list<std::list<Neighbor>>& list) {
	if (list.empty()) std::cout << "Lista jest pusta." << std::endl;
	else {
		int lengthMST = 0;
		std::list<std::list<Neighbor>>::iterator it;
		std::list<Neighbor>::iterator itN;
		std::cout << "u->[v|w]\n";
		int i = 0;
		for (it = list.begin(); it != list.end(); it++) {
			std::cout << i++;

			for (Neighbor n : *it) {
				std::cout << "->[" << n.destination << "|" << n.weight << "]";
				lengthMST += n.weight;
			}
			std::cout << "\n";
		}
		std::cout << "\nRozmiar drzewa: " << lengthMST / 2 << "\n\n";
	}
}

void MST::displayList() {
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