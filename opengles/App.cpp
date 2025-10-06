
#include <windows.h>
#include <tchar.h>
#include "GLESContext.h"

LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{

    ::WNDCLASSEXA winClass;
    winClass.lpszClassName = "opengles";
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

    int nWidth = 480;
    int nHeight = 320;

    auto hWnd = CreateWindowEx(
        NULL,
        TEXT("opengles"),
        TEXT("opengles"),
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        nWidth,
        nHeight,
        0,
        0,
        hInstance,
        0
    );
    UpdateWindow(hWnd);
    ShowWindow(hWnd, SW_SHOW);

    GLESContext context;

    if (!context.setup(hWnd, GetDC(hWnd)))
    {
        return 0;
    }

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
        else 
        {
            // 执行清除操作时，用来填充绘图缓冲区的颜色
            glClearColor(1, 0, 0, 1);
            // 设置视口，左下角坐标为(0,0)，视口宽度为nWidth，高度为nHeight。
            // 方便NDC映射到屏幕坐标系。
            // opengl约定是右手坐标系，但是NDC是左手坐标系，屏幕内是Z轴正方向，越向屏幕内越远离摄像机。
            glViewport(0, 0, nWidth, nHeight);
            // glMatrixMode(GL_PROJECTION);
            // 执行​​清除操作，将整个颜色缓冲区填充为设定的清除颜色
            glClear(GL_COLOR_BUFFER_BIT);
            // 交换缓冲区
            context.swapBuffer();
        }
    }

    context.shutdown();

    return  0;
}