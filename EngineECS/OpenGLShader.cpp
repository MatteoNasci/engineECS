#include "OpenGLShader.h"


bool engineECS::OpenGLShader::checkCompiledShader(const GLuint shader)
{
	int isCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == FALSE)
	{
		int maxLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		char* infoLog = new char[maxLength];

		if (infoLog)
		{
			glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);
			std::cerr << infoLog << std::endl;

			delete[] infoLog;
		}
		return false;
	}
	return true;
}
engineECS::OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource) : isProgramCreated(false), projectionUniform(), program(), modelUniform(), viewUniform(), bonesMatrixUniform()
{
	program = glCreateProgram();

	const GLuint vertexShader = engineECS::OpenGLShader::compileShader(vertexSource, GL_VERTEX_SHADER);
	if (!engineECS::OpenGLShader::checkCompiledShader(vertexShader))
	{
		return;
	}

	const GLuint fragmentShader = engineECS::OpenGLShader::compileShader(fragmentSource, GL_FRAGMENT_SHADER);
	if (!engineECS::OpenGLShader::checkCompiledShader(fragmentShader))
	{
		glDeleteShader(vertexShader);
		return;
	}

	GLuint geometryShader = {};
	bool useGeometry = (geometrySource.size() > 0);
	if (useGeometry)
	{
		geometryShader = engineECS::OpenGLShader::compileShader(geometrySource, GL_GEOMETRY_SHADER);
		if (!engineECS::OpenGLShader::checkCompiledShader(geometryShader))
		{
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return;
		}
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	if (useGeometry)
	{
		glAttachShader(program, geometryShader);
	}

	glLinkProgram(program);

	int isLinked;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == FALSE)
	{
		/* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
		int maxLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		char* infoLog = new char[maxLength];

		/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		std::cerr << infoLog << std::endl;

		delete[] infoLog;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	if (useGeometry)
	{
		glDetachShader(program, geometryShader);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (useGeometry)
	{
		glDeleteShader(geometryShader);
	}

	if (isLinked == TRUE)
	{
		modelUniform = glGetUniformLocation(program, "model");
		viewUniform = glGetUniformLocation(program, "view");
		projectionUniform = glGetUniformLocation(program, "projection");
		bonesMatrixUniform = glGetUniformLocation(program, "bones_matrix");

		isProgramCreated = true;
	}
}

GLuint engineECS::OpenGLShader::compileShader(const std::string& shaderSource, const GLenum shaderType)
{
	const GLuint shader = glCreateShader(shaderType);
	const GLchar* source = shaderSource.c_str();
	const GLint sourceLength = static_cast<GLint>(shaderSource.size());

	glShaderSource(shader, 1, &source, &sourceLength);
	glCompileShader(shader);

	return shader;
}
bool engineECS::OpenGLShader::isProgramSuccessfullyCreated() const
{
	return isProgramCreated;
}
engineECS::OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(program);
}