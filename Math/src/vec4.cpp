#include "vec4.h"
#include <math.h>

namespace cgmath{
    float& vec4::operator[](int i) { return (&x)[i]; }
    const float& vec4::operator[](int i) const { return (&x)[i]; }

    vec4& vec4::operator*=(float s) {
        x *= s; y *= s; z *= s; w *= s;
        return *this;
    }

    vec4& vec4::operator/=(float s) {
        x /= s; y /= s; z /= s; w /= s;
        return *this;
    }
    
    vec4& vec4::operator+=(const vec4& v) {
        x += v.x; y += v.y; z += v.z; w += v.w;
        return *this;
    }

    vec4& vec4::operator-=(const vec4& v) {
        x -= v.x; y -= v.y; z -= v.z; w -= v.w;
        return *this;
    }

    bool vec4::operator==(const vec4& v) const {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    float vec4::magnitude() const {
        return sqrt(x*x + y*y + z*z + w*w);
    }

    void vec4::normalize() {
        float mag = this->magnitude(); 
        x /= mag;
        y /= mag;
        z /= mag;
        w /= mag;
    }
    
    // friend
    std::ostream& operator<<(std::ostream& o, const vec4& v) {
        return o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    }
}