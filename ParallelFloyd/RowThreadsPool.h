#pragma once
#include<vector>
#include"RowThread.h"
#include"Barrier.h"
#include <iostream>
class RowThreadsPool
{
public:
	RowThreadsPool();
	~RowThreadsPool();
	int** calcMatrix(int** matr, int count);

private:
	int threadsCount;
	std::vector<RowThread*> threads;
	barrier *barrier1;
	void stopThreads();
};

RowThreadsPool::RowThreadsPool()
{
}

RowThreadsPool::~RowThreadsPool()
{
	for (int i = 0; i < threadsCount; i++) {
		delete threads.at(i);
	}
}

inline int ** RowThreadsPool::calcMatrix(int ** matr, int count)
{
	threadsCount = count - 1;
	barrier1 = new barrier(count);
	for (int i = 0; i < threadsCount; i++) {
		RowThread *rowThread = new RowThread(barrier1, i);
		threads.push_back(rowThread);
	}
	int ** newMatrix = new int*[count];
	for (int i = 0; i < count; i++) {
		newMatrix[i] = new int[i];
	}
	for (int k = 0; k < count; k++) {
		for (int i = 0, j = 0; i < count; i++, j++) {
			if (i != k){
				threads.at(j)->startCalcMatr(matr, newMatrix, k, i, count);
			}
			else {
				j--;
			}
		}
		for (int i = 0; i < count; i++) {
			newMatrix[k][i] = matr[k][i];
		}
		barrier1->wait();
		int** temp = newMatrix;
		newMatrix = matr;
		matr = temp;
	}
	stopThreads();
	return matr;
}

inline void RowThreadsPool::stopThreads()
{
	for (int i = 0; i < threadsCount; i++) {
		threads.at(i)->stop();
	}
}
