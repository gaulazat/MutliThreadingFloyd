#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Barrier.h"
class RowThread
{
public:
	RowThread(barrier *bar, int n) {
		enabled = true;
		barrier = bar;
		numThread = n;
		thread1 = std::thread(&RowThread::threadFn, this);
	}
	~RowThread();
	void startCalcMatr(int** matrix1, int** newMatrix1, int k, int numRow, int countRow1);
	void stop();
private:
	std::mutex mutex1;
	std::thread	thread1;
	std::condition_variable cv;
	bool enabled = true;
	void threadFn();
	barrier *barrier;

	bool isUpdated = false;
	//номер итерации
	int k;
	//номер строки
	int numRow;
	//обслуживаемая матрица
	int** matrix;
	//матрица результаты
	int** newMatrix;
	int countRow;
	int numThread;
	void calcMatrix();
	
	int min(int a, int b);
};
RowThread::~RowThread()
{
}

inline void RowThread::startCalcMatr(int ** matrix1, int ** newMatrix1, int k1, int numRow1, int countRow1)
{
	matrix = matrix1;
	newMatrix = newMatrix1;
	countRow = countRow1;
	k = k1;
	numRow = numRow1;
	enabled = true;
	isUpdated = true;
	cv.notify_one();
}

inline void RowThread::stop()
{
	isUpdated = true;
	enabled = false;
	cv.notify_one();
	thread1.join();
}

inline void RowThread::threadFn()
{
	while (enabled)
	{
		std::unique_lock<std::mutex> locker(mutex1);
		// Ожидаем уведомления, и убедимся что это не ложное пробуждение
		cv.wait(locker, [&]() { return isUpdated; });
		isUpdated = false;
		if (!enabled) {
			break;
		}
		calcMatrix();
		isUpdated = false;
		barrier->wait();
	}
}

inline void RowThread::calcMatrix()
{
	for (int j = 0; j < countRow; j++) {
		if (j == k) {
			newMatrix[numRow][j] = matrix[numRow][j];
		} else {
			newMatrix[numRow][j] = min(matrix[numRow][j], matrix[numRow][k] + matrix[k][j]);
		}
	}
}

int RowThread::min(int a, int b) {
	return a < b ? a : b;
}


