#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Renderer.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Camera.h"
#include "PointLight.h"
#include "FrameBuffer.h"

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
    // 是否已经初始化过
    bool m_initialized = false;

public:
    // gl上下文
    SDL_GLContext m_glcontext = nullptr;
    // 场景
    std::shared_ptr<Scene> m_scene;
    // 离屏场景
    std::shared_ptr<Scene> m_offscreenScene;
    // 离屏渲染对应的帧缓冲
    std::shared_ptr<FrameBuffer> m_offscreenFB = nullptr;
    // 渲染器
    std::shared_ptr<Renderer> m_renderer;
    // 平行光
    std::shared_ptr<DirectionalLight> m_dirLight = nullptr;
    // 环境光
    std::shared_ptr<AmbientLight> m_ambLight = nullptr;
    // 点光源
    std::vector<std::shared_ptr<PointLight>> m_pointLights;
    // 探照灯
    std::shared_ptr<SpotLight> m_spotLight = nullptr;
};