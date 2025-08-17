#include <windows.h>
#include <tchar.h>
#include "inc/Raster.h"
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

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

fs::path getResourcePath()
{
    char buffer[MAX_PATH]{""};
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    fs::path exePath(buffer);

    return exePath.parent_path();
}

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
    fs::path imagePath = resourcePath / ".." / ".." / "res" / "bg.png";
    std::unique_ptr<szmath::Image> imagePtr(szmath::Image::loadFromFile(imagePath.string().c_str()));
    if (!imagePtr) 
    {
        MessageBox(nullptr, "Failed to load image", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    imagePath = resourcePath / ".." / ".." / "res" / "colorkey.png";
    std::unique_ptr<szmath::Image> colorKeyImagePtr(szmath::Image::loadFromFile(imagePath.string().c_str()));
    if (!colorKeyImagePtr)
    {
        MessageBox(nullptr, "Failed to load image", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    imagePath = resourcePath / ".." / ".." / "res" / "grass.png";
    std::unique_ptr<szmath::Image> alphaImagePtr(szmath::Image::loadFromFile(imagePath.string().c_str()));
    if (!alphaImagePtr)
    {
        MessageBox(nullptr, "Failed to load image", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    imagePath = resourcePath / ".." / ".." / "res" / "scale.jpg";
    std::unique_ptr<szmath::Image> scaleImagePtr(szmath::Image::loadFromFile(imagePath.string().c_str()));
    if (!scaleImagePtr)
    {
        MessageBox(nullptr, "Failed to load image", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    szmath::Raster raster(
        static_cast<int32_t>(width), 
        static_cast<int32_t>(height),
        buffer);

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

        //
        /*{
             
        }*/

        // 三角形绘制填充，图片绘制相关
        /*{
            //szmath::int2 pt[3] =
            //{
            //    szmath::int2(230,-80),
            //    szmath::int2(10,280),
            //    szmath::int2(600,280),
            //};

            szmath::int2 pt[3] =
            {
                szmath::int2(100,10),
                szmath::int2(10,100),
                szmath::int2(200,100),
            };

            //szmath::int2  pt[3] =
            //{
            //    szmath::int2(100,10),
            //    szmath::int2(10,150),
            //    szmath::int2(200,100),
            //};

            szmath::Rgba color1(255, 0, 0);
            szmath::Rgba color2(0, 255, 0);
            szmath::Rgba color3(0, 0, 255);

            raster.drawTriangle(pt[0], pt[1], pt[2], color1, color2, color3);

            raster.drawImage(100, 100, imagePtr.get());
            raster.drawImageWidthColorKey(100, 100, colorKeyImagePtr.get(), szmath::Rgba(255, 0, 0));
            raster.drawImageAlphaTest(100, 310, alphaImagePtr.get(), 100);
            raster.drawImageAlphaBlend(250, 310, alphaImagePtr.get(), 0.3);
            raster.drawImageAlpha(400, 310, alphaImagePtr.get(), 0.5f);
            raster.drawImage(100, 450, scaleImagePtr.get(), 50, 50, 30, 50);
        }*/

        // 图像缩放绘制
        {
            raster.drawImage(0, 0, imagePtr.get());
            raster.drawImageScale(100, 100, 250, 100, scaleImagePtr.get());
        }

        BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);
    }

    return  0;
}