#pragma once

#define NOMINMAX
#include <Windows.h>
#include <vector>
#include <thread>
class Shader;

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void RegisterShader(Shader& shader);

	void Update(float deltaTime);

protected:

	void CheckForChanges();
	static void UnixTimeToFileTime(time_t t, FILETIME* pft);
	const float RELOAD_CYCLE = 1.0f;
	float m_timeSinceLastCheck;
	std::vector<Shader*> m_shaders;
	std::thread* m_ioThread;
	bool m_isWorking;
};

