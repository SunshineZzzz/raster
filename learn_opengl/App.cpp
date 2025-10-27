
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <filesystem>

#include "inc/GLContext.h"
#include "inc/CheckRstErr.h"
#include "inc/CameraControl.h"
#include "inc/OrthographicCamera.h"
#include "inc/PerspectiveCamera.h"
#include "inc/TrackBallCameraControl.h"
#include "inc/GameCameraControl.h"
#include "inc/Geometry.h"

auto nWidth = 800;
auto nHeight = 600;
SDL_Window* window = nullptr;
std::unique_ptr<Camera> camera = nullptr;
std::unique_ptr<CameraControl> cameraControl = nullptr;
std::unique_ptr<GLContext> glcontext = nullptr;

// 绕着Z轴旋转
void DoRotationZTransform(glm::mat4& oriM)
{
	float angle = 1.0f;
	oriM = glm::rotate(oriM, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

// 先平移再叠加Z轴旋转
void DoTranslateAndRotateZTransform(glm::mat4& oriM)
{
	// 第一次平移以后，旋转
	// 第二次在第一次的基础上平移(xy轴发生变化，z轴没变，斜着平移)，再旋转
	// ...
	float angle = 1.0f;
	oriM = glm::translate(oriM, glm::vec3(0.01f, 0.0f, 0.0f));
	oriM = glm::rotate(oriM, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

// 先旋转再叠加平移
void DoRotateZAndTranslateTransform(glm::mat4& oriM)
{
	// 第一次旋转以后，平移(xy轴发成变化，z轴没变，斜着平移)
	// 第二次在第一次的基础上旋转，再平移
	// ...
	float angle = 1.0f;
	oriM = glm::rotate(oriM, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
	oriM = glm::translate(oriM, glm::vec3(0.01f, 0.0f, 0.0f));
}

void DoOnceTransform(glm::mat4& oriM)
{
	// 第一进入这里，oriM是一个4x4的单位矩阵
	oriM = glm::scale(oriM, glm::vec3(0.5f, 1.0f, 1.0f));
	/*
	* [1 0 0 0]  [0.5f 0    0    0]   [0.5 0 0 0]
	* [0 1 0 0]  [0    1.0f 0    0]   [0   1 0 0]
	* [0 0 1 0]  [0    0    1.0f 0]   [0   0 1 0]
	* [0 0 0 1]  [0    0    0    1]   [0   0 0 1]
	*/
}

// 先做一次缩放，再叠加平移 
void DoScaleAndTranslateTransform(glm::mat4& oriM)
{
	static bool isDoOneceTransform = false;
	if (!isDoOneceTransform)
	{
		DoOnceTransform(oriM);
		isDoOneceTransform = true;
	}
	// 第一次进入这里，oriM：
	/*
	* [0.5 0 0 0]
	* [0   1 0 0]
	* [0   0 1 0]
	* [0   0 0 1]
	*/
	oriM = glm::translate(oriM, glm::vec3(0.01f, 0.0f, 0.0f));
	/*
	* [0.5 0 0 0]  [1 0 0 0.01]   [0.5 0 0 0.004999(1*(0.01*0.05f))]
	* [0   1 0 0]  [0 1	0    0]   [0   1 0        0]
	* [0   0 1 0]  [0 0	1    0]   [0   0 1        0]
	* [0   0 0 1]  [0 0	0    1]   [0   0 0        1]
	*/
	/*
	* [0.5 0 0 0.004999]  [1 0 0 0.01]  [0.5 0 0  0.00999999978(2*(0.01f*0.5f))]
	* [0   1 0        0]  [0 1	0   0]  [0   1 0              0]
	* [0   0 1        0]  [0 0	1   0]  [0   0 1              0]
	* [0   0 0        1]  [0 0	0   1]  [0   0 0              1]
	*/
}

// 准备摄像机相关
void PrepareCamera()
{
	// 准备摄像机视图变化矩阵+投影矩阵
	// float size = 6.0f;
	// camera.reset(new OrthographicCamera(-size, size, size, -size, size, -size));
	camera.reset(new PerspectiveCamera(60.f, (float)(nWidth / nHeight), 0.1f, 1000.0f));

	cameraControl.reset(new TrackBallCameraControl());
	// cameraControl.reset(new GameCameraControl);
	cameraControl->SetCamera(camera.get());
	cameraControl->SetSensitivity(0.4f);
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if (!GLContext::InitGLAttributes())
    {
        SDL_Log("couldn't initialize opengl attributes error,%s", SDL_GetError());
		return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("learn opengl", nWidth, nHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("couldn't create window error,%s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    glcontext = std::make_unique<GLContext>(window);

	// 开启深度检测
	glEnable(GL_DEPTH_TEST);
	// 深度检测的比较函数, GL_LESS: 当前片元深度值小于当前深度缓冲去中深度值时通过测试
	glDepthFunc(GL_LESS);

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
	// 准备纹理
	if (!glcontext->PrepareTexture(
		{ 
			"assets/textures/earth.png",
		}, 
		{ 
			// 都用的0号纹理单元，依靠各自的Bind函数来切换绑定纹理贴图
			0,
		}
	))
	{
		SDL_Log("couldn't prepare texture error");
		return SDL_APP_FAILURE;
	}
	// 准备模型变化矩阵
	if (!glcontext->PrepareModel(glm::mat4(1.0f)))
	{
		SDL_Log("couldn't prepare model error");
		return SDL_APP_FAILURE;
	}
	// 准备摄像机相关
	PrepareCamera();
	// 准备几何体
	// 1.其实就是准备vao
	// if (!glcontext->PrepareGeometry(Geometry::CreateBox(3.0f)))
	// if (!glcontext->PrepareGeometry(Geometry::CreateSphere(3.0f)))
	if (!glcontext->PrepareGeometry(Geometry::CreatePlane(3.0f, 2.0f)))
	{
		SDL_Log("couldn't prepare geometry error");
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
			nWidth = event->window.data1;
			nHeight = event->window.data2;

            GL_CALL(glViewport(0, 0, nWidth, nHeight))

			PrepareCamera();
		}
        break;
        case SDL_EVENT_KEY_DOWN:
		{
			cameraControl->OnKey(event->key.scancode, true, event->key.repeat);
		}
		break;
		case SDL_EVENT_KEY_UP:
		{
			cameraControl->OnKey(event->key.scancode, false, event->key.repeat);
		}
		break;
		case SDL_EVENT_MOUSE_MOTION:
		{
			cameraControl->OnCursor(event->motion.x, event->motion.y);
		}
		break;
		case SDL_EVENT_MOUSE_WHEEL:
		{
			cameraControl->OnScroll(event->wheel.y);
		}
		break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			cameraControl->OnMouse(event->button.button, true, event->button.x, event->button.y);
		}
		break;
		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			cameraControl->OnMouse(event->button.button, false, event->button.x, event->button.y);
		}
		break;
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
	// 清除颜色缓冲区(被清理为glClearColor设置的颜色)|清理深度缓冲区(被清理为1.0)
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));

	glcontext->BeginShader();

	// glcontext->SetUniformFloat("time", (SDL_GetTicks()/1000.0f));
	// 纹理采样器设置为纹理单元0
	glcontext->SetUniformInt("sampler", 0);
	// 设置变化矩阵
	// DoRotationZTransform(glcontext->m_modelMatrix);
	// DoTranslateAndRotateZTransform(glcontext->m_modelMatrix);
	// DoRotateZAndTranslateTransform(glcontext->m_modelMatrix);
	// DoScaleAndTranslateTransform(glcontext->m_modelMatrix);
	glcontext->SetUniformMatrix4x4("modelMatrix", glcontext->m_modelMatrix);
	glcontext->SetUniformMatrix4x4("viewMatrix", glcontext->m_viewMatrix);
	glcontext->SetUniformMatrix4x4("projectionMatrix", glcontext->m_projectionMatrix);

	// 2.绑定VAO
	GL_CALL(glBindVertexArray(glcontext->m_geometry->GetVao()));
	
	// 3.发出绘制指令
	GL_CALL(glDrawElements(GL_TRIANGLES, glcontext->m_geometry->GetIndicesCount(), GL_UNSIGNED_INT, 0));
	
	// 4.解绑VAO
	glBindVertexArray(0);

	glcontext->EndShader();
	glcontext->SwapWindow();
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	cameraControl->Update();
	glcontext->PrepareViewMatrix(camera->GetViewMatrix());
	glcontext->PrepareProjection(camera->GetProjectionMatrix());
	render();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	camera = nullptr;
	cameraControl = nullptr;
    glcontext = nullptr;
    SDL_DestroyWindow(window);
    SDL_Quit();
}