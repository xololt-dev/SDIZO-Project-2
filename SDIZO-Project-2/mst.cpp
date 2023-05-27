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
		neighborMatrix.generateEmpty(vertexAmount);
		neighborList.generateEmpty(vertexAmount);

		Edge tempEdge;
		ListMember<List<Neighbor>>* iterList = neighborList.front();
		// Zrobiæ dodawanie po jednym elem.
		while (file >> tempEdge.source) {
			file >> tempEdge.destination;
			file >> tempEdge.weight;
			
			iterList = neighborList.front();

			if (tempEdge.source < tempEdge.destination) {
				for (int i = 0; i < tempEdge.source; i++) iterList = iterList->next;

				iterList->data.push_back(Neighbor{ tempEdge.weight, tempEdge.destination });
				for (int i = 0; i < (tempEdge.destination - tempEdge.source); i++) iterList = iterList->next;

				iterList->data.push_back(Neighbor{tempEdge.weight, tempEdge.source});
			}
			else {
				for (int i = 0; i < tempEdge.destination; i++) iterList = iterList->next;

				iterList->data.push_back(Neighbor{ tempEdge.weight, tempEdge.source });
				for (int i = 0; i < (tempEdge.source - tempEdge.destination); i++) iterList = iterList->next;

				iterList->data.push_back(Neighbor{ tempEdge.weight, tempEdge.destination });
			}
			neighborMatrix.addValue(tempEdge.source, tempEdge.destination, tempEdge.weight);
			neighborMatrix.addValue(tempEdge.destination, tempEdge.source, tempEdge.weight);
		}
		file.close();
	}
	else std::cout << "Plik nie zostal otworzony!\n";
}

void MST::generateGraph(int sideLength, int density) {
	int random, edgesLeft = sideLength * (sideLength - 1) * density / 200;
	if (edgesLeft < sideLength || !sideLength || !density) return;

	// cleanup
	while (!prioQueue.empty()) prioQueue.pop();
	neighborList.clear();
	neighborMatrix.clear();

	int randValue = 9;

	srand(time(NULL));

	neighborList.generateEmpty(sideLength);
	neighborMatrix.generateEmpty(sideLength);

	Edge e;
	ListMember<List<Neighbor>>* iterList = neighborList.front();
	// dojscie do kazdego wierzcholka
	for (int i = 1; i < sideLength; i++) {
		e.destination = i;
		do {
			e.source = rand() % i;
		} while (e.source == i);
		e.weight = 1 + rand() % randValue;

		iterList = neighborList.front();
		for (int i = 0; i < e.source; i++) iterList = iterList->next;

		iterList->data.push_back(Neighbor{ e.weight, e.destination, 0, 0 });
		neighborMatrix.addValue(e.source, e.destination, e.weight);
		neighborMatrix.addValue(e.destination, e.source, e.weight);

		for (int i = 0; i < (e.destination - e.source); i++) iterList = iterList->next;
		iterList->data.push_back(Neighbor{e.weight, e.source, 0, 0});

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
		neighborMatrix.addValue(dest, random, weight);
		
		iterList = neighborList.front();
		for (int i = 0; i < dest; i++) iterList = iterList->next;
		iterList->data.push_back(Neighbor{weight, random, 0, 0});

		edgesLeft--;
	}
}

void MST::algorithmPrimMatrix(bool display) {
	outputMatrix.clear();
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}

	auto start = std::chrono::system_clock::now();
	int edgesLeft = neighborMatrix.size() - 1;
	int vertexEdgesToAdd = 0;
	List<int> vertLeft;
	for (int i = 0; i <= edgesLeft; i++) {
		vertLeft.push_back(i);
	}

	// generate "empty"
	outputMatrix.generateEmpty(edgesLeft + 1);

	// dopóki nie sprawdzimy ka¿dego wierzcho³ka
	while (edgesLeft) {
		edgesLeft--;
		vertLeft.deleteFromList(vertexEdgesToAdd);

		// wype³nienie kolejki krawêdziami
		// jeœli jest to wierzcho³ek który chcemy dodaæ
		Edge temp;
		temp.source = vertexEdgesToAdd;

		for (int i = 0; i < neighborMatrix.size(); i++) {
			bool doWstawienia = 1;
			temp.destination = i;
			temp.weight = neighborMatrix.valueInPosition(vertexEdgesToAdd, i);

			// jesli jest krawedz
			if (outputMatrix.valueInPosition(vertexEdgesToAdd, i) != neighborMatrix.valueInPosition(vertexEdgesToAdd, i)) {
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
			else doWstawienia = 0;

			if (doWstawienia) prioQueue.push(temp);
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

			int srcCount = 0;
			int dstCount = 0;
			for (int i = 0; i < neighborMatrix.size(); i++) {
				if (outputMatrix.valueInPosition(src, i)) srcCount++;
				if (outputMatrix.valueInPosition(dest, i)) dstCount++;
				if (srcCount && dstCount) {
					petla = 1;
					break;
				}
			}
		} while (petla);

		// dodaj do matrycy

		outputMatrix.addValue(src, dest, toAdd.weight);
		outputMatrix.addValue(dest, src, toAdd.weight);

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

	auto end = std::chrono::system_clock::now();
	auto elapsed = end - start;
	std::cout << elapsed.count() << '\n';

	displayMatrix(display);
}

void MST::algorithmPrimList(bool display) {
	outputList.clear();
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}

	int edgesLeft = this->neighborList.size() - 1;
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
			listOut->data.push_back(Neighbor{ toAdd.weight, toAdd.source });
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

	displayList(display);
}

void MST::algorithmKruskalMatrix(bool display) {
	outputMatrix.clear();
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}

	auto start = std::chrono::system_clock::now();
	int edgesLeft = neighborMatrix.size();

	int* parent = new int[edgesLeft];
	int* rank = new int[edgesLeft];
	outputMatrix.generateEmpty(edgesLeft);

	//makeSet + dodanie krawedzi do kolejki
	for (int i = 0; i < edgesLeft; i++) {
		parent[i] = i;
		rank[i] = 0;
		
		for (int j = i + 1; j < edgesLeft; j++) {
			int weight = neighborMatrix.valueInPosition(i, j);
			if (weight) prioQueue.push(Edge{weight, i, j});
		}
	}

	Edge e;
	while (!prioQueue.empty()) {
		e = prioQueue.top();
		prioQueue.pop();

		// findSet
		if (findSet(e.source, parent) != findSet(e.destination, parent)) {
			// dodaj do macierzy
			outputMatrix.addValue(e.source, e.destination, e.weight);
			outputMatrix.addValue(e.destination, e.source, e.weight);

			//union
			unionSet(e.source, e.destination, rank, parent);
		}
	}

	auto end = std::chrono::system_clock::now();
	auto elapsed = end - start;
	std::cout << elapsed.count() << '\n';

	displayMatrix(display);
}

void MST::algorithmKruskalList(bool display) {
	outputList.clear();
	while (!prioQueue.empty()) prioQueue.pop();

	auto start = std::chrono::system_clock::now();

	int edgesLeft = neighborMatrix.size();

	int* parent = new int[edgesLeft];
	int* rank = new int[edgesLeft];
	outputList.generateEmpty(edgesLeft);

	ListMember<List<Neighbor>>* outer = neighborList.front();
	ListMember<Neighbor>* inner = outer->data.front();
	//makeSet + dodanie krawedzi do kolejki
	for (int i = 0; i < edgesLeft; i++) {
		parent[i] = i;
		rank[i] = 0;

		inner = outer->data.front();
		while (inner) {
			if (inner->data.destination >= i) prioQueue.push(Edge{ inner->data.weight, i, inner->data.destination });
			inner = inner->next;
		}
		outer = outer->next;
	}

	Edge e;
	while (!prioQueue.empty()) {
		e = prioQueue.top();
		prioQueue.pop();

		// findSet
		if (findSet(e.source, parent) != findSet(e.destination, parent)) {
			// dodaj do listy
			outer = outputList.front();
			if (e.source < e.destination) {
				for (int i = 0; i < e.source; i++) outer = outer->next;
				outer->data.push_back(Neighbor{ e.weight, e.destination });
				for (int i = 0; i < (e.destination - e.source); i++) outer = outer->next;
				outer->data.push_back(Neighbor{ e.weight, e.source });
			}
			else {
				for (int i = 0; i < e.destination; i++) outer = outer->next;
				outer->data.push_back(Neighbor{ e.weight, e.source });
				for (int i = 0; i < (e.source - e.destination); i++) outer = outer->next;
				outer->data.push_back(Neighbor{ e.weight, e.destination });
			}
			
			//union
			unionSet(e.source, e.destination, rank, parent);
		}
	}

	auto end = std::chrono::system_clock::now();
	auto elapsed = end - start;
	std::cout << elapsed.count() << '\n';

	displayList(display);
}

void MST::unionSet(int x, int y, int* rank, int* parent) {
	/*
	Union(x, y)
		a = Find(x)
		b = Find(y)
		if rank[a] < rank[b]
			parent[a] = b
		else
			parent[b] = a;
	if rank[a] = rank[b]
		rank[a] = rank[a] + 1
	*/
	int a = findSet(x, parent);
	int b = findSet(y, parent);

	if (rank[a] < rank[b]) parent[a] = b;
	else parent[b] = a;

	if (rank[a] = rank[b]) rank[a]++;
}

int MST::findSet(int x, int* parent) {
	/*
	Find(x)
	if parent[x] <> x
		parent[x] = Find(parent[x])
		return parent[x]
	*/
	if (x != parent[x]) parent[x] = findSet(parent[x], parent);
	return parent[x];
}