#pragma once
#include <iostream>
#include "vec3.h"

namespace cgmath {
    class mat3 {
        private:
			float n[3][3] = {0};

        public:
			mat3() { }


			mat3(float diagonal) { n[0][0] = n[1][1] = n[2][2] = diagonal; }
			mat3(const vec3& i, const vec3& j, const vec3& k): n{ {i.x, i.y, i.z}, {j.x, j.y, j.z}, {k.x, k.y, k.z} } { }

            vec3& operator[](int); // write
            const vec3& operator[](int) const; // access
            bool operator==(const mat3&) const;

            static float determinant(const mat3&);
            static mat3 inverse(const mat3&);
            static mat3 transpose(const mat3&);

            friend std::ostream& operator<<(std::ostream&, const mat3&);
    };

    inline vec3  operator*(const mat3& m, const vec3& v) { return m[0] * v.x + m[1] * v.y + m[2] * v.z; }
    inline mat3  operator*(const mat3& m, const mat3& n) { return mat3( m * n[0], m * n[1], m * n[2] ); }
}