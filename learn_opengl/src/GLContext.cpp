#include <glm/gtc/matrix_transform.hpp>

#include "../inc/GLContext.h"

bool GLContext::InitGLAttributes()
{

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    return true;
}

GLContext::GLContext(SDL_Window* window)
{
    if (m_initialized || !window)
    {
        return;
    }
    m_initialized = true;
    m_window = window;

    setup();
}

GLContext::~GLContext()
{
    shutdown();
}


void GLContext::SwapWindow()
{
    if (!m_initialized)
    {
        return;
    }

    SDL_GL_SwapWindow(m_window);
}


void GLContext::setup()
{
    // 创建GL上下文
    m_glcontext = SDL_GL_CreateContext(m_window);
    if (!m_glcontext)
    {
        SDL_Log("setup SDL_GL_CreateContext error,%s", SDL_GetError());
        return;
    }

    // 激活GL上下文，使其成为当前线程的绘图目标
    if (!SDL_GL_MakeCurrent(m_window, m_glcontext))
    {
        SDL_Log("setup SDL_GL_MakeCurrent error,%s", SDL_GetError());
        return;
    }

    // 加载GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        SDL_Log("gladLoadGLLoader Errors: %s", SDL_GetError());
        return;
    }
}

void GLContext::shutdown()
{
    if (m_glcontext)
    {
        SDL_GL_DestroyContext(m_glcontext);
    }

    m_window = nullptr;
}

