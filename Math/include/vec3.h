#pragma once
#include <iostream>

namespace cgmath {
    class vec3 {
        public:
            float x, y, z;
            
            vec3(): x(), y(), z() { }
            vec3(float x, float y, float z): x(x), y(y), z(z) { }

            
            float& operator[](int);
            const float& operator[](int) const;

            vec3& operator*=(float);

            vec3& operator/=(float);
            
            vec3& operator+=(const vec3&);

            vec3& operator-=(const vec3&);

            bool operator==(const vec3&) const;

            float magnitude() const;

            void normalize();

            static float magnitude(const vec3&);
            static vec3 normalize(const vec3&);
			static float dot(const vec3&, const vec3&);
			static vec3 cross(const vec3&, const vec3&);


            friend std::ostream& operator<<(std::ostream&, const vec3&);
    };

    inline vec3 operator*(const vec3& v, float t) { return vec3( t * v.x, t * v.y, t * v.z );  }
    inline vec3 operator*(float t, const vec3& v) { return vec3( t * v.x, t * v.y, t * v.z );  }
    inline vec3 operator/(const vec3& v, float t) { return vec3( v.x / t, v.y / t, v.z / t );  }
    inline vec3 operator+(const vec3& u, const vec3& v) { return vec3( u.x + v.x, u.y + v.y, u.z + v.z );  }
    inline vec3 operator-(const vec3& u, const vec3& v) { return vec3( u.x - v.x, u.y - v.y, u.z - v.z );  }
    inline vec3 operator-(const vec3& v) { return vec3( -v.x, -v.y, -v.z );  }
}