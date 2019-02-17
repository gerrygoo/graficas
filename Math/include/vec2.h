#pragma once
#include <iostream>

namespace cgmath {
    class vec2 {
        public:
            float x, y;
            
            vec2(): x(), y(){ }
            vec2(float x, float y): x(x), y(y) { }

            
            float& operator[](int);
            const float& operator[](int) const;

            vec2& operator*=(float);

            vec2& operator/=(float);
            
            vec2& operator+=(const vec2&);

            vec2& operator-=(const vec2&);

            bool operator==(const vec2&) const;

            float magnitude() const;

            void normalize();

            static float magnitude(const vec2&);
            static vec2 normalize(const vec2&);
            static float dot(const vec2&, const vec2&);


            friend std::ostream& operator<<(std::ostream&, const vec2&);
    };

    inline vec2 operator*(const vec2& v, float t) { return vec2( t * v.x, t * v.y );  }
    inline vec2 operator*(float t, const vec2& v) { return vec2( t * v.x, t * v.y );  }
    inline vec2 operator/(const vec2& v, float t) { return vec2( v.x / t, v.y / t );  }
    inline vec2 operator+(const vec2& u, const vec2& v) { return vec2( u.x + v.x, u.y + v.y );  }
    inline vec2 operator-(const vec2& u, const vec2& v) { return vec2( u.x - v.x, u.y - v.y );  }
    inline vec2 operator-(const vec2& v) { return vec2( -v.x, -v.y );  }
}