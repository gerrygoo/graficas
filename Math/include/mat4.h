#pragma once
#include <iostream>
#include "vec4.h"
#include "vec3.h"


namespace cgmath {
	class mat4 {
	private:
		float n[4][4] = { 0 };

	public:
		mat4() { }


		mat4(float diagonal) { n[0][0] = n[1][1] = n[2][2] = n[3][3] = diagonal; }
		mat4(const vec4& i, const vec4& j, const vec4& k, const vec4& l) : n{ {i.x, i.y, i.z, i.w}, {j.x, j.y, j.z, j.w}, {k.x, k.y, k.z, k.w}, {l.x, l.y, l.z, l.w} } { }

		vec4& operator[](int); // write
		const vec4& operator[](int) const; // access
		bool operator==(const mat4&) const;

		static mat4 inverse(const mat4&);
		static mat4 look_at(const vec3&, const vec3&, const vec3&);

		friend std::ostream& operator<<(std::ostream&, const mat4&);
	};

	inline vec4  operator*(const mat4& m, const vec4& v) { return m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w; }
	inline mat4  operator*(const mat4& m, const mat4& n) { return mat4(m * n[0], m * n[1], m * n[2], m * n[3]); }
}