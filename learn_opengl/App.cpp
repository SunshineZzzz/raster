
#define SDL_MAIN_USE_CALLBACKS 1
#define STB_IMAGE_IMPLEMENTATION 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stb/stb_image.h>

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
		-0.5f, 0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	float colors[] = 
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f
	};

	float uvs[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	unsigned int indices[] = 
	{
		0, 1, 2,
		2, 1, 3
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

// 准备纹理
void PrepareTexture() 
{
	int width, height, channels;
	// 反转y轴
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("assets/textures/hinata.jpg", &width, &height, &channels, STBI_rgb_alpha);

	// 创建纹理对象
	glGenTextures(1, &glcontext->m_texture);
	// 激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	// 绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, glcontext->m_texture);
	// 纹理对象glcontext->m_texture就被映射到了纹理单元GL_TEXTURE0
	// 开辟显存，并上传数据
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	// 释放数据
	stbi_image_free(data);

	// 设置纹理的过滤方式
	// 需要的像素比实际纹理对象像素多，采用线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 需要的像素比实际纹理对象像素少，采用临近过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 设置纹理的包裹方式
	// u纹理坐标超出[0,1]范围，采用重复模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// v纹理坐标超出[0,1]范围，采用重复模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	// 准备shader
    if (!glcontext->PrepareShader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"))
	{
		SDL_Log("couldn't prepare shader error");
		return SDL_APP_FAILURE;
	}
	// 1.准备各种数据
	// 准备VAO
	PrepareVAO();
	// 准备纹理
	PrepareTexture();

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
	glcontext->SetUniformInt("sampler", 0);

	// 2.绑定VAO
	GL_CALL(glBindVertexArray(glcontext->m_vao));
	
	// 3.发出绘制指令
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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