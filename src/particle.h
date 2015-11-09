#ifndef PARTICLE_H
#define PARTICLE_H

#include <math.h>
#include <SDL2/SDL.h>

#define SQUARE(X) ((X) * (X))
#define dabs(D) ((D) > 0 ? (D) : 0 - (D))

#define G 6.67300E-11

#define SCALE_RATIO 56696980
#define SCALE_DOWN(X) ((X) / SCALE_RATIO)
#define SCALE_UP(X) ((X) * SCALE_RATIO)

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

typedef struct _particle_system {
	Particle *particles;
	unsigned int size;
	unsigned int count;
} ParticleSystem;

ParticleSystem * NewParticleSystem(void);

void AddParticle(ParticleSystem *PS, double m, double r, double x, double y, double vx, double vy, double ax, double ay);

void DestroyParticleSystem(ParticleSystem *PS);

ParticleSystem * LoadParticleSystem(const char *path);

double GravitationalForce(const Particle *B1, const Particle *B2);

Vector NetGravitationalForce(Particle *PA, int n, int index);

/* Update the position, velocity, and acceleration of each particle */
void UpdateParticleSystem(ParticleSystem *s);

/* render the particles */
void RenderParticleSystem(ParticleSystem *S, SDL_Texture *texture, SDL_Renderer *r, SDL_Rect *Camera);

/* find the gravitational force between two bodies */
double GravitationalForce(const Particle *B1, const Particle *B2);

#endif
