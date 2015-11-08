#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "particle.h"

#define SCREEN_WIDTH  1376
#define SCREEN_HEIGHT 768

SDL_Window * MakeWindow(int w, int h);
SDL_Renderer * MakeRenderer(SDL_Window *win);
SDL_Texture * LoadTexture(SDL_Renderer *r, const char *p);

int main(void) {
	int frame = 1;
	SDL_Texture *circle;
	SDL_Window *win;
	SDL_Renderer *renderer;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	int delay = 100;

	/* sdl init crap */
	win = MakeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	renderer = MakeRenderer(win);
	const uint8_t *KeyboardState = SDL_GetKeyboardState(NULL);

	circle = LoadTexture(renderer, "../img/circle.png");
	/* end */

	Particle *P = MakeParticleArray(3);
/*
	P[0].position.x = 75000000 + 149604000;
	P[0].position.y = 40000000;
	P[0].properties.mass = 5.972E24;
	P[0].velocity.x = 0;
	P[0].velocity.y = -32;


	P[1].position.x = 384472 + 75000000 + 149604000;
	P[1].position.y = 40000000;
	P[1].properties.mass = 7.34767309E22;
	P[1].velocity.x = 0;
	P[1].velocity.y = -2;

	P[2].position.x = 75000000;
	P[2].position.y = 40000000;
	P[2].properties.mass = 1.989e30;
*/


	P[0].position.x = 320000;
	P[0].position.y = 100000;
	P[0].properties.mass = 100000000000000000000.0f;
	P[0].properties.radius = 8;

	P[1].position.x = 160000;
	P[1].position.y = 380000;
	P[1].properties.mass = 1000000000000000000.0f;
	P[1].velocity.x = 100;
	P[1].velocity.y = -50;
	P[1].properties.radius = 4;

	P[2].position.x = 480000;
	P[2].position.y = 380000;
	P[2].properties.mass = 1000000000000000000.0f;
	P[2].velocity.x = 50;
	P[2].velocity.y = -100;
	P[2].properties.radius = 4;

	while (!KeyboardState[SDL_SCANCODE_Q]) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		UpdateParticles(P, 3);

		RenderParticles(P, 3, circle, renderer);

		if (KeyboardState[SDL_SCANCODE_0]) {
			delay = 16;
		}
		if (KeyboardState[SDL_SCANCODE_1]) {
			delay = 100;
		}
		if (KeyboardState[SDL_SCANCODE_2]) {
			delay = 200;
		}
		if (KeyboardState[SDL_SCANCODE_3]) {
			delay = 300;
		}
		if (KeyboardState[SDL_SCANCODE_4]) {
			delay = 400;
		}
		SDL_Delay(delay);

		SDL_RenderPresent(renderer);
		SDL_PumpEvents();
	}

	SDL_DestroyTexture(circle);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	IMG_Quit();
	return 0;
}

SDL_Window * MakeWindow(int w, int h) {
	SDL_Window * ret = SDL_CreateWindow(
		"Gravisim",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		SDL_WINDOW_SHOWN
	);

	if (!ret) {
		fprintf(stderr, "Could not create window\n");
		return NULL;
	}
	return ret;
}

SDL_Renderer * MakeRenderer(SDL_Window *window) {
	SDL_Renderer *ret;

	ret = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (!ret) {
		fprintf(stderr, "Could not create renderer");
		return NULL;
	}

	return ret;
}

SDL_Texture * LoadTexture(SDL_Renderer *r, const char *p) {
	SDL_Texture *ret;
	SDL_Surface *s = IMG_Load(p);
	if (!s) puts("FUCK");
	SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, 0, 255, 255));
	ret = SDL_CreateTextureFromSurface(r, s);
	if (!ret) puts("DAMN IT");
	SDL_FreeSurface(s);

	return ret;
}
