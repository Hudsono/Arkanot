#pragma once

#include <iostream>
#include <chrono>

using namespace std::chrono;
using namespace std;

#define DateTime time_point<system_clock>
#define TimeNow system_clock::now()
//#define S_to_MS(x) std::chrono::time_point_cast<std::chrono::milliseconds>(x) #x
#define TimeNowMS time_point_cast<milliseconds>(TimeNow)

class Stopwatch
{
private:
	//std::chrono::time_point<std::chrono::system_clock> timeLastCalled;
	DateTime timeLastCalled;

public:
	Stopwatch()
	{
		timeLastCalled = TimeNowMS;
		cout << "New Stopwatch!" << endl;
	}

	long ElapsedTime();
};

