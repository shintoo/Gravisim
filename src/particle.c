#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "particle.h"
#include "constants.h"

ParticleSystem * LoadParticleSystem(const char *path) {
	FILE *src;
	ParticleSystem *ret;

	src = fopen(path, "rb");
	if (!src) {
		fprintf(stderr, "Could not open file: %s\n", path);
		exit(EXIT_FAILURE);
	}

	ret = malloc(sizeof(ParticleSystem));

	fread(&ret->count, sizeof(unsigned int), 1, src);
	ret->particles = malloc(ret->count * sizeof(Particle) * 2);
	fread(ret->particles, sizeof(Particle), ret->count, src);
	ret->size = ret->count * 2;

	fclose(src);
	return ret;
}

void DestroyParticleSystem(ParticleSystem *PS) {
	free(PS->particles);
	free(PS);
	PS = NULL;
}

double GravitationalForce(const Particle *P1, const Particle *P2) {
	double x_dist = P1->position.x - P2->position.x;
	double y_dist = P1->position.y - P2->position.y;
	double r_squared = (SQUARE(x_dist) + SQUARE(y_dist));
	if (x_dist == 0 && y_dist == 0) return 0;

	return G * (P1->properties.mass * P2->properties.mass) / r_squared;
}

/* Calculate the Fnet for PA[index] using all of PA */
Vector NetGravitationalForce(Particle *PA, int n, int index) {
	Vector F;
	Vector ret = {0, 0};
	double theta;
	double Fg;

	for (int i = 0; i < n; i++) {
		if (i == index) {
			continue;
		}
		
		Fg = GravitationalForce(PA + index, PA + i);
		if (Fg == 0) continue;
		theta = atan2(PA[i].position.y - PA[index].position.y, PA[i].position.x - PA[index].position.x);

		F.x = Fg * cos(theta);
		F.y = Fg * sin(theta);
/*
		if (PA[index].position.x > PA[i].position.x) {
			F.x = 0 - F.x;
		}
		if (PA[index].position.y > PA[i].position.y) {
			F.y = 0 - F.y;
		}
*/
		printf(" F%d%d:\t\t{%g, %g}, %g\n", i, index, F.x, F.y, theta);
		ret.x += F.x;
		ret.y += F.y;
	}

	return ret;
}

ParticleSystem * NewParticleSystem(void) {
	ParticleSystem *ret;

	ret = malloc(sizeof(ParticleSystem));
	ret->particles = malloc(5 * sizeof(Particle));
	ret->size = 5;
	ret->count = 0;
	return ret;
}

void AddParticle(ParticleSystem *PS, double m, double r, double x, double y, double vx, double vy, double ax, double ay) {
	PS->particles[PS->count].properties.mass = m;
	PS->particles[PS->count].properties.radius = r;
	PS->particles[PS->count].position.x = x;
	PS->particles[PS->count].position.y = y;
	PS->particles[PS->count].velocity.x = vx;
	PS->particles[PS->count].velocity.y = vy;
	PS->particles[PS->count].acceleration.x = ax;
	PS->particles[PS->count].acceleration.y = ay;

	PS->count++;
	if (PS->count == PS->size) {
		PS->particles = realloc(PS->particles, PS->size * 2 * sizeof(Particle));
	}
}

void UpdateParticleSystem(ParticleSystem *S) {
	Particle *PA = S->particles;

	for (int i = 0; i < S->count; i++) {
		for (int k = 0; k < S->count; k++) {
			if (i == k) continue;
		}

		PA[i].NetGravitationalForce = NetGravitationalForce(PA, S->count, i);
		printf("P[%d]: Fnet:\t{%g, %g}\n", i, PA[i].NetGravitationalForce.x, PA[i].NetGravitationalForce.y);
		PA[i].acceleration.x = PA[i].NetGravitationalForce.x / PA[i].properties.mass;
		PA[i].acceleration.y = PA[i].NetGravitationalForce.y / PA[i].properties.mass;

		PA[i].velocity.x += PA[i].acceleration.x;
		PA[i].velocity.y += PA[i].acceleration.y;
		
		PA[i].position.x += PA[i].velocity.x;
		PA[i].position.y += PA[i].velocity.y;
	}
}

void RenderParticleSystem(ParticleSystem *S, SDL_Texture *texture, SDL_Renderer *r, SDL_Rect *Camera) {
	Particle *P = S->particles;
	SDL_Rect dst;
	for (int i = 0; i < S->count; i++) {
		dst.x = P[i].position.x / 1000 - P[i].properties.radius - Camera->x;
		dst.y = P[i].position.y / 1000 - P[i].properties.radius - Camera->y;
		dst.w = dst.h = 2 * P[i].properties.radius;
		if (P[i].properties.radius == 1) {
			SDL_RenderDrawPoint(r, dst.x, dst.y);
		} else {
			SDL_RenderCopy(r, texture, NULL, &dst);
		}
	}
}
