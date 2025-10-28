
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
#include "inc/PhongMaterial.h"
#include "inc/Mesh.h"
#include "inc/WhiteMaterial.h"
#include "inc/PointLight.h"

auto nWidth = 800;
auto nHeight = 600;
SDL_Window* window = nullptr;
std::shared_ptr<Camera> camera = nullptr;
std::unique_ptr<CameraControl> cameraControl = nullptr;
std::unique_ptr<GLContext> glcontext = nullptr;

void Prepare()
{
	// 渲染器
	glcontext->m_renderer = std::make_unique<Renderer>();

	// 创建几何体Geometry
	// auto geometry01 = Geometry::CreateSphere(1.5f);
	auto geometry01 = Geometry::CreateBox(2.5f);
	// 创建材质Material并且配置材质属性
	auto material01 = new PhongMaterial();
	material01->m_shiness = 16.0f;
	material01->m_diffuse = std::make_unique<Texture>("assets/textures/box.png", 0);
	material01->m_specularMask = std::make_unique<Texture>("assets/textures/sp_mask.png", 1);
	
	auto mesh01 = std::make_shared<Mesh>(geometry01, material01);

	// 创建白色物体，作为点光源
	auto geometryWhite = Geometry::CreateSphere(0.1f);
	auto materialWhite = new WhiteMaterial();
	auto meshWhite = std::make_shared<Mesh>(geometryWhite, materialWhite);
	meshWhite->SetPosition(glm::vec3(2.0, 0.0, 0.0));

	// 生成网格Mesh
	glcontext->m_meshes.emplace_back(mesh01);
	glcontext->m_meshes.emplace_back(meshWhite);

	glcontext->m_dirLight = std::make_shared<DirectionalLight>();
	glcontext->m_dirLight->m_direction = glm::vec3(-1.0f, 0.0f, -1.0f);
	glcontext->m_ambLight = std::make_shared<AmbientLight>();
	glcontext->m_ambLight->m_color = glm::vec3(0.1f);

	glcontext->m_pointLight = std::make_shared<PointLight>();
	glcontext->m_pointLight->SetPosition(meshWhite->GetPosition());
	glcontext->m_pointLight->m_k2 = 0.017f;
	glcontext->m_pointLight->m_k1 = 0.07f;
	glcontext->m_pointLight->m_kc = 1.0f;
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

	// 准备摄像机相关
	PrepareCamera();
	Prepare();

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

void LightTransform() {
	float xPos = glm::sin((SDL_GetTicks() / 1000.0f)) + 3.0f;
	glcontext->m_meshes[1]->SetPosition(glm::vec3(xPos, 0.0f, 0.0f));
	glcontext->m_pointLight->SetPosition(glm::vec3(xPos, 0.0f, 0.0f));
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	cameraControl->Update();
	LightTransform();
	glcontext->m_renderer->Render(glcontext->m_meshes, camera, glcontext->m_pointLight, glcontext->m_ambLight);
	glcontext->SwapWindow();
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