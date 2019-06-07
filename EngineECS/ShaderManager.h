#pragma once

#include "OpenGLShader.h"

#include <queue>
#include <string>

namespace engineECS
{
	class ShaderManager
	{
	public:
		static int addCompiledShader(const engineECS::OpenGLShader& shader, bool& outSuccess);
		static engineECS::OpenGLShader& getCompiledShader(const int index);
		static int getMaxCompiledShadersCount();
		static int getCompiledShadersCount();
		static engineECS::OpenGLShader* getCompiledShaders();
		static bool tryCreateOpenglShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource, int& outProgramIndex);

		static void initialize();

		static bool isInitialized();

	private:
		static bool initialized;
		static std::queue<int> recycler;
	};
}

