#include <math.h>

#include "mat4.h"

namespace cgmath {
	vec4& mat4::operator[](int i) { return reinterpret_cast<vec4&>(n[i]); }
	const vec4& mat4::operator[](int i) const { return reinterpret_cast<const vec4&>(n[i]); }

	bool mat4::operator==(const mat4& m) const {
		return this->operator[](0) == m[0] && this->operator[](1) == m[1] && this->operator[](3) == m[3];
	};

	mat4 mat4::inverse(const mat4& m) {
		float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		cgmath::vec4 Fac0(Coef00, Coef00, Coef02, Coef03);
		cgmath::vec4 Fac1(Coef04, Coef04, Coef06, Coef07);
		cgmath::vec4 Fac2(Coef08, Coef08, Coef10, Coef11);
		cgmath::vec4 Fac3(Coef12, Coef12, Coef14, Coef15);
		cgmath::vec4 Fac4(Coef16, Coef16, Coef18, Coef19);
		cgmath::vec4 Fac5(Coef20, Coef20, Coef22, Coef23);

		cgmath::vec4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
		cgmath::vec4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
		cgmath::vec4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
		cgmath::vec4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

		cgmath::vec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		cgmath::vec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		cgmath::vec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		cgmath::vec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		cgmath::vec4 SignA(+1, -1, +1, -1);
		cgmath::vec4 SignB(-1, +1, -1, +1);
		mat4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		cgmath::vec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		cgmath::vec4 Dot0(m[0] * Row0);
		float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		float OneOverDeterminant = 1.0f / Dot1;

		Inverse[0] *= OneOverDeterminant;
		Inverse[1] *= OneOverDeterminant;
		Inverse[2] *= OneOverDeterminant;
		Inverse[3] *= OneOverDeterminant;

		return Inverse;
	}

	mat4 mat4::perspective(float aspect_ratio, float fov, float near_plane, float far_plane) {
		fov = fov * (3.14159265358979f / 180.0f);
		mat4 to_ret;
		to_ret[0].x = 1.0f / (aspect_ratio * tan(fov / 2.0f));
		to_ret[1].y = 1.0f / tan(fov / 2.0f);
		to_ret[2].z -= (far_plane + near_plane) / (far_plane - near_plane);
		to_ret[3].z -= 2.0f * far_plane * near_plane / (far_plane - near_plane);
		to_ret[2].w = -1;
		return to_ret;
	}

	std::ostream& operator<<(std::ostream& o, const mat4& m) {
		return o
			<< m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << '\n'
			<< m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << '\n'
			<< m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << '\n'
			<< m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3];
	}
}