
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
#include "inc/SpotLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_opengl3.h"

#include "inc/AssimpLoader.h"
#include "inc/DepthMaterial.h"
#include "inc/OpacityMaskMaterial.h"

auto nWidth = 900;
auto nHeight = 800;
SDL_Window* window = nullptr;
std::shared_ptr<Camera> camera = nullptr;
std::unique_ptr<CameraControl> cameraControl = nullptr;
std::unique_ptr<GLContext> glcontext = nullptr;
glm::vec3 clearColor{};

void InitImGUI()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	// 设置ImGui与GLFW和OpenGL的绑定
	ImGui_ImplSDL3_InitForOpenGL(window, glcontext->m_glcontext);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void SetModelBlend(Object* obj, bool blend, float opacity) {
	if (obj->getType() == ObjectType::Mesh) 
	{
		Mesh* mesh = (Mesh*)obj;
		Material* mat = mesh->m_material.get();
		mat->m_blend = blend;
		mat->m_opacity = opacity;
		mat->m_depthWrite = false;
	}

	auto children = obj->GetChildren();
	for (int i = 0; i < children.size(); i++) 
	{
		SetModelBlend(children[i], blend, opacity);
	}
}

void Prepare()
{
	// 渲染器
	glcontext->m_renderer = std::make_shared<Renderer>();
	// 场景
	glcontext->m_scene = std::make_shared<Scene>();

	auto grassModel = AssimpLoader::Load("assets/fbx/grass.fbx");
	grassModel->SetScale(glm::vec3(0.02f));
	glcontext->m_scene->AddChild(grassModel);

	auto grassMat = new OpacityMaskMaterial();
	grassMat->m_diffuse = new Texture("assets/textures/grass.jpg", 0);
	grassMat->m_opacityMask = new Texture("assets/textures/grassMask.png", 1);
	grassMat->m_blend = true;
	grassMat->m_depthWrite = false;

	glcontext->m_renderer->m_globalMaterial = grassMat;

	glcontext->m_dirLight = std::make_shared<DirectionalLight>();
	glcontext->m_dirLight->m_direction = glm::vec3(-1.0f);
	glcontext->m_dirLight->m_specularIntensity = 0.1f;

	glcontext->m_ambLight = std::make_shared<AmbientLight>();
	glcontext->m_ambLight->m_color = glm::vec3(0.1f);
}

// 准备摄像机相关	
void PrepareCamera()
{
	// 准备摄像机视图变化矩阵+投影矩阵
	// float size = 6.0f;
	// camera.reset(new OrthographicCamera(-size, size, size, -size, size, -size));
	camera.reset(new PerspectiveCamera(60.f, (float)(nWidth / nHeight), 0.1f, 1000.0f));

	// cameraControl.reset(new TrackBallCameraControl());
	cameraControl.reset(new GameCameraControl);
	cameraControl->SetCamera(camera.get());
	cameraControl->SetSensitivity(0.4f);
	((GameCameraControl*)cameraControl.get())->SetSpeed(0.06f);
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
	
	InitImGUI();

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
	ImGui_ImplSDL3_ProcessEvent(event);
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse || io.WantCaptureKeyboard) 
	{
		return SDL_APP_CONTINUE;
	}

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

void RenderIMGUI() 
{
	// 开启当前的IMGUI渲染
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	// 决定当前的GUI上面有哪些控件，从上到下
	ImGui::Begin("Hello, world!");
	ImGui::Text("ChangeColor Demo");
	ImGui::Button("Test Button", ImVec2(40, 20));
	ImGui::ColorEdit3("Clear Color", (float*)&clearColor);
	ImGui::End();

	// 执行UI渲染
	ImGui::Render();
	// 获取当前窗体的宽高
	int display_w, display_h;
	SDL_GetWindowSize(window, &display_w, &display_h);
	// 重置视口大小
	glViewport(0, 0, display_w, display_h);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	cameraControl->Update();
	glcontext->m_renderer->SetClearColor(clearColor);
	glcontext->m_renderer->Render(glcontext->m_scene.get(), camera,
		glcontext->m_dirLight, glcontext->m_ambLight);
	RenderIMGUI();
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