#include <iostream>
#include <fstream>
#include "graph.hpp"
// #include "util.hpp"

// za³adowaæ liczbê krawêdzi, liczbê wierzcho³ków i ka¿d¹ krawêdŸ
void Graph::readFromFile(std::string FileName) {
	std::fstream file;
	file.open(FileName, std::ios::in);

	if (file.good()) {
		int edgesAmount = 0, vertexAmount = 0;
		int temp = 0;

		while (!prioQueue.empty()) prioQueue.pop();
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
/*
void Graph::generateGraph(int sideLength, int density, bool directed) {
	if (!sideLength || !density) return;
	// cleanup
	while (!prioQueue.empty()) prioQueue.pop();
	//neighborList.clear();
	// neighborMatrix.clear();
	//myNeighborMatrix.clear();

	int randValue = 1000;

	srand(time(NULL));
	
	//myNeighborMatrix.generateEmpty(sideLength);
	//neighborMatrix = new int* [sideLength];
	for (int i = 0; i < sideLength; i++)
	{
		//neighborMatrix[i] = new int[sideLength];
		for (int j = 0; j < sideLength; j++)
		{
		//	neighborMatrix[i][j] = 0;
		}
		//std::list<Neighbor> temp = { Neighbor{-1, -1 } };
		//neighborList.push_back(temp);
	}

	int random, edgesLeft = sideLength * (sideLength - 1) * density / 200;
	//std::list<std::list<Neighbor>>::iterator outsideL = neighborList.begin();
	std::list<Neighbor>::iterator insideL;
	Edge e;

	for (int i = 0; i < sideLength - 1; i++) {
		if (i) {
			e.source = i;
			e.destination = rand() % i;
			e.weight = 1 + rand() % randValue;

			outsideL = neighborList.begin();
			for (int j = 0; j < e.destination; j++) outsideL++;

			if (outsideL->begin()->destination == -1) {
				*(outsideL->begin()) = Neighbor{ e.weight, e.source };
			}
			else {
				outsideL->push_back(Neighbor{ e.weight, e.source });
			}

			for (int j = 0; j < (e.source - e.destination); j++) outsideL++;

			if (outsideL->begin()->destination == -1) {
				*(outsideL->begin()) = Neighbor{ e.weight, e.destination };
			}
			else {
				outsideL->push_back(Neighbor{ e.weight, e.destination });
			}

			myNeighborMatrix.addValue(e.source, e.destination, e.weight);
			myNeighborMatrix.addValue(e.destination, e.source, e.weight);

			neighborMatrix[e.source][e.destination] = e.weight;
			neighborMatrix[e.destination][e.source] = e.weight;

			edgesLeft--;
		}
		else {
			e.source = i;
			e.destination = i + 1;
			e.weight = 1 + rand() % randValue;

			outsideL = neighborList.begin();
			for (int j = 0; j < e.source; j++) outsideL++;

			if (outsideL->begin()->destination == -1) {
				*(outsideL->begin()) = Neighbor{ e.weight, e.destination };
			}
			else {
				outsideL->push_back(Neighbor{ e.weight, e.destination });
			}

			outsideL++;

			if (outsideL->begin()->destination == -1) {
				*(outsideL->begin()) = Neighbor{ e.weight, e.source };
			}
			else {
				outsideL->push_back(Neighbor{ e.weight, e.source });
			}
			
			myNeighborMatrix.addValue(e.source, e.destination, e.weight);
			myNeighborMatrix.addValue(e.destination, e.source, e.weight);

			neighborMatrix[e.source][e.destination] = e.weight;
			neighborMatrix[e.destination][e.source] = e.weight;

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
			//outsideL = neighborList.begin();
			//for (int i = 0; i < random; i++) outsideL++;
		} while (outsideL->sideLength() == sideLength - 1);

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
		int dest = rand() % exists.sideLength();
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

		outsideL = neighborList.begin();
		for (int i = 0; i < dest; i++) outsideL++;

		if (outsideL->begin()->destination != -1) {
			outsideL->push_back(Neighbor{ weight, random });
		}
		else {
			*(outsideL->begin()) = Neighbor{ weight, random };
		}

		myNeighborMatrix.addValue(random, dest, weight);
		myNeighborMatrix.addValue(dest, random, weight);

		neighborMatrix[random][dest] = weight;
		neighborMatrix[dest][random] = weight;

		edgesLeft--;
	}
}

void Graph::displayMSTMatrix(int**& matrix) {
	//if (matrix.empty()) std::cout << "Macierz jest pusta." << std::endl;
	//else {
		int lengthMST = 0;
		//int spacesNeeded = log10(neighborMatrix.sideLength()) + 1;

		//std::list<std::list<int>>::iterator outside = matrix.begin();
		int i = 0;
		//for (int s = -1; s < spacesNeeded; s++) std::cout << " ";

		//while (outside != matrix.end()) {
			std::cout << i++ << " ";
			//outside++;
		//}
		std::cout << "\n";

		//for (int s = 0; s < spacesNeeded; s++) std::cout << " ";
		std::cout << "|";

		i *= 2;
		for (--i; i > 0; i--) std::cout << "-";
		std::cout << "\n";

		//for (outside = matrix.begin(); outside != matrix.end(); ++outside) {
			std::cout << i++;
			//for (int s = 1; s < spacesNeeded; s++) std::cout << " ";
			std::cout << "|";

			//std::list<int>::iterator inside = outside->begin();

			//for (inside; inside != outside->end(); inside++) {
				//std::cout << *inside << " ";
				//lengthMST += *inside;
			//}
			std::cout << "\n";
		//}
		std::cout << "\nRozmiar drzewa: " << lengthMST / 2 << "\n\n";
	//}
}
*/
void Graph::displayMSTList() {
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

void Graph::algorithmPrimListN() {
	outputList.clear();
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}

	int edgesLeft = this->neighborList.sideLength() - 1;
	int vertexEdgesToAdd = 0;
	List<int> vertLeft;
	for (int i = 0; i <= edgesLeft; i++) {
		vertLeft.push_back(i);
	}

	// generate "empty"
	outputList.generateEmpty(edgesLeft + 1);

	// dopóki nie sprawdzimy ka¿dego wierzcho³ka
	while (edgesLeft) {
		edgesLeft--;
		vertLeft.deleteFromList(vertexEdgesToAdd);

		ListMember<List<Neighbor>>* listOvrl = neighborList.front();
		ListMember<List<Neighbor>>* listOut = outputList.front();

		for (int i = 0; i < vertexEdgesToAdd; i++) {
			listOvrl = listOvrl->next;
			listOut = listOut->next;
		}
		ListMember<Neighbor>* listInside = listOvrl->data.front();
		
		// wype³nienie kolejki krawêdziami
		// jeœli jest to wierzcho³ek który chcemy dodaæ
		Edge temp;
		temp.source = vertexEdgesToAdd;

		while (listInside) {
			// jesli po³¹czenie
			// czy to potrzebne
			bool doWstawienia = 1;
			temp.destination = listInside->data.destination;
			temp.weight = listInside->data.weight;

			// jesli bylo brane pod uwage
			if (!listOut->data.empty()) {
				ListMember<Neighbor>* listOutInside = listOut->data.front();

				while (listOutInside) {
					if (listOutInside->data.destination == temp.destination) {
						doWstawienia = 0;
						break;
					}
					listOutInside = listOutInside->next;
				}
			}

			if (doWstawienia) {
				// jesli jest w kopcu
				std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> tempQueue = prioQueue;
				while (!prioQueue.empty()) {
					if (prioQueue.top().weight > temp.weight) break;
					if (temp == prioQueue.top()) {
						doWstawienia = 0;
						break;
					}
					prioQueue.pop();
				}
				prioQueue = tempQueue;
			}

			if (doWstawienia) prioQueue.push(temp);
			listInside = listInside->next;
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

			listOut = outputList.front();
			ListMember<List<Neighbor>>* listOutSecond = outputList.front();
			for (int i = 0; i < src; i++) listOutSecond = listOutSecond->next;
			for (int i = 0; i < dest; i++) listOut = listOut->next;
			
			petla = !(listOut->data.empty() || listOutSecond->data.empty());
		} while (petla);

		// dodaj do matrycy

		listOut = outputList.front();
		if (toAdd.source < toAdd.destination) {
			for (int i = 0; i < toAdd.source; i++) listOut = listOut->next;
			listOut->data.push_back(Neighbor{ toAdd.weight, toAdd.destination });
			for (int i = 0; i < (toAdd.destination - toAdd.source); i++) listOut = listOut->next;
			listOut->data.push_back(Neighbor{ toAdd.weight, toAdd.source});
		}
		else {
			for (int i = 0; i < toAdd.destination; i++) listOut = listOut->next;
			listOut->data.push_back(Neighbor{ toAdd.weight, toAdd.source });
			for (int i = 0; i < (toAdd.source - toAdd.destination); i++) listOut = listOut->next;
			listOut->data.push_back(Neighbor{ toAdd.weight, toAdd.destination });
		}		

		listOut = outputList.front();
		// vertexEdgesToAdd = toAdd.destination;
		if (!vertLeft.empty()) {
			vertexEdgesToAdd = vertLeft.front()->data;
			ListMember<int>* t = vertLeft.front();
			while (t) {
				if (t->data == toAdd.destination) {
					vertexEdgesToAdd = toAdd.destination;
					break;
				}
				t = t->next;
			}
		}
	}
	while (!prioQueue.empty()) prioQueue.pop();
}