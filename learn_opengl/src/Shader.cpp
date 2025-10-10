#include <SDL3/SDL.h>

#include<string>
#include<fstream>
#include<sstream>

#include "../inc/Shader.h"
#include "../inc/CheckRstErr.h"

static std::tuple<const std::string, bool> readShaderFromFile(const char* szFilePath)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(szFilePath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		return {e.what(), false};
	}

	return {shaderCode, true};
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
	// 1.读取shader代码
	auto [vertexCode, ok1] = readShaderFromFile(vertexPath);
	if (!ok1)
	{
		SDL_Log("read vertex shader error,%s", vertexCode.c_str());
		return;
	}
	auto [fragmentCode, ok2] = readShaderFromFile(fragmentPath);
	if (!ok2)
	{
		SDL_Log("read fragment shader error,%s", fragmentCode.c_str());
		return;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// 2.创建shader程序
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	// 3.shader程序输入代码
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

	// 4.执行shader代码编译 
	glCompileShader(vertex);
	// 检查编译错误
	auto errStr = checkShaderErrors(vertex, "COMPILE");
	if (errStr != "") 
	{
		SDL_Log("compile vertex shader error,%s", errStr.c_str());

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return;
	}
	glCompileShader(fragment);
	errStr = checkShaderErrors(fragment, "COMPILE");
	if (errStr != "") 
	{
		SDL_Log("compile fragment shader error,%s", errStr.c_str());

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return;
	}

	// 5.创建一个Program壳子
	m_program = glCreateProgram();

	// 6.将vs与fs编译好的结果放到program
	glAttachShader(m_program, vertex);
	glAttachShader(m_program, fragment);

	// 7.执行program的链接操作，形成最终可执行shader程序
	glLinkProgram(m_program);
	// 检查链接错误
	errStr = checkShaderErrors(m_program, "LINK");
	if (errStr != "")
	{
		SDL_Log("link shader error,%s", errStr.c_str());

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteProgram(m_program);
		m_program = 0;
		return;
	}

	// 8.清理
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	m_initialized = true;
}

Shader::~Shader() 
{
	if (!m_initialized) 
	{
		return;
	}

	if (glIsProgram(m_program)) 
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

bool Shader::IsInitialized() const
{
	return m_initialized;
}

void Shader::Begin() 
{
	if (!m_initialized) 
	{
		return;
	}
	GL_CALL(glUseProgram(m_program));
}

void Shader::End() 
{
	if (!m_initialized) 
	{
		return;
	}
	GL_CALL(glUseProgram(0));
}

void Shader::SetUniformFloat(const std::string& name, float value)
{
	// 1.通过名称拿到Uniform变量的位置Location
	GLint location = GL_CALL(glGetUniformLocation(m_program, name.c_str()));
	// 2.通过Location更新Uniform变量的值
	GL_CALL(glUniform1f(location, value));
}

void Shader::SetUniformVector3(const std::string& name, float x, float y, float z)
{
	GLint location = GL_CALL(glGetUniformLocation(m_program, name.c_str()));
	GL_CALL(glUniform3f(location, x, y, z));
}

void Shader::SetUniformVector3(const std::string& name, const float* values)
{
	GLint location = GL_CALL(glGetUniformLocation(m_program, name.c_str()));
	GL_CALL(glUniform3fv(location, 1, values));
}

void Shader::SetUniformInt(const std::string& name, int value)
{
	GLint location = GL_CALL(glGetUniformLocation(m_program, name.c_str()));
	GL_CALL(glUniform1i(location, value));
}

const std::string Shader::checkShaderErrors(GLuint target, std::string type) 
{
	int success = 0;
	char infoLog[1024] = {};
	GLsizei logSize = sizeof(infoLog);

	if (type == "COMPILE") 
	{
		glGetShaderiv(target, GL_COMPILE_STATUS, &success);
		if (!success) 
		{
			glGetShaderInfoLog(target, logSize, NULL, infoLog);
			return infoLog;
		}
	}
	else if (type == "LINK")
	{
		glGetProgramiv(target, GL_LINK_STATUS, &success);
		if (!success) 
		{
			glGetProgramInfoLog(target, logSize, NULL, infoLog);
			return infoLog;
		}
	}
	else 
	{
		return "type is wrong";
	}

	return "";
}