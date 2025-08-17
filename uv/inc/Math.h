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
    using byte = unsigned char;
    using uint = uint32_t;
    template <typename T>
    struct tmat3x3;
    using matrix3 = tmat3x3<float>;

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
        using type =  tvec3<T>;

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
    tvec3<T> operator* (T const& s, tvec3<T> const& v) noexcept
    {
        return tvec3<T>(
            T(s) * v.x,
            T(s) * v.y,
            T(s) * v.z);
    }

    template <typename T>
    tvec3<T> operator* (tvec3<T> const& v1, tvec3<T> const& v2) noexcept
    {
        return tvec3<T>(
            v1.x * T(v2.x),
            v1.y * T(v2.y),
            v1.z * T(v2.z));
    }

    template <typename T>
    tvec3<T> operator/ (tvec3<T> const& v, T const& s) noexcept
    {
        return tvec3<T>(
            v.x / T(s),
            v.y / T(s),
            v.z / T(s));
    }

    template <typename T>
    tvec3<T> operator/ (T const& s, tvec3<T> const& v) noexcept
    {
        return tvec3<T>(
            T(s) / v.x,
            T(s) / v.y,
            T(s) / v.z);
    }

    template <typename T>
    tvec3<T> operator/ (tvec3<T> const& v1, tvec3<T> const& v2) noexcept
    {
        return tvec3<T>(
            v1.x / T(v2.x),
            v1.y / T(v2.y),
            v1.z / T(v2.z));
    }

    template <typename T>
    tvec3<T> operator- (tvec3<T> const& v) noexcept
    {
        return tvec3<T>(
            -v.x,
            -v.y,
            -v.z);
    }

    template <typename T>
    tvec3<T> operator++ (tvec3<T> const& v, int) noexcept
    {
        return tvec3<T>(
            v.x + T(1),
            v.y + T(1),
            v.z + T(1));
    }

    template <typename T>
    tvec3<T> operator-- (tvec3<T> const& v, int) noexcept
    {
        return tvec3<T>(
            v.x - T(1),
            v.y - T(1),
            v.z - T(1));
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
}