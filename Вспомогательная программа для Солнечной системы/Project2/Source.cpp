#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdlib> 
#include <cmath> 
#include <fstream>
#include <iomanip>
#include <stdio.h>

using namespace std;

int main() {
	FILE * out;
	out = fopen("output.txt", "w");

	double mas[] = {
	  //a,            e,           v-орбита, r,        v-оси
		0.1,          0.1,         0.1,      696000.0, 7284.0,
		57909227.0,   0.20563593,  47.36,    2439.7,   10.892,
		108208930.0,  0.0068,      35.02,    6051.8,   6.52,
		149598261.0,  0.01671123,  29.783,   6371.0,   1674.4,
		227943820.0,  0.0933941,   24.13,    3389.5,   868.22,
		778547200.0,  0.048775,    13.07,    69911.0,  12.6,
		1429394069.0, 0.055723219, 9.69,     58232.0,  9.87,
		2876679082.0, 0.044405586, 6.81,     25362.0,  2.59,
		4503443661.0, 0.011214269, 5.4349,   24622.0,  2.68
	};

	double orbita, os;

	for (int i = 0; i < 45; i += 5) {

		orbita = (360.0 * mas[i+2]) / (3.14159265358979323846 *(mas[i] + mas[i] * (sqrt(1 - (mas[i+1] * mas[i+1])))));
		os = (360.0 * mas[i+4]) / (2.0*3.14159265358979323846*mas[i+3] * 3600);

		fprintf(out, "%d\n", i/5);
		fprintf(out, "%.20f\n", os);
		fprintf(out, "%.20f\n", orbita);
	}

	fclose(out);
	system("pause");
	return 0;
}