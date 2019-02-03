#include "complex.h"

int main(int argc, char* argv[])
{
	cgmath::complex c1;
	cgmath::complex c2(1.0f, -2.0f);

	std::cout << c1 << '	' << c2 << std::endl;

	std::cin.get();
	return 0;
}