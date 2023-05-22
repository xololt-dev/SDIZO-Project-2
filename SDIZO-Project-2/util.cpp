#include "util.hpp"
#include <iostream>

Matrix::Matrix() {
	tab = NULL;
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
		std::cout << "\n\n";
	}
}

bool Matrix::empty() {
	return !cnt;
}

void Matrix::clear() {
	for (int i = 0; i < sideLength; i++)
		delete[] tab[i];
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