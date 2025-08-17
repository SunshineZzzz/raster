#include <windows.h>
#include <tchar.h>
#include "inc/Raster.h"
#include <filesystem>
#include <memory>
#include "inc/Camera.h"

namespace fs = std::filesystem;

szmath::Camera g_camera;
szmath::int2 g_rButtonDown;
bool g_rButtonFlag = false;
szmath::int2 g_lButtonDown;
bool g_lButtonFlag = false;

szmath::float3 calcIntersectPoint(szmath::Ray& ray)
{
    szmath::float3 pos = ray.getOrigin();
    float tm = abs((pos.y) / ray.getDirection().y);
    szmath::float3 target = ray.getPoint(tm);
    return szmath::float3(target.x, 0, target.z);
}

LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_LBUTTONDOWN:
    {
        g_lButtonDown.x = LOWORD(lParam);
        g_lButtonDown.y = HIWORD(lParam);
        g_lButtonFlag = true;
    }
    break;
    case  WM_LBUTTONUP:
    {
        g_lButtonFlag = false;
    }
    break;
    case WM_MOUSEMOVE:
    {
        auto x = LOWORD(lParam);
        auto y = HIWORD(lParam);
        if (g_rButtonFlag)
        {
            auto offsetX = x - g_rButtonDown.x;
            auto offsetY = y - g_rButtonDown.y;
            g_camera.rotateViewY(offsetX);
            g_camera.rotateViewX(offsetY);
            g_rButtonDown.x = x;
            g_rButtonDown.y = y;
        }
        if (g_lButtonFlag)
        {
            szmath::Ray ray0 = g_camera.createRayFromScreen(x, y);
            szmath::Ray ray1 = g_camera.createRayFromScreen(g_lButtonDown.x, g_lButtonDown.y);

            szmath::float3 pos0 = calcIntersectPoint(ray0);
            szmath::float3 pos1 = calcIntersectPoint(ray1);

            szmath::float3 offset = pos1 - pos0;

            g_lButtonDown = szmath::int2(x, y);

            szmath::float3 newEye = g_camera.getEye() + offset;
            szmath::float3 newTgt = g_camera.getTarget() + offset;

            g_camera.setEye(newEye);
            g_camera.setTarget(newTgt);

            g_camera.update();
        }
    }
    break;
    case WM_RBUTTONDOWN:
    {
        g_rButtonDown.x = LOWORD(lParam);
        g_rButtonDown.y = HIWORD(lParam);
        g_rButtonFlag = true;
    }
    break;
    case WM_RBUTTONUP:
    {
        g_rButtonFlag = false;
    }
    break;
    case WM_MOUSEWHEEL:
    {
        auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (delta > 0)
        {
            auto len = szmath::length(g_camera._eye - g_camera._target);
            len *= 1.2f;
            g_camera._eye = g_camera._target - g_camera._dir * len;
        }
        else
        {
            auto len = szmath::length(g_camera._eye - g_camera._target);
            len *= 0.9f;
            g_camera._eye = g_camera._target - g_camera._dir * len;
        }
        g_camera.update();
    }
    break;
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

fs::path getResourcePath()
{
    char buffer[MAX_PATH]{ "" };
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    fs::path exePath(buffer);

    return exePath.parent_path();
}

struct Vertex
{
    float x, y, z;
    float u, v;
    szmath::Rgba color;
};

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // 注册窗口类
    ::WNDCLASSEXA winClass;
    winClass.lpszClassName = "Raster";
    winClass.cbSize = sizeof(::WNDCLASSEX);
    winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    winClass.lpfnWndProc = windowProc;
    winClass.hInstance = hInstance;
    winClass.hIcon = 0;
    winClass.hIconSm = 0;
    winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winClass.hbrBackground = (HBRUSH)(BLACK_BRUSH);
    winClass.lpszMenuName = NULL;
    winClass.cbClsExtra = 0;
    winClass.cbWndExtra = 0;
    RegisterClassExA(&winClass);

    // 创建窗口
    auto hWnd = CreateWindowEx(
        NULL,
        TEXT("Raster"),
        TEXT("Raster"),
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        800,
        600,
        0,
        0,
        hInstance,
        0
    );
    UpdateWindow(hWnd);
    ShowWindow(hWnd, SW_SHOW);

    // 获取客户区
    RECT rt = { 0 };
    GetClientRect(hWnd, &rt);
    auto width = rt.right - rt.left;
    auto height = rt.bottom - rt.top;
    void* buffer = nullptr;

    auto hDC = GetDC(hWnd);
    auto hMem = ::CreateCompatibleDC(hDC);

    BITMAPINFO bmpInfor;
    bmpInfor.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfor.bmiHeader.biWidth = width;
    // 负值表示原点在左上角，这样子逻辑坐标和设备坐标一致
    bmpInfor.bmiHeader.biHeight = -height;
    bmpInfor.bmiHeader.biPlanes = 1;
    bmpInfor.bmiHeader.biBitCount = 32;
    bmpInfor.bmiHeader.biCompression = BI_RGB;
    bmpInfor.bmiHeader.biSizeImage = 0;
    bmpInfor.bmiHeader.biXPelsPerMeter = 0;
    bmpInfor.bmiHeader.biYPelsPerMeter = 0;
    bmpInfor.bmiHeader.biClrUsed = 0;
    bmpInfor.bmiHeader.biClrImportant = 0;

    auto hBmp = CreateDIBSection(hDC, &bmpInfor, DIB_RGB_COLORS, &buffer, 0, 0);
    if (hBmp == NULL)
    {
        return -1;
    }
    // 双重缓冲
    SelectObject(hMem, hBmp);

    fs::path resourcePath = getResourcePath();
    fs::path imagePath = resourcePath / ".." / ".." / "res" / "1.jpg";
    std::unique_ptr<szmath::Image> imagePtr(szmath::Image::loadFromFile(imagePath.string().c_str()));
    if (!imagePtr)
    {
        MessageBox(nullptr, "Failed to load image", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    szmath::Raster raster(
        static_cast<int32_t>(width),
        static_cast<int32_t>(height),
        buffer);
    
    /*
	* [vertex data] --object coordinate--> [model matrix] --eye coordinate-->
	* [view matrix] --> [projection matrix] --clip coordinate--> [divide by w] --normalized deviced  coordinate-->
	* viewport transform --> [window coordinate]
    */

    g_camera.setViewSize(width, height);
    g_camera.perspective(60, (float)(width) / (float)(height), 0.1, 10000);
    g_camera.update();

    raster.setViewPort(0, 0, width, height);
    /*
    * fovy = 60°，y方向的张角
    * aspect = 1.4
    * zNear = 1
    * zFar = 100
    * 
    * 深度 d=∣z∣
    * y_range=[−d×tan(30), d×tan(30)]
    * x_range=[−d×tan(30)×aspect, d×tan(30)×aspect]
    */
    raster.setPerspective(60, (float)(width) / (float)(height), 0.1, 10000);
    raster.setView(g_camera.getView());

    MSG msg = { 0 };
    while (true)
    {
        if (msg.message == WM_DESTROY
            || msg.message == WM_CLOSE
            || msg.message == WM_QUIT)
        {
            break;
        }
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        raster.clear();

        raster.setView(g_camera.getView());

        Vertex vertexs[] =
        {
            {-1,0,1, 0,0, szmath::Rgba()},
            {1,0,1,  1,0, szmath::Rgba()},
            {1,0,-1, 1,1, szmath::Rgba()},

            {-1,0,1,  0,0, szmath::Rgba()},
            {1,0,-1,  1,1, szmath::Rgba()},
            {-1,0,-1, 0,1, szmath::Rgba()},
        };

        for (int i = 0; i < 6; ++i)
        {
            vertexs[i].x *= 100;
            vertexs[i].z *= 100;

            vertexs[i].u *= 10;
            vertexs[i].v *= 10;
        }

        imagePtr->setWrapType(0);

        raster.bindTexture(imagePtr.get());

        raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
        raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
        raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

        raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);

        BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);
    }

    return  0;
}