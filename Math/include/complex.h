#pragma once
#include <iostream>

namespace cgmath {
	class complex {
		public:
			float real, imag; // adjacent in memory

			complex();
			complex(float, float);

			void print() const;

			friend inline std::ostream& operator<<(std::ostream& os, const complex& c) {
				os << c.real << " + " << c.imag << 'i';
				return os;
			}

			void add(const complex&);
			complex& operator+=(const complex&);

			static complex add(const complex&, const complex&);
	};

	inline complex operator+(const complex& a, const complex& b) { return complex(a.real + b.real, a.imag + b.imag); }
}