#include "mat3.h"

namespace cgmath {
    

    vec3& mat3::operator[](int i) { return reinterpret_cast<vec3&>(n[i]); }
    const vec3& mat3::operator[](int i) const { return reinterpret_cast<const vec3&>(n[i]); }
    
    bool mat3::operator==(const mat3& m) const {
        return this->operator[](0) == m[0] && this->operator[](1) == m[1] && this->operator[](2) == m[2];
    };

    float mat3::determinant(const mat3& m) {
        return (
            m[0][0] * (m[1][1]*m[2][2] - m[2][1]*m[1][2]) -
            m[1][0] * (m[0][1]*m[2][2] - m[2][1]*m[0][2]) +
            m[2][0] * (m[0][1]*m[1][2] - m[1][1]*m[0][2])
        );
    }

    mat3 mat3::inverse(const mat3& m) {
        mat3 original(m);
        mat3 inverse(1.0f);

        for (int i = 0; i < 3; i++ ) {
            float a = original[i][i];
			original[i] /= a;
			inverse[i] /= a;

            //zero up
            for (int j = i - 1; j >= 0; j--) {
                float b = original[j][i];
				original[j] += original[i] * -b;
				inverse[j] += inverse[i] * -b;
            }

            //zero down
            for (int j = i + 1; j < 3; j++) {
                float b = original[j][i];
				original[j] += original[i] * -b;
				inverse[j] += inverse[i] * -b;
            }
        }

        return inverse;
    }

    mat3 mat3::transpose(const mat3& m) {
        return mat3(
            vec3(m[0].x, m[1].x, m[2].x),
            vec3(m[0].y, m[1].y, m[2].y),
            vec3(m[0].z, m[1].z, m[2].z)
        );
    }

    std::ostream& operator<<(std::ostream& o, const mat3& m) {
        return o 
			<< m[0][0] << " " << m[1][0] << " " << m[2][0] << '\n'
			<< m[0][1] << " " << m[1][1] << " " << m[2][1] << '\n'
            << m[0][2] << " " << m[1][2] << " " << m[2][2];
    }
}