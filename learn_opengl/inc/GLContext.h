#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Renderer.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Camera.h"

#include <vector>
#include <memory>

class GLContext
{
public:
    // GL属性设置
    static bool InitGLAttributes();

public:    
    GLContext(SDL_Window* window);
    ~GLContext();

    // 交换显卡内存缓冲区
    void SwapWindow();

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

public:
    // 网格s
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    // 渲染器
    std::shared_ptr<Renderer> m_renderer;
    // 平行光
    std::shared_ptr<DirectionalLight> m_dirLight = nullptr;
    // 环境光
    std::shared_ptr<AmbientLight> m_ambLight = nullptr;
};