#pragma once

#include "OpenGLShader.h"
#include "constants.h"
#include <queue>
#include <string>

namespace engineECS
{
	class ShaderManager
	{
	public:
		const engineECS::OpenGLShader& getCompiledShader(const int index) const;
		int getMaxCompiledShadersCount() const;
		int getCompiledShadersCount() const;
		const engineECS::OpenGLShader* getCompiledShaders() const;
		bool tryCreateOpenglShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource, int& outProgramIndex);
		ShaderManager();
	private:
		bool tryAddCompiledShader(engineECS::OpenGLShader&& shader, int& outIndex);

		std::queue<int> recycler;
		engineECS::OpenGLShader programs[engineECS::MaxUniquePrograms];
	};
}

