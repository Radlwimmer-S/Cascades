#include "FileWatcher.h"

//FileWatcher::FileWatcher(const char* filePath, Delegate& delegate, long msCycle) : m_path(new std::experimental::filesystem::path(filePath)), m_lastLoad(new std::chrono::system_clock::time_point(std::experimental::filesystem::last_write_time(m_path))), m_fileCount(1), m_callBack(delegate), m_msCycle(msCycle), m_stop(false)
//{
//	m_watcherThread = std::thread(&FileWatcher::CheckFile, this);
//}

//FileWatcher::FileWatcher(const char** filePath, int fileCount, Delegate& delegate, long msCycle) : m_path(new std::experimental::filesystem::path[fileCount]), m_lastLoad(new std::chrono::system_clock::time_point[fileCount]), m_fileCount(fileCount), m_callBack(delegate), m_msCycle(msCycle), m_stop(false)
//{
//	for (int i = 0; i < m_fileCount; ++i)
//	{
//		m_path[i] = std::experimental::filesystem::path(filePath[i]);
//		m_lastLoad[i] = std::experimental::filesystem::last_write_time(m_path[i]);
//	}
//	m_watcherThread = std::thread(&FileWatcher::CheckFile, this);
//}

FileWatcher::~FileWatcher()
{
	delete[] m_path;
	delete[] m_lastLoad;
	m_stop = true;
	m_watcherThread.join();
}

void FileWatcher::CheckFile() const
{
	while (!m_stop)
	{
		for (int i = 0; i < m_fileCount; ++i)
		{
			std::chrono::system_clock::time_point modifyStamp = std::experimental::filesystem::last_write_time(m_path[i]);

			if (modifyStamp > m_lastLoad[i])
			{
				m_callBack.Invoke();
				m_lastLoad[i] = modifyStamp;
			}
		}

		std::this_thread::sleep_for(std::chrono::system_clock::duration(m_msCycle));
	}
}
