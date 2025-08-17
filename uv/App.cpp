#include <windows.h>
#include <tchar.h>
#include "inc/Raster.h"
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

struct Vertex
{
    float x{0.f}, y{0.f};
    float u{0.f}, v{0.f};
    szmath::Rgba color{255, 255, 255, 255};

    Vertex(float x, float y, float u, float v, szmath::Rgba color)
		: x(x), y(y), u(u), v(v), color(color) {
	}
};

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

    szmath::Raster raster(width, height, buffer);

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
            // 利用纹理包装跑马灯
            static float step = 0;

            Vertex vertexs[] =
            {
                {10,10,   0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,10,  2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {10,10,   0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {10,210,  0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            for (int i = 0; i < 6; ++i)
            {
                vertexs[i].v += step;
            }

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);

            step += 0.01f;
        }
        */

        /*
        {
            // 纹理超过就用最后一个
            Vertex vertexs[] =
            {
                {10,10,   0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,10,  2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {10,10,   0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {10,210,  0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);
        }
        */

        /*
        {
            // 利用齐次坐标做线性变化和平移
            Vertex vertexs[] =
            {
                {10,10,   0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,10,  2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {10,10,   0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {10,210,  0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            static float trans = 0;
            // 没有线性变化，只有平移
            szmath::matrix3 mat;
            mat.translate(trans, trans);
            
            for (auto i = 0; i < 6; ++i) 
            {
                // 生成齐次项次向量
                szmath::float3 pos(vertexs[i].x, vertexs[i].y, 1);
                // 平移后
                pos = mat * pos;
                // 重新赋值
                vertexs[i].x = pos.x;
                vertexs[i].y = pos.y;
            }

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);

            trans += 1;
        }
        */

        /*{
            // 利用齐次坐标做缩放和平移
            Vertex vertexs[] =
            {
                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,-10, 2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {-10,210, 0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            static float scales = -2;
            szmath::matrix3 mat;
            // y轴翻转了，并且放大
            mat.scale(1, scales);

            for (int i = 0; i < 6; ++i)
            {
                szmath::float3 pos(vertexs[i].x, vertexs[i].y, 1);
                pos = mat * pos;
                vertexs[i].x = pos.x;
                vertexs[i].y = pos.y;
            }

            // 平移
            szmath::matrix3 matT;
            matT.translate(200, 200);
            for (int i = 0; i < 6; ++i)
            {
                szmath::float3  pos(vertexs[i].x, vertexs[i].y, 1);

                pos = matT * pos;
                vertexs[i].x = pos.x;
                vertexs[i].y = pos.y;
            }

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);
        }*/

        /*
        {
            // 利用齐次坐标沿着左上角也就是坐标原点做旋转
            Vertex vertexs[] =
            {
                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,-10, 2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {-10,210, 0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            static float angles = 0;
            szmath::matrix3 rot;
            rot.rotate(angles);

            for (int i = 0; i < 6; ++i)
            {
                szmath::float3 pos(vertexs[i].x, vertexs[i].y, 1);
                pos = rot * pos;
                vertexs[i].x = pos.x;
                vertexs[i].y = pos.y;
            }

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);

            angles += 0.1f;
        }
        */

        /*
        {
            // 利用齐次坐标将矩形的中心点移动到屏幕坐标中心，再做旋转，这个时候就是按照自身中心旋转了
            Vertex vertexs[] =
            {
                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,-10, 2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {-10,210, 0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            // 把正方形的原点于坐标系原点重合，在旋转的话就是按照自身中心旋转
            for (int i = 0; i < 6; ++i)
            {
                vertexs[i].x -= 110;
                vertexs[i].y -= 110;
            }

            static float angles = 0;
            szmath::matrix3 rot;
            rot.rotate(angles);

            for (int i = 0; i < 6; ++i)
            {
                szmath::float3 pos(vertexs[i].x, vertexs[i].y, 1);
                pos = rot * pos;
                vertexs[i].x = pos.x;
                vertexs[i].y = pos.y;
            }

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);

            angles += 0.1f;
        }
        */

        /*
        {
            // 利用齐次坐标将矩形的中心点移动到屏幕坐标中心，再做旋转，再还原坐标，这个时候图像完整的按照自身旋转了
            Vertex vertexs[] =
            {
                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,-10, 2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {-10,210, 0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            // 把正方形的原点于坐标系原点重合，在旋转的话就是按照自身中心旋转
            for (int i = 0; i < 6; ++i)
            {
                vertexs[i].x -= 110;
                vertexs[i].y -= 110;
            }

            static float angles = 0;
            szmath::matrix3 rot;
            rot.rotate(angles);

            for (int i = 0; i < 6; ++i)
            {
                szmath::float3 pos(vertexs[i].x, vertexs[i].y, 1);
                pos = rot * pos;
                vertexs[i].x = pos.x;
                vertexs[i].y = pos.y;
            }

            // 再把坐标还原回去
            for (int i = 0; i < 6; ++i)
            {
                vertexs[i].x += 110;
                vertexs[i].y += 110;
            }

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);

            angles += 0.1f;
        }
        */

        /*
        {
            // 利用齐次坐标将矩形的中心点移动到屏幕坐标中心，再做旋转，再还原坐标，这个时候图像完整的按照自身旋转了
            // 上面描述，矩阵的复合操作，可以(平移矩阵*旋转矩阵)*平移矩阵相乘，最后再做用到矩形的每个坐标上，完成了上述变化
            Vertex vertexs[] =
            {
                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,-10, 2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {-10,210, 0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            szmath::matrix3 matTrans;
            matTrans.translate(-110, -110);

            static float angles = 0;
            szmath::matrix3 rot;
            rot.rotate(angles);

            szmath::matrix3 matTrans1;
            matTrans1.translate(110, 110);

            // 这里需要注意矩阵是不满足交换律A.B != B.A
            // 我们这里是 变化矩阵3*变化矩阵2*变化矩阵1*列向量，所以要下面这种写法：
            szmath::matrix3 all = matTrans1 * (rot * matTrans);

            for (int i = 0; i < 6; ++i)
            {
                szmath::float3 pos(vertexs[i].x, vertexs[i].y, 1);
                pos = all * pos;
                vertexs[i].x = pos.x;
                vertexs[i].y = pos.y;
            }

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);

            angles += 0.1f;
        }
        */

        {
            // 封装接口来完成上面的操作
            Vertex vertexs[] =
            {
                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {210,-10, 2.0f,0.0f, szmath::Rgba(255,255,255,255)},

                {-10,-10, 0.0f,0.0f, szmath::Rgba(255,255,255,255)},
                {210,210, 2.0f,2.0f, szmath::Rgba(255,255,255,255)},
                {-10,210, 0.0f,2.0f, szmath::Rgba(255,255,255,255)},
            };

            szmath::matrix3 matTrans;
            matTrans.translate(-110, -110);

            static float angles = 0;
            szmath::matrix3 rot;
            rot.rotate(angles);

            szmath::matrix3 matTrans1;
            matTrans1.translate(110, 110);

            // 这里需要注意矩阵是不满足交换律A.B != B.A
            // 我们这里是 变化矩阵3*变化矩阵2*变化矩阵1*列向量，所以要下面这种写法：
            szmath::matrix3 all = matTrans1 * (rot * matTrans);

            raster.loadIdentity();
            raster.loadMatrix(all);

            scaleImagePtr->setWrapType(1);

            raster.bindTexture(scaleImagePtr.get());

            raster.vertexPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].x);
            raster.textureCoordPointer(2, szmath::DATETYPE::DT_FLOAT, sizeof(Vertex), &vertexs[0].u);
            raster.colorPointer(4, szmath::DATETYPE::DT_BYTE, sizeof(Vertex), &vertexs[0].color);

            raster.drawArrays(szmath::DRAWMODE::DM_TRIANGES, 0, 6);

            angles += 0.1f;
        }

        BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);
    }

    return  0;
}