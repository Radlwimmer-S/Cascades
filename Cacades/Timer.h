#pragma once
#include "Delegate.h"
#include <chrono>

namespace cr = std::chrono;
typedef cr::high_resolution_clock timerClock;

class Timer
{
public:
	static std::chrono::steady_clock::duration Time(Delegate delegate);
	static void PrintToConsole(const char* description, std::chrono::steady_clock::duration dur);
	static void TimeAndPrint(const char* description, Delegate delegate);
};

