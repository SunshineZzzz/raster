#include "../inc/Math.h"

namespace szmath
{
    Rgba::Rgba(value_type r, value_type g, value_type b, value_type a) noexcept
        : _r(r), _g(g), _b(b), _a(a) {
    }
     
    Rgba::Rgba(uint32_t rgba) noexcept
    {
        _color = rgba;
    }

    Rgba::value_type Rgba::r() const noexcept { return _r; }
    Rgba::value_type Rgba::g() const noexcept { return _g; }
    Rgba::value_type Rgba::b() const noexcept { return _b; }
    Rgba::value_type Rgba::a() const noexcept { return _a; }

    void Rgba::r(value_type val) noexcept { _r = val; }
    void Rgba::g(value_type val) noexcept { _g = val; }
    void Rgba::b(value_type val) noexcept { _b = val; }
    void Rgba::a(value_type val) noexcept { _a = val; }

    bool operator==(const Rgba& lhs, const Rgba& rhs) noexcept {
        return lhs._r == rhs._r && lhs._g == rhs._g &&
            lhs._b == rhs._b && lhs._a == rhs._a;
    }
    bool operator!=(const Rgba& lhs, const Rgba& rhs) noexcept {
        return !(lhs == rhs);
    }

    Rgba::operator uint_type() const noexcept {
        return toUint();
    }

    Rgba::uint_type Rgba::toUint() const noexcept {
        return _color;
    }
}