#include <math.h>
#include "vec3.h"

namespace cgmath {
    // members
    float& vec3::operator[](int i) { return (&x)[i]; }
    const float& vec3::operator[](int i) const { return (&x)[i]; }

    vec3& vec3::operator*=(float s) {
        x *= s; y *= s; z *= s;
        return *this;
    }

    vec3& vec3::operator/=(float s) {
        x /= s; y /= s; z /= s;
        return *this;
    }

    vec3& vec3::operator+=(const vec3& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    vec3& vec3::operator-=(const vec3& v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    bool vec3::operator==(const vec3& v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    float vec3::magnitude() const {
        return sqrt(x*x + y*y + z*z);
    }

    void vec3::normalize() {
        float mag = this->magnitude(); 
        x /= mag;
        y /= mag;
        z /= mag;
    }

    // statics
    float vec3::magnitude(const vec3& v) {
        return v.magnitude();
    }

    vec3 vec3::normalize(const vec3& v) {
        vec3 normalized = vec3(v);
        normalized.normalize();
        return normalized;
    }

    float vec3::dot(const vec3& u, const vec3& v) {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

	vec3 vec3::cross(const vec3& u, const vec3& v) {
		return vec3(
			u[1] * v[2] - u[2] * v[1],
			u[2] * v[0] - u[0] * v[2],
			u[0] * v[1] - u[1] * v[0]
		);
	}

    // friends
    std::ostream& operator<<(std::ostream& o, const vec3& v) {
        return o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
};