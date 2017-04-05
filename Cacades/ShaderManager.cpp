#pragma once
#include "ShaderManager.h"
#include "Shader.h"

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

void ShaderManager::RegisterShader(Shader& shader)
{
	m_shaders.push_back(&shader);
}

void ShaderManager::Update(float deltaTime)
{
	m_timeSinceLastCheck += deltaTime;

	if (m_timeSinceLastCheck < RELOAD_CYCLE)
		return;

	if (m_isWorking)
		return;

	m_isWorking = true;
	m_ioThread = new std::thread(&ShaderManager::CheckForChanges, this);

	m_timeSinceLastCheck -= RELOAD_CYCLE;
}

void ShaderManager::CheckForChanges()
{
	for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		const char* const* files = (*it)->GetSourceFiles();
		for (int i = 0; i < 3; ++i)
		{
			const char* file = files[i];

			if (file == nullptr)
				continue;

			HANDLE pFile;
			FILETIME creationTime, lpLastAccessTime, lastWriteTime;

			pFile = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			
			// Retrieve the file times for the file.
			if (GetFileTime(pFile, &creationTime, &lpLastAccessTime, &lastWriteTime) == false)
				continue;

			long isNew = CompareFileTime(&lastWriteTime, (*it)->GetLastBuildTime());
			
			if (isNew == 1)
				(*it)->IsDirty(true);
		}
	}
	m_isWorking = false;
}

void ShaderManager::UnixTimeToFileTime(time_t t, FILETIME* pft)
{
	// Note that LONGLONG is a 64-bit value
	LONGLONG ll;

	ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = ll >> 32;
}
