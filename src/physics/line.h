#pragma once

#include "types.h"
#include "physics/vertex.h"

struct PosDir 
{
	f32 p, d; // 64 bit very good
};

struct LineSegment
{
	PosDir x, y;
	LineSegment* next;

	LineSegment() : x{0,0}, y{0,0} {}

	LineSegment(LineSegment& other) : 
		x{other.x}, y{other.y}, next(other.next) {};

	LineSegment(f32 sx, f32 sy, f32 ex, f32 ey) :
		x{sx, ex-sx}, 
		y{sy, ey-sy}
	{}

	void setup(f32 sx, f32 sy, f32 ex, f32 ey) {
		x = {sx, ex-sx};
		y = {sy, ey-sy};
	}

	void setup(Vertex a, Vertex b) {
		setup(a.x, a.y, b.x, b.y);
	}

	Vertex start() { return {x.p, y.p}; }
	
	Vertex start(f32 _x, f32 _y) {
		f32 x_dir = x.p + x.d - _x;
		f32 y_dir = y.p + y.d - _y;

		x = { _x, _x + x_dir };
		y = { _y, _y + y_dir };

		return start();
	}

	Vertex end() { return {x.p + x.d, y.p + y.d}; }
	
	Vertex end(f32 _x, f32 _y) {
		x.d = _x - x.p;
		y.d = _y - y.p;

		return end();
	}

	LineSegment* lerp(f32* s, f32* position) {
		f32 t = *s;

		// closed form equation of position
		f32 nx = x.p + x.d * t;
		f32 ny = y.p + y.d * t;

		position[0] = nx;
		position[1] = ny;

		if(t >= 1.0f) {
			return next;
		} else {
			return this;
		}
	}
};