#include <iostream>
#include <cstdlib> 
#include <cmath>
#include <windows.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"
#include "SDL2/SDL_image.h"                                                                                         
#include "SDL2/SDL_mixer.h"

#include "GL/gl.h"
#include "GL/glu.h"

#include "cleanup.h"
#include "figures.h"
#include "interface.h"

const char * TITLE = "Model of the Solar System";
constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

SDL_Cleanup<SDL_Window>		win{ SDL_DestroyWindow };
SDL_Cleanup<void> context{ SDL_GL_DeleteContext };

void setupGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);	
}

void PerspectiveMode() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0, double(WIDTH) / double(HEIGHT), 0.05, 2000000000000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OrthoMode() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLuint loadTexture(const char * filename) {
	SDL_Surface *pic, *convpic;

	pic = IMG_Load(filename);
	if (!pic) {
		std::cerr << "loadTexture failed:" << SDL_GetError() << std::endl;
		exit(1);
	}

	convpic = SDL_CreateRGBSurface(
		0, pic->w, pic->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	SDL_BlitSurface(pic, nullptr, convpic, nullptr);
	SDL_FreeSurface(pic);

	GLuint result;
	glGenTextures(1, &result);

	glBindTexture(GL_TEXTURE_2D, result);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		convpic->w, convpic->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, convpic->pixels);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,
		convpic->w, convpic->h, GL_RGBA,
		GL_UNSIGNED_BYTE, convpic->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	SDL_FreeSurface(convpic);
	return result;
}

#ifdef _WIN32 
#undef main
#endif

int main(int argc, char*argv[]) {
	// Инициализация SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init() failed:" << SDL_GetError() << std::endl;
		exit(1);
	}
	//Создание окна
	win = SDL_CreateWindow(
		TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!win) {
		std::cerr << "SDL_CreateWindow() failed:" << SDL_GetError() << std::endl;
		exit(1);
	}
	//Создание контекста (внутреннего состояния) OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	context = SDL_GL_CreateContext(win);
	if (!context) {
		std::cerr << "SDL_GL_CreateContext() failed:" << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_GL_SetSwapInterval(1);

	//Информация планет 
	double info_solar_system[] = {
		//радиус, дистанция от солнца, угол поворота вокруг оси и S, угол поворота вокруг солнца и S
		696000.0, 0.0,                 0.00016656388009789736, 0.0,  0.0,                    0.0, //Sun
		2439.7,   57910000.0,          0.00007105445916124622, 0.0,  0.00004736442614044202, 252.25090552, //Mercury
		6051.8,	  108000000.0,         0.00001714680308270527, 0.0,  0.00001854302962060160, 181.97980085, //Venus
		6371.0,   150000000.0,         0.00418284471375089586, 0.0,  0.00001140761492689394, 100.46645683, //Earth
		3389.5,   228000000.0,         0.00407675187169922928, 0.0,  0.00000607858191709904, 355.43299958, //Mars
		69911.0,  778570000.0,         0.00000286843598712346, 0.0,  0.00000096243586892869, 34.35151874,  //Jupiter
		58232.0,  1430000000.0,        0.00000269758773237568, 0.0,  0.00000038871558769600, 50.07744430,  //Saturn
		25362.0,  2800000000.0,        0.00000162531071133195, 0.0,  0.00000013570397810459, 314.05500511, //Uranus
		24622.0,  4550000000.0,        0.00000173233387818325, 0.0,  0.00000006914855529141, 304.34866548  //Neptune
	};
	//Текстуры планет
	GLuint tex_solar_system[] = {
		loadTexture("texturs/sun.jpg"),
		loadTexture("texturs/mercury.jpg"),
		loadTexture("texturs/venus.jpg"),
		loadTexture("texturs/earth.jpg"),
		loadTexture("texturs/mars.jpg"),
		loadTexture("texturs/jupiter.jpg"),
		loadTexture("texturs/saturn.jpg"),
		loadTexture("texturs/uranus.jpg"),
		loadTexture("texturs/neptune.jpg")
	};
	//Кнопки===============================================================
	//Блок графики
	Button graphic_area{ loadTexture("texturs/graphic.jpg"),0,645,300,720 };
	Button low_graphic{ NULL,5,675,98,710 };
	Button average_graphic{ NULL,101,675,195,710 };
	Button high_graphic{ NULL,198,675,292,710 };
	//Блок камеры
	Button camera_area{ loadTexture("texturs/camera.jpg"),980,645,1280,720 };
	Button slow_camera{ NULL,987,675,1080,710 };
	Button average_camera{ NULL,1084,675,1178,710 };
	Button fast_camera{ NULL,1181,675,1275,710 };
	//Блок скорости
	Button speed_area{ loadTexture("texturs/speed.jpg"),440,0,840,50 };
	Button speed_x1{ NULL,560,10,595,35 };
	Button speed_x500{ NULL,602,10,662,35 };
	Button speed_x1000{ NULL,670,10,739,35 };
	Button speed_x10000{ NULL,747,10,828,35 };
	//Блок космических объектов
	Button objects_area{ loadTexture("texturs/objects.jpg"),1130,0,1280,375 };
	Button sun		{ NULL,1143,53,1215,73 };
	Button mercury	{ NULL,1143,117,1263,137 };
	Button venus	{ NULL,1143,148,1231,168 };
	Button earth	{ NULL,1143,179,1221,199 };
	Button mars		{ NULL,1143,210,1213,230 };
	Button jupiter	{ NULL,1143,241,1238,261 };
	Button saturn	{ NULL,1143,272,1229,292 };
	Button uranus	{ NULL,1143,303,1207,323 };
	Button neptune	{ NULL,1143,334,1230,354 };
	//Блок описаний к космическим объектам
	Button info_sun{ loadTexture("texturs/info_sun.jpg"),10,10,330,500, false };
	Button X_info_sun{ NULL,300,19,320,40,false};
	//=====================================================================
	//Нужные переменные--------------------
	//Скорость планет
	double speed = 1.0;
	//Масштаб для радиусов
	double scale_r = 1.0 / 10000.0;
	//Масштаб для дистанций
	double scale_d = 1.0 / 700000.0;
	//Координаты камеры
	double xGx = -550.0;
	double yGy = 0.0;
	double zGz = 200.0;
	//Координаты направления взгляда
	double xOx = 0.0;
	double yOy = 0.0;
	double zOz = 0.0;
	//Чувствительность камеры
	double sensety = 1.0;
	//Углы камеры
	double angle_U_D = 115.0;
	double angle_R_L = 0.0;
	//Параметры графики
	int orbital_parameter = 40;
	int meridians = 16;
	int parallels = 8;
	//Переменая для представления объектов
	int i=-1;
	//-------------------------------------
	setupGL();
	auto keys = SDL_GetKeyboardState(nullptr);
	SDL_Event event;
	for (;;) {
		//Цикл обработки событий
		while (SDL_PollEvent(&event)) {
			SDL_PumpEvents();
			switch (event.type) {
			case SDL_QUIT: exit(0); break;
			case SDL_MOUSEBUTTONUP:
				//Обработка кнопок
				//Блок графики
				if (low_graphic.click(event)) {
					orbital_parameter = 20;
					meridians = 8;
					parallels = 4;
				}
				if (average_graphic.click(event)) {
					orbital_parameter = 40;
					meridians = 16;
					parallels = 8;
				}
				if (high_graphic.click(event)) {
					orbital_parameter = 60;
					meridians = 32;
					parallels = 16;
				}
				//Блок камеры
				if (slow_camera.click(event)) { sensety = 0.1; }
				if (average_camera.click(event)) { sensety = 1.0; }
				if (fast_camera.click(event)) { sensety = 10.0; }
				//Блок скорости
				if (speed_x1.click(event)) { speed = 1.0; }
				if (speed_x500.click(event)) { speed = 500.0; }
				if (speed_x1000.click(event)) { speed = 1000.0; }
				if (speed_x10000.click(event)) { speed = 10000.0; }
				//Блок объектов
				if (sun.click(event)) { 
					i = 0; 
					X_info_sun.visible(true);
					info_sun.visible(true);
				}
				if (mercury.click(event)) { i = 6; }
				if (venus.click(event)) { i = 12; }
				if (earth.click(event)) { i = 18; }
				if (mars.click(event)) { i = 24; }
				if (jupiter.click(event)) { i = 30; }
				if (saturn.click(event)) { i = 36; }
				if (uranus.click(event)) { i = 42; }
				if (neptune.click(event)) { i = 48; }
				//Блок описаний к объектам
				if (X_info_sun.click(event)) { 
					i = -1;
					X_info_sun.visible(false);
					info_sun.visible(false);
				}
				break;
			}
		}
		//Расчет камеры
		if (i == -1) { free_camera(sensety, angle_U_D, angle_R_L, xGx, yGy, zGz, xOx, yOy, zOz); }
		else { static_camera(info_solar_system, i, scale_r, scale_d, xGx, yGy, zGz, xOx, yOy, zOz); }
			
		//Передвижение планет по орбите и вращение вокруг оси
		for (int i = 3; i < 54; i += 6) {
			info_solar_system[i] += info_solar_system[i - 1] * speed;
			if (info_solar_system[i] >= 360.0) {
				info_solar_system[i] -= 360.0;
			};
			info_solar_system[i + 2] += info_solar_system[i + 1] * speed;
			if (info_solar_system[i + 2] >= 360.0) {
				info_solar_system[i + 2] -= 360.0;
			};
		}

		int x, y;
		SDL_GetMouseState(&x,&y);
		cout<< "x = " << x << '\n' << "y = "<< y << '\n';
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Рисование
		PerspectiveMode();
		//Камера: глаза, объект, угол
		gluLookAt(xGx, yGy, zGz, xOx, yOy, zOz, 0.0, 0.0, 1.0);
		//3-мерные объекты
		SolarSystem ss{ info_solar_system,tex_solar_system,meridians,parallels,orbital_parameter,scale_r,scale_d };
		ss.drawSolarSystem();
		//2-мерные объекты
		OrthoMode();
		graphic_area.drawButton();
		camera_area.drawButton();
		speed_area.drawButton();
		objects_area.drawButton();
		info_sun.drawButton();

		SDL_GL_SwapWindow(win);
	}

	return 0;
}