#pragma once
#include <climits>
#include <iostream>

struct Edge {
	int weight;
	int source;
	int destination;

	inline bool operator==(const Edge& rhs) {
		return (((destination == rhs.destination && source == rhs.source) || (destination == rhs.source && source == rhs.destination)) && weight == rhs.weight);
	}
	inline bool operator!=(const Edge& rhs) {
		return (destination != rhs.destination || source != rhs.source || weight != rhs.weight);
	}
	inline bool operator<(const Edge& rhs) {
		return (weight < rhs.weight);
	}
	inline bool operator>(const Edge& rhs) {
		return (weight > rhs.weight);
	}
	inline bool operator<=(const Edge& rhs) {
		return (weight <= rhs.weight);
	}
	inline bool operator>=(const Edge& rhs) {
		return (weight >= rhs.weight);
	}
};

inline bool operator==(const Edge& lhs, const Edge& rhs) {
	return (lhs.destination == rhs.destination && lhs.source == rhs.source && lhs.weight == rhs.weight);
}
inline bool operator<(const Edge& lhs, const Edge& rhs) {
	return (lhs.weight < rhs.weight);
}
inline bool operator>(const Edge& lhs, const Edge& rhs) {
	return (lhs.weight > rhs.weight);
}

struct Neighbor {
	int weight;
	int destination;
	// int parent;
	// int rank;

	/*
	inline bool operator!=(const Neighbor& rhs) {
		return (destination != rhs.destination || parent != rhs.parent || weight != rhs.weight || rank != rhs.rank);
	}
	*/
	inline bool operator!=(const Neighbor& rhs) {
		return (destination != rhs.destination || weight != rhs.weight);
	}
};

template <typename T> struct ListMember {
	ListMember<T>* prev;
	ListMember<T>* next;
	T data;
};

class Matrix {
public:
	Matrix(); //konstruktor - wywoływany automatycznie przy tworzeniu obieku
	~Matrix(); //destrukor - wywływany automatycznie przy usuwaniu obiektu

	void generateEmpty(int sideLength);
	void addValue(int x, int y, int value);
	void deleteValue(int x, int y);
	int valueInPosition(int x, int y);

	void display(bool mst = 0);
	bool empty();
	void clear();
	int size();
	int count();
	
private:
	int** tab;
	int sideLength; // sideLength x sideLength
	int cnt; //ilość elementów 
};

template <typename T> class List {
public:
	List(); //konstruktor - wywo³ywany automatycznie przy tworzeniu obieku
	~List(); //destrukor - wyw³ywany automatycznie przy usuwaniu obiektu

	ListMember<T>* front();
	ListMember<T>* back();
	void push_back(T value, bool increment = true);
	void push_back(List<T>& value, bool increment = true);
	void clear();
	void generateEmpty(int sideLength);
	void deleteFromList(T value);
	int size();
	bool empty();

private:
	ListMember<T>* firstMember;
	ListMember<T>* lastMember;
	int cnt;
};

template <typename T> class Table {
public:
	Table();
	~Table(); //destrukor - wywływany automatycznie przy usuwaniu obiektu

	void addValue(int index, T value);
	void changeValue(int index, T value, bool increment = false);
	void deleteFromIndex(int index);
	void generateEmpty(int sideLength);
	void clear();
	int length();
	int count();
	bool empty();

private:
	T* tab;
	int sideLength;
	int cnt;
};

class Heap {
public:
	Heap();		//konstruktor - wywoływany automatycznie przy tworzeniu obieku
	~Heap();	//destrukor - wywływany automatycznie przy usuwaniu obiektu

	void heapify(int index);
	void buildHeap();
	void resize();

	bool isValueInHeap(Edge value);
	void push(Edge value);
	Edge pop();
	bool empty();
	void release();

	void display();
	void displayHeap(int index);
private:
	Edge* tab;
	int cnt;		// amount of values in table
	int tab_size;	// table size
};

template<typename T> List<T>::List() {
	cnt = 0;
	firstMember = nullptr;
	lastMember = nullptr;
}

template<typename T> List<T>::~List() {
	clear();
	firstMember = nullptr;
	lastMember = nullptr;
}

template<typename T> ListMember<T>* List<T>::front() {
	return this->firstMember;
}

template<typename T> ListMember<T>* List<T>::back() {
	return this->lastMember;
}

template<typename T> void List<T>::push_back(T value, bool increment) {
	ListMember<T>* temp = new ListMember<T>;
	
	if (lastMember != nullptr) lastMember->next = temp;

	temp->data = value;
	temp->next = nullptr;
	temp->prev = lastMember;

	lastMember = temp;

	if (firstMember == nullptr) firstMember = temp;

	if (increment) cnt++;
}
template<typename T> void List<T>::push_back(List<T>& value, bool increment) {
	ListMember<List<T>>* temp = new ListMember<List<T>>;

	if (lastMember != nullptr) lastMember->next = temp;
	
	temp->data = value;
	temp->next = nullptr;
	temp->prev = lastMember;

	lastMember = temp;

	if (firstMember == nullptr) firstMember = temp;

	if (increment) cnt++;
}

template<typename T> void List<T>::clear() {
	ListMember<T>* temp;

	while (firstMember != lastMember) {
		temp = firstMember->next;
		delete firstMember;
		firstMember = nullptr;
		firstMember = temp;
	}
	delete lastMember;
	lastMember = nullptr;
	firstMember = nullptr;
}

template<typename T> void List<T>::generateEmpty(int sideLength) {
	ListMember<T>* temp = new ListMember<T>;

	if (lastMember != nullptr) lastMember->next = temp;

	temp->data = T{};
	temp->next = nullptr;
	temp->prev = lastMember;

	lastMember = temp;

	if (firstMember == nullptr) firstMember = temp;

	for (int i = 0; i < sideLength - 1; i++) {
		push_back(T{});
	}
	
	cnt = sideLength;
}

template<typename T> void List<T>::deleteFromList(T value) {
	// if list is empty, return with information
	if (!cnt) return;

	ListMember<T>* temp = firstMember;

	for (int i = 0; i < cnt; i++) {
		// go through each list member until value is found
		if (temp->data != value) {
			temp = temp->next;
		}
		else {
			if (temp->prev == NULL) {
				if (temp->next == NULL) firstMember = lastMember = NULL;
				else {
					temp->next->prev = NULL;
					firstMember = temp->next;
				}
			}
			else {
				if (temp->next == NULL) {
					lastMember = temp->prev;
					temp->prev->next = NULL;
				}
				else temp->next->prev = temp->prev;

				temp->prev->next = temp->next;
			}

			delete temp;

			cnt--;

			return;
		}
	}
	// if value not found, return a message
	// std::cout << "Nie znaleziono wartosci do usuniecia!\n";
}

template<> inline void List<Neighbor>::deleteFromList(Neighbor value) {
	// if list is empty, return with information
	if (!cnt) return;

	ListMember<Neighbor>* temp = firstMember;

	for (int i = 0; i < cnt; i++) {
		// go through each list member until value is found
		if (temp->data != value) {
			temp = temp->next;
		}
		else {
			if (temp->prev == NULL) {
				if (temp->next == NULL) firstMember = lastMember = NULL;
				else {
					temp->next->prev = NULL;
					firstMember = temp->next;
				}
			}
			else {
				if (temp->next == NULL) {
					lastMember = temp->prev;
					temp->prev->next = NULL;
				}
				else temp->next->prev = temp->prev;

				temp->prev->next = temp->next;
			}

			delete temp;

			cnt--;

			return;
		}
	}
	// if value not found, return a message
	// std::cout << "Nie znaleziono wartosci do usuniecia!\n";
}

template<typename T> int List<T>::size() {
	return cnt;
}

template<typename T> bool List<T>::empty() {
	return !cnt;
}

template<typename T> Table<T>::Table() {
	tab = nullptr;
	sideLength = 0;
	cnt = 0;
}

template<typename T> Table<T>::~Table() {
	clear();
	tab = nullptr;
}

template<typename T> void Table<T>::addValue(int index, T value) {
	T* tabTemp = new T[sideLength + 1];
	bool passedInputValue = false;

	if (index >= sideLength) tabTemp[sideLength] = value;
	else tabTemp[index] = value;

	// copying the table
	for (int i = 0; i < sideLength; i++) {
		if (i == index) passedInputValue = true;

		tabTemp[i + passedInputValue] = tab[i];
	}

	delete[] tab;

	++cnt;
	sideLength++;

	tab = tabTemp;
}

template<typename T> void Table<T>::changeValue(int index, T value, bool increment) {
	tab[index] = value;
	if (increment) cnt++;
}

template<typename T> void Table<T>::deleteFromIndex(int index) {
	// if table is empty, return with message
	if (!cnt) {
		std::cout << "Tablica jest pusta! \n";
		return;
	}
	if (index >= cnt) {
		std::cout << "Podany indeks jest wiekszy od tablicy!\n";
		return;
	}

	--sideLength;
	--cnt;
	if (!sideLength) {
		delete[] tab;
		tab = NULL;
	}
	else {
		T* tabTemp = new T[sideLength];

		// copying the values
		int i = 0;
		for (; i < index; i++) {
			tabTemp[i] = tab[i];
		}
		for (i = index + 1; i <= cnt; i++) {
			tabTemp[i - 1] = tab[i];
		}

		delete[] tab;

		tab = tabTemp;
	}
}

template<typename T> void Table<T>::generateEmpty(int sideLength) {
	tab = new T[sideLength];
	for (int i = 0; i < sideLength; i++) {
		tab[0] = nullptr;
	}
	sideLength = sideLength;
	cnt = 0;
}

template<typename T> void Table<T>::clear() {
	delete[] tab;
}

template<typename T> int Table<T>::length() {
	return sideLength;
}

template<typename T> int Table<T>::count() {
	return cnt;
}

template<typename T> bool Table<T>::empty() {
	return !cnt;
}