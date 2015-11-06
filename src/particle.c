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
	Vector ret = {0, 0};
	double theta;

	for (int i = 0; i < n; i++) {
		if (i == index) {
			continue;
		}
		theta = atan((PA[i].position.x - PA[index].position.x) / (PA[i].position.y - PA[index].position.y));
		ret.x += GravitationalForce(PA + i, PA + index) * cos(theta);
		ret.y += GravitationalForce(PA + i, PA + index) * sin(theta);
	}

	return ret;
}


void UpdateParticles(Particle *PA, int n) {
	for (int i = 0; i < n; i++) {
		PA[i].NetGravitationalForce = NetGravitationalForce(PA, n, i);
		PA[i].acceleration.x = PA[i].NetGravitationalForce.x / PA[i].properties.mass;
		PA[i].acceleration.y = PA[i].NetGravitationalForce.y / PA[i].properties.mass;

		PA[i].velocity.x += PA[i].acceleration.x;
		PA[i].velocity.y += PA[i].acceleration.y;
		
		PA[i].position.x += PA[i].velocity.x;
		PA[i].position.y += PA[i].velocity.y;
	}
}
