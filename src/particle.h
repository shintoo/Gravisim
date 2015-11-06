#ifndef PARTICLE_H
#define PARTICLE_H

#include <math.h>

#define SQUARE(X) ((X) * (X))

#define G 6.67300E-11

typedef struct s_vector {
	double x;
	double y;
} Vector;

typedef struct s_particle {
	/* Cartesian position of the particle */
	Vector position;

	/* Differential of the position */
	Vector velocity;

	/* Differential of the velocity */
	Vector acceleration;

	/* Various properties, redundant for ease */
	struct {
		double mass; /* kg */
		double density; /* kg/m^3 */
		double volume; /* m^2 */
		double radius; /* m */
	} properties;

	Vector NetGravitationalForce;

} Particle;

/* read an array of bodies from a file */
Particle * ReadBodies(const char *path);

double GravitationalForce(const Particle *B1, const Particle *B2);

Vector NetGravitationalForce(Particle *PA, int n, int index);

/* Update the position, velocity, and acceleration of each particle */
void UpdateParticles(Particle *Bodies, int n);

/* render the particles */
void RenderParticles(Particle *P, int n, SDL_Texture *texture);

/* find the gravitational force between two bodies */
double GravitationalForce(const Particle *B1, const Particle *B2);

/* main loop, UpdateBodies(), SDL stuff, etc */
void RunSim(Particle *Bodies);

/* Delete all the bodies, end the SDL stuff */
void EndSim(Particle *Bodies);

#endif
