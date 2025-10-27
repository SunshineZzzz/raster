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
    // 保证析构顺序
    m_shader.reset();
    m_vTextures.resize(0);

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

bool GLContext::PrepareGeometry(Geometry* gy)
{
    m_geometry.reset(gy);
	return true;
}

bool GLContext::PrepareShader(const char* vertexPath, const char* fragmentPath)
{
    m_shader = std::make_unique<Shader>(vertexPath, fragmentPath);
    return m_shader->IsInitialized();
}

bool GLContext::PrepareTexture(const std::vector<std::string>& vPaths, const std::vector<unsigned int>& vUnits)
{
    if (vPaths.size() != vUnits.size())
    {
        SDL_Log("vPaths size != vUnits size");
        return false;
    }
    for (int i = 0; i < vPaths.size(); i++)
	{
        m_vTextures.emplace_back(std::make_unique<Texture>(vPaths[i], vUnits[i]));
	}
    bool ok = true;
    for (auto& t : m_vTextures)
	{
        if (!t->IsInitialized())
        {
            ok = false;
			break;
        }
	}
    if (!ok)
	{
        m_vTextures.resize(0);
	}
    return ok;
}

bool GLContext::PrepareModel(const glm::mat4& modelMatrix)
{
    m_modelMatrix = modelMatrix;
	return true;
}

bool GLContext::PrepareViewMatrix(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
    // lookat: 生成一个viewMatrix
    // eye: 当前摄像机所在的位置
    // center: 当前摄像机看向的那个点
    // up: 穹顶向量
    m_viewMatrix = glm::lookAt(eye, center, up);
    return true;
}

bool GLContext::PrepareViewMatrix(const glm::mat4& viewMatrix)
{
	m_viewMatrix = viewMatrix;
	return true;
}

bool GLContext::PrepareProjection(const glm::mat4& projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
    return true;
}

bool GLContext::PrepareProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
    m_projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    return true;
}

bool GLContext::PrepareProjection(float fovy, float aspect, float zNear, float zFar)
{
	m_projectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);
	return true;
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

void GLContext::SetUniformVector3(const std::string& name, const float* value)
{
    m_shader->SetUniformVector3(name, value);
}

void GLContext::SetUniformVector3(const std::string& name, const glm::vec3 value)
{
    m_shader->SetUniformVector3(name, value);
}

void GLContext::SetUniformInt(const std::string& name, int value)
{
    m_shader->SetUniformInt(name, value);
}

void GLContext::SetUniformMatrix4x4(const std::string& name, glm::mat4 value)
{
	m_shader->SetUniformMatrix4x4(name, value);
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
    m_geometry.reset(nullptr);

    if (m_glcontext)
    {
        SDL_GL_DestroyContext(m_glcontext);
    }

    m_window = nullptr;
}

