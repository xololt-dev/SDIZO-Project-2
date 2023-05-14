#include <iostream>
#include <conio.h>
#include <string>

#include "prim_mst.hpp"

void displaySubMenu(std::string info);
void displayMainMenu();

Prim_MST mst;

int main()
{
	
	displayMainMenu();
	return 0;
}

void displaySubMenu(std::string info) {
	std::cout << std::endl;
	std::cout << info << std::endl;
	std::cout << "1. Wczytaj z pliku" << std::endl;
	std::cout << "2. Utworz losowo" << std::endl;
	std::cout << "3. Wyswietl" << std::endl;
	std::cout << "4. Algorytm 1" << std::endl;
	std::cout << "5. Algorytm 2" << std::endl;
	std::cout << "0. Powrot do menu" << std::endl;
	std::cout << "Podaj opcje:";
}

void menu_MST()
{
	char opt;
	std::string fileName;
	int index, value;

	do {
		displaySubMenu("--- MST ---");
		opt = _getche();
		std::cout << std::endl;
		switch (opt) {
		case '1': //tutaj wczytytwanie tablicy z pliku
			std::cout << " Podaj nazwê zbioru:";
			std::cin >> fileName;
			mst.readFromFile(fileName);
			// myTab.loadFromFile(fileName);
			// myTab.display();
			break;

		case '2': //tutaj usuwanie elemenu z tablicy
			std::cout << " podaj index:";
			std::cin >> index;
			// myTab.deleteFromTable(index);
			// myTab.display();
			break;

		case '3': //tutaj dodawanie elemetu do tablicy
			mst.displayMST();
			break;

		case '4': //tutaj znajdowanie elemetu w tablicy
			mst.algorithm();
			break;

		case '5':  //tutaj generowanie  tablicy
			std::cout << "Podaj ilosc elementów tablicy:";
			std::cin >> value;
			// myTab.generateTable(value);
			// myTab.display();
			break;

		}
	} while (opt != '0');
}

void displayMainMenu() {
	char option;
	//inicjalizacja class algo?

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
			// menu_list();
			break;

		case '3':
			// menu_heap();
			break;
		}
	} while (option != '0');
}