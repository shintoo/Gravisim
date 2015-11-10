#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "particle.h"
#include "constants.h"

#define SCREEN_WIDTH  1600
#define SCREEN_HEIGHT 900

SDL_Window * MakeWindow(int w, int h);
SDL_Renderer * MakeRenderer(SDL_Window *win);
SDL_Texture * LoadTexture(SDL_Renderer *r, const char *p);
void UpdateCamera(const uint8_t *ks, SDL_Rect *C);
int main(void) {
	SDL_Texture *circle;
	SDL_Texture *bg;
	SDL_Window *win;
	SDL_Renderer *renderer;
	SDL_Rect Camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	unsigned int frame = 1;


	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	/* sdl init */
	win = MakeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
	renderer = MakeRenderer(win);
	const uint8_t *KeyboardState = SDL_GetKeyboardState(NULL);

	circle = LoadTexture(renderer, "img/circle.png");
	bg = LoadTexture(renderer, "img/bg.png");

	/* end */

//	ParticleSystem *PS = LoadParticleSystem("ex/solarsystem.grv");
	ParticleSystem *PS = NewParticleSystem(renderer);
	AddParticle(PS, 100, 8, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, -5, 0, 0);
	AddParticle(PS, 1000, 12, SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 - 50, -5, -10, 0, 0);
	AddParticle(PS, 100000000, 16, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 150, -5, -10, 0, 0);
	AddParticle(PS, 10000000000000, 64, SCREEN_WIDTH, 1, -4, 5, 0, 0);
	AddParticle(PS, 1000000000000000, 96, 0, 0, 0, 0, 0, 0);
	printf("count: %d\n", PS->count);
	for (int i = 0; i < PS->count; i++) {
		printf("mass %d: %g at {%g, %g} going {%g, %g}\n",
			i, PS->particles[i].properties.mass,
			SCALE_DOWN(PS->particles[i].position.x), PS->particles[i].position.y,
			PS->particles[i].velocity.x, PS->particles[i].velocity.y);
	}

	while (!KeyboardState[SDL_SCANCODE_Q]) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bg, NULL, NULL);
		UpdateCamera(KeyboardState, &Camera);

		RenderParticleSystem(PS, circle, renderer, &Camera, 1);

		if (!KeyboardState[SDL_SCANCODE_P]) {
//			if (frame % 100 == 0) {
				UpdateParticleSystem(PS);
//			}
		}


		SDL_Delay(16);

		SDL_RenderPresent(renderer);
		SDL_PumpEvents();
		frame++;
	}

	free(PS->particles);
	free(PS);
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

void UpdateCamera(const uint8_t *ks, SDL_Rect *c) {
	if (ks[SDL_SCANCODE_W]) {
		c->y -= 5;
	}
	if (ks[SDL_SCANCODE_S]) {
		c->y += 5;
	}
	if (ks[SDL_SCANCODE_A]) {
		c->x -= 5;
	}
	if (ks[SDL_SCANCODE_D]) {
		c->x += 5;
	}
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

	SDL_SetColorKey(s, SDL_TRUE, SDL_MapRGB(s->format, 0, 255, 255));
	ret = SDL_CreateTextureFromSurface(r, s);

	SDL_FreeSurface(s);

	return ret;
}
