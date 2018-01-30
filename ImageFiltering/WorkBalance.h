#pragma once
#include <functional>
class WorkBalance
{
private:
	const int aThreads;
public:
	const int * threadStart;
	const int * threadEnd;

	WorkBalance(int aWork, int aThreads);
	void executeParallel(std::function<void(int,int)> executable) const;
	~WorkBalance();
};

