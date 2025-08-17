#pragma once

#include "../inc/Math.h"
#include "../inc/Image.h"

namespace szmath
{
    enum class DRAWMODE
    {
        DM_POINTS = 0,
        DM_LINES = 1,
        DM_LINE_LOOP = 2,
        DM_LINE_STRIP = 3,
        DM_TRIANGES = 4,
    };

    enum class DATETYPE
    {
        DT_NONE,
        DT_BYTE,
        DT_FLOAT,
        DT_DOUBLE,
    };

    struct DateElementDes
    {
        uint _size{0};
        DATETYPE _type{DATETYPE::DT_NONE};
        uint _stride{0};
        const void* _data{nullptr};
    };

    class Span
    {
    public:
        int32_t _xStart;
        int32_t _xEnd;

        Rgba _colorStart;
        Rgba _colorEnd;

        float2 _uvStart;
        float2 _uvEnd;

        int32_t     _y;

    public:
		Span(int32_t xStart, int32_t xEnd, int32_t y, Rgba colorStart, Rgba colorEnd, float2 uvStart, float2 uvEnd) noexcept
        {
            if (xStart < xEnd)
            {
                _xStart = xStart;
                _xEnd = xEnd;

                _colorStart = colorStart;
                _colorEnd = colorEnd;

                _uvStart = uvStart;
                _uvEnd = uvEnd;

                _y = y;
            }
            else
            {
                _xStart = xEnd;
                _xEnd = xStart;

                _colorStart = colorEnd;
                _colorEnd = colorStart;

                _uvStart = uvEnd;
                _uvEnd = uvStart;

                _y = y;
            }
        }
    };

    class Edge
    {
    public:
        int32_t _x1;
        int32_t _y1;
        float2 _uv1;
        Rgba _color1;

        int32_t _x2;
        int32_t _y2;
        float2 _uv2;
        Rgba _color2;

        Edge(int32_t x1, int32_t y1, Rgba color1, float2 uv1, int32_t x2, int32_t y2, Rgba color2, float2 uv2) noexcept
        {
            if (y1 < y2)
            {
                _x1 = x1;
                _y1 = y1;
                _uv1 = uv1;
                _color1 = color1;

                _x2 = x2;
                _y2 = y2;
                _uv2 = uv2;
                _color2 = color2;
            }
            else
            {
                _x1 = x2;
                _y1 = y2;
                _uv1 = uv2;
                _color1 = color2;

                _x2 = x1;
                _y2 = y1;
                _uv2 = uv1;
                _color2 = color1;
            }
        }
    };

    class Raster
    {
    public:
        uint* _buffer;
        uint _width;
        uint _height;
        Rgba _color;

        Image* _texture;
        
        matrix3 _matModel;

        DateElementDes _poitionPointer;
        DateElementDes _colorPointer;
        DateElementDes _uvPointer;

        DateElementDes _defaultColorPointer;
        DateElementDes _defaultUVPointer;
        Rgba _defaultColorArray[3];
        float2 _detaultUVArray[3];

    public:
        Raster(uint w, uint h, void* buffer) noexcept;

        void clear(Rgba color = Rgba{ 0,0,0,0 });

        void drawImage(int32_t startX, int32_t startY, const Image* image) noexcept
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

        // 操作矩阵
        void loadMatrix(const szmath::matrix3& mat) noexcept
        {
            _matModel = mat;
        }

        // 重置操作矩阵
        void loadIdentity() noexcept
        {
            _matModel = szmath::matrix3(1);
        }

        void  vertexPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept
        {
            _poitionPointer._size = size;
            _poitionPointer._type = type;
            _poitionPointer._stride = stride;
            _poitionPointer._data = data;
        }

        void colorPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept
        {
            _colorPointer._size = size;
            _colorPointer._type = type;
            _colorPointer._stride = stride;
            _colorPointer._data = data;
        }

        void textureCoordPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept
        {
            _uvPointer._size = size;
            _uvPointer._type = type;
            _uvPointer._stride = stride;
            _uvPointer._data = data;
        }

        inline void bindTexture(Image* image) noexcept
        {
            _texture = image;
        }

        void drawArrays(DRAWMODE mode, uint start, uint count) noexcept
        {
            if (!_poitionPointer._data)
            {
                return;
            }

            DateElementDes colorPointerdesc = _colorPointer;
            DateElementDes uvPointerdesc = _uvPointer;
            if (!colorPointerdesc._data)
            {
                colorPointerdesc = _defaultColorPointer;
            }
            if (!uvPointerdesc._data)
            {
                uvPointerdesc = _defaultUVPointer;
            }
            char* posData = (char*)_poitionPointer._data;
            char* cData = (char*)colorPointerdesc._data;
            char* uvData = (char*)uvPointerdesc._data;

            for (auto i = start; i < start + count; i += 3)
            {
                float* fData = reinterpret_cast<float*>(posData);
                float3 p0(fData[0], fData[1], 1);
                // p0齐次列向量，_matModel齐次复合变化矩阵，所以是 _matModel * p0
                // 比如: 变化矩阵3*变化矩阵2*变化矩阵1*p0
                // 复合变化矩阵 = 变化矩阵3*(变化矩阵*变化矩阵1)
                // 因为矩阵不满足交换律
                p0 = _matModel * p0;
                posData += _poitionPointer._stride;

                fData = reinterpret_cast<float*>(posData);
                float3 p1(fData[0], fData[1], 1);
                p1 = _matModel * p1;
                posData += _poitionPointer._stride;

                fData = reinterpret_cast<float*>(posData);
                float3 p2(fData[0], fData[1], 1);
                p2 = _matModel * p2;
                posData += _poitionPointer._stride;

                Rgba* pColor = reinterpret_cast<Rgba*>(cData);
                Rgba c0(*pColor);
                cData += _colorPointer._stride;

                pColor = reinterpret_cast<Rgba*>(cData);
                Rgba c1(*pColor);
                cData += _colorPointer._stride;

                pColor = reinterpret_cast<Rgba*>(cData);
                Rgba c2(*pColor);
                cData += _colorPointer._stride;

                float* pUV = reinterpret_cast<float*>(uvData);
                float2 uv0(pUV[0], pUV[1]);
                uvData += _uvPointer._stride;

                pUV = reinterpret_cast<float*>(uvData);
                float2 uv1(pUV[0], pUV[1]);
                uvData += _uvPointer._stride;

                pUV = reinterpret_cast<float*>(uvData);
                float2 uv2(pUV[0], pUV[1]);
                uvData += _uvPointer._stride;

                Edge edges[3] =
                {
                    Edge(p0.x,p0.y,c0,uv0, p1.x,p1.y,c1,uv1),
                    Edge(p1.x,p1.y,c1,uv1, p2.x,p2.y,c2,uv2),
                    Edge(p2.x,p2.y,c2,uv2, p0.x,p0.y,c0,uv0),
                };
                drawTrianle(edges);
            }
        }

        void  drawTrianle(Edge eges[3]) noexcept
        {
            int32_t iMax = 0;
            int32_t length = eges[0]._y2 - eges[0]._y1;

            for (int32_t i = 1; i < 3; ++i)
            {
                int32_t len = eges[i]._y2 - eges[i]._y1;
                if (len > length)
                {
                    length = len;
                    iMax = i;
                }
            }
            int32_t iShort1 = (iMax + 1) % 3;
            int32_t iShort2 = (iMax + 2) % 3;

            drawEdge(eges[iMax], eges[iShort1], _texture);
            drawEdge(eges[iMax], eges[iShort2], _texture);
        }

        void drawEdge(const Edge& e1, const Edge& e2, Image* image) noexcept
        {
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

            float xOffset1 = e1._x2 - e1._x1;
            float scale1 = (float)(e2._y1 - e1._y1) / yOffset1;
            float step1 = 1.0f / yOffset1;

            for (auto y = e2._y1; y < e2._y2; ++y)
            {
                auto x1 = e1._x1 + (int32_t)(scale1 * xOffset1);
                auto x2 = e2._x1 + (int32_t)(scale * xOffset);

                Rgba color2 = colorLerp(e2._color1, e2._color2, scale);
                Rgba color1 = colorLerp(e1._color1, e1._color2, scale1);

                float2 uvStart = uvLerp(e1._uv1, e1._uv2, scale1);
                float2 uvEnd = uvLerp(e2._uv1, e2._uv2, scale);

                Span span(x1, x2, y, color1, color2, uvStart, uvEnd);
                drawSpan(span, image);

                scale += step;
                scale1 += step1;
            }
        }

        void drawSpan(const Span& span, Image* image) noexcept
        {
            auto length = span._xEnd - span._xStart;
            float scale = 0;
            float step = 1.0f / length;

            for (auto x = span._xStart; x < span._xEnd; ++x)
            {
                // Rgba color = colorLerp(span._colorStart, span._colorEnd, scale);
                float2 uv = uvLerp(span._uvStart, span._uvEnd, scale);
                Rgba pixel = image->pixelUV(uv.x, uv.y);
                // Rgba dst = color + pixel;
              
                setPixel(x, span._y, pixel);

                scale += step;
            }
        }

        inline void setPixelEx(uint x, uint y, Rgba color) noexcept
        {
            _buffer[y * _width + x] = color.toUint();
        }

        inline void setPixel(uint x, uint y, Rgba color) noexcept
        {
            if (x >= _width || y >= _height)
            {
                return;
            }
            _buffer[y * _width + x] = color.toUint();
        }
    };
}
