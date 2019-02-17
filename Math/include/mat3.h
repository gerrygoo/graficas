// #include "vec3.h"
#include "./vec3.h"
#include <iostream>

namespace cgmath {
    class mat3 {
        private:
            float n[3][3] = {0};

        public:
            mat3() {}
            mat3(float);
            mat3(const vec3&, const vec3&, const vec3&);

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