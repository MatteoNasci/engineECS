#include "ShaderManager.h"
#include "constants.h"
#include <queue>

bool engineECS::ShaderManager::initialized = false;
std::queue<int> engineECS::ShaderManager::recycler;
bool engineECS::ShaderManager::isInitialized()
{
	return engineECS::ShaderManager::initialized;
}
int engineECS::ShaderManager::addCompiledShader(const engineECS::OpenGLShader& shader, bool& outSuccess)
{
	if (engineECS::ShaderManager::recycler.size() == 0 || !shader.isProgramSuccessfullyCreated())
	{
		outSuccess = false;
		return -1;
	}

	outSuccess = true;
	const int newIndex = engineECS::ShaderManager::recycler.front();
	engineECS::ShaderManager::recycler.pop();

	getCompiledShaders()[newIndex] = shader;
	return newIndex;
}
engineECS::OpenGLShader& engineECS::ShaderManager::getCompiledShader(const int index)
{
	return getCompiledShaders()[index];
}
int engineECS::ShaderManager::getMaxCompiledShadersCount()
{
	return engineECS::MaxUniquePrograms;
}
int engineECS::ShaderManager::getCompiledShadersCount()
{
	return engineECS::MaxUniquePrograms - static_cast<int>(engineECS::ShaderManager::recycler.size());
}
engineECS::OpenGLShader* engineECS::ShaderManager::getCompiledShaders()
{
	static engineECS::OpenGLShader compiledShaders[engineECS::MaxUniquePrograms];
	return compiledShaders;
}
void engineECS::ShaderManager::initialize()
{
	if (!engineECS::ShaderManager::initialized)
	{
		engineECS::ShaderManager::initialized = !engineECS::ShaderManager::initialized;
		while (engineECS::ShaderManager::recycler.size() > 0)
		{
			engineECS::ShaderManager::recycler.pop();
		}
		for (register int i = 0; i < engineECS::MaxUniquePrograms; ++i)
		{
			engineECS::ShaderManager::recycler.push(i);
		}
	}
}
bool engineECS::ShaderManager::tryCreateOpenglShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource, int& outProgramIndex)
{
	bool result;
	outProgramIndex = addCompiledShader(engineECS::OpenGLShader(vertexSource, fragmentSource, geometrySource), result);
		return result;
}
