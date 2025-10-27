#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Geometry.h"

#include <vector>
#include <memory>

class GLContext
{
public:
    // GL属性设置
    static bool InitGLAttributes();

public:
    // 几何对象
    std::unique_ptr<Geometry> m_geometry = nullptr;
    
    GLContext(SDL_Window* window);
    ~GLContext();

    // 交换显卡内存缓冲区
    void SwapWindow();
    // 准备几何体
    bool PrepareGeometry(Geometry* gy);
    // 准备Shader
    bool PrepareShader(const char* vertexPath, const char* fragmentPath);
    // 准备texture
    bool PrepareTexture(const std::vector<std::string>& vPaths, const std::vector<unsigned int>& vUnits);
    // 准备模型变化矩阵
    bool PrepareModel(const glm::mat4& modelMatrix);
    // 准备视图变化矩阵
    bool PrepareViewMatrix(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
    bool PrepareViewMatrix(const glm::mat4& viewMatrix);
	// 准备投影变化矩阵
    bool PrepareProjection(const glm::mat4& projectionMatrix);
	bool PrepareProjection(float left, float right, float bottom, float top, float zNear, float zFar);
    bool PrepareProjection(float fovy, float aspect, float zNear, float zFar);
    // 开始使用Shader
    void BeginShader();
	// 结束使用Shader
	void EndShader();
    // 设置uniform变量
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformVector3(const std::string& name, float x, float y, float z);
    void SetUniformVector3(const std::string& name, const float* value);
    void SetUniformVector3(const std::string& name, const glm::vec3 value);
    void SetUniformInt(const std::string& name, int value);
    void SetUniformMatrix4x4(const std::string& name, glm::mat4 value);
private:
    // GL初始化
    void setup();
    // GL销毁
    void shutdown();

    // SDL窗口指针
    SDL_Window* m_window = nullptr;
    // gl上下文
    SDL_GLContext m_glcontext = nullptr;
    // 是否已经初始化过
    bool m_initialized = false;
    // 封装的gl着色器对象
    std::unique_ptr<Shader> m_shader = nullptr;

public:
    // 封装的gl纹理对象们
    std::vector<std::unique_ptr<Texture>> m_vTextures;
    // 模型变化矩阵
    glm::mat4 m_modelMatrix = glm::identity<glm::mat4>();
    // 摄像机，视图变化矩阵
    glm::mat4 m_viewMatrix = glm::identity<glm::mat4>();
    // 投影变化矩阵
    glm::mat4 m_projectionMatrix = glm::identity<glm::mat4>();
};