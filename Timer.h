#ifndef PUZZLE_TIMER
#define PUZZLE_TIMER

#include <sys/time.h>

class Timer{

public:
	Timer(){}
	void StartTimer()
	{
		struct timeval tim;
		gettimeofday(&tim, NULL);
		startTime = tim.tv_sec+(tim.tv_usec/1000000.0);
	}
	double GetElapsedTime()	// Returns the time in seconds
	{
		struct timeval tim;
		gettimeofday(&tim, NULL);
		double currentTime = tim.tv_sec+(tim.tv_usec/1000000.0);
		
		return currentTime - startTime;
	}
private:
	double startTime;
};

#endif
