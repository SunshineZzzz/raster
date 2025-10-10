#pragma once

#include <glad/glad.h>
#include<string>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	// 是否初始化成功
	bool isInitialized() const;
	// 开始使用当前Shader
	void Begin();
	// 结束使用当前Shader
	void End();
	// 设置uniform变量
	void SetUniformFloat(const std::string& name, float value);
	void SetUniformVector3(const std::string& name, float x, float y, float z);
	void SetUniformVector3(const std::string& name, const float* values);
	void SetUniformInt(const std::string& name, int value);

private:
	// 检测shader错误
	const std::string checkShaderErrors(GLuint target, std::string type);

	bool m_initialized = false;
	GLuint m_program{ 0 };
};
