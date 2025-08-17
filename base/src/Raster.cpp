#include "../inc/Raster.h"
#include <algorithm>

namespace szmath 
{
    Span::Span(int32_t xStart, int32_t xEnd, int32_t y, Rgba colorStart, Rgba colorEnd) noexcept
    {
        // 从左到右扫描
        if (xStart < xEnd)
        {
            _xStart = xStart;
            _xEnd = xEnd;
            _y = y;
            _colorStart = colorStart;
            _colorEnd = colorEnd;
        }
        else
        {
            _xStart = _xEnd;
            _xEnd = _xStart;
            _y = y;
            _colorStart = colorEnd;
            _colorEnd = colorStart;
        }
    }

    Edge::Edge(int32_t x1, int32_t y1, Rgba color1, int32_t x2, int32_t y2, Rgba color2) noexcept
    {
		// 从上到下扫描
        if (y1 < y2)
        {
            _x1 = x1;
            _y1 = y1;
			_color1 = color1;

            _x2 = x2;
            _y2 = y2;
			_color2 = color2;
        }
        else
        {
            _x1 = x2;
            _y1 = y2;
            _color1 = color2;

            _x2 = x1;
            _y2 = y1;
            _color2 = color1;
        }
    }

    Raster::Raster(int32_t w, int32_t h, void* buffer) noexcept
    {
        _width = w;
        _height = h;
        _buffer = (uint32_t*)buffer;
    }

    void Raster::clear(Rgba color) noexcept
    {
		std::fill_n(_buffer, _width * _height, color.toUint());
    }

    void Raster::drawPoint(int32_t x, int32_t y, Rgba color, int32_t ptSize) noexcept
    {
        switch (ptSize)
        {
        case 1:
            setPixel(x, y, color);
            break;
        case 2:
            setPixel(x + 0, y + 0, color);
            setPixel(x + 1, y + 0, color);
            setPixel(x + 0, y + 1, color);
            setPixel(x + 1, y + 1, color);
            break;
        case 3:
            setPixel(x - 1, y - 1, color);
            setPixel(x + 0, y - 1, color);
            setPixel(x + 1, y - 1, color);

            setPixel(x - 1, y + 0, color);
            setPixel(x + 0, y + 0, color);
            setPixel(x + 1, y + 0, color);

            setPixel(x - 1, y + 1, color);
            setPixel(x + 0, y + 1, color);
            setPixel(x + 1, y + 1, color);
            break;
        }
    }

    void Raster::drawLine(float2 pt1, float2 pt2, Rgba color1, Rgba color2) noexcept
    {
        auto xOffset = pt1.x - pt2.x;
        auto yOffset = pt1.y - pt2.y;

        if (xOffset == 0 && yOffset == 0)
        {
            setPixel(pt1.x, pt1.y, color1);
        }

        if (fabs(xOffset) > fabs(yOffset))
        {
            // 水平扫描
            auto xMin = 0.f;
            auto xMax = 0.f;

            if (pt1.x < pt2.x)
            {
                xMin = pt1.x;
                xMax = pt2.x;
            }
            else
            {
                xMin = pt2.x;
                xMax = pt1.x;
            }

            auto lenth = xMax - xMin;
            auto slope = yOffset / xOffset;
            auto scale = 0.f;
			auto step = 1.0f / lenth;
            for (auto x = xMin; x <= xMax; x += 1.0f)
            {
                // DDA算法
                auto y = pt1.y + (x - pt1.x) * slope;
                auto scaler = (x - xMin) / lenth;
                Rgba color = colorLerp(color1, color2, scaler);
                setPixel(x, y, color);
            }
        }
        else
        {
			// 垂直扫描
            auto yMin = 0.f;
            auto yMax = 0.f;

            if (pt1.y < pt2.y)
            {
                yMin = pt1.y;
                yMax = pt2.y;
            }
            else
            {
                yMin = pt2.y;
                yMax = pt1.y;
            }

            auto lenth = yMax - yMin;
            auto slope = xOffset / yOffset;
            for (auto y = yMin; y <= yMax; y += 1.0f)
            {
                // DDA算法
                auto x = pt1.x + (y - pt1.y) * slope;
                auto scaler = (y - yMin) / lenth;
                Rgba color = colorLerp(color1, color2, scaler);
                setPixel(x, y, color);
            }
        }
    }

    void Raster::drawArrays(DRAWMODE mode, const float2* points, int32_t count, Rgba color) noexcept
    {
        switch (mode)
        {
        case DRAWMODE::DM_POINTS:
        {
            for (auto i = 0; i < count; ++i)
            {
                drawPoint(points[i].x, points[i].y, color, 1);
            }
        }
        break;
        case DRAWMODE::DM_LINES:
        {
            count = count / 2 * 2;
            for (auto i = 0; i < count; i += 2)
            {
                drawLine(points[i], points[i + 1], color, color);
            }
        }
        break;
        case DRAWMODE::DM_LINE_LOOP:
        {
            drawLine(points[0], points[1], color, color);
            for (auto i = 2; i < count; ++i)
            {
                drawLine(points[i - 1], points[i], color, color);
            }
            drawLine(points[0], points[count - 1], color, color);
        }
        break;
        case DRAWMODE::DM_LINE_STRIP:
        {
            drawLine(points[0], points[1], color, color);
            for (auto i = 2; i < count; ++i)
            {
                drawLine(points[i - 1], points[i], color, color);
            }
        }
        break;
        default:
            break;
        }
    }

    void Raster::drawFilleRect(int32_t startX, int32_t startY, int32_t w, int32_t h, Rgba color) noexcept
    {
        auto left = tmax<int32_t>(startX, 0);
        auto top = tmax<int32_t>(startY, 0);
        auto right = tmin<int32_t>(startX + w, _width);
        auto bottom = tmin<int32_t>(startY + h, _height);

        for (auto x = left; x < right; ++x)
        {
            for (auto y = top; y < bottom; ++y)
            {
                setPixelEx(x, y, color);
            }
        }
    }

    void Raster::drawRect(const int2* points, const Rgba* colors) noexcept
    {
        auto left = tmax<int32_t>(points[0].x, 0);
        auto top = tmax<int32_t>(points[0].y, 0);
        auto right = tmin<int32_t>(points[2].x, _width);
        auto bottom = tmin<int32_t>(points[2].y, _height);

        auto w = static_cast<float>(right - left);
        auto h = static_cast<float>(bottom - top);

        for (auto x = left; x < right; ++x)
        {
            auto color1 = colorLerp(colors[0], colors[1], (x - left) / w);
            auto color2 = colorLerp(colors[3], colors[2], (x - left) / w);

            for (auto y = top; y < bottom; ++y)
            {
                auto color = colorLerp(color1, color2, (y - top) / h);
                setPixelEx(x, y, color);
            }
        }
    }

    void Raster::drawTriangle(int2 p0, int2 p1, int2 p2, Rgba c0, Rgba c1, Rgba c2) noexcept
    {
        if (!isInRect(p0) && !isInRect(p1) && !isInRect(p2))
        {
            return;
        }

        Edge edges[3] =
        {
            Edge(p0.x, p0.y, c0, p1.x, p1.y, c1),
            Edge(p1.x, p1.y, c1, p2.x, p2.y, c2),
            Edge(p2.x, p2.y, c2, p0.x, p0.y, c0),
        };

        // 寻找主边，需要水平扫描线以主边分为2个三角形
        auto iMax = 0;
        auto length = edges[0]._y2 - edges[0]._y1;

        for (auto i = 1; i < 3; ++i)
        {
            int len = edges[i]._y2 - edges[i]._y1;
            if (len > length)
            {
                length = len;
                iMax = i;
            }
        }

        auto iShort1 = (iMax + 1) % 3;
        auto iShort2 = (iMax + 2) % 3;

        // 画上半部分 or 下半部分
        drawEdge(edges[iMax], edges[iShort1]);
        // 画上半部分 or 下半部分
        drawEdge(edges[iMax], edges[iShort2]);
    }

    void Raster::drawImage(int32_t startX, int32_t startY, const Image* image) noexcept
    {
        auto left = tmax<int32_t>(startX, 0);
        auto top = tmax<int32_t>(startY, 0);
        auto right = tmin<int32_t>(startX + image->width(), _width);
        auto bottom = tmin<int32_t>(startY + image->height(), _height);

        for (auto x = left; x < right; ++x)
        {
            for (auto y = top; y < bottom; ++y)
            {
                Rgba color = image->pixelAt(x - left, y - top);
                setPixelEx(x, y, color);
            }
        }
    }

    void Raster::drawImage(int32_t startX, int32_t startY, const Image* image, int32_t x1, int32_t y1, int32_t w, int32_t h) noexcept
    {
        auto left = tmax<int32_t>(startX, 0);
        auto top = tmax<int32_t>(startY, 0);
        auto right = tmin<int32_t>(startX + w, _width);
        auto bottom = tmin<int32_t>(startY + h, _height);

        for (auto x = left; x < right; ++x)
        {
            for (auto y = top; y < bottom; ++y)
            {
                Rgba srcColor = image->pixelAt(x - left + x1, y - top + y1);
                setPixelEx(x, y, srcColor);
            }
        }
    }

    void Raster::drawImageWidthColorKey(int32_t startX, int32_t startY, const Image* image, Rgba key) noexcept
    {
        auto left = tmax<int32_t>(startX, 0);
        auto top = tmax<int32_t>(startY, 0);
        auto right = tmin<int32_t>(startX + image->width(), _width);
        auto bottom = tmin<int32_t>(startY + image->height(), _height);

        for (auto x = left; x < right; ++x)
        {
            for (auto y = top; y < bottom; ++y)
            {
                Rgba color = image->pixelAt(x - left, y - top);
                if (color != key)
                {
                    setPixelEx(x, y, color);
                }
            }
        }
    }
    
    void Raster::drawImageAlphaTest(int32_t startX, int32_t startY, const Image* image, byte alpha) noexcept
    {
        auto left = tmax<int32_t>(startX, 0);
        auto top = tmax<int32_t>(startY, 0);
        auto right = tmin<int32_t>(startX + image->width(), _width);
        auto bottom = tmin<int32_t>(startY + image->height(), _height);

        for (auto x = left; x < right; ++x)
        {
            for (auto y = top; y < bottom; ++y)
            {
                Rgba color = image->pixelAt(x - left, y - top);
                if (color.a() > alpha)
                {
                    setPixelEx(x, y, color);
                }
            }
        }
    }

    void Raster::drawImageAlphaBlend(int32_t startX, int32_t startY, const Image* image, float alphaScale) noexcept
    {
        if (alphaScale < 0.0f)
        {
            alphaScale = 0.0f;
        }
        else if (alphaScale > 1.0f)
        {
            alphaScale = 1.0f;
        }

        auto left = tmax<int32_t>(startX, 0);
        auto top = tmax<int32_t>(startY, 0);
        auto right = tmin<int32_t>(startX + image->width(), _width);
        auto bottom = tmin<int32_t>(startY + image->height(), _height);

        for (auto x = left; x < right; ++x)
        {
            for (auto y = top; y < bottom; ++y)
            {
                Rgba srcColor = image->pixelAt(x - left, y - top);
                Rgba dstColor = getPixelEx(x, y);
				// 原像素颜色不透明就用原像素颜色
				// 原像素颜色透明就用目标像素颜色
                // 两者之间作线性插值
                Rgba color = colorLerp(dstColor, srcColor, srcColor.a() / 255.0f * alphaScale);
                setPixelEx(x, y, color);
            }
        }
    }

    void Raster::drawImageAlpha(int32_t startX, int32_t startY, const Image* image, float alphaScale) noexcept
    {
        if (alphaScale < 0.0f) 
        {
            alphaScale = 0.0f;
        }
        else if (alphaScale > 1.0f) 
        {
            alphaScale = 1.0f;
        }

        auto left = tmax<int32_t>(startX, 0);
        auto top = tmax<int32_t>(startY, 0);
        auto right = tmin<int32_t>(startX + image->width(), _width);
        auto bottom = tmin<int32_t>(startY + image->height(), _height);

        for (auto x = left; x < right; ++x)
        {
            for (auto y = top; y < bottom; ++y)
            {
                Rgba srcColor = image->pixelAt(x - left, y - top);
                Rgba dstColor = getPixelEx(x, y);
                // alphaScale决定偏向谁
                Rgba color = colorLerp(dstColor, srcColor, alphaScale);
                setPixelEx(x, y, color);
            }
        }
    }

    void Raster::drawImageScale(int32_t dstX, int32_t dstY, int32_t dstW, int32_t dstH, const Image * image) noexcept
    {
        auto xScale = image->width() / static_cast<float>(dstW);
        auto yScale = image->height() / static_cast<float>(dstH);

        for (auto x = dstX; x < static_cast<int32_t>(dstX + dstW); ++x)
        {
            for (auto y = dstY; y < static_cast<int32_t>(dstY + dstH); ++y)
            {
                auto xx = (x - dstX) * xScale;
                auto yy = (y - dstY) * yScale;

                Rgba srcColor = image->pixelAt(xx, yy);
                setPixelEx(x, y, srcColor);
            }
        }
    }

    void Raster::drawEdge(const Edge & e1, const Edge & e2) noexcept
    {
        // e1是主边，e2是其他边
        auto yOffset1 = e1._y2 - e1._y1;
        if (yOffset1 == 0)
        {
            return;
        }

        auto yOffset = e2._y2 - e2._y1;
        if (yOffset == 0)
        {
            return;
        }

        float xOffset = e2._x2 - e2._x1;
        float scale = 0;
        float step = 1.0f / yOffset;
        auto startY = tmax<int32_t>(e2._y1, 0);
        auto endY = tmin<int32_t>(e2._y2, _height);
        scale += (startY - e2._y1) / yOffset;

        float xOffset1 = e1._x2 - e1._x1;
        float scale1 = (float)(e2._y1 - e1._y1) / yOffset1;
        float step1 = 1.0f / yOffset1;
        auto startY1 = tmax<int32_t>(e1._y1, 0);
        auto endY1 = tmin<int32_t>(e1._y2, _height);
        scale1 += (startY1 - e1._y1) / yOffset1;

        for (int y = startY; y < endY; ++y)
        {
            // DDA
            int x1 = e1._x1 + (int)(scale1 * xOffset1);
            int x2 = e2._x1 + (int)(scale * xOffset);
            Rgba color2 = colorLerp(e2._color1, e2._color2, scale);
            Rgba color1 = colorLerp(e1._color1, e1._color2, scale1);

            Span span(x1, x2, y, color1, color2);
            drawSpan(span);

            scale += step;
            scale1 += step1;
        }
    }

    void Raster::drawSpan(const Span& span, Rgba color) noexcept
    {
        float length = span._xEnd - span._xStart;
        float scale = 0;
        float step = 1.0f / length;
        auto startX = tmax<int32_t>(span._xStart, 0);
        auto endX = tmin<int32_t>(span._xEnd, _width);
        scale += (startX - span._xStart) / length;
        for (int x = startX; x < endX; ++x)
        {
            Rgba color = colorLerp(
                span._colorStart
                , span._colorEnd
                , scale
            );
            scale += step;

            setPixelEx(x, span._y, color);
        }
    }
}