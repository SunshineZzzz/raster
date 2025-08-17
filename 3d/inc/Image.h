#pragma once

#include "stdint.h"
#include "string.h"
#include "Math.h"
#include "FreeImage.h"

namespace szmath
{
    class Image
    {
    protected:
        int32_t _width;
        int32_t _height;
        uint32_t* _pixel;
        int32_t _wrapType;

    public:
        Image(int32_t w, int32_t h, const void* const data) noexcept
        {
            _wrapType = 0;
            if (w == 0 || h == 0 || data == 0)
            {
                _width = 0;
                _height = 0;
                _pixel = 0;
            }
            else
            {
                _width = w;
                _height = h;
                _pixel = new uint32_t[w * h];
                memcpy(_pixel, data, w * h * sizeof(uint32_t));
            }
        }

        ~Image() noexcept
        {
            delete[] _pixel;
        }

        Image(const Image&) = delete;
        Image& operator=(const Image&) = delete;

        int32_t width() const noexcept
        {
            return _width;
        }

        int32_t height() const noexcept
        {
            return _height;
        }

        Rgba pixelAt(int32_t x, int32_t y) const noexcept
        {
            return Rgba(_pixel[y * _width + x]);
        }

        Rgba pixelUV(float u, float v) const noexcept
        {
            float x = u * _width;
            float y = v * _height;
            if (_wrapType == 0)
            {
                return  pixelAt((unsigned)(x) % _width, (unsigned)(y) % _height);
            }
            else
            {
                if (x >= _width)
                {
                    x = _width - 1;
                }
                if (y >= _height)
                {
                    y = _height - 1;
                }
                return  pixelAt(x, y);
            }
        }

        void setWrapType(int32_t type) noexcept
        {
            _wrapType = type;
        }

    public:
        static Image* loadFromFile(const char* fileName) noexcept
        {
            // 1.获取图片格式
            FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
            if (fifmt == FIF_UNKNOWN)
            {
                return nullptr;
            }

            // 2.加载图片
            FIBITMAP* dib = FreeImage_Load(fifmt, fileName, 0);

            // 3.获取数据指针
            FIBITMAP* temp = dib;
            dib = FreeImage_ConvertTo32Bits(dib);
            FreeImage_Unload(temp);

            BYTE* pixels = (BYTE*)FreeImage_GetBits(dib);
            auto width = FreeImage_GetWidth(dib);
            auto height = FreeImage_GetHeight(dib);

            // 4.翻转，目的是保持图像坐标和显示器一样
            int pitch = width * 4;
            BYTE* row = new BYTE[width * 4];
            for (int j = 0; j < height / 2; j++)
            {
                memcpy(row, pixels + j * pitch, pitch);
                memcpy(pixels + j * pitch, pixels + (height - j - 1) * pitch, pitch);
                memcpy(pixels + (height - j - 1) * pitch, row, pitch);
            }
            delete[] row;

            // 5.生成image对象
            Image* image = new Image(width, height, pixels);
            FreeImage_Unload(dib);

            return image;
        }
    };
}