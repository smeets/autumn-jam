#pragma once

#include "types.h"
#include <SDL2/SDL.h>

struct Curve
{
	f32 t0, t1;
	f32 y0, y1;

	Curve() : t0(0), t1(1), y0(0), y1(1) {}

	// sample y(t)
	f32 sample(f32 t) {
		f32 s = (t - t0) / (t1 - t0);
		if (s > 1) s = 1;
		else if (s < 0) s = 0;
		return y0 + (y1 - y0) * s;
	}
};