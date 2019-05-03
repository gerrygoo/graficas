#pragma once
#include <iostream>

namespace cgmath {
    class vec4 {
        public:
            float x, y, z, w;

            vec4(): x(), y(), z(), w() { }
            vec4(float x, float y, float z, float w): x(x), y(y), z(z), w(w) { }


            float& operator[](int);
            const float& operator[](int) const;

            vec4& operator*=(float);

            vec4& operator/=(float);

            vec4& operator+=(const vec4&);

            vec4& operator-=(const vec4&);

            bool operator==(const vec4&) const;

            float magnitude() const;

            void normalize();


            friend std::ostream& operator<<(std::ostream&, const vec4&);
    };


    inline vec4 operator*(const vec4& u, const vec4& v) { return vec4( u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w );  }
    inline vec4 operator*(const vec4& v, float t) { return vec4( t * v.x, t * v.y, t * v.z, t * v.w );  }
    inline vec4 operator*(float t, const vec4& v) { return vec4( t * v.x, t * v.y, t * v.z, t * v.w );  }
    inline vec4 operator/(const vec4& v, float t) { return vec4( v.x / t, v.y / t, v.z / t, v.w / t );  }
    inline vec4 operator+(const vec4& u, const vec4& v) { return vec4( u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w );  }
    inline vec4 operator-(const vec4& u, const vec4& v) { return vec4( u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w );  }
    inline vec4 operator-(const vec4& v) { return vec4( -v.x, -v.y, -v.z, -v.w );  }
}