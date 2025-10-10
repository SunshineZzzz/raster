
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <memory>
#include <filesystem>

#include "inc/GLContext.h"
#include "inc/CheckRstErr.h"

std::unique_ptr<GLContext> glcontext;
SDL_Window* window = nullptr;

// 准备VAO，交错顶点(坐标+颜色)数据
void PrepareVAOByInterleavedData()
{
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	GL_CALL(glGenBuffers(1, &glcontext->m_interleavedVbo));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_interleavedVbo));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GL_CALL(glGenVertexArrays(1, &glcontext->m_vao));
	GL_CALL(glBindVertexArray(glcontext->m_vao));

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_interleavedVbo));
	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
	GL_CALL(glEnableVertexAttribArray(1));
	GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));

	glBindVertexArray(0);
}

// 准备VAO，独立顶点(坐标,颜色)数据
void PrepareVAOBySingleData()
{
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	float colors[] = {
	   1.0f, 0.0f, 0.0f,
	   0.0f, 1.0f, 0.0f,
	   0.0f, 0.0f, 1.0f
	};

	glGenBuffers(1, &glcontext->m_posVbo);
	glGenBuffers(1, &glcontext->m_colorVbo);

	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenVertexArrays(1, &glcontext->m_vao);
	glBindVertexArray(glcontext->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

// 准备VAO，2个三角形数据
void PrepareVAO2TriangleData() {
	float positions[] = 
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.8f, 0.8f, 0.0f,
		0.8f, 0.0f, 0.0f
	};

	glGenBuffers(1, &glcontext->m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenVertexArrays(1, &glcontext->m_vao);
	glBindVertexArray(glcontext->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glEnableVertexAttribArray(0);
	// 配置当前的VAO，告诉它去当前绑定的VBO里，以每12字节(3*sizeof(float))的间隔，读取3个浮点数作为位置属性，
	// 并将这些数据喂给着色器中的Location 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

// 准备VAO，使用EBO
void PrepareVAOAndEBO() 
{
	float positions[] = 
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};
	// 三角行顶点索引
	unsigned int indices[] = 
	{
		0, 1, 2,
		2, 1, 3
	};

	glGenBuffers(1, &glcontext->m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);


	glGenBuffers(1, &glcontext->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glcontext->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &glcontext->m_vao);
	glBindVertexArray(glcontext->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glcontext->m_ebo);

	glBindVertexArray(0);
}

// 准备VAO
void PrepareVAO() {
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
		0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = 
	{
		0, 1, 2
	};

	glGenBuffers(1, &glcontext->m_posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &glcontext->m_colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &glcontext->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glcontext->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &glcontext->m_vao);
	glBindVertexArray(glcontext->m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, glcontext->m_colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glcontext->m_ebo);

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
	// 准备shader
    if (!glcontext->PrepareShader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"))
	{
		SDL_Log("couldn't prepare shader error");
		return SDL_APP_FAILURE;
	}
	// 1.准备vao
	// 准备VAO，交错顶点(坐标+颜色)数据
	// render需要glDrawArrays(GL_TRIANGLES, 0, 3);
	// PrepareVAOByInterleavedData();
	
	// 准备VAO，独立顶点(坐标,颜色)数据
	// render需要glDrawArrays(GL_TRIANGLES, 0, 3);
	// PrepareVAOBySingleData();

	// 准备VAO，2个三角形数据
	// render需要glDrawArrays(GL_TRIANGLE_STRIP 0, 6);
	// PrepareVAO2TriangleData();

	// 准备VAO，2个三角形数据，采用GL_TRIANGLE_FAN模式
	// render需要glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	// PrepareVAO2TriangleData();

	// 准备VAO，2个三角形数据，采用GL_LINES模式
	// render需要glDrawArrays(GL_LINES, 0, 6);
	// PrepareVAO2TriangleData();

	// 准备VAO，2个三角形数据，采用GL_LINE_STRIP模式
	// render需要glDrawArrays(GL_LINE_STRIP, 0, 6);
	// PrepareVAO2TriangleData();

	// 准备VAO，使用EBO
	// render需要glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// PrepareVAOAndEBO();

	// 准备VAO
	// render需要glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	PrepareVAO();

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

	// 2.绑定VAO
	GL_CALL(glBindVertexArray(glcontext->m_vao));
	
	// 3.发出绘制指令
	// glDrawArrays(GL_TRIANGLES, 0, 3);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
	// glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
	// glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	// glDrawArrays(GL_LINES, 0, 6);
	// glDrawArrays(GL_LINE_STRIP, 0, 6);
	// 6 - 不是顶点数量，而是要从EBO中读取的索引的总数
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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