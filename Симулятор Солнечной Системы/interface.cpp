#ifdef _WIN32 
#include <Windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL2/SDL.h"
#include "interface.h"

auto keys = SDL_GetKeyboardState(nullptr);

void free_camera(
		double sensety,
		double& angle_U_D, double& angle_R_L,
		double& xGx, double& yGy, double& zGz,
		double& xOx, double& yOy, double& zOz) {
	//Поворот камеры
	if (keys[SDL_SCANCODE_UP]) { angle_U_D--; }
	if (keys[SDL_SCANCODE_DOWN]) { angle_U_D++; }
	if (keys[SDL_SCANCODE_LEFT]) { angle_R_L++; }
	if (keys[SDL_SCANCODE_RIGHT]) { angle_R_L--; }
	//Движение камеры
	if (keys[SDL_SCANCODE_W]) {
		xGx = xGx + cos(angle_R_L * M_PI / 180)*sensety;
		yGy = yGy + sin(angle_R_L * M_PI / 180)*sensety;
	}
	if (keys[SDL_SCANCODE_S]) {
		xGx = xGx - cos(angle_R_L * M_PI / 180)*sensety;
		yGy = yGy - sin(angle_R_L * M_PI / 180)*sensety;
	}
	if (keys[SDL_SCANCODE_A]) {
		xGx = xGx + cos((angle_R_L + 90) * M_PI / 180)*sensety;
		yGy = yGy + sin((angle_R_L + 90) * M_PI / 180)*sensety;
	}
	if (keys[SDL_SCANCODE_D]) {
		xGx = xGx - cos((angle_R_L + 90) * M_PI / 180)*sensety;
		yGy = yGy - sin((angle_R_L + 90) * M_PI / 180)*sensety;
	}
	if (keys[SDL_SCANCODE_LSHIFT]) zGz = zGz - sensety;
	if (keys[SDL_SCANCODE_SPACE]) zGz = zGz + sensety;
	//Ограничение камеры
	if (angle_U_D > 180)angle_U_D = 179.999;
	if (angle_U_D < 0)angle_U_D = 0.001;
	//Расчет взгляда
	xOx = xGx + sin(angle_U_D * M_PI / 180)*cos(angle_R_L * M_PI / 180);
	yOy = yGy + sin(angle_U_D * M_PI / 180)*sin(angle_R_L * M_PI / 180);
	zOz = zGz + cos(angle_U_D * M_PI / 180);
}

void static_camera(
	double mas[],int i,
	double scale_r, double scale_d,
	double& xGx, double& yGy, double& zGz,
	double& xOx, double& yOy, double& zOz) {
	xOx = yOy = zOz = zGz = 0.0;
	double r = 3 * mas[i] * scale_r + mas[i+1] * scale_d;
	xGx = r*cos(mas[i+5] * M_PI / 180);
	yGy = r*sin(mas[i+5] * M_PI / 180);
}

Button::Button(
	GLuint Texture, 
	int left_top_corner_x, int left_top_corner_y,
	int right_botom_corner_x, int right_bottom_corner_y,
	bool visible) {
	Button::left_top_corner_x = left_top_corner_x;
	Button::left_top_corner_y = left_top_corner_y;
	Button::right_botom_corner_x = right_botom_corner_x;
	Button::right_bottom_corner_y = right_bottom_corner_y;
	Button::button_visible = visible;

	ButtonList = glGenLists(1);
	glNewList(ButtonList, GL_COMPILE);

		glBindTexture(GL_TEXTURE_2D, Texture);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex2d(left_top_corner_x, left_top_corner_y);
		glTexCoord2d(0, 1);
		glVertex2d(left_top_corner_x, right_bottom_corner_y);
		glTexCoord2d(1, 1);
		glVertex2d(right_botom_corner_x, right_bottom_corner_y);
		glTexCoord2d(1, 0);
		glVertex2d(right_botom_corner_x, left_top_corner_y);
		glEnd();
	
	glEndList();
}
Button::~Button() {
	glDeleteLists(ButtonList, 1);
}
void Button::drawButton() {
	if (button_visible == true) { glCallList(ButtonList); }
}
bool Button::click(SDL_Event event) {
	if (button_visible == true) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			if ((event.button.y >= left_top_corner_y) && (event.button.y <= right_bottom_corner_y)
				&& (event.button.x <= right_botom_corner_x) && (event.button.x >= left_top_corner_x)) {
				return true;
			}
		}
	}
	return false;
}
void Button::visible(bool visible) {
	button_visible = visible;
}
