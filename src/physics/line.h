#pragma once

#include "types.h"

struct PosDir 
{
	f32 p, d; // 64 bit very good
};

struct LineSegment
{
	PosDir x, y;
	LineSegment* next;

	LineSegment() : x{0,0}, y{0,0} {}

	LineSegment(f32 sx, f32 sy, f32 ex, f32 ey) :
		x{sx, ex-sx}, 
		y{sy, ey-sy}
	{}

	void setup(f32 sx, f32 sy, f32 ex, f32 ey) {
		x = {sx, ex-sx};
		y = {sy, ey-sy};
	}

	LineSegment* lerp(f32* s, f32* position) {
		f32 t = *s;

		// closed form equation of position
		f32 nx = x.p + x.d * t;
		f32 ny = y.p + y.d * t;

		// do some checks:
		// - outside line segment?
		// - move onto next line segment?
		// finally set some position
		position[0] = nx;
		position[1] = ny;
		if(t >= 1.0f) {
			return next;
		} else {
			return this;
		}
	}
};