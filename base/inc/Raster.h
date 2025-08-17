#pragma once

#include "Math.h"
#include "Image.h"

namespace szmath 
{
    enum class DRAWMODE
    {
        // 点绘制
        DM_POINTS = 0,
        // 线段
        DM_LINES = 1,
        // 依次连接成线段，并自动闭合
        DM_LINE_LOOP = 2,
        // 依次连接成连续线段，但不闭合
        DM_LINE_STRIP = 3,
    };

    class Span
    {
    public:
        int32_t _xStart;
        int32_t _xEnd;
        int32_t _y;
        Rgba _colorStart;
        Rgba _colorEnd;

        Span(int32_t xStart, int32_t xEnd, int32_t y, Rgba colorStart, Rgba colorEnd) noexcept;
    };

    class Edge
    {
    public:
        int32_t _x1;
        int32_t _y1;
		Rgba _color1;

        int32_t _x2;
        int32_t _y2;
		Rgba _color2;

        Edge(int32_t x1, int32_t y1, Rgba color1, int32_t x2, int32_t y2, Rgba color2) noexcept;
    };

    class Raster
    {
    public:
        uint32_t* _buffer;
        int32_t _width;
        int32_t _height;

    public:
        Raster(int32_t w, int32_t h, void* buffer) noexcept;

        void clear(Rgba color = Rgba{0,0,0,0}) noexcept;
        void drawPoint(int32_t x, int32_t y, Rgba color, int32_t ptSize) noexcept;
        void drawLine(float2 pt1, float2 pt2, Rgba color1, Rgba color2) noexcept;
        void drawArrays(DRAWMODE mode, const float2* points, int32_t count, Rgba color = Rgba{}) noexcept;
        void drawFilleRect(int32_t startX, int32_t startY, int32_t w, int32_t h, Rgba color = Rgba{}) noexcept;
        void drawRect(const int2* points, const Rgba* colors) noexcept;
        void drawTriangle(int2 p0, int2 p1, int2 p2, Rgba c0, Rgba c1, Rgba c2) noexcept;
        // 目标(image)图像绘制
        void drawImage(int32_t startX, int32_t startY, const Image* image) noexcept;
		// 目标(image)图像绘制，startX,startY屏幕坐标，x1,y1图片左上角坐标，w,h图片宽高
        void drawImage(int32_t startX, int32_t startY, const Image* image, int32_t x1, int32_t y1, int32_t w, int32_t h) noexcept;
		// 目标(image)图像和指定颜色过滤绘制图像
        void drawImageWidthColorKey(int32_t startX, int32_t startY, const Image* image, Rgba key) noexcept;
		// 目标(image)图像和指定alpha进行判断绘制图像，大于判断
        void drawImageAlphaTest(int32_t startX, int32_t startY, const Image* image, byte alpha) noexcept;
		// 背景图像和目标(image)图像进行alpha混合绘制图像
        void drawImageAlphaBlend(int32_t startX, int32_t startY, const Image* image, float alphaScale) noexcept;
		// 背景图像和目标(image)图像进行指定alphaScale值混合绘制图像
        void drawImageAlpha(int32_t startX, int32_t startY, const Image* image, float alphaScale) noexcept;
		// 目标(image)图像绘制到dstX, dstY位置，缩放或者扩大到dstW, dstH大小，最近点采样
        // TODO高质量采样
        void drawImageScale(int32_t dstX, int32_t dstY, int32_t dstW, int32_t dstH, const Image* image) noexcept;

    private:
        void drawEdge(const Edge& e1, const Edge& e2) noexcept;
        void drawSpan(const Span& span, Rgba color = Rgba{}) noexcept;
        
        inline bool isInRect(int2 pt) noexcept
        {
            if (pt.x >= 0 && pt.x <= _width && pt.y >= 0 && pt.y <= _height)
            {
                return  true;
            }
            return  false;
        }

        inline Rgba getPixelEx(int32_t x, int32_t y) noexcept
        {
            return Rgba(_buffer[y * _width + x]);
        }

        inline void setPixelEx(int32_t x, int32_t y, Rgba color)
        {
            _buffer[y * _width + x] = color.toUint();
        }

        inline void setPixel(int32_t x, int32_t y, Rgba color)
        {
			if (x < 0 || y < 0)
			{
				return;
			}

            if (x >= _width || y >= _height)
            {
                return;
            }

            _buffer[y * _width + x] = color.toUint();
        }
    };
}