#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include<string>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	// 是否已经初始化过
	bool IsInitialized() const;
	// 开始使用当前Shader
	void Begin();
	// 结束使用当前Shader
	void End();
	// 设置uniform变量
	void SetUniformFloat(const std::string& name, float value);
	void SetUniformVector3(const std::string& name, float x, float y, float z);
	void SetUniformVector3(const std::string& name, const float* values);
	void SetUniformInt(const std::string& name, int value);
	void SetUniformMatrix4x4(const std::string& name, glm::mat4 value);
private:
	// 检测shader错误
	const std::string checkShaderErrors(GLuint target, std::string type);

	// 是否已经初始化过
	bool m_initialized = false;
	// 着色器程序Id
	GLuint m_program{ 0 };
};
