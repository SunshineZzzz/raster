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

bool GLContext::PrepareShader(const char* vertexPath, const char* fragmentPath)
{
    m_shader = std::make_unique<Shader>(vertexPath, fragmentPath);
    return m_shader->isInitialized();
}

void GLContext::BeginShader() 
{
    m_shader->Begin();
}

void GLContext::EndShader()
{
    m_shader->End();
}

void GLContext::SetUniformFloat(const std::string& name, float value)
{
    m_shader->SetUniformFloat(name, value);
}

void GLContext::SetUniformVector3(const std::string& name, float x, float y, float z)
{
    m_shader->SetUniformVector3(name, x, y, z);
}

void GLContext::SetUniformVector3(const std::string& name, const float* values)
{
    m_shader->SetUniformVector3(name, values);
}

void GLContext::SetUniformInt(const std::string& name, int value)
{
    m_shader->SetUniformInt(name, value);
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
    if (glIsVertexArray(m_vao))
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }

    if (glIsBuffer(m_interleavedVbo))
    {
        glDeleteBuffers(1, &m_interleavedVbo);
        m_interleavedVbo = 0;
    }

    if (glIsBuffer(m_posVbo))
	{
		glDeleteBuffers(1, &m_posVbo);
        m_posVbo = 0;
	}

    if (glIsBuffer(m_colorVbo))
	{
		glDeleteBuffers(1, &m_colorVbo);
        m_colorVbo = 0;
	}

    if (glIsBuffer(m_uvVbo))
    {
        glDeleteBuffers(1, &m_uvVbo);
        m_uvVbo = 0;
    }

    if (glIsTexture(m_texture))
    {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }

    if (glIsBuffer(m_ebo))
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

    if (m_glcontext)
    {
        SDL_GL_DestroyContext(m_glcontext);
    }
}

