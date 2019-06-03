#pragma once

#include "Math.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <string>
#include <iostream>

namespace engineECS
{

	class OpenGLShader
	{
	public:
		static GLuint compileShader(const std::string& shader_source, const GLenum shader_type);
		static bool checkCompiledShader(const GLuint shader);
		bool isProgramSuccessfullyCreated() const;

		OpenGLShader(const std::string& vertex_source, const std::string& fragment_source, const std::string& geometry_source = "");

		~OpenGLShader();

	protected:
		GLuint program;
		GLint modelUniform;
		GLint viewUniform;
		GLint projectionUniform;
		GLint bonesMatrixUniform;

	private:
		bool isProgramCreated;
	};
}
