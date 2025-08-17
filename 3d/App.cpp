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
    fs::path imagePath = resourcePath / ".." / ".." / "res" / "bg.png";
    std::unique_ptr<szmath::Image> imagePtr(szmath::Image::loadFromFile(imagePath.string().c_str()));
    if (!imagePtr)
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
    
    /*
	* [vertex data] --object coordinate--> [model matrix] --eye coordinate-->
	* [view matrix] --> [projection matrix] --clip coordinate--> [divide by w] --normalized deviced  coordinate-->
	* viewport transform --> [window coordinate]
    */

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
    raster.setPerspective(60, (float)(width) / (float)(height), 1, 100);

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

        raster.drawImage(0, 0, imagePtr.get());

        /*
        {
            // 3D下绘制三角形
            Vertex vertexs[] =
            {
                {-1.0f,0.0f, -2.0f,0.0f, 0.0f, szmath::Rgba(255,0,0,255)},
                {0.0f,1.0f,  -9.0f,1.0f, 1.0f, szmath::Rgba(0,255,0,255)},
                {1.0f,0.0f,  -2.0f,1.0f, 0.0f, szmath::Rgba(0,0,255,255)},
            };

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);

            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);

            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 3);

            BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);
        }
        */

        /*
        {
            // 观察近大远小
            Vertex  vertexs[] =
            {
                {-1.0f,0.0f,-2.0f, 0.0f,0.0f, szmath::Rgba(255,0,0,255)},
                {0.0f,1.0f,-9.0f,  1.0f,1.0f, szmath::Rgba(0,255,0,255)},
                {1.0f,0.0f,-2.0f,  1.0f,0.0f, szmath::Rgba(0,0,255,255)},
            };

            szmath::matrix4 matTrans;
            static float transZ(-1);
            matTrans.translate(0, 0, transZ);

            raster.loadMatrix(matTrans);

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 3);

            BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);

            transZ -= 1.0f;
        }
        */

        /*
        {
            // 绕轴旋转
            Vertex  vertexs[] =
            {
                {-1.0f,0.0f,-2.0f, 0.0f,0.0f, szmath::Rgba(255,0,0,255)},
                {0.0f,1.0f,-9.0f,  1.0f,1.0f, szmath::Rgba(0,255,0,255)},
                {1.0f,0.0f,-2.0f,  1.0f,0.0f, szmath::Rgba(0,0,255,255)},
            };

            szmath::matrix4 matTrans;
            static float angle(0);
            matTrans.rotateZ(angle);

            raster.loadMatrix(matTrans);

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 3);

            BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);

            angle += 0.5f;
        }
        */

        {
            // 复合变化
            Vertex vertexs[] =
            {
                {-1.0f,0.0f,-2.0f, 0.0f,0.0f, szmath::Rgba(255,0,0,255)},
                {0.0f,1.0f,-9.0f,  1.0f,1.0f, szmath::Rgba(0,255,0,255)},
                {1.0f,0.0f,-2.0f,  1.0f,0.0f, szmath::Rgba(0,0,255,255)},
            };

            szmath::matrix4 matTrans1;
            static float angle(0);
            matTrans1.rotateZ(angle);

            szmath::matrix4 matTrans2;
            matTrans2.scale(10, 3, 3);

            raster.loadMatrix(matTrans1 * matTrans2);

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 3);

            BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);

            angle += 0.5f;
        }
    }

    return  0;
}