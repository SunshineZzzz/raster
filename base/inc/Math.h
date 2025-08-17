#pragma once

#include <float.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

namespace szmath
{
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
    using byte = unsigned char;

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

        value_type x = T{0};
        value_type y = T{0};

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

        tvec2<T>& operator= (tvec2<T> const& v) noexcept
        {
            if (*this == v) {
                return *this;
            }

            this->x = v.x;
            this->y = v.y;
            return *this;
        }
        template <typename U>
        tvec2<T>& operator= (tvec2<U> const& v) noexcept
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
}