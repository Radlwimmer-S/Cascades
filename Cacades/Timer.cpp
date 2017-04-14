#include "Timer.h"
#include <iostream>
#include <iomanip>
#include <chrono>

timerClock::duration Timer::Time(Delegate delegate)
{
	auto startTime = timerClock::now();
	delegate.Invoke();
	auto endTime = timerClock::now();
	return endTime - startTime;
}

void Timer::PrintToConsole(const char* description, timerClock::duration dur)
{
	auto hours = cr::duration_cast<cr::hours>(dur);
	dur -= hours;

	auto mins = cr::duration_cast<cr::minutes>(dur);
	dur -= mins;

	auto secs = cr::duration_cast<cr::seconds>(dur);
	dur -= secs;

	auto millis = cr::duration_cast<cr::milliseconds>(dur);

	std::cout << "INFO::TIME " << description << ": ";
	std::cout << std::setfill('0');
	std::cout << std::setw(2) << hours.count() << ':'
		<< std::setw(2) << mins.count() << ':'
		<< std::setw(2) << secs.count() << '.'
		<< std::setw(3) << millis.count() << "\n";
}

void Timer::TimeAndPrint(const char* description, Delegate delegate)
{
	PrintToConsole(description, Time(delegate));
}
