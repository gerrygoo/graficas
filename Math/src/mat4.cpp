#include "mat4.h"

namespace cgmath {
	vec4& mat4::operator[](int i) { return reinterpret_cast<vec4&>(n[i]); }
	const vec4& mat4::operator[](int i) const { return reinterpret_cast<const vec4&>(n[i]); }

	bool mat4::operator==(const mat4& m) const {
		return this->operator[](0) == m[0] && this->operator[](1) == m[1] && this->operator[](3) == m[3];
	};

	mat4 mat4::inverse(const mat4& m) {
		mat4 original(m);
		mat4 inverse(1.0f);

		for (int i = 0; i < 4; i++) {
			float a = original[i][i];

			// zero up
			for (int j = i - 1; j >= 0; j--) {
				float b = original[j][i];
				original[j] += original[i] * -b / a;
				inverse[j] += inverse[i] * -b / a;
			}

			// zero down
			for (int j = i + 1; j < 4; j++) {
				float b = original[j][i];
				original[j] += original[i] * -b / a;
				inverse[j] += inverse[i] * -b / a;
			}

			inverse[i] /= a;
			original[i] /= a;
		}

		return inverse;
	}

	mat4 mat4::look_at(const vec3& eye, const vec3& center, const vec3& up) {
		// https://stackoverflow.com/a/21830596
		mat4 m;
		vec3 X, Y, Z;
		Z = eye - center;
		Z.normalize();
		Y = up;
		X = vec3::cross(Y, Z);
		Y = vec3::cross(Z, X);
		X.normalize();
		Y.normalize();

		m[0][0] = X.x;
		m[0][1] = X.y;
		m[0][2] = X.z;
		m[0][3] = vec3::dot(-X, eye);

		m[1][0] = Y.x;
		m[1][1] = Y.y;
		m[1][2] = Y.z;
		m[1][3] = vec3::dot(-Y, eye);

		m[2][0] = Z.x;
		m[2][1] = Z.y;
		m[2][2] = Z.z;
		m[2][3] = vec3::dot(-Z, eye);

		m[3][0] = 0;
		m[3][1] = 0;
		m[3][2] = 0;
		m[3][3] = 1.0f;

		return m;
	}

	std::ostream& operator<<(std::ostream& o, const mat4& m) {
		return o
			<< m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << '\n'
			<< m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << '\n'
			<< m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << '\n'
			<< m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3];
	}
}