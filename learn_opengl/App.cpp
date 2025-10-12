
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <memory>
#include <filesystem>

#include "inc/GLContext.h"
#include "inc/CheckRstErr.h"

std::unique_ptr<GLContext> glcontext = nullptr;
SDL_Window* window = nullptr;

// 准备VAO
void PrepareVAO() 
{
	float positions[] = 
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	float colors[] = 
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	};

	float uvs[] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
	};

	unsigned int indices[] = 
	{
		0, 1, 2,
	};

	// 创建VBO
	glGenBuffers(1, &glcontext->m_posVbo);
	// 绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	// 上传数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &glcontext->m_colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &glcontext->m_uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &glcontext->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glcontext->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 创建VAO
	glGenVertexArrays(1, &glcontext->m_vao);
	// 绑定VAO
	glBindVertexArray(glcontext->m_vao);

	// 加入位置属性信息给VAO
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// 加入颜色属性信息给VAO
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// 加入纹理坐标属性信息给VAO
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	// 加入索引信息给VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glcontext->m_ebo);

	// 解绑VAO
	glBindVertexArray(0);
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!GLContext::InitGLAttributes())
    {
        SDL_Log("couldn't initialize opengl attributes error,%s", SDL_GetError());
		return SDL_APP_FAILURE;
    }

    auto nWidth = 800;
    auto nHeight = 600;

    window = SDL_CreateWindow("learn opengl", nWidth, nHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("couldn't create window error,%s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    glcontext = std::make_unique<GLContext>(window);
    // 设置视口，左下角坐标为(0,0)，视口宽度为nWidth，高度为nHeight。
    // 方便NDC映射到屏幕坐标系。
    // opengl约定是右手坐标系，但是NDC是左手坐标系，屏幕内是Z轴正方向，越向屏幕内越远离摄像机。
    GL_CALL(glViewport(0, 0, nWidth, nHeight));
	// 设置清除颜色
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	// 1.准备各种数据
	// 准备shader
    if (!glcontext->PrepareShader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"))
	{
		SDL_Log("couldn't prepare shader error");
		return SDL_APP_FAILURE;
	}
	// 准备VAO
	PrepareVAO();
	// 准备纹理
	if (!glcontext->PrepareTexture(
		{ 
			"assets/textures/wall.jpg",
		}, 
		{ 
			0,
		}
	))
	{
		SDL_Log("couldn't prepare texture error");
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    switch(event->type)
	{
		case SDL_EVENT_WINDOW_RESIZED:
		{
			auto nWidth = event->window.data1;
			auto nHeight = event->window.data2;

            GL_CALL(glViewport(0, 0, nWidth, nHeight))
		}
        break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_QUIT:
        {
            return SDL_APP_SUCCESS;
        }
        break;
	}

    return SDL_APP_CONTINUE;
}

void render()
{
	// 清除颜色缓冲区
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	glcontext->BeginShader();

	glcontext->SetUniformFloat("time", (SDL_GetTicks()/1000.0f));
	// 纹理采样器设置为纹理单元0
	glcontext->SetUniformInt("sampler", 0);

	// 2.绑定VAO
	GL_CALL(glBindVertexArray(glcontext->m_vao));
	
	// 3.发出绘制指令
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	// 4.解绑VAO
	glBindVertexArray(0);

	glcontext->EndShader();
	glcontext->SwapWindow();
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	render();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    glcontext = nullptr;
    SDL_DestroyWindow(window);
    SDL_Quit();
}