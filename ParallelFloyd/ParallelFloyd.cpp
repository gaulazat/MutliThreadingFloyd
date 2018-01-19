// ParallelFloyd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "RowThreadsPool.h"

using namespace std;

void printMatrix(int** matrix, int numberOfVert) {
	ofstream f("matrixOut1.txt");
	for (int i = 0; i < numberOfVert; i++) {
		for (int j = 0; j < numberOfVert; j++) {
			f << matrix[i][j] << " ";
		}
		f << endl;
	}
}

int min(int a, int b) {
	return a < b ? a : b;
}

//matrix - матрица смежности
void originalFloydWarshall(int **matrix, int numberOfVert) {
	//Пробегаемся по всем вершинам и ищем более короткий путь
	//через вершину k
	for (int k = 0; k < numberOfVert; k++) {
		for (int i = 0; i < numberOfVert; i++) {
			for (int j = 0; j < numberOfVert; j++) {
				//Новое значение ребра равно минимальному между старым
				//и суммой ребер i <-> k + k <-> j (если через k пройти быстрее)
				matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
			}
		}
	}

	return;
}

int readMatrFromFile(int** &a) {
	ifstream f("matrix1.txt");
	int count = 10;
	a = new int*[count];
	for (int i = 0; i < count; i++) {
		a[i] = new int[count];
	}
	/*int b[6][6] = {
		{ 0, 2, 5, 7, 100000, 100000 },
		{ 100000, 0, 100000, 4, 100000 ,8},
		{ 100000, 100000, 0, 100000, 6, 100000 },
		{ 100000, 100000, 100000 ,0,4,3},
		{ 3, 100000, 100000, 100000, 0,2},
		{ 100000, 100000, 1, 100000, 100000, 0}
	};*/
	for (int i = 0; i < count; i++){
		for (int j = 0; j < count; j++){
			if (i != j){
				a[i][j] = rand() % 1000 + 1;;
			}
			else {
				a[i][j] = 0;
			}
		}
	}
	return count;
}

void main() {
	int ** a;
	int count = readMatrFromFile(a); 	
	unsigned int start_time = clock();
	RowThreadsPool pool;
	a = pool.calcMatrix(a, count);
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time;
	printMatrix(a, count);
	cout <<"parallel "<< search_time;
	originalFloydWarshall(a, count);
	unsigned int end_time2 = clock();
	cout << endl << "serial " << end_time2 - end_time;
	cin >> count;
}