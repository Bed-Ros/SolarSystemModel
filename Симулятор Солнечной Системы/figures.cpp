#ifdef _WIN32 
#include <Windows.h>
#endif

#include "SDL2/SDL.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "figures.h"

void cyl_conv(double r, double phi, double zz, double& x, double& y, double& z) {
	x = r*cos(phi);
	y = r*sin(phi);
	z = zz;
}

void sph_conv(double r, double phi, double theta, double& x, double& y, double& z) {
	x = r*cos(phi)*sin(theta);
	y = r*sin(phi)*sin(theta);
	z = r*cos(theta);
}

void drawSphere(double radius, int meridians, int parallels, bool smooth, GLuint Texture) {
	double m_step = 2.0*M_PI / meridians;
	double p_step = M_PI / parallels;

	for (int i = 0; i < meridians; i++) {
		for (int j = 0; j < parallels; j++) {

			double 
				x1, y1, z1,
				x2, y2, z2,
				x3, y3, z3,
				x4, y4, z4;
			double nx, ny, nz;

			sph_conv(radius, i*m_step, j*p_step, x1, y1, z1);
			sph_conv(radius, i*m_step, (j + 1)*p_step, x2, y2, z2);
			sph_conv(radius, (i + 1)*m_step, (j + 1)*p_step, x3, y3, z3);
			sph_conv(radius, (i + 1)*m_step, j*p_step, x4, y4, z4);

			glBindTexture(GL_TEXTURE_2D, Texture);

			glBegin(GL_QUADS);

			if (!smooth) {
				cyl_conv((i + j)*p_step, (i + j)*p_step, (i + j)*p_step*m_step, nx, ny, nz);
				glNormal3d(nx, ny, nz);
			}

			if (smooth)glNormal3d(x1, y1, z1);
			glTexCoord2d(double(i) / meridians, double(j) / parallels);
			glVertex3d(x1, y1, z1);

			if (smooth)glNormal3d(x2, y2, z2);
			glTexCoord2d(double(i) / meridians, double(j + 1) / parallels);
			glVertex3d(x2, y2, z2);

			if (smooth)glNormal3d(x3, y3, z3);
			glTexCoord2d(double(i + 1) / meridians, double(j + 1) / parallels);
			glVertex3d(x3, y3, z3);

			if (smooth)glNormal3d(x4, y4, z4);
			glTexCoord2d(double(i + 1) / meridians, double(j) / parallels);
			glVertex3d(x4, y4, z4);

			glEnd();
		}
	}
}

void drawOrbit(double radius, int number_of_angles) {
	double step = 2 * M_PI / (double)number_of_angles;
	double x1, y1, z1, x2, y2, z2;

	for (int i = 0; i < number_of_angles; i++) {

		cyl_conv(radius, i*step, 0, x1, y1, z1);
		cyl_conv(radius, (i + 1)*step, 0, x2, y2, z2);

		glBegin(GL_LINES);
		glVertex3d(x1, y1, z1);
		glVertex3d(x2, y2, z2);
		glEnd();
	}
}



SolarSystem::SolarSystem(
	double mas[], GLuint tex_mas[],
	int meridians, int parallels, int orbital_parameter,
	double scale_r, double scale_d) {
	SolarSystemList = glGenLists(1);
	glNewList(SolarSystemList, GL_COMPILE);
	int j = 0;
	glPushMatrix();

	for (int i = 0; mas[i]; i += 6) {

		glPushMatrix();
		glRotated(mas[i + 5], 0.0, 0.0, 1.0);//поворот по орбите
		glTranslated(mas[i + 1] * scale_d, 0.0, 0.0);//дальность от солнца
		glRotated(mas[i + 3], 0.0, 0.0, 1.0); //поворот планеты вокруг оси
		drawSphere(mas[i] * scale_r, meridians, parallels, 1, tex_mas[j]); //планета
		glPopMatrix();

		glPushMatrix();
		drawOrbit(mas[i + 1] * scale_d, orbital_parameter);
		glPopMatrix();

		j++;
	}

	glPopMatrix();

	glEndList();
}
SolarSystem::~SolarSystem() {
	glDeleteLists(SolarSystemList, 1);
}
void SolarSystem::drawSolarSystem() {
	glCallList(SolarSystemList);
}