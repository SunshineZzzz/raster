
#include "../inc/Raster.h"
#include <algorithm>

namespace szmath
{
    Raster::Raster(uint w, uint h, void* buffer) noexcept
    {
        _texture = nullptr;
        _width = w;
        _height = h;
        _buffer = (uint*)buffer;

        memset(&_poitionPointer, 0, sizeof(_poitionPointer));
        memset(&_colorPointer, 0, sizeof(_colorPointer));
        memset(&_uvPointer, 0, sizeof(_uvPointer));

        _defaultColorPointer._size = 4;
        _defaultColorPointer._type = DATETYPE::DT_BYTE;
        _defaultColorPointer._stride = sizeof(Rgba);
        _defaultColorPointer._data = _defaultColorArray;

        _defaultUVPointer._size = 2;
        _defaultUVPointer._type = DATETYPE::DT_FLOAT;
        _defaultUVPointer._stride = sizeof(float2);
        _defaultUVPointer._data = _detaultUVArray;

        _matModel = szmath::matrix4(1);
        _matView = szmath::matrix4(1);
        _matProj = szmath::matrix4(1);
    }

    void Raster::clear(Rgba color) noexcept
    {
        std::fill_n(_buffer, _width * _height, color.toUint());
    }

    void Raster::drawImage(uint startX, uint startY, const Image* image) noexcept
    {
        auto left = tmax<uint>(startX, 0);
        auto top = tmax<uint>(startY, 0);
        auto right = tmin<uint>(startX + image->width(), _width);
        auto bottom = tmin<uint>(startY + image->height(), _height);

        for (auto x = left; x < right; ++x)
        {
            for (auto y = top; y < bottom; ++y)
            {
                Rgba color = image->pixelAt(x - left, y - top);
                setPixelEx(x, y, color);
            }
        }
    }

    void Raster::loadViewMatrix(const matrix4& mat) noexcept
    {
        _matView = mat;
    }

    void Raster::loadViewIdentity() noexcept
    {
        _matView = matrix4(1);
    }

    void Raster::loadProjMatrix(const matrix4& mat) noexcept
    { 
        _matProj = mat;
    }

    void Raster::loadProjIdentity() noexcept
    {
        _matProj = matrix4(1);
    }

    void Raster::loadMatrix(const matrix4& mat) noexcept
    {
        _matModel = mat;
    }

    void Raster::loadIdentity() noexcept
    {
        _matModel = matrix4(1);
    }

    void Raster::setPerspective(float fovy, float aspect, float zNear, float zFar) noexcept
    {
        loadProjMatrix(perspective<float>(fovy, aspect, zNear, zFar));
    }

    void Raster::lookat(float3 const& eye, float3 const& center, float3 const& up) noexcept
    {
        loadViewMatrix(lookAt(eye, center, up));
    }

    void Raster::setViewPort(uint x, uint y, uint w, uint h) noexcept
    {
        (void)x;
        (void)y;
        _viewPort.x = static_cast<float>(w);
        _viewPort.y = static_cast<float>(h);
    }

    void Raster::bindTexture(Image* image) noexcept
    {
        _texture = image;
    }

    void Raster::vertexPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept
    {
        _poitionPointer._size = size;
        _poitionPointer._type = type;
        _poitionPointer._stride = stride;
        _poitionPointer._data = data;
    }

    void Raster::textureCoordPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept
    {
        _uvPointer._size = size;
        _uvPointer._type = type;
        _uvPointer._stride = stride;
        _uvPointer._data = data;
    }

    void Raster::colorPointer(uint size, DATETYPE type, uint stride, const void* data) noexcept
    {
        _colorPointer._size = size;
        _colorPointer._type = type;
        _colorPointer._stride = stride;
        _colorPointer._data = data;
    }

    void Raster::drawArrays(DRAWMODE pri, uint start, uint count) noexcept
    {
        (void)pri;

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

        _matProjView = _matProj * _matView;
        // 传PV，loadFrustum提取的是世界空间的视锥体，必须传世界坐标系的点
        // 传MVP，loadFrustum提取的是模型空间的视锥体，你必须传局部坐标系的点
        _frust.loadFrustum(_matProjView.transpose());

        for (auto i = start; i < start + count; i += 3)
        {
            float* fData = (float*)posData;
            float3 p01(fData[0], fData[1], fData[2]);
            posData += _poitionPointer._stride;
            fData = (float*)(posData);

            float3 p11(fData[0], fData[1], fData[2]);
            posData += _poitionPointer._stride;
            fData = (float*)(posData);

            float3  p21(fData[0], fData[1], fData[2]);
            posData += _poitionPointer._stride;

            p01 = p01 * _matModel;
            p11 = p11 * _matModel;
            p21 = p21 * _matModel;

            // 只要有一个在就行
            if (!(_frust.pointInFrustum(p01)
                || _frust.pointInFrustum(p11)
                || _frust.pointInFrustum(p21)
                ))
            {
                continue;
            }

			bool isRemove1 = false;
            p01 = piplineTransform(p01, isRemove1);
            bool isRemove2 = false;
            p11 = piplineTransform(p11, isRemove2);
            bool isRemove3 = false;
            p21 = piplineTransform(p21, isRemove3);

            if (isRemove1 || isRemove2 || isRemove3) 
            {
				continue;
            }

            // 转化为屏幕坐标
            int2 p0(p01.x, p01.y);
            int2 p1(p11.x, p11.y);
            int2 p2(p21.x, p21.y);

            Rgba c0(*(Rgba*)cData);
            cData += _colorPointer._stride;
            Rgba c1(*(Rgba*)cData);
            cData += _colorPointer._stride;
            Rgba c2(*(Rgba*)cData);
            cData += _colorPointer._stride;

            float* pUV = (float*)uvData;
            float2 uv0(pUV[0], pUV[1]);
            uvData += _uvPointer._stride;

            pUV = (float*)uvData;
            float2 uv1(pUV[0], pUV[1]);
            uvData += _uvPointer._stride;

            pUV = (float*)uvData;
            float2  uv2(pUV[0], pUV[1]);
            uvData += _uvPointer._stride;

            Edge edges[3] =
            {
                Edge(p0.x,p0.y, c0, uv0, p1.x,p1.y, c1, uv1),
                Edge(p1.x,p1.y, c1, uv1, p2.x,p2.y, c2, uv2),
                Edge(p2.x,p2.y, c2, uv2, p0.x,p0.y, c0, uv0),
            };
            drawTrianle(edges);
        }
    }
}