#include "vec2.h"
#include <math.h>

namespace cgmath {
    // members
    float& vec2::operator[](int i) { return (&x)[i]; }
    const float& vec2::operator[](int i) const { return (&x)[i]; }

    vec2& vec2::operator*=(float s) {
        x *= s; y *= s;
        return *this;
    }

    vec2& vec2::operator/=(float s) {
        x /= s; y /= s;
        return *this;
    }

    vec2& vec2::operator+=(const vec2& v) {
        x += v.x; y += v.y;
        return *this;
    }

    vec2& vec2::operator-=(const vec2& v) {
        x -= v.x; y -= v.y;
        return *this;
    }

    bool vec2::operator==(const vec2& v) const {
        return x == v.x && y == v.y;
    }

    float vec2::magnitude() const {
        return sqrt(x*x + y*y);
    }

    void vec2::normalize() {
        float mag = this->magnitude(); 
        x /= mag;
        y /= mag;
    }

    // statics
    float vec2::magnitude(const vec2& v) {
        return v.magnitude();
    }

    vec2 vec2::normalize(const vec2& v) {
        vec2 normalized = vec2(v);
        normalized.normalize();
        return normalized;
    }

    float vec2::dot(const vec2& u, const vec2& v) {
        return u.x * v.x + u.y * v.y;
    }

    // friends
    std::ostream &operator<<(std::ostream& o, const vec2& v) {
        return o << "(" << v.x << ", " << v.y <<  ")";
    }
};