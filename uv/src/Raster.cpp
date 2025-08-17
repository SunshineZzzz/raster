
#include "../inc/Raster.h"
#include <algorithm>

namespace szmath
{
    Raster::Raster(uint w, uint h, void* buffer) noexcept
    {
        _texture = 0;
        _width = w;
        _height = h;
        _buffer = reinterpret_cast<uint*>(buffer);

        _defaultColorPointer._size = 4;
        _defaultColorPointer._type = DATETYPE::DT_BYTE;
        _defaultColorPointer._stride = sizeof(Rgba);
        _defaultColorPointer._data = _defaultColorArray;

        _defaultUVPointer._size = 2;
        _defaultUVPointer._type = DATETYPE::DT_FLOAT;
        _defaultUVPointer._stride = sizeof(float2);
        _defaultUVPointer._data = _detaultUVArray;
    }

    void Raster::clear(Rgba color)
    {
		std::fill_n(_buffer, _width * _height, color.toUint());
    }
}