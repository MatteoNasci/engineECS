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
		GLuint getProgram() const;
		GLint getModelUniform() const;
		GLint getViewUniform() const;
		GLint getProjectionUniform() const;
		GLint getBonesMatrixUniform() const;

	private:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");
		OpenGLShader();
		OpenGLShader(OpenGLShader& other) = default;
		OpenGLShader& operator=(OpenGLShader& other) = default;
		OpenGLShader(OpenGLShader&& other);
		OpenGLShader& operator=(OpenGLShader&& other);
		~OpenGLShader();

		GLuint program;
		GLint modelUniform;
		GLint viewUniform;
		GLint projectionUniform;
		GLint bonesMatrixUniform;
		bool isProgramCreated : 1;
		bool isProgramEmpty : 1;

		friend class ShaderManager;
	};
}
