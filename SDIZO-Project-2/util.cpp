#include "util.hpp"
#include <iostream>

Matrix::Matrix() {
	tab = nullptr;
	sideLength = 0;
	cnt = 0;
}

Matrix::~Matrix() {	
	clear();
	cnt = 0;
}

void Matrix::generateEmpty(int sideLength) {
	tab = new int* [sideLength];
	for (int i = 0; i < sideLength; i++)
	{
		tab[i] = new int[sideLength];
		for (int j = 0; j < sideLength; j++)
		{
			tab[i][j] = 0;
		}
	}
	Matrix::sideLength = sideLength;
}

void Matrix::addValue(int x, int y, int value) {
	tab[x][y] = value;
	cnt++;
}

void Matrix::deleteValue(int x, int y) {
	tab[x][y] = 0;
	cnt--;
}

int Matrix::valueInPosition(int x, int y) {
	return tab[x][y];
}

void Matrix::display(bool mst) {
	if (empty()) std::cout << "Macierz jest pusta." << std::endl;
	else {
		int lengthMST = 0;
		int spacesNeeded = log10(sideLength) + 1;

		for (int s = -1; s < spacesNeeded; s++) std::cout << " ";

		for (int i = 0; i < sideLength; i++) {
			std::cout << i << " ";
		}
		std::cout << "\n";

		for (int s = 0; s < spacesNeeded; s++) std::cout << " ";
		std::cout << "|";

		for (int i = sideLength*2 - 1; i > 0; i--) std::cout << "-";
		std::cout << "\n";

		for (int i = 0; i < sideLength; i++) {
			std::cout << i;
			for (int s = 1; s < spacesNeeded; s++) std::cout << " ";
			std::cout << "|";

			for (int j = 0; j < sideLength; j++) {
				std::cout << tab[i][j] << " ";
				lengthMST += tab[i][j];
			}
			std::cout << "\n";
		}
		if (mst) std::cout << "\nRozmiar drzewa: " << lengthMST / 2;
		std::cout << "\n";
	}
}

bool Matrix::empty() {
	return !cnt;
}

void Matrix::clear() {
	for (int i = sideLength - 1; i >= 0; i--) {
		delete[] tab[i];
		tab[i] = nullptr;
	}
	delete[] tab;
	tab = nullptr;
	sideLength = 0;
}

int Matrix::size() {
	return sideLength;
}

int Matrix::count() {
	return cnt;
}

Heap::Heap() {
	tab = nullptr;
	cnt = 0;
	tab_size = 0;
}

Heap::~Heap() {
	delete[] tab;
	tab = nullptr;
}

void Heap::heapify(int index) {
	int smallest;
	int l = 2 * index + 1;
	int r = 2 * index + 2;

	if (l < cnt && tab[l] < tab[index]) smallest = l;
	else smallest = index;

	if (r < cnt && tab[r] < tab[smallest]) smallest = r;

	if (smallest != index) {
		Edge temp = tab[index];
		tab[index] = tab[smallest];
		tab[smallest] = temp;
		// display();
		heapify(smallest);
	}
}

/// <summary>
/// Build heap - Floyd's algorithm
/// </summary>
void Heap::buildHeap() {
	for (int i = floor(cnt / 2) - 1; i >= 0; i--) heapify(i);
}

/// <summary>
/// Checking if value is in the heap
/// </summary>
/// <param name="value - value we're searching for"></param>
bool Heap::isValueInHeap(Edge value)
{
	for (int i = 0; i < cnt; i++) {
		if (tab[i] == value) return true;
	}
	return false;
}

/// <summary>
/// Insert value to the heap
/// </summary>
/// <param name="value - value to be inserted"></param>
void Heap::push(Edge value) {
	// jesli nie ma miejsca, realokujemy
	if (cnt == tab_size) resize();

	if (!cnt) {
		tab[0] = value;
		cnt++;
		return;
	}

	int i = cnt;
	int parent = floor((i - 1) / 2);

	while (i > 0 && tab[parent] > value) {
		tab[i] = tab[parent];
		i = parent;
		parent = (int) floor((i - 1) / 2);
	}

	cnt++;
	tab[i] = value;
}

/// <summary>
/// Pop value from the heap
/// </summary>
Edge Heap::pop() {
	Edge root = Edge{ NULL, NULL, NULL };
	//std::cout << "Pop\n";
	if (!cnt) {
		// std::cout << "Kopiec pusty!" << std::endl;
		return root;
	}
	root = tab[0];
	
	if (cnt > 1) {
		tab[0] = tab[--cnt];
		//display();
		tab[cnt] = Edge{ NULL, NULL, NULL };
		//display();
		heapify(0);
		//std::cout << "Po heapify\n";
		//display();
	}
	else {
		tab[0] = Edge{ NULL, NULL, NULL };
		cnt--;
	}

	return root;
}

bool Heap::empty() {
	return !cnt;
}

/// <summary>
/// Resize maximum heap size by 6
/// </summary>
void Heap::resize() {
	int new_size = cnt;
	if (cnt > 5) {
		new_size = 2 * cnt;// pow(2, ceil(log2(cnt)) + 1) - 1;
	}
	else new_size = 6;

	Edge* tabTemp = nullptr;
	try {
		tabTemp = new Edge[new_size];
	}
	catch (const std::bad_alloc& e) {
		std::cerr << "bad_alloc detected: " << e.what();
	}

	if (tab != nullptr) {
		for (int i = 0; i < cnt; i++) {
			tabTemp[i] = tab[i];
		}
	}

	delete[] tab;
	tab = nullptr;

	tab_size = new_size;
	tab = tabTemp;
}

void Heap::release() {
	delete[] tab;
	tab = nullptr;
	tab_size = cnt = 0;
}

void Heap::display() {
	std::cout << "Size: " << cnt << "\n";
	for (int i = 0; i < cnt; i++) {
		std::cout << tab[i].weight << " ";
	}
	std::cout << "\n";

	if (cnt || tab != NULL) {
		std::cout << "'--";
		std::cout << tab[0].weight << std::endl;
	}
	displayHeap(1);
	displayHeap(2);
}

void Heap::displayHeap(int index)
{
	if (index >= cnt) return;

	float n = floor(log2(index + 1));

	while (n > 0) {
		std::cout << "   ";
		n--;
	}
	if (cnt > index + 1 && index % 2 == 1) std::cout << "|--";
	else std::cout << "'--";
	std::cout << tab[index].weight << std::endl;

	displayHeap(2 * index + 1);
	displayHeap(2 * index + 2);
}