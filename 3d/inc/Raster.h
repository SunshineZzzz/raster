#pragma once

#include "Math.h"
#include "Image.h"

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
        DT_BYTE,
        DT_FLOAT,
        DT_DOUBLE,
    };

    struct DateElementDes
    {
        uint _size;
        DATETYPE _type;
        uint _stride;
        const void* _data;
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

        int32_t _y;

    public:
        Span(int32_t xStart, int32_t xEnd, int32_t y, Rgba colorStart, Rgba colorEnd, float2 uvStart, float2 uvEnd)
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

        Edge(int32_t x1, int32_t y1, Rgba color1, float2 uv1, int32_t x2, int32_t y2, Rgba color2, float2 uv2)
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

    struct Vertex
    {
        int2    p0;
        float2  uv0;
        Rgba    c0;

        int2    p1;
        float2  uv1;
        Rgba    c1;

        int2    p2;
        float2  uv2;
        Rgba    c2;

    };

    class Raster
    {
    public:
        uint* _buffer;
        uint _width;
        uint _height;

		// 纹理
        Image* _texture;
		// 模型矩阵
        matrix4 _matModel;
		// 观察矩阵
        matrix4 _matView;
        // 透视投影矩阵
        matrix4 _matProj;

        matrix4 _matProjView;
        float2 _viewPort;
        Frustum _frust;

        DateElementDes _poitionPointer;
        DateElementDes _colorPointer;
        DateElementDes _uvPointer;

        DateElementDes _defaultColorPointer;
        DateElementDes _defaultUVPointer;
        Rgba _defaultColorArray[3];
        float2 _detaultUVArray[3];

    public:
        Raster(uint w, uint h, void* buffer) noexcept;
        void clear(Rgba color = Rgba{ 0,0,0,0 }) noexcept;
        void drawImage(uint startX, uint startY, const Image* image) noexcept;
        void loadViewMatrix(const matrix4& mat) noexcept;
        void loadViewIdentity() noexcept;
        void loadProjMatrix(const matrix4& mat) noexcept;
        void loadProjIdentity() noexcept;
        void loadMatrix(const matrix4& mat) noexcept;
        void loadIdentity() noexcept;
		// 生成投影矩阵，近平面相对Y轴左右对称，近平面相对X轴上下对称，摄像机朝向-Z轴
        // fovy:y方向张角
		// aspect:近平面宽高比
		// zNear:近平面距离
		// zFar:远平面距离
        void setPerspective(float fovy, float aspect, float zNear, float zFar) noexcept;
        // 生成观察矩阵
        void lookat(float3 const& eye, float3 const& center, float3 const& up) noexcept;
		// 设置视口
        void setViewPort(uint x, uint y, uint w, uint h) noexcept;
        void bindTexture(Image* image) noexcept;
        void vertexPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept;
        void textureCoordPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept;
        void colorPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept;
        void drawArrays(DRAWMODE pri, uint start, uint count) noexcept;

    protected:
        float3 piplineTransform(float3 pos, bool& isRemove) noexcept
        {
            isRemove = false;

            float4 world(pos.x, pos.y, pos.z, 1);
            float4 screen = _matProjView * world;

            if (screen.w == 0.0f)
            {
				isRemove = true;
                return float3(0.f,0.f,0.f);
            }

			// 剪裁空间坐标系 -> NDC坐标系
            screen.x /= screen.w;
            screen.y /= screen.w;
            screen.z /= screen.w;

			// [-1,1] -> [0,1]
            screen.x = screen.x * 0.5f + 0.5f;
            screen.y = screen.y * 0.5f + 0.5f;
            screen.z = screen.z * 0.5f + 0.5f;

            // [0,1] -> viewport
            screen.x = screen.x * _viewPort.x;
            screen.y = _height - screen.y * _viewPort.y;

            return float3(screen.x, screen.y, screen.z);
        }

        void drawTrianle(Edge edges[3]) noexcept
        {
            int32_t iMax = 0;
            int32_t length = edges[0]._y2 - edges[0]._y1;

            for (int32_t i = 1; i < 3; ++i)
            {
                int32_t len = edges[i]._y2 - edges[i]._y1;
                if (len > length)
                {
                    length = len;
                    iMax = i;
                }
            }
            int32_t iShort1 = (iMax + 1) % 3;
            int32_t iShort2 = (iMax + 2) % 3;

            drawEdge(edges[iMax], edges[iShort1], _texture);
            drawEdge(edges[iMax], edges[iShort2], _texture);
        }

        void drawEdge(const Edge& e1, const Edge& e2, Image* image) noexcept
        {
            float yOffset1 = e1._y2 - e1._y1;
            if (yOffset1 == 0)
            {
                return;
            }

            float yOffset = e2._y2 - e2._y1;
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

            for (int32_t y = e2._y1; y < e2._y2; ++y)
            {
                int32_t x1 = e1._x1 + (int)(scale1 * xOffset1);
                int32_t x2 = e2._x1 + (int)(scale * xOffset);
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

        void drawTriangle(const Vertex& vertex, Image* image) noexcept
        {
            Edge eges[3] =
            {
                Edge(vertex.p0.x,vertex.p0.y,vertex.c0,  vertex.uv0, vertex.p1.x,vertex.p1.y,vertex.c1,  vertex.uv1),
                Edge(vertex.p1.x,vertex.p1.y,vertex.c1,  vertex.uv1, vertex.p2.x,vertex.p2.y,vertex.c2,  vertex.uv2),
                Edge(vertex.p2.x,vertex.p2.y,vertex.c2,  vertex.uv2, vertex.p0.x,vertex.p0.y,vertex.c0,  vertex.uv0),
            };

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

            drawEdge(eges[iMax], eges[iShort1], image);
            drawEdge(eges[iMax], eges[iShort2], image);
        }

        void drawSpan(const Span& span, Image* image) noexcept
        {
            auto length = span._xEnd - span._xStart;
            auto scale = 0.f;
            auto step = 1.0f / length;
            for (auto x = span._xStart; x < span._xEnd; ++x)
            {
                Rgba color = colorLerp(span._colorStart, span._colorEnd, scale);
                float2 uv = uvLerp(span._uvStart, span._uvEnd, scale);
                Rgba pixel = image->pixelUV(uv.x, uv.y);

                // Rgba  dst = color + pixel;
                scale += step;
                setPixel(x, span._y, color);
            }
        }
  
        inline void setPixelEx(uint32_t x, uint32_t y, Rgba color) noexcept
        {
            _buffer[y * _width + x] = color.toUint();
        }

        inline  Rgba getPixel(uint32_t x, uint32_t y)
        {
            return  Rgba(_buffer[y * _width + x]);
        }

        inline void setPixel(uint32_t x, uint32_t y, Rgba color)
        {
            if (x >= _width || y >= _height)
            {
                return;
            }
            _buffer[y * _width + x] = color.toUint();
        }
    };
}
