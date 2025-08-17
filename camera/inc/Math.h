#pragma once

#include <float.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

namespace szmath
{
    constexpr double PI = 3.14159265358979323846;
    constexpr double TWO_PI = 6.28318530717958647692;
    constexpr double HALF_PI = 1.57079632679489661923;
    constexpr decltype(auto) DEG2RAD(double theta) {
        return 0.01745329251994329 * theta;
    }
    constexpr auto RAD2DEG = 57.2957795130823208;

    constexpr int32_t make_int(int16_t a, int16_t b) noexcept {
        return static_cast<int32_t>(a) | (static_cast<int32_t>(b) << 16);
    }
    template <typename T>
    constexpr T tmin(const T& a, const T& b) {
        return (a < b) ? a : b;
    }
    template <typename T>
    constexpr T tmax(const T& a, const T& b) {
        return (a > b) ? a : b;
    }
    template <typename T>
    struct tvec2;
    using int2 = tvec2<int32_t>;
    using float2 = tvec2<float>;
    template <typename T>
    struct tvec3;
    using float3 = tvec3<float>;
    template <typename T>
    struct tvec4;
    using float4 = tvec4<float>;
    using byte = unsigned char;
    using uint = uint32_t;
    template <typename T>
    struct tmat3x3;
    using matrix3 = tmat3x3<float>;
    template <typename T>
    struct tmat4x4;
    using matrix4 = tmat4x4<float>;
	template <typename T>
    struct tfrustum;
    using Frustum = tfrustum<float>;
    template <typename T>
    struct tray;
    using Ray = tray<float>;

    class Rgba
    {
    public:
        using value_type = uint8_t;
        using uint_type = uint32_t;

        Rgba(value_type r = 255, value_type g = 255,
            value_type b = 255, value_type a = 255) noexcept;
        Rgba(uint32_t rgba) noexcept;

        value_type r() const noexcept;
        value_type g() const noexcept;
        value_type b() const noexcept;
        value_type a() const noexcept;

        void r(value_type val) noexcept;
        void g(value_type val) noexcept;
        void b(value_type val) noexcept;
        void a(value_type val) noexcept;

        friend bool operator==(const Rgba& lhs, const Rgba& rhs) noexcept;
        friend bool operator!=(const Rgba& lhs, const Rgba& rhs) noexcept;
        friend Rgba operator+(const Rgba& left, const Rgba& right) noexcept;

        explicit operator uint_type() const noexcept;

        uint_type toUint() const noexcept;

    private:
        union {
            struct {
                value_type _b;
                value_type _g;
                value_type _r;
                value_type _a;
            };
            uint32_t _color;
        };
    };

    inline Rgba colorLerp(const Rgba& c1, const Rgba& c2, float s) noexcept
    {
        Rgba color;

        color.r(static_cast<Rgba::value_type>((c1.r() + s * (c2.r() - c1.r()))));
        color.g(static_cast<Rgba::value_type>((c1.g() + s * (c2.g() - c1.g()))));
        color.b(static_cast<Rgba::value_type>((c1.b() + s * (c2.b() - c1.b()))));
        color.a(static_cast<Rgba::value_type>((c1.a() + s * (c2.a() - c1.a()))));
        return color;
    }

    template <typename T>
    struct tvec2
    {
        using value_type = T;
        using size_type = size_t;
        using type = tvec2<T>;

        value_type x = value_type{ 0 };
        value_type y = value_type{ 0 };

        tvec2() = default;
        explicit tvec2(value_type s) noexcept
            : x(s), y(s) {
        }
        tvec2(value_type x, value_type y) noexcept
            : x(x), y(y) {
        }
        tvec2(const tvec2& v) noexcept
            : x(v.x), y(v.y) {
        }
        template <typename U>
        tvec2(U const& x) noexcept
            : x(value_type(x)), y(value_type(x)) {
        }
        template <typename U, typename V>
        tvec2(U const& a, V b) noexcept
            : x(value_type(a)), y(value_type(b)) {
        }
        template <typename U>
        tvec2(tvec2<U> const& v) noexcept
            : x(value_type(v.x)), y(value_type(v.y)) {
        }

        tvec2<T>& operator=(tvec2<T> const& v) noexcept
        {
            if (*this == v) {
                return *this;
            }

            this->x = v.x;
            this->y = v.y;
            return *this;
        }
        template <typename U>
        tvec2<T>& operator=(tvec2<U> const& v) noexcept
        {
            this->x = T(v.x);
            this->y = T(v.y);
            return *this;
        }

        template <typename U>
        tvec2<T>& operator+=(U const& s) noexcept
        {
            this->x += T(s);
            this->y += T(s);
            return *this;
        }
        template <typename U>
        tvec2<T>& operator+=(tvec2<U> const& v) noexcept
        {
            this->x += T(v.x);
            this->y += T(v.y);
            return *this;
        }

        template <typename U>
        tvec2<T>& operator-=(U const& s) noexcept
        {
            this->x -= T(s);
            this->y -= T(s);
            return *this;
        }
        template <typename U>
        tvec2<T>& operator-=(tvec2<U> const& v) noexcept
        {
            this->x -= T(v.x);
            this->y -= T(v.y);
            return *this;
        }

        template <typename U>
        tvec2<T>& operator*=(U s) noexcept
        {
            this->x *= T(s);
            this->y *= T(s);
            return *this;
        }
        template <typename U>
        tvec2<T>& operator*=(tvec2<U> const& v) noexcept
        {
            this->x *= T(v.x);
            this->y *= T(v.y);
            return *this;
        }

        template <typename U>
        tvec2<T>& operator/=(U s)
        {
            this->x /= T(s);
            this->y /= T(s);
            return *this;
        }
        template <typename U>
        tvec2<T>& operator/=(tvec2<U> const& v)
        {
            this->x /= T(v.x);
            this->y /= T(v.y);
            return *this;
        }

        tvec2<T>& operator++() noexcept
        {
            ++this->x;
            ++this->y;
            return *this;
        }

        tvec2<T>& operator--() noexcept
        {
            --this->x;
            --this->y;
            return *this;
        }

        constexpr size_type length() const noexcept
        {
            return 2;
        }

        value_type& operator[](size_type i) noexcept
        {
            assert(i < length());
            return (i == 0) ? x : y;
        }
        value_type const& operator[](size_type i) const noexcept
        {
            assert(i < length());
            return (i == 0) ? x : y;
        }

        void makeCeil(tvec2<T> cmp)
        {
            if (cmp.x > x) x = cmp.x;
            if (cmp.y > y) y = cmp.y;
        }

        void makeFloor(tvec2<T> cmp)
        {
            if (cmp.x < x) x = cmp.x;
            if (cmp.y < y) y = cmp.y;
        }
    };

    template <typename T>
    bool operator==(tvec2<T> const& v1, tvec2<T> const& v2) noexcept
    {
        return (v1.x == v2.x) && (v1.y == v2.y);
    }

    template <typename T>
    bool operator!=(tvec2<T> const& v1, tvec2<T> const& v2) noexcept
    {
        return !(v1 == v2);
    }

    template <typename T>
    tvec2<T> operator+(tvec2<T> const& v, T s) noexcept
    {
        return tvec2<T>(v.x + s, v.y + s);
    }

    template <typename T>
    tvec2<T> operator+(T s, tvec2<T> const& v) noexcept
    {
        return v + s;
    }

    template <typename T>
    tvec2<T> operator+(tvec2<T> const& v1, tvec2<T> const& v2) noexcept
    {
        return tvec2<T>(v1.x + v2.x, v1.y + v2.y);
    }

    template <typename T>
    tvec2<T> operator-(tvec2<T> const& v, T s) noexcept
    {
        return tvec2<T>(v.x - s, v.y - s);
    }

    template <typename T>
    tvec2<T> operator-(T s, tvec2<T> const& v) noexcept
    {
        return tvec2<T>(s - v.x, s - v.y);
    }

    template <typename T>
    tvec2<T> operator-(tvec2<T> const& v1, tvec2<T> const& v2) noexcept
    {
        return tvec2<T>(v1.x - v2.x, v1.y - v2.y);
    }

    template <typename T>
    tvec2<T> operator*(tvec2<T> const& v, T s) noexcept
    {
        return tvec2<T>(v.x * s, v.y * s);
    }

    template <typename T>
    tvec2<T> operator*(T s, tvec2<T> const& v) noexcept
    {
        return v * s;
    }

    template <typename T>
    tvec2<T> operator*(tvec2<T> const& v1, tvec2<T> const& v2) noexcept
    {
        return tvec2<T>(v1.x * v2.x, v1.y * v2.y);
    }

    template <typename T>
    tvec2<T> operator/(tvec2<T> const& v, T s)
    {
        return tvec2<T>(v.x / s, v.y / s);
    }

    template <typename T>
    tvec2<T> operator/(T s, tvec2<T> const& v)
    {
        return tvec2<T>(s / v.x, s / v.y);
    }

    template <typename T>
    tvec2<T> operator/(tvec2<T> const& v1, tvec2<T> const& v2)
    {
        return tvec2<T>(v1.x / v2.x, v1.y / v2.y);
    }

    template <typename T>
    tvec2<T> operator-(tvec2<T> const& v) noexcept
    {
        return tvec2<T>(-v.x, -v.y);
    }

    template <typename T>
    tvec2<T> operator++(tvec2<T>& v, int) noexcept
    {
        tvec2<T> temp = v;
        v.x += 1;
        v.y += 1;
        return temp;
    }

    template <typename T>
    tvec2<T> operator--(tvec2<T>& v, int) noexcept
    {
        tvec2<T> temp = v;
        v.x -= 1;
        v.y -= 1;
        return temp;
    }

    inline float2 uvLerp(const float2& c1, const float2& c2, float s) noexcept
    {
        float2 color;
        color.x = (c1.x + s * (c2.x - c1.x));
        color.y = (c1.y + s * (c2.y - c1.y));
        return color;
    }

    template <typename T>
    struct tvec3
    {
        using value_type = T;
        using size_type = size_t;
        using type = tvec3<T>;

        value_type x = value_type{ 0 };
        value_type y = value_type{ 0 };
        value_type z = value_type{ 0 };

        constexpr size_type length() const noexcept
        {
            return 3;
        }

        value_type& operator[](size_type i) noexcept
        {
            assert(i < this->length());
            return (&x)[i];
        }

        value_type const& operator[](size_type i) const noexcept
        {
            assert(i < this->length());
            return (&x)[i];
        }

        tvec3() = default;
        tvec3(tvec3<T> const& v) noexcept
            : x(v.x), y(v.y), z(v.z) {
        }

        explicit tvec3(value_type s) noexcept
            : x(s), y(s), z(s) {
        }

        tvec3(value_type s0, value_type s1, value_type s2) noexcept
            : x(s0), y(s1), z(s2) {
        }

        template <typename U>
        tvec3(U s) noexcept
            : x(value_type(s)), y(value_type(s)), z(value_type(s)) {
        }

        template <typename A, typename B, typename C>
        tvec3(A x, B y, C z) noexcept
            : x(value_type(x)), y(value_type(y)), z(value_type(z)) {
        }

        template <typename A, typename B>
        tvec3(tvec2<A> const& v, B s) noexcept
            : x(value_type(v.x)), y(value_type(v.y)), z(value_type(s)) {
        }

        template <typename A, typename B>
        tvec3(A s, tvec2<B> const& v) noexcept
            : x(value_type(s)), y(value_type(v.x)), z(value_type(v.y)) {
        }

        template <typename U>
        tvec3(tvec3<U> const& v) noexcept
            : x(value_type(v.x)), y(value_type(v.y)), z(value_type(v.z)) {
        }

        tvec3<T>& operator=(tvec3<T> const& v) noexcept
        {
            if (*this == v)
            {
                return *this;
            }

            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            return *this;
        }

        template <typename U>
        tvec3<T>& operator=(tvec3<U> const& v) noexcept
        {
            this->x = T(v.x);
            this->y = T(v.y);
            this->z = T(v.z);
            return *this;
        }

        template <typename U>
        tvec3<T>& operator+=(U const& s) noexcept
        {
            this->x += T(s);
            this->y += T(s);
            this->z += T(s);
            return *this;
        }

        template <typename U>
        tvec3<T>& operator+=(tvec3<U> const& v) noexcept
        {
            this->x += T(v.x);
            this->y += T(v.y);
            this->z += T(v.z);
            return *this;
        }

        template <typename U>
        tvec3<T>& operator-=(U const& s) noexcept
        {
            this->x -= T(s);
            this->y -= T(s);
            this->z -= T(s);
            return *this;
        }

        template <typename U>
        tvec3<T>& operator-=(tvec3<U> const& v) noexcept
        {
            this->x -= T(v.x);
            this->y -= T(v.y);
            this->z -= T(v.z);
            return *this;
        }

        template <typename U>
        tvec3<T>& operator*=(U const& s) noexcept
        {
            this->x *= T(s);
            this->y *= T(s);
            this->z *= T(s);
            return *this;
        }

        template <typename U>
        tvec3<T>& operator*=(tvec3<U> const& v) noexcept
        {
            this->x *= T(v.x);
            this->y *= T(v.y);
            this->z *= T(v.z);
            return *this;
        }

        template <typename U>
        tvec3<T>& operator/=(U const& s) noexcept
        {
            this->x /= T(s);
            this->y /= T(s);
            this->z /= T(s);
            return *this;
        }

        template <typename U>
        tvec3<T>& operator/=(tvec3<U> const& v) noexcept
        {
            this->x /= T(v.x);
            this->y /= T(v.y);
            this->z /= T(v.z);
            return *this;
        }

        tvec3<T>& operator++() noexcept
        {
            ++this->x;
            ++this->y;
            ++this->z;
            return *this;
        }
        tvec3<T>& operator--() noexcept
        {
            --this->x;
            --this->y;
            --this->z;
            return *this;
        }

        void makeFloor(const tvec3<T>& cmp) noexcept
        {
            if (cmp.x < x) x = cmp.x;
            if (cmp.y < y) y = cmp.y;
            if (cmp.z < z) z = cmp.z;
        }

        void makeCeil(const tvec3<T>& cmp) noexcept
        {
            if (cmp.x > x) x = cmp.x;
            if (cmp.y > y) y = cmp.y;
            if (cmp.z > z) z = cmp.z;
        }

        T lengthf() const
        {
            return (T)sqrtf(x * x + y * y + z * z);
        }
    };

    template<typename T>
    bool operator >(const tvec3<T>& left, const tvec3<T>& right) noexcept
    {
        return  left.x > right.x && left.y > right.y && left.z > right.z;
    }

    template<typename T>
    bool operator <(const tvec3<T>& left, const tvec3<T>& right) noexcept
    {
        return  left.x < right.x && left.y < right.y && left.z < right.z;
    }

    template <typename T>
    bool operator==(tvec3<T> const& v1, tvec3<T> const& v2) noexcept
    {
        return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
    }

    template <typename T>
    bool operator!=(tvec3<T> const& v1, tvec3<T> const& v2) noexcept
    {
        return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
    }

    template <typename T>
    tvec3<T> operator+(tvec3<T> const& v, T const& s) noexcept
    {
        return tvec3<T>(
            v.x + T(s),
            v.y + T(s),
            v.z + T(s));
    }

    template <typename T>
    tvec3<T> operator+ (T const& s, tvec3<T> const& v) noexcept
    {
        return tvec3<T>(
            T(s) + v.x,
            T(s) + v.y,
            T(s) + v.z);
    }

    template <typename T>
    tvec3<T> operator+ (tvec3<T> const& v1, tvec3<T> const& v2) noexcept
    {
        return tvec3<T>(
            v1.x + T(v2.x),
            v1.y + T(v2.y),
            v1.z + T(v2.z));
    }

    template <typename T>
    tvec3<T> operator- (tvec3<T> const& v, T const& s) noexcept
    {
        return tvec3<T>(
            v.x - T(s),
            v.y - T(s),
            v.z - T(s));
    }

    template <typename T>
    tvec3<T> operator- (T const& s, tvec3<T> const& v) noexcept
    {
        return tvec3<T>(
            T(s) - v.x,
            T(s) - v.y,
            T(s) - v.z);
    }

    template <typename T>
    tvec3<T> operator- (tvec3<T> const& v1, tvec3<T> const& v2) noexcept
    {
        return tvec3<T>(
            v1.x - T(v2.x),
            v1.y - T(v2.y),
            v1.z - T(v2.z));
    }

    template <typename T>
    tvec3<T> operator*(tvec3<T> const& v, T const& s) noexcept
    {
        return tvec3<T>(
            v.x * T(s),
            v.y * T(s),
            v.z * T(s));
    }

    template <typename T>
    tvec3<T> operator*(T const& s, tvec3<T> const& v) noexcept
    {
        return tvec3<T>(
            T(s) * v.x,
            T(s) * v.y,
            T(s) * v.z);
    }

    template <typename T>
    tvec3<T> operator*(tvec3<T> const& v1, tvec3<T> const& v2) noexcept
    {
        return tvec3<T>(
            v1.x * T(v2.x),
            v1.y * T(v2.y),
            v1.z * T(v2.z));
    }

    template <typename T>
    tvec3<T> operator/(tvec3<T> const& v, T const& s) noexcept
    {
        return tvec3<T>(
            v.x / T(s),
            v.y / T(s),
            v.z / T(s));
    }

    template <typename T>
    tvec3<T> operator/(T const& s, tvec3<T> const& v) noexcept
    {
        return tvec3<T>(
            T(s) / v.x,
            T(s) / v.y,
            T(s) / v.z);
    }

    template <typename T>
    tvec3<T> operator/(tvec3<T> const& v1, tvec3<T> const& v2) noexcept
    {
        return tvec3<T>(
            v1.x / T(v2.x),
            v1.y / T(v2.y),
            v1.z / T(v2.z));
    }

    template <typename T>
    tvec3<T> operator-(tvec3<T> const& v) noexcept
    {
        return tvec3<T>(
            -v.x,
            -v.y,
            -v.z);
    }

    template <typename T>
    tvec3<T> operator++(tvec3<T> const& v, int) noexcept
    {
        return tvec3<T>(
            v.x + T(1),
            v.y + T(1),
            v.z + T(1));
    }

    template <typename T>
    tvec3<T> operator--(tvec3<T> const& v, int) noexcept
    {
        return tvec3<T>(
            v.x - T(1),
            v.y - T(1),
            v.z - T(1));
    }

    template <typename T>
    tvec3<T> rotateX(const tvec3<T>& v, T angle) noexcept
    {
        tvec3<T> res(v);
        T c = cos(T(DEG2RAD(angle)));
        T s = sin(T(DEG2RAD(angle)));

        res.y = v.y * c - v.z * s;
        res.z = v.y * s + v.z * c;
        return res;
    }

    template <typename T>
    tvec3<T> rotateY(tvec3<T> const& v, T angle) noexcept
    {
        tvec3<T> res = v;

        T c = cos(T(DEG2RAD(angle)));
        T s = sin(T(DEG2RAD(angle)));

        res.x = v.x * c + v.z * s;
        res.z = -v.x * s + v.z * c;
        return res;
    }

    template <typename T>
    tvec3<T> rotateZ(tvec3<T> const& v, T angle) noexcept
    {

        tvec3<T> res = v;

        T c = cos(DEG2RAD(angle));
        T s = sin(DEG2RAD(angle));

        res.x = v.x * c - v.y * s;
        res.y = v.x * s + v.y * c;
        return res;
    }

    template <typename T>
    struct tmat3x3
    {
        using value_type = T;
        using size_type = size_t;
        using col_type = tvec3<T>;
        using type = tmat3x3<T>;
        using transpose_type = tmat3x3<T>;

    private:
        col_type value[3];

    public:
        size_type length() const noexcept
        {
            return 3;
        }

        size_type col_size() const noexcept
        {
            return 3;
        }

        size_type row_size() const noexcept
        {
            return 3;
        }

        tmat3x3() noexcept
        {
            value_type const Zero(0);
            value_type const One(1);
            this->value[0] = col_type(One, Zero, Zero);
            this->value[1] = col_type(Zero, One, Zero);
            this->value[2] = col_type(Zero, Zero, One);
        }

        tmat3x3(tmat3x3<T> const& m) noexcept
        {
            this->value[0] = m.value[0];
            this->value[1] = m.value[1];
            this->value[2] = m.value[2];
        }

        tmat3x3(value_type const& s) noexcept
        {
            value_type const Zero(0);
            this->value[0] = col_type(s, Zero, Zero);
            this->value[1] = col_type(Zero, s, Zero);
            this->value[2] = col_type(Zero, Zero, s);
        }

        tmat3x3
        (
            value_type const& x0, value_type const& y0, value_type const& z0,
            value_type const& x1, value_type const& y1, value_type const& z1,
            value_type const& x2, value_type const& y2, value_type const& z2
        ) noexcept
        {
            this->value[0] = col_type(x0, y0, z0);
            this->value[1] = col_type(x1, y1, z1);
            this->value[2] = col_type(x2, y2, z2);
        }

        tmat3x3
        (
            col_type const& v0,
            col_type const& v1,
            col_type const& v2
        ) noexcept
        {
            this->value[0] = v0;
            this->value[1] = v1;
            this->value[2] = v2;
        }

        template <typename U>
        tmat3x3(U const& s) noexcept
        {
            value_type const Zero(0);
            this->value[0] = tvec3<T>(value_type(s), Zero, Zero);
            this->value[1] = tvec3<T>(Zero, value_type(s), Zero);
            this->value[2] = tvec3<T>(Zero, Zero, value_type(s));
        }

        template <
            typename X1, typename Y1, typename Z1,
            typename X2, typename Y2, typename Z2,
            typename X3, typename Y3, typename Z3
        >
        tmat3x3
        (
            X1 const& x1, Y1 const& y1, Z1 const& z1,
            X2 const& x2, Y2 const& y2, Z2 const& z2,
            X3 const& x3, Y3 const& y3, Z3 const& z3
        ) noexcept
        {
            this->value[0] = col_type(value_type(x1), value_type(y1), value_type(z1));
            this->value[1] = col_type(value_type(x2), value_type(y2), value_type(z2));
            this->value[2] = col_type(value_type(x3), value_type(y3), value_type(z3));
        }

        template <typename V1, typename V2, typename V3>
        tmat3x3
        (
            tvec3<V1> const& v1,
            tvec3<V2> const& v2,
            tvec3<V3> const& v3
        ) noexcept
        {
            this->value[0] = col_type(v1);
            this->value[1] = col_type(v2);
            this->value[2] = col_type(v3);
        }

        template <typename U>
        tmat3x3(tmat3x3<U> const& m) noexcept
        {
            this->value[0] = col_type(m[0]);
            this->value[1] = col_type(m[1]);
            this->value[2] = col_type(m[2]);
        }

        // TODO 矩阵不一定有逆，需要判断
        tmat3x3<T> _inverse() const noexcept
        {
            T S00 = value[0][0];
            T S01 = value[0][1];
            T S02 = value[0][2];

            T S10 = value[1][0];
            T S11 = value[1][1];
            T S12 = value[1][2];

            T S20 = value[2][0];
            T S21 = value[2][1];
            T S22 = value[2][2];

            tmat3x3<T> Inverse(
                S11 * S22 - S21 * S12,
                S12 * S20 - S22 * S10,
                S10 * S21 - S20 * S11,
                S02 * S21 - S01 * S22,
                S00 * S22 - S02 * S20,
                S01 * S20 - S00 * S21,
                S12 * S01 - S11 * S02,
                S10 * S02 - S12 * S00,
                S11 * S00 - S10 * S01);

            T Determinant = S00 * (S11 * S22 - S21 * S12)
                - S10 * (S01 * S22 - S21 * S02)
                + S20 * (S01 * S12 - S11 * S02);

            Inverse /= Determinant;
            return Inverse;
        }

        col_type& operator[](size_type i) noexcept
        {
            assert(i < this->length());
            return this->value[i];
        }

        col_type const& operator[](size_type i) const noexcept
        {
            assert(i < this->length());
            return this->value[i];
        }

        tmat3x3<T>& operator=(tmat3x3<T> const& m) noexcept
        {
            this->value[0] = m[0];
            this->value[1] = m[1];
            this->value[2] = m[2];
            return *this;
        }

        template <typename U>
        tmat3x3<T>& operator=(tmat3x3<U> const& m) noexcept
        {
            this->value[0] = m[0];
            this->value[1] = m[1];
            this->value[2] = m[2];
            return *this;
        }

        template <typename U>
        tmat3x3<T>& operator+= (U const& s) noexcept
        {
            this->value[0] += s;
            this->value[1] += s;
            this->value[2] += s;
            return *this;
        }

        template <typename U>
        tmat3x3<T>& operator+=(tmat3x3<U> const& m) noexcept
        {
            this->value[0] += m[0];
            this->value[1] += m[1];
            this->value[2] += m[2];
            return *this;
        }

        template <typename U>
        tmat3x3<T>& operator-=(U const& s) noexcept
        {
            this->value[0] -= s;
            this->value[1] -= s;
            this->value[2] -= s;
            return *this;
        }

        template <typename U>
        tmat3x3<T>& operator-=(tmat3x3<U> const& m) noexcept
        {
            this->value[0] -= m[0];
            this->value[1] -= m[1];
            this->value[2] -= m[2];
            return *this;
        }

        template <typename U>
        tmat3x3<T>& operator*=(U const& s) noexcept
        {
            this->value[0] *= s;
            this->value[1] *= s;
            this->value[2] *= s;
            return *this;
        }

        template <typename U>
        tmat3x3<T>& operator*=(tmat3x3<U> const& m) noexcept
        {
            return (*this = *this * m);
        }

        template <typename U>
        tmat3x3<T>& operator/=(U const& s) noexcept
        {
            this->value[0] /= s;
            this->value[1] /= s;
            this->value[2] /= s;
            return *this;
        }

        template <typename U>
        tmat3x3<T>& operator/= (tmat3x3<U> const& m) noexcept
        {
            return (*this = *this / m);
        }

        tmat3x3<T>& operator++() noexcept
        {
            ++this->value[0];
            ++this->value[1];
            ++this->value[2];
            return *this;
        }
        tmat3x3<T>& operator--() noexcept
        {
            --this->value[0];
            --this->value[1];
            --this->value[2];
            return *this;
        }

        tvec3<T> operator*(const tvec3<T>& v) const noexcept
        {

            return  tvec3<T>(
                value[0][0] * v[0] + value[1][0] * v[1] + value[2][0] * v[2]
                , value[0][1] * v[0] + value[1][1] * v[1] + value[2][1] * v[2]
                , value[0][2] * v[0] + value[1][2] * v[1] + value[2][2] * v[2]
            );
        }

        // 齐次坐标
        tvec2<T> operator*(const tvec2<T>& v) const noexcept
        {
            return  tvec2<T>(
                value[0][0] * v[0] + value[1][0] * v[1] + value[2][0]
                , value[0][1] * v[0] + value[1][1] * v[1] + value[2][1]
            );
        }

        void scale(T x, T y) noexcept
        {
            this->value[0] = col_type(value_type(x), value_type(0), value_type(0));
            this->value[1] = col_type(value_type(0), value_type(y), value_type(0));
            this->value[2] = col_type(value_type(0), value_type(0), value_type(1));
        }

        void rotate(T angle) noexcept
        {
            T rad = DEG2RAD(angle);
            T c = cos(rad);
            T s = sin(rad);
            this->value[0] = col_type(value_type(c), value_type(-s), value_type(0));
            this->value[1] = col_type(value_type(s), value_type(c), value_type(0));
            this->value[2] = col_type(value_type(0), value_type(0), value_type(1));
        }

        void translate(T x, T y) noexcept
        {
            this->value[0] = col_type(value_type(1), value_type(0), value_type(0));
            this->value[1] = col_type(value_type(0), value_type(1), value_type(0));
            this->value[2] = col_type(value_type(x), value_type(y), value_type(1));
        }
    };


    template <typename T>
    tmat3x3<T> operator+(tmat3x3<T> const& m, T const& s) noexcept
    {
        return tmat3x3<T>(
            m[0] + s,
            m[1] + s,
            m[2] + s);
    }

    template <typename T>
    tmat3x3<T> operator+(T const& s, tmat3x3<T> const& m) noexcept
    {
        return tmat3x3<T>(
            m[0] + s,
            m[1] + s,
            m[2] + s);
    }

    template <typename T>
    tmat3x3<T> operator+(tmat3x3<T> const& m1, tmat3x3<T> const& m2) noexcept
    {
        return tmat3x3<T>(
            m1[0] + m2[0],
            m1[1] + m2[1],
            m1[2] + m2[2]);
    }

    template <typename T>
    tmat3x3<T> operator-(tmat3x3<T> const& m, T const& s) noexcept
    {
        return tmat3x3<T>(
            m[0] - s,
            m[1] - s,
            m[2] - s);
    }

    template <typename T>
    tmat3x3<T> operator-(T const& s, tmat3x3<T> const& m) noexcept
    {
        return tmat3x3<T>(
            s - m[0],
            s - m[1],
            s - m[2]);
    }

    template <typename T>
    tmat3x3<T> operator-(tmat3x3<T> const& m1, tmat3x3<T> const& m2) noexcept
    {
        return tmat3x3<T>(
            m1[0] - m2[0],
            m1[1] - m2[1],
            m1[2] - m2[2]);
    }

    template <typename T>
    tmat3x3<T> operator*(tmat3x3<T> const& m, T const& s) noexcept
    {
        return tmat3x3<T>(
            m[0] * s,
            m[1] * s,
            m[2] * s);
    }

    template <typename T>
    tmat3x3<T> operator*(T const& s, tmat3x3<T> const& m) noexcept
    {
        return tmat3x3<T>(
            m[0] * s,
            m[1] * s,
            m[2] * s);
    }

    template <typename T>
    tvec3<T> operator*(tmat3x3<T> const& m, tvec3<T> const& v) noexcept
    {
        return  tvec3<T>(
            m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
            m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
    }

    template <typename T>
    tvec3<T> operator*(tvec3<T> const& v, tmat3x3<T> const& m) noexcept
    {
        return tvec3<T>(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
    }

    template <typename T>
    tmat3x3<T> operator*(tmat3x3<T> const& m1, tmat3x3<T> const& m2) noexcept
    {
        T const srcA00 = m1[0][0];
        T const srcA01 = m1[0][1];
        T const srcA02 = m1[0][2];
        T const srcA10 = m1[1][0];
        T const srcA11 = m1[1][1];
        T const srcA12 = m1[1][2];
        T const srcA20 = m1[2][0];
        T const srcA21 = m1[2][1];
        T const srcA22 = m1[2][2];

        T const srcB00 = m2[0][0];
        T const srcB01 = m2[0][1];
        T const srcB02 = m2[0][2];
        T const srcB10 = m2[1][0];
        T const srcB11 = m2[1][1];
        T const srcB12 = m2[1][2];
        T const srcB20 = m2[2][0];
        T const srcB21 = m2[2][1];
        T const srcB22 = m2[2][2];

        tmat3x3<T> res;
        res[0][0] = srcA00 * srcB00 + srcA10 * srcB01 + srcA20 * srcB02;
        res[0][1] = srcA01 * srcB00 + srcA11 * srcB01 + srcA21 * srcB02;
        res[0][2] = srcA02 * srcB00 + srcA12 * srcB01 + srcA22 * srcB02;
        res[1][0] = srcA00 * srcB10 + srcA10 * srcB11 + srcA20 * srcB12;
        res[1][1] = srcA01 * srcB10 + srcA11 * srcB11 + srcA21 * srcB12;
        res[1][2] = srcA02 * srcB10 + srcA12 * srcB11 + srcA22 * srcB12;
        res[2][0] = srcA00 * srcB20 + srcA10 * srcB21 + srcA20 * srcB22;
        res[2][1] = srcA01 * srcB20 + srcA11 * srcB21 + srcA21 * srcB22;
        res[2][2] = srcA02 * srcB20 + srcA12 * srcB21 + srcA22 * srcB22;
        return res;
    }

    template <typename T>
    tmat3x3<T> operator/(tmat3x3<T> const& m, T const& s) noexcept
    {
        return tmat3x3<T>(
            m[0] / s,
            m[1] / s,
            m[2] / s);
    }

    template <typename T>
    tmat3x3<T> operator/(T const& s, tmat3x3<T> const& m) noexcept
    {
        return tmat3x3<T>(
            s / m[0],
            s / m[1],
            s / m[2]
        );
    }

    template <typename T>
    tvec3<T> operator/(tmat3x3<T> const& m, tvec3<T> const& v) noexcept
    {
        return m._inverse() * v;
    }

    template <typename T>
    tvec3<T> operator/(tvec3<T> const& v, tmat3x3<T> const& m) noexcept
    {
        return v * m._inverse();
    }

    template <typename T>
    tmat3x3<T> operator/(tmat3x3<T> const& m1, tmat3x3<T> const& m2) noexcept
    {
        return m1 * m2._inverse();
    }

    template <typename T>
    tmat3x3<T> const operator-(tmat3x3<T> const& m) noexcept
    {
        return tmat3x3<T>(
            -m[0],
            -m[1],
            -m[2]);
    }

    template <typename T>
    tmat3x3<T> const operator++(tmat3x3<T> const& m, int) noexcept
    {
        return tmat3x3<T>(
            m[0] + T(1),
            m[1] + T(1),
            m[2] + T(1));
    }

    template <typename T>
    tmat3x3<T> const operator--(tmat3x3<T> const& m, int) noexcept
    {
        return tmat3x3<T>(
            m[0] - T(1),
            m[1] - T(1),
            m[2] - T(1));
    }

    template <typename T>
    bool operator==(tmat3x3<T> const& m1, tmat3x3<T> const& m2) noexcept
    {
        return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]);
    }

    template <typename T>
    bool operator!=(tmat3x3<T> const& m1, tmat3x3<T> const& m2) noexcept
    {
        return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]);
    }

    template <typename T>
    struct tvec4
    {
        using value_type = T;
        using size_type = size_t ;
        using type = tvec4<T>;

        value_type x{ value_type(0) }, y{ value_type(0) }, z{ value_type{0} }, w{ value_type{0} };

        constexpr size_type length() const noexcept
        {
            return 4;
        }

        value_type& operator[](size_type i) noexcept
        {
            assert(i < this->length());
            return (&x)[i];
        }

        value_type const& operator[](size_type i) const noexcept
        {
            assert(i < this->length());
            return (&x)[i];
        }

        tvec4() = default;
        tvec4(tvec3<T> const& v, T s) noexcept 
            : x(v.x), y(v.y), z(v.z), w(s) {
        }
        tvec4(T s) noexcept
            : x(s), y(s), z(s), w(s) {
        }
        tvec4(tvec4<T> const& v) noexcept 
            : x(v.x), y(v.y), z(v.z), w(v.w) {
        }

        template <typename A, typename B>
        tvec4(tvec3<A> const& v, B s) noexcept
            : x(value_type(v.x)), y(value_type(v.y)), z(value_type(v.z)), w(value_type(s)) {
        }

        template <typename A, typename B>
        tvec4(A s, tvec3<B> const& v) noexcept 
            : x(value_type(s)), y(value_type(v.x)), z(value_type(v.y)), w(value_type(v.z)) {
        }

        template<typename U>
        tvec4(tvec4<U> const& v) noexcept
            : x(value_type(v.x)), y(value_type(v.y)), z(value_type(v.z)), w(value_type(v.w)) {
        }

        tvec4
        (
            value_type s1,
            value_type s2,
            value_type s3,
            value_type s4
        ) noexcept
            : x(s1), y(s2), z(s3), w(s4) {
        }

        tvec4<T>& operator=(tvec4<T> const& v) noexcept
        {
            if (this == &v) {
                return *this;
			}

            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = v.w;
            return *this;
        }

        template <typename U>
        tvec4<T>& operator= (tvec4<U> const& v) noexcept
        {
            this->x = T(v.x);
            this->y = T(v.y);
            this->z = T(v.z);
            this->w = T(v.w);
            return *this;
        }

        template <typename U>
        tvec4<T>& operator+=(U const& s) noexcept
        {
            this->x += T(s);
            this->y += T(s);
            this->z += T(s);
            this->w += T(s);
            return *this;
        }

        template <typename U>
        tvec4<T>& operator+=(tvec4<U> const& v) noexcept
        {
            this->x += T(v.x);
            this->y += T(v.y);
            this->z += T(v.z);
            this->w += T(v.w);
            return *this;
        }

        template <typename U>
        tvec4<T>& operator-=(U const& s) noexcept
        {
            this->x -= T(s);
            this->y -= T(s);
            this->z -= T(s);
            this->w -= T(s);
            return *this;
        }

        template <typename U>
        tvec4<T>& operator-=(tvec4<U> const& v) noexcept
        {
            this->x -= T(v.x);
            this->y -= T(v.y);
            this->z -= T(v.z);
            this->w -= T(v.w);
            return *this;
        }

        template <typename U>
        tvec4<T>& operator*=(U const& s) noexcept
        {
            this->x *= T(s);
            this->y *= T(s);
            this->z *= T(s);
            this->w *= T(s);
            return *this;
        }

        template <typename U>
        tvec4<T>& operator*=(tvec4<U> const& v) noexcept
        {
            this->x *= T(v.x);
            this->y *= T(v.y);
            this->z *= T(v.z);
            this->w *= T(v.w);
            return *this;
        }

        template <typename U>
        tvec4<T>& operator/=(U const& s) noexcept
        {
            this->x /= T(s);
            this->y /= T(s);
            this->z /= T(s);
            this->w /= T(s);
            return *this;
        }

        template <typename U>
        tvec4<T>& operator/=(tvec4<U> const& v) noexcept
        {
            this->x /= T(v.x);
            this->y /= T(v.y);
            this->z /= T(v.z);
            this->w /= T(v.w);
            return *this;
        }

        tvec4<T>& operator++() noexcept
        {
            ++this->x;
            ++this->y;
            ++this->z;
            ++this->w;
            return *this;
        }

        tvec4<T>& operator--() noexcept
        {
            --this->x;
            --this->y;
            --this->z;
            --this->w;
            return *this;
        }
    };

    template <typename T>
    tvec4<T> operator+(tvec4<T> const& v, T const& s) noexcept
    {
        return tvec4<T>(
            v.x + s,
            v.y + s,
            v.z + s,
            v.w + s);
    }

    template <typename T>
    tvec4<T> operator+(T const& s, tvec4<T> const& v) noexcept
    {
        return tvec4<T>(
            s + v.x,
            s + v.y,
            s + v.z,
            s + v.w);
    }

    template <typename T>
    tvec4<T> operator+(tvec4<T> const& v1, tvec4<T> const& v2) noexcept
    {
        return tvec4<T>(
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z,
            v1.w + v2.w);
    }

    template <typename T>
    tvec4<T> operator-(tvec4<T> const& v, T const& s) noexcept
    {
        return tvec4<T>(
            v.x - s,
            v.y - s,
            v.z - s,
            v.w - s);
    }

    template <typename T>
    tvec4<T> operator-(T const& s, tvec4<T> const& v) noexcept
    {
        return tvec4<T>(
            s - v.x,
            s - v.y,
            s - v.z,
            s - v.w);
    }

    template <typename T>
    tvec4<T> operator-(tvec4<T> const& v1, tvec4<T> const& v2) noexcept
    {
        return tvec4<T>(
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z,
            v1.w - v2.w);
    }

    template <typename T>
    tvec4<T> operator*(tvec4<T> const& v, T const& s) noexcept
    {
        return tvec4<T>(
            v.x * s,
            v.y * s,
            v.z * s,
            v.w * s);
    }

    template <typename T>
    tvec4<T> operator*(T const& s, tvec4<T> const& v) noexcept
    {
        return tvec4<T>(
            s * v.x,
            s * v.y,
            s * v.z,
            s * v.w);
    }

    template <typename T>
    tvec4<T> operator*(tvec4<T> const& v1, tvec4<T> const& v2) noexcept
    {
        return tvec4<T>(
            v1.x * v2.x,
            v1.y * v2.y,
            v1.z * v2.z,
            v1.w * v2.w);
    }

    template <typename T>
    tvec4<T> operator/(tvec4<T> const& v, T const& s) noexcept
    {
        return tvec4<T>(
            v.x / s,
            v.y / s,
            v.z / s,
            v.w / s);
    }

    template <typename T>
    tvec4<T> operator/(T const& s, tvec4<T> const& v) noexcept
    {
        return tvec4<T>(
            s / v.x,
            s / v.y,
            s / v.z,
            s / v.w);
    }

    template <typename T>
    tvec4<T> operator/(tvec4<T> const& v1, tvec4<T> const& v2) noexcept
    {
        return tvec4<T>(
            v1.x / v2.x,
            v1.y / v2.y,
            v1.z / v2.z,
            v1.w / v2.w);
    }

    template <typename T>
    tvec4<T> operator-(tvec4<T> const& v) noexcept
    {
        return tvec4<T>(
            -v.x,
            -v.y,
            -v.z,
            -v.w);
    }

    template <typename T>
    bool operator==(tvec4<T> const& v1, tvec4<T> const& v2) noexcept
    {
        return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w);
    }

    template <typename T>
    bool operator!=(tvec4<T> const& v1, tvec4<T> const& v2) noexcept
    {
        return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z) || (v1.w != v2.w);
    }

    template <typename T>
    struct tmat4x4
    {
        using value_type = T;
        using size_type = size_t;
        using col_type = tvec4<T>;
        using row_type = tvec4<T>;
        using type = tmat4x4<T>;
        using transpose_type = tmat4x4<T>;

    private:
        col_type value[4];

    public:
        tmat4x4<T> inverse() const noexcept
        {
            value_type subFactor00 = this->value[2][2] * this->value[3][3] - this->value[3][2] * this->value[2][3];
            value_type subFactor01 = this->value[2][1] * this->value[3][3] - this->value[3][1] * this->value[2][3];
            value_type subFactor02 = this->value[2][1] * this->value[3][2] - this->value[3][1] * this->value[2][2];
            value_type subFactor03 = this->value[2][0] * this->value[3][3] - this->value[3][0] * this->value[2][3];
            value_type subFactor04 = this->value[2][0] * this->value[3][2] - this->value[3][0] * this->value[2][2];
            value_type subFactor05 = this->value[2][0] * this->value[3][1] - this->value[3][0] * this->value[2][1];
            value_type subFactor06 = this->value[1][2] * this->value[3][3] - this->value[3][2] * this->value[1][3];
            value_type subFactor07 = this->value[1][1] * this->value[3][3] - this->value[3][1] * this->value[1][3];
            value_type subFactor08 = this->value[1][1] * this->value[3][2] - this->value[3][1] * this->value[1][2];
            value_type subFactor09 = this->value[1][0] * this->value[3][3] - this->value[3][0] * this->value[1][3];
            value_type subFactor10 = this->value[1][0] * this->value[3][2] - this->value[3][0] * this->value[1][2];
            value_type subFactor11 = this->value[1][1] * this->value[3][3] - this->value[3][1] * this->value[1][3];
            value_type SubFactor12 = this->value[1][0] * this->value[3][1] - this->value[3][0] * this->value[1][1];
            value_type subFactor13 = this->value[1][2] * this->value[2][3] - this->value[2][2] * this->value[1][3];
            value_type subFactor14 = this->value[1][1] * this->value[2][3] - this->value[2][1] * this->value[1][3];
            value_type subFactor15 = this->value[1][1] * this->value[2][2] - this->value[2][1] * this->value[1][2];
            value_type subFactor16 = this->value[1][0] * this->value[2][3] - this->value[2][0] * this->value[1][3];
            value_type subFactor17 = this->value[1][0] * this->value[2][2] - this->value[2][0] * this->value[1][2];
            value_type subFactor18 = this->value[1][0] * this->value[2][1] - this->value[2][0] * this->value[1][1];

            tmat4x4<T> res(
                +this->value[1][1] * subFactor00 - this->value[1][2] * subFactor01 + this->value[1][3] * subFactor02,
                -this->value[1][0] * subFactor00 + this->value[1][2] * subFactor03 - this->value[1][3] * subFactor04,
                +this->value[1][0] * subFactor01 - this->value[1][1] * subFactor03 + this->value[1][3] * subFactor05,
                -this->value[1][0] * subFactor02 + this->value[1][1] * subFactor04 - this->value[1][2] * subFactor05,

                -this->value[0][1] * subFactor00 + this->value[0][2] * subFactor01 - this->value[0][3] * subFactor02,
                +this->value[0][0] * subFactor00 - this->value[0][2] * subFactor03 + this->value[0][3] * subFactor04,
                -this->value[0][0] * subFactor01 + this->value[0][1] * subFactor03 - this->value[0][3] * subFactor05,
                +this->value[0][0] * subFactor02 - this->value[0][1] * subFactor04 + this->value[0][2] * subFactor05,

                +this->value[0][1] * subFactor06 - this->value[0][2] * subFactor07 + this->value[0][3] * subFactor08,
                -this->value[0][0] * subFactor06 + this->value[0][2] * subFactor09 - this->value[0][3] * subFactor10,
                +this->value[0][0] * subFactor11 - this->value[0][1] * subFactor09 + this->value[0][3] * SubFactor12,
                -this->value[0][0] * subFactor08 + this->value[0][1] * subFactor10 - this->value[0][2] * SubFactor12,

                -this->value[0][1] * subFactor13 + this->value[0][2] * subFactor14 - this->value[0][3] * subFactor15,
                +this->value[0][0] * subFactor13 - this->value[0][2] * subFactor16 + this->value[0][3] * subFactor17,
                -this->value[0][0] * subFactor14 + this->value[0][1] * subFactor16 - this->value[0][3] * subFactor18,
                +this->value[0][0] * subFactor15 - this->value[0][1] * subFactor17 + this->value[0][2] * subFactor18);

            value_type determinant =
                +this->value[0][0] * res[0][0]
                + this->value[0][1] * res[1][0]
                + this->value[0][2] * res[2][0]
                + this->value[0][3] * res[3][0];

            res /= determinant;
            return res;
        }

    public:
        constexpr size_type length() const noexcept
        {
            return 4;
        }

        constexpr size_type col_size() const noexcept
        {
            return 4;
        }

        constexpr size_type row_size() const noexcept
        {
            return 4;
        }

        void identify() noexcept
        {
            this->value[0] = col_type(1, 0, 0, 0);
            this->value[1] = col_type(0, 1, 0, 0);
            this->value[2] = col_type(0, 0, 1, 0);
            this->value[3] = col_type(0, 0, 0, 1);
        }

        col_type& operator[](size_type i) noexcept
        {
            assert(i < this->length());
            return this->value[i];
        }

        col_type const& operator[](size_type i) const noexcept
        {
            assert(i < this->length());
            return this->value[i];
        }


        tmat4x4() = default;

        tmat4x4(tmat4x4<T> const& m) noexcept
        {
            this->value[0] = m.value[0];
            this->value[1] = m.value[1];
            this->value[2] = m.value[2];
            this->value[3] = m.value[3];
        }

        tmat4x4(tmat3x3<T> const& m) noexcept
        {
            this->value[0] = col_type(m[0], value_type(0));
            this->value[1] = col_type(m[1], value_type(0));
            this->value[2] = col_type(m[2], value_type(0));
            this->value[3] = col_type(value_type(0), value_type(0), value_type(0), value_type(1));
        }

        tmat4x4(value_type s) noexcept
        {
            value_type const Zero(0);
            this->value[0] = col_type(s, Zero, Zero, Zero);
            this->value[1] = col_type(Zero, s, Zero, Zero);
            this->value[2] = col_type(Zero, Zero, s, Zero);
            this->value[3] = col_type(Zero, Zero, Zero, s);
        }

        tmat4x4
        (
            value_type const& x0, value_type const& y0, value_type const& z0, value_type const& w0,
            value_type const& x1, value_type const& y1, value_type const& z1, value_type const& w1,
            value_type const& x2, value_type const& y2, value_type const& z2, value_type const& w2,
            value_type const& x3, value_type const& y3, value_type const& z3, value_type const& w3
        ) noexcept
        {
            this->value[0] = col_type(x0, y0, z0, w0);
            this->value[1] = col_type(x1, y1, z1, w1);
            this->value[2] = col_type(x2, y2, z2, w2);
            this->value[3] = col_type(x3, y3, z3, w3);
        }

        tmat4x4
        (
            col_type const& v0,
            col_type const& v1,
            col_type const& v2,
            col_type const& v3
        ) noexcept
        {
            this->value[0] = v0;
            this->value[1] = v1;
            this->value[2] = v2;
            this->value[3] = v3;
        }

        template <typename U>
        tmat4x4(tmat4x4<U> const& m) noexcept
        {
            this->value[0] = col_type(m[0]);
            this->value[1] = col_type(m[1]);
            this->value[2] = col_type(m[2]);
            this->value[3] = col_type(m[3]);
        }

        template <typename U>
        tmat4x4(U const& s) noexcept
        {
            value_type const Zero(0);
            this->value[0] = tvec4<T>(value_type(s), Zero, Zero, Zero);
            this->value[1] = tvec4<T>(Zero, value_type(s), Zero, Zero);
            this->value[2] = tvec4<T>(Zero, Zero, value_type(s), Zero);
            this->value[3] = tvec4<T>(Zero, Zero, Zero, value_type(s));
        }
        template <
            typename X1, typename Y1, typename Z1, typename W1,
            typename X2, typename Y2, typename Z2, typename W2,
            typename X3, typename Y3, typename Z3, typename W3,
            typename X4, typename Y4, typename Z4, typename W4>
        tmat4x4
        (
            X1 const& x1, Y1 const& y1, Z1 const& z1, W1 const& w1,
            X2 const& x2, Y2 const& y2, Z2 const& z2, W2 const& w2,
            X3 const& x3, Y3 const& y3, Z3 const& z3, W3 const& w3,
            X4 const& x4, Y4 const& y4, Z4 const& z4, W4 const& w4
        ) noexcept
        {
            this->value[0] = col_type(value_type(x1), value_type(y1), value_type(z1), value_type(w1));
            this->value[1] = col_type(value_type(x2), value_type(y2), value_type(z2), value_type(w2));
            this->value[2] = col_type(value_type(x3), value_type(y3), value_type(z3), value_type(w3));
            this->value[3] = col_type(value_type(x4), value_type(y4), value_type(z4), value_type(w4));
        }
        template <typename V1, typename V2, typename V3, typename V4>
        tmat4x4
        (
            tvec4<V1> const& v1,
            tvec4<V2> const& v2,
            tvec4<V3> const& v3,
            tvec4<V4> const& v4
        ) noexcept
        {
            this->value[0] = col_type(v1);
            this->value[1] = col_type(v2);
            this->value[2] = col_type(v3);
            this->value[3] = col_type(v4);
        }

        T const* data() const noexcept
        {
            return &this->value[0][0];
        }

        tmat4x4<T>& operator=(tmat4x4<T> const& m) noexcept
        {
            this->value[0] = m[0];
            this->value[1] = m[1];
            this->value[2] = m[2];
            this->value[3] = m[3];
            return *this;
        }

        template <typename U>
        tmat4x4<T>& operator=(tmat4x4<U> const& m) noexcept
        {
            this->value[0] = m[0];
            this->value[1] = m[1];
            this->value[2] = m[2];
            this->value[3] = m[3];
            return *this;
        }

        template <typename U>
        tmat4x4<T>& operator+=(U const& s) noexcept
        {
            this->value[0] += s;
            this->value[1] += s;
            this->value[2] += s;
            this->value[3] += s;
            return *this;
        }

        template <typename U>
        tmat4x4<T>& operator+=(tmat4x4<U> const& m) noexcept
        {
            this->value[0] += m[0];
            this->value[1] += m[1];
            this->value[2] += m[2];
            this->value[3] += m[3];
            return *this;
        }

        template <typename U>
        tmat4x4<T>& operator-=(U const& s) noexcept
        {
            this->value[0] -= s;
            this->value[1] -= s;
            this->value[2] -= s;
            this->value[3] -= s;
            return *this;
        }

        template <typename U>
        tmat4x4<T>& operator-=(tmat4x4<U> const& m) noexcept
        {
            this->value[0] -= m[0];
            this->value[1] -= m[1];
            this->value[2] -= m[2];
            this->value[3] -= m[3];
            return *this;
        }

        template <typename U>
        tmat4x4<T>& operator*=(U const& s) noexcept
        {
            this->value[0] *= s;
            this->value[1] *= s;
            this->value[2] *= s;
            this->value[3] *= s;
            return *this;
        }

        template <typename U>
        tmat4x4<T>& operator*=(tmat4x4<U> const& m) noexcept
        {
            return (*this = *this * m);
        }

        template <typename U>
        tmat4x4<T>& operator/=(U const& s) noexcept
        {
            this->value[0] /= s;
            this->value[1] /= s;
            this->value[2] /= s;
            this->value[3] /= s;
            return *this;
        }
        template <typename U>
        tmat4x4<T>& operator/=(tmat4x4<U> const& m) noexcept
        {
            return (*this = *this / m);
        }

        tmat4x4<T>& operator++() noexcept
        {
            ++this->value[0];
            ++this->value[1];
            ++this->value[2];
            ++this->value[3];
            return *this;
        }

        tmat4x4<T>& operator--() noexcept
        {
            --this->value[0];
            --this->value[1];
            --this->value[2];
            --this->value[3];
            return *this;
        }

        tmat4x4<T>& translate(value_type x, value_type y, value_type z) noexcept
        {
            this->value[0] = col_type(1, 0, 0, 0);
            this->value[1] = col_type(0, 1, 0, 0);
            this->value[2] = col_type(0, 0, 1, 0);
            this->value[3] = col_type(x, y, z, 1);
            return *this;
        }

        template<typename U>
        tmat4x4<T>& translate(U x, U y, U z) noexcept
        {
            this->value[0] = col_type(1, 0, 0, 0);
            this->value[1] = col_type(0, 1, 0, 0);
            this->value[2] = col_type(0, 0, 1, 0);
            this->value[3] = col_type(T(x), T(y), T(z), 1);
            return *this;
        }

        tmat4x4<T>& translate(tvec3<T> const& pos) noexcept
        {
            this->value[0] = col_type(1, 0, 0, 0);
            this->value[1] = col_type(0, 1, 0, 0);
            this->value[2] = col_type(0, 0, 1, 0);
            this->value[3] = col_type(pos.x, pos.y, pos.z, 1);
            return *this;
        }

        template<typename U>
        tmat4x4<T>& translate(tvec3<U> const& pos) noexcept
        {
            this->value[0] = col_type(1, 0, 0, 0);
            this->value[1] = col_type(0, 1, 0, 0);
            this->value[2] = col_type(0, 0, 1, 0);
            this->value[3] = col_type(T(pos.x), T(pos.y), T(pos.z), 1);
            return *this;
        }

        tmat4x4<T>& rotate(value_type angle, tvec3<T> const& v) noexcept
        {
            T a = DEG2RAD(angle);
            T c = cos(a);
            T s = sin(a);

            tvec3<T> axis = normalize(v);

            tvec3<T> temp = (T(1) - c) * axis;

            tmat4x4<T> res;
            this->value[0][0] = c + temp[0] * axis[0];
            this->value[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
            this->value[0][2] = 0 + temp[0] * axis[2] - s * axis[1];
            this->value[0][3] = 0;

            this->value[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
            this->value[1][1] = c + temp[1] * axis[1];
            this->value[1][2] = 0 + temp[1] * axis[2] + s * axis[0];
            this->value[1][3] = 0;

            this->value[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
            this->value[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
            this->value[2][2] = c + temp[2] * axis[2];
            this->value[2][3] = 0;

            this->value[3][0] = 0;
            this->value[3][1] = 0;
            this->value[3][2] = 0;
            this->value[3][3] = 1;
            return *this;
        }

        tmat4x4<T>& rotateX(value_type angle) noexcept
        {
            T a = DEG2RAD(angle);
            T c = cos(a);
            T s = sin(a);

            this->value[0] = col_type(1, 0, 0, 0);
            this->value[1] = col_type(0, c, s, 0);
            this->value[2] = col_type(0, -s, c, 0);
            this->value[3] = col_type(0, 0, 0, 1);

            return *this;
        }

        template<typename U>
        tmat4x4<T>& rotateX(U angle) noexcept
        {
            T a = DEG2RAD(angle);
            T c = cos(a);
            T s = sin(a);

            this->value[0] = col_type(1, 0, 0, 0);
            this->value[1] = col_type(0, c, s, 0);
            this->value[2] = col_type(0, -s, c, 0);
            this->value[3] = col_type(0, 0, 0, 1);

            return *this;
        }

        tmat4x4<T>& rotateY(value_type angle) noexcept
        {
            T a = DEG2RAD(angle);
            T c = cos(a);
            T s = sin(a);

            this->value[0] = col_type(c, 0, -s, 0);
            this->value[1] = col_type(0, 1, 0, 0);
            this->value[2] = col_type(s, 0, c, 0);
            this->value[3] = col_type(0, 0, 0, 1);
            return *this;

        }
        template<typename U>
        tmat4x4<T>& rotateY(U angle) noexcept
        {
            T a = DEG2RAD(angle);
            T c = cos(a);
            T s = sin(a);

            this->value[0] = col_type(c, 0, -s, 0);
            this->value[1] = col_type(0, 1, 0, 0);
            this->value[2] = col_type(s, 0, c, 0);
            this->value[3] = col_type(0, 0, 0, 1);
            return *this;
        }

        tmat4x4<T>& rotateZ(value_type angle) noexcept
        {
            T a = T(DEG2RAD(angle));
            T c = cos(a);
            T s = sin(a);

            this->value[0] = col_type(c, s, 0, 0);
            this->value[1] = col_type(-s, c, 0, 0);
            this->value[2] = col_type(0, 0, 1, 0);
            this->value[3] = col_type(0, 0, 0, 1);
            return *this;
        }

        template<typename U>
        tmat4x4<T>& rotateZ(U angle) noexcept
        {
            T a = DEG2RAD(angle);

            T c = cos(a);
            T s = sin(a);

            this->value[0] = col_type(c, s, 0, 0);
            this->value[1] = col_type(-s, c, 0, 0);
            this->value[2] = col_type(0, 0, 1, 0);
            this->value[3] = col_type(0, 0, 0, 1);
            return *this;
        }


        tmat4x4<T> rotateXY(T angleX, T angleY) noexcept
        {
            T cosX = cos(DEG2RAD(angleX));
            T sinX = sin(DEG2RAD(angleX));
            T cosY = cos(DEG2RAD(angleY));
            T sinY = sin(DEG2RAD(angleY));

            this->value[0] = col_type(cosY, -sinX * sinY, cosX * sinY, 0);
            this->value[1] = col_type(0, cosX, sinX, 0);
            this->value[2] = col_type(-sinY, -sinX * cosY, cosX * cosY, 0);
            this->value[3] = col_type(0, 0, 0, 1);
            return *this;
        }

        tmat4x4<T> rotateYX(T angleX, T angleY) noexcept
        {
            T cosX = cos(DEG2RAD(angleX));
            T sinX = sin(DEG2RAD(angleX));
            T cosY = cos(DEG2RAD(angleY));
            T sinY = sin(DEG2RAD(angleY));

            this->value[0] = col_type(cosY, 0, sinY, 0);
            this->value[1] = col_type(-sinX * sinY, cosX, sinX * cosY, 0);
            this->value[2] = col_type(-cosX * sinY, -sinX, cosX * cosY, 0);
            this->value[3] = col_type(0, 0, 0, 1);

            return *this;
        }

        tmat4x4<T> rotateYXZ(T yaw, T pitch, T roll) noexcept
        {
            T tmp_ch = cos(DEG2RAD(yaw));
            T tmp_sh = sin(DEG2RAD(yaw));
            T tmp_cp = cos(DEG2RAD(pitch));
            T tmp_sp = sin(DEG2RAD(pitch));
            T tmp_cb = cos(DEG2RAD(roll));
            T tmp_sb = sin(DEG2RAD(roll));

            tmat4x4<T> Result;
            this->value[0][0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
            this->value[0][1] = tmp_sb * tmp_cp;
            this->value[0][2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
            this->value[0][3] = T(0);
            this->value[1][0] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
            this->value[1][1] = tmp_cb * tmp_cp;
            this->value[1][2] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
            this->value[1][3] = T(0);
            this->value[2][0] = tmp_sh * tmp_cp;
            this->value[2][1] = -tmp_sp;
            this->value[2][2] = tmp_ch * tmp_cp;
            this->value[2][3] = T(0);
            this->value[3][0] = T(0);
            this->value[3][1] = T(0);
            this->value[3][2] = T(0);
            this->value[3][3] = T(1);

            return *this;
        }

        tmat4x4<T> yawPitchRoll(T yaw, T pitch, T roll) noexcept
        {
            T tmp_ch = cos(DEG2RAD(yaw));
            T tmp_sh = sin(DEG2RAD(yaw));
            T tmp_cp = cos(DEG2RAD(pitch));
            T tmp_sp = sin(DEG2RAD(pitch));
            T tmp_cb = cos(DEG2RAD(roll));
            T tmp_sb = sin(DEG2RAD(roll));

            this->value[0][0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
            this->value[0][1] = tmp_sb * tmp_cp;
            this->value[0][2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
            this->value[0][3] = T(0);
            this->value[1][0] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
            this->value[1][1] = tmp_cb * tmp_cp;
            this->value[1][2] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
            this->value[1][3] = T(0);
            this->value[2][0] = tmp_sh * tmp_cp;
            this->value[2][1] = -tmp_sp;
            this->value[2][2] = tmp_ch * tmp_cp;
            this->value[2][3] = T(0);
            this->value[3][0] = T(0);
            this->value[3][1] = T(0);
            this->value[3][2] = T(0);
            this->value[3][3] = T(1);

            return *this;
        }

        tmat4x4<T>& scale(tvec3<T> const& s) noexcept
        {
            this->value[0] = col_type(s[0], 0, 0, 0);
            this->value[1] = col_type(0, s[1], 0, 0);
            this->value[2] = col_type(0, 0, s[2], 0);
            this->value[3] = col_type(0, 0, 0, 1);

            return *this;
        }

        tmat4x4<T>& scale(value_type x, value_type y, value_type z) noexcept
        {
            this->value[0] = col_type(x, 0, 0, 0);
            this->value[1] = col_type(0, y, 0, 0);
            this->value[2] = col_type(0, 0, z, 0);
            this->value[3] = col_type(0, 0, 0, 1);

            return *this;
        }

        template<typename U>
        tmat4x4<T>& scale(U x, U y, U z) noexcept
        {
            this->value[0] = col_type(value_type(x), 0, 0, 0);
            this->value[1] = col_type(0, value_type(y), 0, 0);
            this->value[2] = col_type(0, 0, value_type(z), 0);
            this->value[3] = col_type(0, 0, 0, 1);

            return *this;
        }

        template<typename U, typename V, typename W>
        tmat4x4<T>& scale(U x, V y, W z) noexcept
        {
            this->value[0] = col_type(value_type(x), 0, 0, 0);
            this->value[1] = col_type(0, value_type(y), 0, 0);
            this->value[2] = col_type(0, 0, value_type(z), 0);
            this->value[3] = col_type(0, 0, 0, 1);
            return *this;
        }

        tmat4x4<T> transpose() const noexcept
        {
            return tmat4x4<T>(
                this->value[0][0], this->value[1][0], this->value[2][0], this->value[3][0],
                this->value[0][1], this->value[1][1], this->value[2][1], this->value[3][1],
                this->value[0][2], this->value[1][2], this->value[2][2], this->value[3][2],
                this->value[0][3], this->value[1][3], this->value[2][3], this->value[3][3]
            );
        }

        tmat4x4<T> extractMatrixRotation() const noexcept
        {
            return tmat4x4<T>(
                this->value[0][0], this->value[0][1], this->value[0][2], 0.0,
                this->value[1][0], this->value[1][1], this->value[1][2], 0.0,
                this->value[2][0], this->value[2][1], this->value[2][2], 0.0,
                0.0, 0.0, 0.0, 1.0
            );
        }
    };

    template <typename T>
    tmat4x4<T> rotateX(T angleX) noexcept
    {
        T cosX = cos(DEG2RAD(angleX));
        T sinX = sin(DEG2RAD(angleX));

        return tmat4x4<T>(
            T(1), T(0), T(0), T(0),
            T(0), cosX, sinX, T(0),
            T(0), -sinX, cosX, T(0),
            T(0), T(0), T(0), T(1));
    }

    template <typename T>
    tmat4x4<T> rotateY(T angleY) noexcept
    {
        T cosY = cos(DEG2RAD(angleY));
        T sinY = sin(DEG2RAD(angleY));

        return tmat4x4<T>(
            cosY, T(0), sinY, T(0),
            T(0), T(1), T(0), T(0),
            -sinY, T(0), cosY, T(0),
            T(0), T(0), T(0), T(1));
    }

    template <typename T>
    tmat4x4<T> rotateZ(T angleZ) noexcept
    {
        T cosZ = (T)cos(DEG2RAD(angleZ));
        T sinZ = (T)sin(DEG2RAD(angleZ));

        return tmat4x4<T>(
            cosZ, sinZ, T(0), T(0),
            -sinZ, cosZ, T(0), T(0),
            T(0), T(0), T(1), T(0),
            T(0), T(0), T(0), T(1));
    }

    template <typename T>
    tmat4x4<T> rotateXY(T angleX, T angleY) noexcept
    {
        T cosX = cos(DEG2RAD(angleX));
        T sinX = sin(DEG2RAD(angleX));
        T cosY = cos(DEG2RAD(angleY));
        T sinY = sin(DEG2RAD(angleY));

        return tmat4x4<T>(
            cosY, -sinX * sinY, cosX * sinY, T(0),
            T(0), cosX, sinX, T(0),
            -sinY, -sinX * cosY, cosX * cosY, T(0),
            T(0), T(0), T(0), T(1));
    }

    template <typename T>
    tmat4x4<T> rotateYX(T angleY, T angleX) noexcept
    {

        T cosX = cos(DEG2RAD(angleX));
        T sinX = sin(DEG2RAD(angleX));
        T cosY = cos(DEG2RAD(angleY));
        T sinY = sin(DEG2RAD(angleY));

        return tmat4x4<T>(
            cosY, T(0), sinY, T(0),
            -sinX * sinY, cosX, sinX * cosY, T(0),
            -cosX * sinY, -sinX, cosX * cosY, T(0),
            T(0), T(0), T(0), T(1));
    }

    template <typename T>
    tmat4x4<T> rotateXZ(T angleX, T angleZ) noexcept
    {
        return rotateX(angleX) * rotateZ(angleZ);
    }

    template <typename T>
    tmat4x4<T> rotateZX(T angleX, T angleZ) noexcept
    {
        return rotateZ(angleZ) * rotateX(angleX);
    }

    template <typename T>
    tmat4x4<T> rotateYXZ(T yaw, T pitch, T roll) noexcept
    {
        T tmp_ch = cos(DEG2RAD(yaw));
        T tmp_sh = sin(DEG2RAD(yaw));
        T tmp_cp = cos(DEG2RAD(pitch));
        T tmp_sp = sin(DEG2RAD(pitch));
        T tmp_cb = cos(DEG2RAD(roll));
        T tmp_sb = sin(DEG2RAD(roll));

        tmat4x4<T> res;
        res[0][0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
        res[0][1] = tmp_sb * tmp_cp;
        res[0][2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
        res[0][3] = T(0);
        res[1][0] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
        res[1][1] = tmp_cb * tmp_cp;
        res[1][2] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
        res[1][3] = T(0);
        res[2][0] = tmp_sh * tmp_cp;
        res[2][1] = -tmp_sp;
        res[2][2] = tmp_ch * tmp_cp;
        res[2][3] = T(0);
        res[3][0] = T(0);
        res[3][1] = T(0);
        res[3][2] = T(0);
        res[3][3] = T(1);
        return res;
    }

    template <typename T>
    tmat4x4<T> yawPitchRoll(T yaw, T pitch, T roll) noexcept
    {
        T tmp_ch = cos(DEG2RAD(yaw));
        T tmp_sh = sin(DEG2RAD(yaw));
        T tmp_cp = cos(DEG2RAD(pitch));
        T tmp_sp = sin(DEG2RAD(pitch));
        T tmp_cb = cos(DEG2RAD(roll));
        T tmp_sb = sin(DEG2RAD(roll));


        tmat4x4<T> res;
        res[0][0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
        res[0][1] = tmp_sb * tmp_cp;
        res[0][2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
        res[0][3] = T(0);
        res[1][0] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
        res[1][1] = tmp_cb * tmp_cp;
        res[1][2] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
        res[1][3] = T(0);
        res[2][0] = tmp_sh * tmp_cp;
        res[2][1] = -tmp_sp;
        res[2][2] = tmp_ch * tmp_cp;
        res[2][3] = T(0);
        res[3][0] = T(0);
        res[3][1] = T(0);
        res[3][2] = T(0);
        res[3][3] = T(1);
        return res;
    }

    template <typename T>
    tvec3<T> operator*(tvec3<T> const& v, tmat4x4<T> const& mat) noexcept
    {
        return tvec3<T>
            (
                v.x * mat[0][0] + v.y * mat[1][0] + v.z * mat[2][0] + 1 * mat[3][0],
                v.x * mat[0][1] + v.y * mat[1][1] + v.z * mat[2][1] + 1 * mat[3][1],
                v.x * mat[0][2] + v.y * mat[1][2] + v.z * mat[2][2] + 1 * mat[3][2]
            );
    }

    template <typename T>
    tmat4x4<T> operator+(tmat4x4<T> const& m, typename tmat4x4<T>::value_type s) noexcept
    {
        return tmat4x4<T>(
            m[0] + s,
            m[1] + s,
            m[2] + s,
            m[3] + s);
    }

    template <typename T>
    tmat4x4<T> operator+(typename tmat4x4<T>::value_type s, tmat4x4<T> const& m) noexcept
    {
        return tmat4x4<T>(
            m[0] + s,
            m[1] + s,
            m[2] + s,
            m[3] + s);
    }

    template <typename T>
    tmat4x4<T> operator+(tmat4x4<T> const& m1, tmat4x4<T> const& m2) noexcept
    {
        return tmat4x4<T>(
            m1[0] + m2[0],
            m1[1] + m2[1],
            m1[2] + m2[2],
            m1[3] + m2[3]);
    }

    template <typename T>
    tmat4x4<T> operator-(tmat4x4<T> const& m, typename tmat4x4<T>::value_type s) noexcept
    {
        return tmat4x4<T>(
            m[0] - s,
            m[1] - s,
            m[2] - s,
            m[3] - s);
    }

    template <typename T>
    tmat4x4<T> operator-(typename tmat4x4<T>::value_type s, tmat4x4<T> const& m) noexcept
    {
        return tmat4x4<T>(
            s - m[0],
            s - m[1],
            s - m[2],
            s - m[3]);
    }

    template <typename T>
    tmat4x4<T> operator-(tmat4x4<T> const& m1, tmat4x4<T> const& m2) noexcept
    {
        return tmat4x4<T>(
            m1[0] - m2[0],
            m1[1] - m2[1],
            m1[2] - m2[2],
            m1[3] - m2[3]);
    }

    template <typename T>
    tmat4x4<T> operator*(tmat4x4<T> const& m, typename tmat4x4<T>::value_type s) noexcept
    {
        return tmat4x4<T>(
            m[0] * s,
            m[1] * s,
            m[2] * s,
            m[3] * s);
    }

    template <typename T>
    tmat4x4<T> operator*(typename tmat4x4<T>::value_type s, tmat4x4<T> const& m) noexcept
    {
        return tmat4x4<T>(
            m[0] * s,
            m[1] * s,
            m[2] * s,
            m[3] * s);
    }

    template <typename T>
    typename tmat4x4<T>::col_type operator*(tmat4x4<T> const& m, typename tmat4x4<T>::row_type const& v) noexcept
    {
        return typename tmat4x4<T>::col_type(
            m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
            m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
            m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
            m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w);
    }

    template <typename T>
    typename tmat4x4<T>::row_type operator*(typename tmat4x4<T>::col_type const& v, tmat4x4<T> const& m) noexcept
    {
        return typename tmat4x4<T>::row_type(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
            m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w);
    }

    template <typename T>
    tmat4x4<T> operator*(tmat4x4<T> const& m1, tmat4x4<T> const& m2) noexcept
    {
        typename tmat4x4<T>::col_type const srcA0 = m1[0];
        typename tmat4x4<T>::col_type const srcA1 = m1[1];
        typename tmat4x4<T>::col_type const srcA2 = m1[2];
        typename tmat4x4<T>::col_type const srcA3 = m1[3];

        typename tmat4x4<T>::col_type const srcB0 = m2[0];
        typename tmat4x4<T>::col_type const srcB1 = m2[1];
        typename tmat4x4<T>::col_type const srcB2 = m2[2];
        typename tmat4x4<T>::col_type const srcB3 = m2[3];

        tmat4x4<T> res;
        res[0] = srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3];
        res[1] = srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3];
        res[2] = srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3];
        res[3] = srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3];
        return res;
    }

    template <typename T>
    tmat4x4<T> operator/(tmat4x4<T> const& m, typename tmat4x4<T>::value_type s) noexcept
    {
        return tmat4x4<T>(
            m[0] / s,
            m[1] / s,
            m[2] / s,
            m[3] / s);
    }

    template <typename T>
    tmat4x4<T> operator/ (typename tmat4x4<T>::value_type s, tmat4x4<T> const& m) noexcept
    {
        return tmat4x4<T>(
            s / m[0],
            s / m[1],
            s / m[2],
            s / m[3]);
    }

    template <typename T>
    typename tmat4x4<T>::col_type operator/(tmat4x4<T> const& m, typename tmat4x4<T>::row_type const& v) noexcept
    {
        return m.inverse() * v;
    }

    template <typename T>
    typename tmat4x4<T>::row_type operator/(typename tmat4x4<T>::col_type const& v, tmat4x4<T> const& m) noexcept
    {
        return v * m.inverse();
    }

    template <typename T>
    tmat4x4<T> operator/(tmat4x4<T> const& m1, tmat4x4<T> const& m2) noexcept
    {
        return m1 * m2.inverse();
    }

    template <typename T>
    tmat4x4<T> const operator-(tmat4x4<T> const& m) noexcept
    {
        return tmat4x4<T>(
            -m[0],
            -m[1],
            -m[2],
            -m[3]);
    }

    template <typename T>
    tmat4x4<T> const operator++(tmat4x4<T> const& m, int) noexcept
    {
        return tmat4x4<T>(
            m[0] + T(1),
            m[1] + T(1),
            m[2] + T(1),
            m[3] + T(1));
    }

    template <typename T>
    tmat4x4<T> const operator--(tmat4x4<T> const& m, int) noexcept
    {
        return tmat4x4<T>(
            m[0] - T(1),
            m[1] - T(1),
            m[2] - T(1),
            m[3] - T(1));
    }

    template <typename T>
    bool operator==(tmat4x4<T> const& m1, tmat4x4<T> const& m2) noexcept
    {
        return (m1[0] == m2[0]) && (m1[1] == m2[1]) && (m1[2] == m2[2]) && (m1[3] == m2[3]);
    }

    template <typename T>
    bool operator!=(tmat4x4<T> const& m1, tmat4x4<T> const& m2) noexcept
    {
        return (m1[0] != m2[0]) || (m1[1] != m2[1]) || (m1[2] != m2[2]) || (m1[3] != m2[3]);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T>
    typename tvec2<T>::value_type length(tvec2<T> const& v) noexcept
    {
        typename tvec2<T>::value_type sqr = v.x * v.x + v.y * v.y;
        return sqrt(sqr);
    }

    template <typename T>
    typename tvec3<T>::value_type length(tvec3<T> const& v) noexcept
    {
        typename tvec3<T>::value_type sqr = v.x * v.x + v.y * v.y + v.z * v.z;
        return sqrt(sqr);
    }

    template <typename T>
    typename tvec4<T>::value_type length(tvec4<T> const& v) noexcept
    {
        typename tvec4<T>::value_type sqr = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
        return sqrt(sqr);
    }

    template <typename T>
    typename tvec2<T>::value_type distance(tvec2<T> const& p0, tvec2<T> const& p1) noexcept
    {
        return length(p1 - p0);
    }

    template <typename T>
    typename tvec3<T>::value_type distance(tvec3<T> const& p0, tvec3<T> const& p1) noexcept
    {
        return length(p1 - p0);
    }

    template <typename T>
    typename tvec4<T>::value_type distance(tvec4<T> const& p0, tvec4<T> const& p1) noexcept
    {
        return length(p1 - p0);
    }

    template <typename T>
    typename tvec2<T>::value_type dot(tvec2<T> const& x, tvec2<T> const& y) noexcept
    {
        return x.x * y.x + x.y * y.y;
    }

    template <typename T>
    typename tvec3<T>::value_type dot(tvec3<T> const& x, tvec3<T> const& y) noexcept
    {
        return x.x * y.x + x.y * y.y + x.z * y.z;
    }

    template <typename T>
    typename tvec4<T>::value_type dot(tvec4<T> const& x, tvec4<T> const& y) noexcept
    {
        return x.x * y.x + x.y * y.y + x.z * y.z + x.w * y.w;
    }

    template <typename T>
    tvec3<T> cross(tvec3<T> const& x, tvec3<T> const& y) noexcept
    {
        return tvec3<T>
            (
                x.y * y.z - y.y * x.z,
                x.z * y.x - y.z * x.x,
                x.x * y.y - y.x * x.y
            );
    }

    template <typename T>
    T inversesqrt(T x) noexcept
    {
        return T(1) / sqrt(x);
    }

    template <typename T>
    tvec2<T> normalize(tvec2<T> const& x) noexcept
    {
        typename tvec2<T>::value_type sqr = x.x * x.x + x.y * x.y;
        return x * inversesqrt(sqr);
    }

    template <typename T>
    tvec3<T> normalize(tvec3<T> const& x) noexcept
    {
        typename tvec3<T>::value_type sqr = x.x * x.x + x.y * x.y + x.z * x.z;
        return x * inversesqrt(sqr);
    }

    template <typename T>
    tvec4<T> normalize(tvec4<T> const& x) noexcept
    {
        typename tvec4<T>::value_type sqr = x.x * x.x + x.y * x.y + x.z * x.z + x.w * x.w;
        return x * inversesqrt(sqr);
    }

    template <typename valType>
    tmat4x4<valType> perspective(valType fovy, valType aspect, valType zNear, valType zFar) noexcept
    {
        valType range = tan(fovy * valType(DEG2RAD(0.5))) * zNear;
        valType left = -range * aspect;
        valType right = range * aspect;
        valType bottom = -range;
        valType top = range;

        tmat4x4<valType> res(valType(0));
        res[0][0] = (valType(2) * zNear) / (right - left);
        res[1][1] = (valType(2) * zNear) / (top - bottom);
        res[2][2] = -(zFar + zNear) / (zFar - zNear);
        res[2][3] = -valType(1);
        res[3][2] = -(valType(2) * zFar * zNear) / (zFar - zNear);
        return  res;
    }
    
    template <typename T>
    tmat4x4<T> lookAt
    (
        tvec3<T> const& eye,
        tvec3<T> const& center,
        tvec3<T> const& up
    ) noexcept
    {
        tvec3<T> f = normalize(center - eye);
        tvec3<T> u = normalize(up);
        tvec3<T> s = normalize(cross(f, u));
        u = cross(s, f);

        tmat4x4<T>    res(1);
        res[0][0] = s.x;
        res[1][0] = s.y;
        res[2][0] = s.z;
        res[0][1] = u.x;
        res[1][1] = u.y;
        res[2][1] = u.z;
        res[0][2] = -f.x;
        res[1][2] = -f.y;
        res[2][2] = -f.z;
        res[3][0] = -dot(s, eye);
        res[3][1] = -dot(u, eye);
        res[3][2] = dot(f, eye);
        return res;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    template<class T>
    class Plane
    {
    public:
        /*
		* 平面已知点P0，平面法向量N，就可以确定一个平面
        * 平面的任一点P，，必然有
		* N-> · (P-> - P0->) = 0 ==》 N-> · P-> - N-> · P0-> = 0
		* N-> · P-> + D = 0
        * D = -(N-> · P0->) ==> D = -(||N->|| · ||P0->|| · cos)
        * 
		* 如果N->是单位向量，则D = -(||P0->|| · cos)
		* D如果小于0，原点在法线指向相反的一侧
		* D如果大于0，原点在法线指向的一侧
		* 
		* 同样上述描述，平面外的一点P，求P到平面的距离
        * 距离 = ||P-> - P0->|| · cos((P-> - P0->)于法向量的夹角)
		* 根据内积的定义可知
		* (P-> - P0->) * N-> = ||P-> - P0->|| · ||N->|| · cos((P-> - P0->)于法向量的夹角)
		* cos((P-> - P0->)于法向量的夹角) = (P-> - P0->) * N-> / ||P-> - P0->|| · ||N->||
		* 带入上面的距离公式
        * 距离 = ||P-> - P0->|| · (P-> - P0->) * N-> / ||P-> - P0->|| · ||N->||
		* 距离 = (P-> - P0->) * N-> / ||N->||
        * 
        * 如果N->是单位向量
        * 距离 = (P-> - P0->) * N->
		* 距离 = P-> * N-> - P0-> * N->
		* 距离 = P-> * N-> + D
        * 
		* 距离如果小于0，说明P在平面的法线指向的相反一侧
		* 距离如果大于0，说明P在平面的法线指向的一侧
        */

		// 平面的单位法向量
        tvec3<T> _normal;
		// 平面和原点的距离
        T _distance;

    public:
        Plane() noexcept
        {
            _normal = tvec3<T>(0, 0, 0);
            _distance = 0.0f;
        }

        Plane(const Plane& right) noexcept
        {
            _normal = right._normal;
            _distance = right._distance;
        }

        Plane(const tvec3<T>& rkNormal, T fConstant) noexcept
        {
            _normal = rkNormal;
			_distance = fConstant;
        }

        Plane(T x, T y, T z, T o) noexcept
        {
            _normal = tvec3<T>(x, y, z);
            T invLen = 1.0f / (_normal).lengthf();
            _normal *= invLen;
            _distance = o * invLen;
        }

        enum class Side
        {
            NO_SIDE,
            POSITIVE_SIDE,
            NEGATIVE_SIDE,
        };

        Side getSide(const tvec3<T>& rkPoint) const noexcept
        {
            float fDistance = getDistance(rkPoint);

            if (fDistance < 0.0)
                return Plane::Side::NEGATIVE_SIDE;

            if (fDistance > 0.0)
                return Plane::Side::POSITIVE_SIDE;

            return Plane::Side::NO_SIDE;
        }

        float getDistance(const tvec3<T>& rkPoint) const noexcept
        {
			return dot(_normal, rkPoint) + _distance;
        }
        
        bool operator==(const Plane& right) const noexcept
        {
            return (right._distance == _distance && right._normal == _normal);
        }

        bool operator!=(const Plane& right) const noexcept
        {
            return (right._distance != _distance || right._normal != _normal);
        }
    };

    // 视椎体
    template<class T>
    class tfrustum
    {
    public:
        enum class FRUSTUM_TYPE
        {
            FRUSTUM_LEFT = 0,
            FRUSTUM_RIGHT = 1,
            FRUSTUM_TOP = 2,
            FRUSTUM_BOTTOM = 3,
            FRUSTUM_FAR = 4,
            FRUSTUM_NEAR = 5,
        };

    public:

		// 从 MVP 矩阵中加载视椎体，行主序
        void loadFrustum(const tmat4x4<T>& mvp) noexcept
        {
            const T* dataPtr = mvp.data();
            _planes[(uint)FRUSTUM_TYPE::FRUSTUM_LEFT] = Plane<T>(dataPtr[12] - dataPtr[0], dataPtr[13] - dataPtr[1], dataPtr[14] - dataPtr[2], dataPtr[15] - dataPtr[3]);
            _planes[(uint)FRUSTUM_TYPE::FRUSTUM_RIGHT] = Plane<T>(dataPtr[12] + dataPtr[0], dataPtr[13] + dataPtr[1], dataPtr[14] + dataPtr[2], dataPtr[15] + dataPtr[3]);

            _planes[(uint)FRUSTUM_TYPE::FRUSTUM_TOP] = Plane<T>(dataPtr[12] - dataPtr[4], dataPtr[13] - dataPtr[5], dataPtr[14] - dataPtr[6], dataPtr[15] - dataPtr[7]);
            _planes[(uint)FRUSTUM_TYPE::FRUSTUM_BOTTOM] = Plane<T>(dataPtr[12] + dataPtr[4], dataPtr[13] + dataPtr[5], dataPtr[14] + dataPtr[6], dataPtr[15] + dataPtr[7]);

            _planes[(uint)FRUSTUM_TYPE::FRUSTUM_FAR] = Plane<T>(dataPtr[12] - dataPtr[8], dataPtr[13] - dataPtr[9], dataPtr[14] - dataPtr[10], dataPtr[15] - dataPtr[11]);
            _planes[(uint)FRUSTUM_TYPE::FRUSTUM_NEAR] = Plane<T>(dataPtr[12] + dataPtr[8], dataPtr[13] + dataPtr[9], dataPtr[14] + dataPtr[10], dataPtr[15] + dataPtr[11]);
        }

        bool pointInFrustum(const tvec3<T>& pos) const noexcept
        {
            for (int i = 0; i < 6; i++)
            {
                if (_planes[i].getDistance(pos) <= 0)
                    return false;
            }
            return true;
        }

        const Plane<T>& getPlane(const int plane) const noexcept
        {
            return _planes[plane];
        }

    protected:
        // 6个面的方程
        Plane<T> _planes[6];
    };

    // 射线
    template<typename T>
    struct tray
    {
        using value_type = T;
        using type = tray<T>;

    protected:
        /*
        * 射线方程：
        * R(t) = _origin + t * _direction
        * t是到_origin的距离，是一个标量
        */

        // 射线的起点
        tvec3<T> _origin;
        // 射线的方向向量，通常这个向量是归一化的，即长度为1
        tvec3<T> _direction;

    public:
        tray() noexcept 
            : _origin(value_type(0), value_type(0), value_type(0)),
            _direction(value_type(0), value_type(0), value_type(1)) {
        }

        tray(const tvec3<T>& origin, const tvec3<T>& direction) noexcept 
            : _origin(origin), _direction(direction) {
        }

        void setOrigin(const tvec3<T>& origin) noexcept
        {
            _origin = origin;
        }

        const tvec3<T>& getOrigin(void) const noexcept
        {
            return _origin;
        }

        void setDirection(const tvec3<T>& dir) noexcept
        {
            _direction = dir;
        }

        const tvec3<T>& getDirection(void) const noexcept
        {
            return _direction;
        }

        tvec3<T> getPoint(T time) const noexcept
        {
            return tvec3<T>(_origin + (_direction * time));
        }
    };
}