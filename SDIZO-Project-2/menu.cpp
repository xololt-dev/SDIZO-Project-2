#include <string>
#include <iostream>
#include <conio.h>

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
		displaySubMenu("--- TABLICA ---");
		opt = _getche();
		std::cout << std::endl;
		switch (opt) {
		case '1': //tutaj wczytytwanie  tablicy z pliku
			std::cout << " Podaj nazwê zbioru:";
			std::cin >> fileName;
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
			std::cout << " podaj index:";
			std::cin >> index;
			std::cout << " podaj wartoœæ:";
			std::cin >> value;

			// myTab.addValue(index, value);
			// myTab.display();
			break;

		case '4': //tutaj znajdowanie elemetu w tablicy
			std::cout << " podaj wartosc:";
			std::cin >> value;
			/*if (myTab.IsValueInTable(value))
				cout << "podana wartosc jest w tablicy";
			else
				cout << "podanej wartosci NIE ma w tablicy";
			*/break;

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
		std::cout << "1. Wyznaczanie minimalnego drzewa rozpinaj¹cego" << std::endl;
		std::cout << "2. Wyznaczanie najkrótszej œcie¿ki w grafie" << std::endl;
		std::cout << "3. Wyznaczanie maksymalnego przep³ywu" << std::endl;
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