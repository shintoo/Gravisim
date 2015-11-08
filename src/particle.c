#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

double GravitationalForce(const Particle *P1, const Particle *P2) {
	double x_dist = P1->position.x - P2->position.x;
	double y_dist = P1->position.y - P2->position.y;
	double r_squared = (SQUARE(x_dist) + SQUARE(y_dist));

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

Particle * MakeParticleArray(int n) {
	Particle *ret;
	Vector none = {0,0};

	ret = malloc(sizeof(Particle) * n);

	for (int i = 0; i < n; i++) {
		ret[i].position = ret[i].velocity = ret[i].acceleration = none;
		ret[i].properties.mass = 0;
	}

	return ret;
}


void UpdateParticles(Particle *PA, int n) {
	for (int i = 0; i < n; i++) {
		PA[i].NetGravitationalForce = NetGravitationalForce(PA, n, i);
		printf("P[%d]: Fnet:\t{%g, %g}\n", i, PA[i].NetGravitationalForce.x, PA[i].NetGravitationalForce.y);
		PA[i].acceleration.x = PA[i].NetGravitationalForce.x / PA[i].properties.mass;
		PA[i].acceleration.y = PA[i].NetGravitationalForce.y / PA[i].properties.mass;

		PA[i].velocity.x += PA[i].acceleration.x;
		PA[i].velocity.y += PA[i].acceleration.y;
		
		PA[i].position.x += PA[i].velocity.x;
		PA[i].position.y += PA[i].velocity.y;
	}
}

void RenderParticles(Particle *P, int n, SDL_Texture *texture, SDL_Renderer *r) {
//	SDL_Rect src = {0, 0, 64, 64};
	SDL_Rect dst;
	SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
	for (int i = 0; i < n; i++) {
		dst.x = P[i].position.x / 1000 - P[i].properties.radius;
		dst.y = P[i].position.y / 1000 - P[i].properties.radius;
		dst.w = dst.h = 2 * P[i].properties.radius;
		SDL_RenderCopy(r, texture, NULL, &dst);
	}
}
