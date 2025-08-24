#pragma once
 
void free_camera(
	double sensety,
	double& angle_U_D, double& angle_R_L,
	double& xGx, double& yGy, double& zGz,
	double& xOx, double& yOy, double& zOz);

void static_camera(
	double mas[], int i,
	double scale_r, double scale_d,
	double& xGx, double& yGy, double& zGz,
	double& xOx, double& yOy, double& zOz);

class Button {
protected:
	GLuint ButtonList;
	int left_top_corner_x;
	int left_top_corner_y;
	int right_botom_corner_x;
	int right_bottom_corner_y;
	bool button_visible;
public:
	Button(
		GLuint Texture,
		int left_top_corner_x, int left_top_corner_y,
		int right_botom_corner_x, int right_bottom_corner_y,
		bool visible = true);
	~Button();

	void drawButton();
	bool click(SDL_Event event);
	void visible(bool visible);
};