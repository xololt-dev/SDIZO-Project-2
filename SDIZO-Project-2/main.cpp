#include <iostream>
#include <conio.h>
#include <string>
#include <chrono>

#include "graph.hpp"
#include "mst.hpp"
#include "fsp.hpp"

void displaySubMenu(std::string info);
void displayMainMenu();

MST mst;
FSP fsp;

int main() {
	displayMainMenu();
	return 0;
}

void displaySubMenu(std::string info) {
	std::cout << std::endl;
	std::cout << info << std::endl;
	std::cout << "1. Wczytaj z pliku" << std::endl;
	std::cout << "2. Utworz losowo" << std::endl;
	std::cout << "3. Wyswietl" << std::endl;
	info == "--- MST ---" ? 
		std::cout << "4. Algorytm Prima" << std::endl : std::cout << "4. Algorytm Dijkstry" << std::endl;
	info == "--- MST ---" ?
		std::cout << "5. Algorytm Kruskala" << std::endl : std::cout << "5. Algorytm Forda-Bellmana" << std::endl;
	std::cout << "0. Powrot do menu" << std::endl;
	std::cout << "Podaj opcje:";
}

void menu_MST() {
	char opt;
	std::string fileName;
	int amount, density;

	do {
		displaySubMenu("--- MST ---");
		opt = _getche();
		std::cout << std::endl;
		switch (opt) {
		case '1': //tutaj wczytytwanie z pliku
			std::cout << " Podaj nazwê zbioru:";
			std::cin >> fileName;
			mst.readFromFile(fileName);
			mst.displayList();
			mst.displayMatrix();
			break;

		case '2': //tutaj generowanie grafu
			std::cout << " Podaj ilosc wierzcholkow: ";
			std::cin >> amount;
			std::cout << " Podaj gestosc (calkowite %): ";
			std::cin >> density;
			mst.generateGraph(amount, density);
			break;

		case '3': //tutaj wyœwietlanie
			mst.displayList();
			mst.displayMatrix();
			break;

		case '4': {//tutaj algorytm Prima
			mst.algorithmPrimMatrix();
			mst.displayMatrix(1);
			mst.algorithmPrimList();
			mst.displayList(1);
			break;
		}
		case '5': {//tutaj algorytm Kruskala
			mst.algorithmKruskalMatrix();
			mst.displayMatrix(1);
			mst.algorithmKruskalList();
			mst.displayList(1);
			break;
		}
		}
	} while (opt != '0');
}

void menu_FSP() {
	char opt;
	std::string fileName;
	int amount, density;

	do {
		displaySubMenu("--- FSP ---");
		opt = _getche();
		std::cout << std::endl;
		switch (opt) {
		case '1': //tutaj wczytytwanie z pliku
			std::cout << " Podaj nazwê zbioru:";
			std::cin >> fileName;

			fsp.readFromFile(fileName);
			fsp.displayList();
			fsp.displayMatrix();
			break;

		case '2': //tutaj generowanie grafu
			std::cout << " Podaj ilosc wierzcholkow: ";
			std::cin >> amount;
			std::cout << " Podaj gestosc (calkowite %): ";
			std::cin >> density;

			fsp.generateGraph(amount, density);
			break;

		case '3': //tutaj wyœwietlanie
			fsp.displayList();
			fsp.displayMatrix();
			break;

		case '4': { //tutaj algorytm Dijkstry
			fsp.dijkstraList(1);
			fsp.dijkstraMatrix(1);
			break;
		}
		case '5': { //tutaj algorytm Forda-Bellmana
			fsp.fordBellmanList(1);
			fsp.fordBellmanMatrix(1);

			break;
		}
		}
	} while (opt != '0');
}

void displayMainMenu() {
	char option;

	do {
		std::cout << std::endl;
		std::cout << "==== MENU GLOWNE ===" << std::endl;
		std::cout << "1. Wyznaczanie minimalnego drzewa rozpinajacego" << std::endl;
		std::cout << "2. Wyznaczanie najkrotszej sciezki w grafie" << std::endl;
		std::cout << "3. Wyznaczanie maksymalnego przeplywu" << std::endl;
		std::cout << "0. Wyjscie" << std::endl;
		std::cout << "Podaj opcje:";
		option = _getche();
		std::cout << std::endl;

		switch (option) {
		case '1':
			menu_MST();
			break;

		case '2':
			menu_FSP();
			break;

		case '3':
			// menu_heap();
			break;
		}
	} while (option != '0');
}