#include "WorkBalance.h"
#include <thread>

WorkBalance::WorkBalance(int aWork, int _aThreads): aThreads(_aThreads) {
	int i = 0, workSoFar = 0, perThread = aWork / aThreads;
	int* tmpThreadStart = new int[aThreads];
	int* tmpThreadEnd = new int[aThreads];
	while (i < aWork % aThreads) {
		tmpThreadStart[i] = workSoFar;
		workSoFar += perThread + 1;
		tmpThreadEnd[i] = workSoFar;
		i++;
	}
	while (i < aThreads) {
		tmpThreadStart[i] = workSoFar;
		workSoFar += perThread;
		tmpThreadEnd[i] = workSoFar;
		i++;
	}
	threadStart = tmpThreadStart;
	threadEnd = tmpThreadEnd;
}

void WorkBalance::executeParallel(std::function<void(int, int)> executable) const
{
	std::thread* threads = new std::thread[aThreads];
	// Give each thread an equal amount to process.
	for (int i = 0; i < aThreads; i++)
	{
		threads[i] = std::thread(executable, threadStart[i], threadEnd[i]);
	}
	// Wait for the threads to finish
	for (int i = 0; i < aThreads; i++)
	{
		threads[i].join();
	}
	delete[] threads;
};

WorkBalance::~WorkBalance()
{
	delete[] threadStart;
	delete[] threadEnd;
}
