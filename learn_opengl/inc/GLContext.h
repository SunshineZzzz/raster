#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "Shader.h"

#include <memory>

class GLContext
{
public:
    // GL属性设置
    static bool InitGLAttributes();

public:
    GLuint m_vao = 0;
    GLuint m_interleavedVbo = 0;
    GLuint m_posVbo = 0;
    GLuint m_colorVbo = 0;
    GLuint m_uvVbo = 0;
    GLuint m_texture = 0;
    GLuint m_ebo = 0;
    
    GLContext(SDL_Window* window);
    ~GLContext();

    // 交换显卡内存缓冲区
    void SwapWindow();
    // 准备Shader
    bool PrepareShader(const char* vertexPath, const char* fragmentPath);
    // 开始使用Shader
    void BeginShader();
	// 结束使用Shader
	void EndShader();
    // 设置uniform变量
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformVector3(const std::string& name, float x, float y, float z);
    void SetUniformVector3(const std::string& name, const float* values);
    void SetUniformInt(const std::string& name, int value);

private:
    // GL初始化
    void setup();
    // GL销毁
    void shutdown();

    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glcontext = nullptr;
    bool m_initialized = false;
    std::unique_ptr<Shader> m_shader = nullptr;
};