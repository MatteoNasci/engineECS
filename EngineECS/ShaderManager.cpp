#include "ShaderManager.h"

bool engineECS::ShaderManager::tryAddCompiledShader(engineECS::OpenGLShader&& shader, int& outIndex)
{
	if (engineECS::ShaderManager::recycler.size() == 0 || !shader.isProgramSuccessfullyCreated())
	{
		outIndex = -1;
		return false;
	}

	outIndex = engineECS::ShaderManager::recycler.front();
	engineECS::ShaderManager::recycler.pop();

	programs[outIndex] = shader;
	shader.isProgramEmpty = true;

	return true;
}
const engineECS::OpenGLShader& engineECS::ShaderManager::getCompiledShader(const int index) const
{
	return programs[index];
}
int engineECS::ShaderManager::getMaxCompiledShadersCount() const
{
	return engineECS::MaxUniquePrograms;
}
int engineECS::ShaderManager::getCompiledShadersCount() const
{
	return engineECS::MaxUniquePrograms - static_cast<int>(engineECS::ShaderManager::recycler.size());
}
const engineECS::OpenGLShader* engineECS::ShaderManager::getCompiledShaders() const
{
	return programs;
}
engineECS::ShaderManager::ShaderManager()
{
	for (register int i = 0; i < engineECS::MaxUniquePrograms; ++i)
	{
		engineECS::ShaderManager::recycler.push(i);
	}
}
bool engineECS::ShaderManager::tryCreateOpenglShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource, int& outProgramIndex)
{
	return tryAddCompiledShader(engineECS::OpenGLShader(vertexSource, fragmentSource, geometrySource), outProgramIndex);
}
