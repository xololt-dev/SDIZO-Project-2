#include "fsp.hpp"

void FSP::dijkstraMatrix() {
	while (!prioQueue.empty()) {
		prioQueue.pop();
	}
	
	int numberOfVertex = neighborMatrix.size();
	int vertexID = 0;

	Matrix outputMatrix;
	outputMatrix.clear();

	for (int j = 0; j < numberOfVertex; j++) {
		sub_list.push_back(0);
	}
	for (int i = 0; i < numberOfVertex; i++) {
		outputMatrix.push_back(sub_list);
	}

	numberOfVertex--;
}
