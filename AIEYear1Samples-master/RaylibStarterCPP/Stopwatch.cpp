#include "Stopwatch.h"

long Stopwatch::ElapsedTime()
{
	DateTime justObserved = timeLastCalled;

	timeLastCalled = TimeNowMS;

	duration<double> elapsedSeconds = timeLastCalled - justObserved;

	return elapsedSeconds.count();
}
