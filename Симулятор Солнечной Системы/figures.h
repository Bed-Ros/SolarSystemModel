#ifndef FIGURES_H
#define FIGURES_H

#include "SDL2/SDL.h"
#include <GL/gl.h>

void drawSphere(double radius,int meridians, int parallels, bool smooth, GLuint Texture);

void drawOrbit(double radius, int number_of_angles);

class SolarSystem{
protected:
	GLuint SolarSystemList;
public:
	SolarSystem(
		double mas[], GLuint tex_mas[],
		int meridians, int parallels, int orbital_parameter,
		double scale, double scale2);
	~SolarSystem();

	void drawSolarSystem();
};

#endif FIGURES_H