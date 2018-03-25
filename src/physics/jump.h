#pragma once

#include <math.h>
#include "physics/vertex.h"

// Curve must have y(0) = 0, otherwise need to solve roots
static const f32 a = -0.45f; 
static const f32 b = 1.6f;
static const f32 c = 0.6f;
struct JumpSegment
{
	Vertex origin;

	JumpSegment(Vertex o) : origin(o) {}

	// Pls forget
	// Newton-Rhapson Method (not tested lol)
	f32 root() {
		f32 x, x_n;
		x = x_n = 0;

		do {
			// x_n+1 = x_n - f(x_n)/f'(x_n)
			x = x_n;
			f32 f = a * x * x + b * x + c;
			f32 df = 2 * a * x + b;
			x_n = x - f/df;
		} while (x_n-x < 0.0001f);

		return x_n;
	}

	void lerp(f32 t, f32* position) {
		static f32 x_vel = 1;
		// at2 + bt + c = 0 at t=0 and t=end?

		// x is linear motion
		// y is parabolic motion

		position[0] = origin.x + t * x_vel; //cos(3.1415/8);
		//position[1] = origin.y +
		//	a * t * t + b * t + c;
		position[1] = origin.y +
			20 * sin(3.1415/8) * t - 0.5 * 20 * t * t;
	}
};