#pragma once
#include <thread>
#include "Delegate.h"
#include <filesystem>

class FileWatcher
{
public:
	//FileWatcher(const char* filePath, Delegate& delegate, long msCycle = 1000);
	FileWatcher(const char** filePath, int fileCount, Delegate& delegate, long msCycle = 1000);
	~FileWatcher();
protected:
	void CheckFile() const;
	std::experimental::filesystem::path* m_path;
	std::chrono::system_clock::time_point* m_lastLoad;
	int m_fileCount;
	std::thread m_watcherThread;
	Delegate& m_callBack;
	long m_msCycle;
	bool m_stop;
};

