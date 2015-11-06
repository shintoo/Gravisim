#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "particle.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window * MakeWindow(int w, int h);
SDL_Renderer * MakeRenderer(SDL_Window *win);

int main(void) {
	int frame = 1;
	SDL_Texture *circle;
	SDL_Window *win;
	SDL_Window *renderer;

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	/* sdl init crap */
	win = MakeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	renderer = MakeRenderer(win);
	const uint8_t *KeyboardState = SDL_GetKeyboardState(NULL);

	circle = 
	/* end */

	Particle P[3];

	P[0].position.x = 0;
	P[0].position.y = 0;
	P[0].properties.mass = 5.71;

	P[1].position.x = 7;
	P[1].position.y = 5.6;
	P[1].properties.mass = 12;

	P[2].position.x = 1;
	P[2].position.x = 9;
	P[2].properties.mass = 3.5;

	while (!KeyboardState[SDL_SCANCODE_Q]) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		UpdateParticles(P, 3);
		RenderParticles(P, 3, circle, renderer);

		SDL_RenderPresent(renderer);
		SDL_PumpEvents;

	}
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
